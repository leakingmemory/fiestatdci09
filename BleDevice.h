//
// Created by sigsegv on 5/25/25.
//

#ifndef FIESTATDCI09_BLEDEVICE_H
#define FIESTATDCI09_BLEDEVICE_H

#include <string>
#include <memory>
#include <functional>
#include <variant>
#include <optional>
#include <gio/gio.h>

class BleDeviceRxCallback;

class BleDeviceRxWaiter {
private:
    std::function<void (const std::string &)> func{};
    bool waiting;
    std::mutex mtx;
public:
    BleDeviceRxWaiter(const std::function<void (const std::string &)> &);
    bool Signal(const std::string &);
    bool Unsubscribe();
};

class BleDevice {
    friend BleDeviceRxCallback;
private:
    std::vector<std::string> msgqueue{};
    std::vector<std::shared_ptr<BleDeviceRxWaiter>> waitqueue{};
    std::mutex mtx{};
    /*
     * Not sure how many of these are "standard" or not, except
     * probably addr which I would guess is unique to the adapter.
     */
    std::unique_ptr<GDBusObjectManager,std::function<void (GDBusObjectManager *)>> mgr{};
    std::unique_ptr<GDBusProxy,std::function<void (GDBusProxy *)>> dev{};
    std::unique_ptr<GDBusProxy,std::function<void (GDBusProxy *)>> rx{};
    std::unique_ptr<GDBusProxy,std::function<void (GDBusProxy *)>> tx{};
    std::string rxPath{};
    std::string txPath{};
    std::unique_ptr<void,std::function<void (void *)>> rxSubscription{};
    std::unique_ptr<GMainLoop,std::function<void (GMainLoop *)>> mainloop{};
public:
    BleDevice() = delete;
    BleDevice(const std::string &addr, const std::string &rxUuid, const std::string &txUuid);
    BleDevice(const std::string &addr) : BleDevice(
                addr,
                "0000fff1-0000-1000-8000-00805f9b34fb",
                "0000fff2-0000-1000-8000-00805f9b34fb"
            ) {}
    void Write(const std::string &);
private:
    void RxCallback(const std::string &);
public:
    std::variant<std::string,std::shared_ptr<BleDeviceRxWaiter>> Read(const std::function<void (const std::string &)> &);
    std::optional<std::string> ReadNonBlocking();
};


#endif //FIESTATDCI09_BLEDEVICE_H
