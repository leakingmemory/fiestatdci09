//
// Created by sigsegv on 5/25/25.
//

#include "ElmDevice.h"
#include "SerialInterface.h"
#include "BleDevice.h"

ElmDevice::ElmDevice(const std::shared_ptr<SerialInterface> &serialInterface) : device(serialInterface) {}
ElmDevice::ElmDevice(const std::shared_ptr<BleDevice> &bleDevice) : device(bleDevice) {}

void ElmDevice::CommitAttributes() {
    struct CommitAttributesVisitor {
        void operator () (std::shared_ptr<SerialInterface> &serialInterface) {
            serialInterface->CommitAttributes();
        }
        void operator () (std::shared_ptr<BleDevice> &bleDevice) {
        }
    };
    std::visit(CommitAttributesVisitor(), device);
}

void ElmDevice::Write(const std::string &msg) {
    struct WriteVisitor {
        const std::string &msg;
        void operator () (std::shared_ptr<SerialInterface> &serialInterface) {
            serialInterface->Write(msg);
        }
        void operator () (std::shared_ptr<BleDevice> &bleDevice) {
            bleDevice->Write(msg);
        }
    } visitor{.msg = msg};
    std::visit(visitor, device);
}

std::string ElmDevice::Read() {
    struct ReadVisitor {
        std::string msg;
        void operator () (std::shared_ptr<SerialInterface> &serialInterface) {
            msg = serialInterface->Read();
        }
        void operator () (std::shared_ptr<BleDevice> &bleDevice) {
            while (true) {
                auto opt = bleDevice->ReadNonBlocking();
                if (!opt) {
                    break;
                }
                if (!opt->empty()) {
                    msg = *opt;
                    break;
                }
            }
        }
    } visitor;
    std::visit(visitor, device);
    return visitor.msg;
}
