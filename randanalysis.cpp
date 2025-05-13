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
    for (decltype(sequence.size()) i = 0; i < (sequence.size() > 0 ? (sequence.size() - 1) : 0); i++) {
        std::cout << "Iteration " << i << "\n";
        std::mutex mtx{};
        std::vector<uint64_t> foundKeys{};
        uint32_t pass1Table[256];
        for (int i = 0; i < 256; i++) {
            pass1Table[i] = SeedKeyPass1(sequence[i], i >> 8);
        }
        std::vector<std::function<void ()>> tasks{};
        for (uint64_t keyHigh = 0; keyHigh < 65536; keyHigh++) {
            tasks.emplace_back([&pass1Table, keyHigh, &sequence, i, &foundKeys, &mtx] () {
                auto pass2 = SeedKeyPass2(pass1Table[keyHigh >> 8], ((uint32_t) (keyHigh & 0xFF)) << 24);
                for (int keyMiddle = 0; keyMiddle < 256; keyMiddle++) {
                    auto pass3a = SeedKeyPass3a(pass2, ((uint32_t) keyMiddle) << 16);
                    for (int keyLow = 0; keyLow < 65536; keyLow += 4) {
                        auto v = SeedKeyPass3bx4(pass3a, (uint32_t) keyLow);
                        if (v.key1 == sequence[i + 1]) {
                            uint64_t key = keyHigh;
                            key = key << 24;
                            key |= keyMiddle << 16;
                            key |= keyLow;
                            std::lock_guard lock{mtx};
                            std::cout << "Iteration key: 0x" << std::hex << key << std::dec << std::endl;
                            foundKeys.emplace_back(key);
                        }
                        if (v.key2 == sequence[i + 1]) {
                            uint64_t key = keyHigh;
                            key = key << 24;
                            key |= keyMiddle << 16;
                            key |= keyLow;
                            key += 1;
                            std::lock_guard lock{mtx};
                            std::cout << "Iteration key: 0x" << std::hex << key << std::dec << std::endl;
                            foundKeys.emplace_back(key);
                        }
                        if (v.key3 == sequence[i + 1]) {
                            uint64_t key = keyHigh;
                            key = key << 24;
                            key |= keyMiddle << 16;
                            key |= keyLow;
                            key += 2;
                            std::lock_guard lock{mtx};
                            std::cout << "Iteration key: 0x" << std::hex << key << std::dec << std::endl;
                            foundKeys.emplace_back(key);
                        }
                        if (v.key4 == sequence[i + 1]) {
                            uint64_t key = keyHigh;
                            key = key << 24;
                            key |= keyMiddle << 16;
                            key |= keyLow;
                            key += 3;
                            std::lock_guard lock{mtx};
                            std::cout << "Iteration key: 0x" << std::hex << key << std::dec << std::endl;
                            foundKeys.emplace_back(key);
                        }
                    }
                }
            });
        }
        std::vector<std::thread> threads{};
        for (int i = 0; i < 4; i++) {
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
        if (i == 0) {
            keys = foundKeys;
            continue;
        }
        std::cout << "Keys remaining:\n";
        auto iterator = keys.begin();
        while (iterator != keys.end()) {
            bool found{false};
            for (auto iterationKey : foundKeys) {
                if (*iterator == iterationKey) {
                    std::cout << "0x" << std::hex << iterationKey << std::dec << "\n";
                    found = true;
                    break;
                }
            }
            if (found) {
                ++iterator;
            } else {
                iterator = keys.erase(iterator);
            }
        }
    }
}