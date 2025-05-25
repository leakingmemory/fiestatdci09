//
// Created by sigsegv on 5/25/25.
//

#include "BleDevice.h"
#include <iostream>
#include <thread>

#define BLUEZ       "org.bluez"

class BleDeviceRxCallback {
public:
    constexpr BleDeviceRxCallback(BleDevice *bleDevice, const std::string &msg) {
        bleDevice->RxCallback(msg);
    }
};

BleDeviceRxWaiter::BleDeviceRxWaiter(const std::function<void(const std::string &)> &func) : func(func), waiting(true), mtx() {}

bool BleDeviceRxWaiter::Signal(const std::string &msg) {
    std::function<void (const std::string &)> func;
    {
        std::lock_guard lock{mtx};
        if (!waiting) {
            return false;
        }
        waiting = false;
        func = this->func;
        this->func = [] (const std::string &) {};
    }
    func(msg);
    return true;
}

bool BleDeviceRxWaiter::Unsubscribe() {
    std::lock_guard lock{mtx};
    if (waiting) {
        waiting = false;
        func = [] (const std::string &) {};
        return true;
    }
    return false;
}

static void notification_cb(GDBusConnection *c,const char *sender,const char *path,
                            const char *iface,const char *sig,GVariant *params,
                            gpointer data) {
    BleDevice *bleDevice = reinterpret_cast<BleDevice *>(data);
    const char *iname; GVariantIter *changes;
    g_variant_get(params,"(&sa{sv}as)",&iname,&changes,NULL);
    if (g_strcmp0(iname,"org.bluez.GattCharacteristic1")) return;
    const char *key; GVariant *val;
    while (g_variant_iter_next(changes,"{&sv}",&key,&val)) {
        if (g_strcmp0(key,"Value")==0) {
            gsize n; const char *b= (const char *) g_variant_get_fixed_array(val,&n,1);
            std::string str{b, n};
            BleDeviceRxCallback(bleDevice, str);
        }
        g_variant_unref(val);
    }
}

BleDevice::BleDevice(const std::string &deviceAddress, const std::string &rxUuid, const std::string &txUuid) {
    GError *err=NULL;

    /* 1. Get an object-manager for all BlueZ objects */
    {
        GDBusObjectManager *mgr =
                g_dbus_object_manager_client_new_for_bus_sync(
                        G_BUS_TYPE_SYSTEM, G_DBUS_OBJECT_MANAGER_CLIENT_FLAGS_NONE,
                        BLUEZ, "/", NULL, NULL, NULL, NULL, &err);
        if (!mgr) {
            g_printerr("manager: %s\n", err->message);
            return;
        }
        std::unique_ptr<GDBusObjectManager,std::function<void (GDBusObjectManager *)>> mgr_guard{mgr, [](GDBusObjectManager *mgr) {
            if (mgr != nullptr) {
                g_object_unref(mgr);
            }
        }};
        this->mgr = std::move(mgr_guard);
    }
    /* 2. Locate the Device1 interface whose Address == DEV_ADDR */
    {
        GList *objs = g_dbus_object_manager_get_objects(&*mgr), *l;
        GDBusProxy *dev = NULL;
        for (l = objs; l; l = l->next) {
            GDBusInterface *iface =
                    g_dbus_object_get_interface(G_DBUS_OBJECT(l->data), "org.bluez.Device1");
            if (!iface) continue;
            GVariant *v_addr = g_dbus_proxy_get_cached_property(
                    G_DBUS_PROXY(iface), "Address");
            const char *addr = NULL;
            if (v_addr) {
                addr = g_variant_get_string(v_addr, NULL);  /* no copy, just pointer */
            }

            /* --- test the address --- */
            if (addr && g_ascii_strcasecmp(addr, deviceAddress.c_str()) == 0) {
                dev = G_DBUS_PROXY(iface);
                g_object_ref(dev);
            }

            /* --- clean up --- */
            if (v_addr)
                g_variant_unref(v_addr);

            g_object_unref(iface);
            if (dev) break;
        }
        g_list_free_full(objs, g_object_unref);
        if (!dev) {
            g_printerr("BLE device not found\n");
            return;
        }
        std::unique_ptr<GDBusProxy,std::function<void (GDBusProxy *)>> dev_guard{dev, [](GDBusProxy *dev) {
            if (dev != nullptr) {
                g_object_unref(dev);
            }
        }};
        this->dev = std::move(dev_guard);
    }
    /* 3. Find RX and TX characteristics by UUID */
    {
        const char *dev_path = g_dbus_proxy_get_object_path(&*dev);
        const char *rx_path = NULL, *tx_path = NULL;

        GList *objs = g_dbus_object_manager_get_objects(&*mgr), *l;
        for (l = objs; l; l = l->next) {
            const char *path = g_dbus_object_get_object_path(G_DBUS_OBJECT(l->data));
            if (g_str_has_prefix(path, dev_path) == FALSE) continue;
            GDBusInterface *ch =
                    g_dbus_object_get_interface(G_DBUS_OBJECT(l->data),
                                                "org.bluez.GattCharacteristic1");
            if (!ch) continue;
            GVariant *v_uuid = g_dbus_proxy_get_cached_property(
                    G_DBUS_PROXY(ch), "UUID");
            const char *uuid = NULL;
            if (v_uuid) {
                uuid = g_variant_get_string(v_uuid, NULL);
            }
            /* ... compare uuid ... */
            if (uuid) {
                if (!rx_path && g_ascii_strcasecmp(uuid, rxUuid.c_str()) == 0) rx_path = path;
                else if (!tx_path && g_ascii_strcasecmp(uuid, txUuid.c_str()) == 0) tx_path = path;
            }
            if (v_uuid)
                g_variant_unref(v_uuid);
            g_object_unref(ch);
            if (rx_path && tx_path) break;
        }
        g_list_free_full(objs, g_object_unref);
        if (!rx_path || !tx_path) {
            g_printerr("RX/TX characteristics not found\n");
            return;
        }
        rxPath = rx_path;
        txPath = tx_path;

        GDBusProxy *rx = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM, (GDBusProxyFlags) 0, NULL,
                                                       BLUEZ, rx_path, "org.bluez.GattCharacteristic1", NULL, NULL);
        GDBusProxy *tx = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM, (GDBusProxyFlags) 0, NULL,
                                                       BLUEZ, tx_path, "org.bluez.GattCharacteristic1", NULL, NULL);
        std::unique_ptr<GDBusProxy,std::function<void (GDBusProxy *)>> rx_guard{rx, [](GDBusProxy *rx) {
            if (rx != nullptr) {
                g_object_unref(rx);
            }
        }};
        std::unique_ptr<GDBusProxy,std::function<void (GDBusProxy *)>> tx_guard{tx, [](GDBusProxy *tx) {
            if (tx != nullptr) {
                g_object_unref(tx);
            }
        }};
        this->rx = std::move(rx_guard);
        this->tx = std::move(tx_guard);
    }
    /* 4. Subscribe to notifications on RX */
    {
        g_dbus_proxy_call_sync(&*rx, "StartNotify", NULL, (GDBusCallFlags) 0, -1, NULL, &err);
        if (err) {
            g_printerr("StartNotify: %s\n", err->message);
            return;
        }

        GDBusConnection *bus = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, NULL);
        auto id = g_dbus_connection_signal_subscribe(bus, BLUEZ,
                                           "org.freedesktop.DBus.Properties", "PropertiesChanged",
                                           rxPath.c_str(), NULL, G_DBUS_SIGNAL_FLAGS_NONE,
                                           notification_cb, (void *) this, NULL);
        g_object_ref(bus);
        std::unique_ptr<void,std::function<void (void *)>> id_guard{(void *) (uintptr_t) 1, [bus, id](void *ref) {
            if (((uintptr_t) ref) == 1) {
                g_dbus_connection_signal_unsubscribe(bus, (guint) (uintptr_t) id);
                g_object_unref(bus);
            }
        }};
        this->rxSubscription = std::move(id_guard);
    }

    /* 5. Run */
    GMainLoop *loop=g_main_loop_new(NULL,FALSE);
    g_main_loop_ref(loop);
    std::thread thread{[loop] () {
        g_main_loop_run(loop);
        g_main_loop_unref(loop);
    }};
    thread.detach();
    std::unique_ptr<GMainLoop,std::function<void (GMainLoop *)>> loop_guard{loop, [](GMainLoop *loop) {
        g_main_loop_quit(loop);
        g_main_loop_unref(loop);
    }};
    this->mainloop = std::move(loop_guard);
    return;
}

void BleDevice::Write(const std::string &str) {
    GVariant *bytes = g_variant_new_fixed_array(G_VARIANT_TYPE_BYTE, str.c_str(), str.size(), 1);
    GVariant *tuple = g_variant_new("(@ay@a{sv})", bytes,
                                    g_variant_new("a{sv}", NULL));
    g_dbus_proxy_call_sync(&*tx,"WriteValue",tuple,(GDBusCallFlags)0,-1,NULL,NULL);
}

void BleDevice::RxCallback(const std::string &msg) {
    std::lock_guard lock{mtx};
    {
        auto waititer = waitqueue.begin();
        if (waititer != waitqueue.end()) {
            std::shared_ptr<BleDeviceRxWaiter> waiter{std::move(*waititer)};
            waititer = waitqueue.erase(waititer);
            if (waiter->Signal(msg)) {
                return;
            }
        }
    }
    msgqueue.emplace_back(msg);
}

std::variant<std::string, std::shared_ptr<BleDeviceRxWaiter>> BleDevice::Read(const std::function<void (const std::string &)> &func) {
    std::lock_guard lock{mtx};
    {
        auto iterator = msgqueue.begin();
        if (iterator != msgqueue.end()) {
            std::string msg{*iterator};
            iterator = msgqueue.erase(iterator);
            return msg;
        }
    }
    std::shared_ptr<BleDeviceRxWaiter> waiter = std::make_shared<BleDeviceRxWaiter>(func);
    waitqueue.emplace_back(waiter);
    return waiter;
}
