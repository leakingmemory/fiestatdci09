//
// Created by sigsegv on 5/12/25.
//

#include "seedkey.h"
#include <iostream>
#include <vector>
#include <functional>
#include <thread>
#include <mutex>

int main() {
    std::vector<uint32_t> sequence = {
            0x6E059A,
            0x4F1B57,
            0x9A5C0B,
            0xDF6178,
            0xAC0D69,
            0x804C13,
            0xB911D6,
            0xD62B47,
            0xCE109B,
            0x1A5150,
            0x6A191D,
            0x162A06,
            0xD75406,
            0x9F3717,
            0x320F75,
            0xA324BE,
            0x464695,
            0xED155D
    };
    std::vector<uint64_t> keys{};
    if (sequence.size() > 0) {
        std::cout << "Iteration 0\n";
        std::mutex mtx{};
        std::vector<uint64_t> foundKeys{};
        std::vector<std::function<void ()>> tasks{};
        for (uint16_t keyHighH = 0; keyHighH < 256; keyHighH++) {
            auto pass1 = SeedKeyPass1(sequence[0], (uint64_t) keyHighH);
            for (uint16_t keyHighL = 0; keyHighL < 256; keyHighL++) {
                tasks.emplace_back([pass1, keyHighH, keyHighL, &sequence, &foundKeys, &mtx]() {
                    auto pass2 = SeedKeyPass2(pass1, ((uint32_t) keyHighL) << 24);
                    for (int keyMiddle = 0; keyMiddle < 256; keyMiddle++) {
                        auto pass3a = SeedKeyPass3a(pass2, ((uint32_t) keyMiddle) << 16);
                        for (int keyLow = 0; keyLow < 65536; keyLow += 16) {
                            auto v = SeedKeyPass3bx16(pass3a, (uint32_t) keyLow);
                            uint16_t keyHigh = keyHighH;
                            keyHigh = keyHigh << 8;
                            keyHigh |= keyHighL;
                            uint64_t key_ = keyHigh;
                            key_ = key_ << 24;
                            key_ |= keyMiddle << 16;
                            key_ |= keyLow;
                            v = SeedKeyX16(v, key_);
                            if (v.value1 == sequence[1]) {
                                auto key = key_;
                                std::lock_guard lock{mtx};
                                std::cout << "Iteration key: 0x" << std::hex << key << std::dec << std::endl;
                                foundKeys.emplace_back(key);
                            }
                            if (v.value2 == sequence[1]) {
                                auto key = key_;
                                key += 1;
                                std::lock_guard lock{mtx};
                                std::cout << "Iteration key: 0x" << std::hex << key << std::dec << std::endl;
                                foundKeys.emplace_back(key);
                            }
                            if (v.value3 == sequence[1]) {
                                auto key = key_;
                                key += 2;
                                std::lock_guard lock{mtx};
                                std::cout << "Iteration key: 0x" << std::hex << key << std::dec << std::endl;
                                foundKeys.emplace_back(key);
                            }
                            if (v.value4 == sequence[1]) {
                                auto key = key_;
                                key += 3;
                                std::lock_guard lock{mtx};
                                std::cout << "Iteration key: 0x" << std::hex << key << std::dec << std::endl;
                                foundKeys.emplace_back(key);
                            }
                            if (v.value5 == sequence[1]) {
                                auto key = key_;
                                key += 4;
                                std::lock_guard lock{mtx};
                                std::cout << "Iteration key: 0x" << std::hex << key << std::dec << std::endl;
                                foundKeys.emplace_back(key);
                            }
                            if (v.value6 == sequence[1]) {
                                auto key = key_;
                                key += 5;
                                std::lock_guard lock{mtx};
                                std::cout << "Iteration key: 0x" << std::hex << key << std::dec << std::endl;
                                foundKeys.emplace_back(key);
                            }
                            if (v.value7 == sequence[1]) {
                                auto key = key_;
                                key += 6;
                                std::lock_guard lock{mtx};
                                std::cout << "Iteration key: 0x" << std::hex << key << std::dec << std::endl;
                                foundKeys.emplace_back(key);
                            }
                            if (v.value8 == sequence[1]) {
                                auto key = key_;
                                key += 7;
                                std::lock_guard lock{mtx};
                                std::cout << "Iteration key: 0x" << std::hex << key << std::dec << std::endl;
                                foundKeys.emplace_back(key);
                            }
                            if (v.value9 == sequence[1]) {
                                auto key = key_;
                                key += 8;
                                std::lock_guard lock{mtx};
                                std::cout << "Iteration key: 0x" << std::hex << key << std::dec << std::endl;
                                foundKeys.emplace_back(key);
                            }
                            if (v.value10 == sequence[1]) {
                                auto key = key_;
                                key += 9;
                                std::lock_guard lock{mtx};
                                std::cout << "Iteration key: 0x" << std::hex << key << std::dec << std::endl;
                                foundKeys.emplace_back(key);
                            }
                            if (v.value11 == sequence[1]) {
                                auto key = key_;
                                key += 10;
                                std::lock_guard lock{mtx};
                                std::cout << "Iteration key: 0x" << std::hex << key << std::dec << std::endl;
                                foundKeys.emplace_back(key);
                            }
                            if (v.value12 == sequence[1]) {
                                auto key = key_;
                                key += 11;
                                std::lock_guard lock{mtx};
                                std::cout << "Iteration key: 0x" << std::hex << key << std::dec << std::endl;
                                foundKeys.emplace_back(key);
                            }
                            if (v.value13 == sequence[1]) {
                                auto key = key_;
                                key += 12;
                                std::lock_guard lock{mtx};
                                std::cout << "Iteration key: 0x" << std::hex << key << std::dec << std::endl;
                                foundKeys.emplace_back(key);
                            }
                            if (v.value14 == sequence[1]) {
                                auto key = key_;
                                key += 13;
                                std::lock_guard lock{mtx};
                                std::cout << "Iteration key: 0x" << std::hex << key << std::dec << std::endl;
                                foundKeys.emplace_back(key);
                            }
                            if (v.value15 == sequence[1]) {
                                auto key = key_;
                                key += 14;
                                std::lock_guard lock{mtx};
                                std::cout << "Iteration key: 0x" << std::hex << key << std::dec << std::endl;
                                foundKeys.emplace_back(key);
                            }
                            if (v.value16 == sequence[1]) {
                                auto key = key_;
                                key += 15;
                                std::lock_guard lock{mtx};
                                std::cout << "Iteration key: 0x" << std::hex << key << std::dec << std::endl;
                                foundKeys.emplace_back(key);
                            }
                        }
                    }
                });
            }
        }
        std::vector<std::thread> threads{};
        for (int i = 0; i < NUM_THREADS; i++) {
            threads.emplace_back([&mtx, &tasks] () {
                while (true) {
                    std::function<void ()> func{};
                    {
                        std::lock_guard lock{mtx};
                        auto iterator = tasks.begin();
                        if (iterator == tasks.end()) {
                            break;
                        }
                        func = *iterator;
                        iterator = tasks.erase(iterator);
                    }
                    func();
                }
            });
        }
        for (auto &thread : threads) {
            thread.join();
        }
        keys = foundKeys;
    }
    for (int i = 1; i < (sequence.size() - 1); i++) {
        std::cout << "Testing against " << i << "\n";
        auto iterator = keys.begin();
        while (iterator != keys.end()) {
            auto key = *iterator;
            auto state = SeedKey(sequence[i], key);
            if (SeedKey(state, key) == sequence[i + 1]) {
                std::cout << "Key works: 0x" << std::hex << key << std::dec << std::endl;
                ++iterator;
            } else {
                ++iterator;
            }
        }
    }
    for (auto key : keys) {
        std::cout << "Passed: " << std::hex << key << std::dec << std::endl;
    }
}