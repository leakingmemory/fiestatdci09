//
// Created by sigsegv on 5/25/25.
//

#ifndef FIESTATDCI09_ELMDEVICE_H
#define FIESTATDCI09_ELMDEVICE_H

#include <variant>
#include <memory>

class SerialInterface;
class BleDevice;

class ElmDevice {
private:
    std::variant<
                std::shared_ptr<SerialInterface>,
                std::shared_ptr<BleDevice>
            > device;
public:
    ElmDevice(const std::shared_ptr<SerialInterface> &serialInterface);
    ElmDevice(const std::shared_ptr<BleDevice> &bleDevice);
    void CommitAttributes();
    void Write(const std::string &msg);
    std::string Read();
};


#endif //FIESTATDCI09_ELMDEVICE_H
