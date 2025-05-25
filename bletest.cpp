//
// Created by sigsegv on 5/24/25.
//

#include "BleDevice.h"
#include <iostream>

int main() {
    BleDevice dev{"C8:2B:5C:00:75:67"};
    dev.Write("ATI\r");
    std::shared_ptr<BleDeviceRxWaiter> waiter{};
    std::cout << "Waiting 15sec while reading\n";
    for (int i = 0; i < 15; i++) {
        while (true) {
            auto rxres = dev.Read([](const std::string &msg) {
                std::string str{msg};
                std::transform(str.begin(), str.end(), str.begin(),
                               [](char ch) { if (ch == '\r') return '\n'; else return ch; });
                std::cout << str;
            });
            if (std::holds_alternative<std::string>(rxres)) {
                std::string str{std::get<std::string>(rxres)};
                std::transform(str.begin(), str.end(), str.begin(),
                               [](char ch) { if (ch == '\r') return '\n'; else return ch; });
                std::cout << str;
            } else {
                waiter = std::get<std::shared_ptr<BleDeviceRxWaiter>>(rxres);
                break;
            }
        }
        sleep(1);
        waiter->Unsubscribe();
    }
}
