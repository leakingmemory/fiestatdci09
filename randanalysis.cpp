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
        std::vector<std::function<void ()>> tasks{};
        for (uint16_t keyHighH = 0; keyHighH < 256; keyHighH++) {
            auto pass1 = SeedKeyPass1(sequence[i], (uint64_t) keyHighH);
            for (uint16_t keyHighL = 0; keyHighL < 256; keyHighL++) {
                tasks.emplace_back([pass1, keyHighH, keyHighL, &sequence, i, &foundKeys, &mtx]() {
                    auto pass2 = SeedKeyPass2(pass1, ((uint32_t) keyHighL) << 24);
                    for (int keyMiddle = 0; keyMiddle < 256; keyMiddle++) {
                        auto pass3a = SeedKeyPass3a(pass2, ((uint32_t) keyMiddle) << 16);
#ifdef USE_AVX2
                        for (int keyLow = 0; keyLow < 65536; keyLow += 16) {
                            auto v = SeedKeyPass3bx16(pass3a, (uint32_t) keyLow);
#else
#ifdef USE_AVX
                        for (int keyLow = 0; keyLow < 65536; keyLow += 8) {
                            auto v = SeedKeyPass3bx8(pass3a, (uint32_t) keyLow);
#else
                        for (int keyLow = 0; keyLow < 65536; keyLow += 4) {
                            auto v = SeedKeyPass3bx4(pass3a, (uint32_t) keyLow);
#endif
#endif
                            if (v.key1 == sequence[i + 1]) {
                                uint16_t keyHigh = keyHighH;
                                keyHigh = keyHigh << 8;
                                keyHigh |= keyHighL;
                                uint64_t key = keyHigh;
                                key = key << 24;
                                key |= keyMiddle << 16;
                                key |= keyLow;
                                std::lock_guard lock{mtx};
                                std::cout << "Iteration key: 0x" << std::hex << key << std::dec << std::endl;
                                foundKeys.emplace_back(key);
                            }
                            if (v.key2 == sequence[i + 1]) {
                                uint16_t keyHigh = keyHighH;
                                keyHigh = keyHigh << 8;
                                keyHigh |= keyHighL;
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
                                uint16_t keyHigh = keyHighH;
                                keyHigh = keyHigh << 8;
                                keyHigh |= keyHighL;
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
                                uint16_t keyHigh = keyHighH;
                                keyHigh = keyHigh << 8;
                                keyHigh |= keyHighL;
                                uint64_t key = keyHigh;
                                key = key << 24;
                                key |= keyMiddle << 16;
                                key |= keyLow;
                                key += 3;
                                std::lock_guard lock{mtx};
                                std::cout << "Iteration key: 0x" << std::hex << key << std::dec << std::endl;
                                foundKeys.emplace_back(key);
                            }
#if defined(USE_AVX) || defined(USE_AVX2)
                            if (v.key5 == sequence[i + 1]) {
                                uint16_t keyHigh = keyHighH;
                                keyHigh = keyHigh << 8;
                                keyHigh |= keyHighL;
                                uint64_t key = keyHigh;
                                key = key << 24;
                                key |= keyMiddle << 16;
                                key |= keyLow;
                                key += 4;
                                std::lock_guard lock{mtx};
                                std::cout << "Iteration key: 0x" << std::hex << key << std::dec << std::endl;
                                foundKeys.emplace_back(key);
                            }
                            if (v.key6 == sequence[i + 1]) {
                                uint16_t keyHigh = keyHighH;
                                keyHigh = keyHigh << 8;
                                keyHigh |= keyHighL;
                                uint64_t key = keyHigh;
                                key = key << 24;
                                key |= keyMiddle << 16;
                                key |= keyLow;
                                key += 5;
                                std::lock_guard lock{mtx};
                                std::cout << "Iteration key: 0x" << std::hex << key << std::dec << std::endl;
                                foundKeys.emplace_back(key);
                            }
                            if (v.key7 == sequence[i + 1]) {
                                uint16_t keyHigh = keyHighH;
                                keyHigh = keyHigh << 8;
                                keyHigh |= keyHighL;
                                uint64_t key = keyHigh;
                                key = key << 24;
                                key |= keyMiddle << 16;
                                key |= keyLow;
                                key += 6;
                                std::lock_guard lock{mtx};
                                std::cout << "Iteration key: 0x" << std::hex << key << std::dec << std::endl;
                                foundKeys.emplace_back(key);
                            }
                            if (v.key8 == sequence[i + 1]) {
                                uint16_t keyHigh = keyHighH;
                                keyHigh = keyHigh << 8;
                                keyHigh |= keyHighL;
                                uint64_t key = keyHigh;
                                key = key << 24;
                                key |= keyMiddle << 16;
                                key |= keyLow;
                                key += 7;
                                std::lock_guard lock{mtx};
                                std::cout << "Iteration key: 0x" << std::hex << key << std::dec << std::endl;
                                foundKeys.emplace_back(key);
                            }
#ifdef USE_AVX2
                            if (v.key9 == sequence[i + 1]) {
                                uint16_t keyHigh = keyHighH;
                                keyHigh = keyHigh << 8;
                                keyHigh |= keyHighL;
                                uint64_t key = keyHigh;
                                key = key << 24;
                                key |= keyMiddle << 16;
                                key |= keyLow;
                                key += 8;
                                std::lock_guard lock{mtx};
                                std::cout << "Iteration key: 0x" << std::hex << key << std::dec << std::endl;
                                foundKeys.emplace_back(key);
                            }
                            if (v.key10 == sequence[i + 1]) {
                                uint16_t keyHigh = keyHighH;
                                keyHigh = keyHigh << 8;
                                keyHigh |= keyHighL;
                                uint64_t key = keyHigh;
                                key = key << 24;
                                key |= keyMiddle << 16;
                                key |= keyLow;
                                key += 9;
                                std::lock_guard lock{mtx};
                                std::cout << "Iteration key: 0x" << std::hex << key << std::dec << std::endl;
                                foundKeys.emplace_back(key);
                            }
                            if (v.key11 == sequence[i + 1]) {
                                uint16_t keyHigh = keyHighH;
                                keyHigh = keyHigh << 8;
                                keyHigh |= keyHighL;
                                uint64_t key = keyHigh;
                                key = key << 24;
                                key |= keyMiddle << 16;
                                key |= keyLow;
                                key += 10;
                                std::lock_guard lock{mtx};
                                std::cout << "Iteration key: 0x" << std::hex << key << std::dec << std::endl;
                                foundKeys.emplace_back(key);
                            }
                            if (v.key12 == sequence[i + 1]) {
                                uint16_t keyHigh = keyHighH;
                                keyHigh = keyHigh << 8;
                                keyHigh |= keyHighL;
                                uint64_t key = keyHigh;
                                key = key << 24;
                                key |= keyMiddle << 16;
                                key |= keyLow;
                                key += 11;
                                std::lock_guard lock{mtx};
                                std::cout << "Iteration key: 0x" << std::hex << key << std::dec << std::endl;
                                foundKeys.emplace_back(key);
                            }
                            if (v.key13 == sequence[i + 1]) {
                                uint16_t keyHigh = keyHighH;
                                keyHigh = keyHigh << 8;
                                keyHigh |= keyHighL;
                                uint64_t key = keyHigh;
                                key = key << 24;
                                key |= keyMiddle << 16;
                                key |= keyLow;
                                key += 12;
                                std::lock_guard lock{mtx};
                                std::cout << "Iteration key: 0x" << std::hex << key << std::dec << std::endl;
                                foundKeys.emplace_back(key);
                            }
                            if (v.key14 == sequence[i + 1]) {
                                uint16_t keyHigh = keyHighH;
                                keyHigh = keyHigh << 8;
                                keyHigh |= keyHighL;
                                uint64_t key = keyHigh;
                                key = key << 24;
                                key |= keyMiddle << 16;
                                key |= keyLow;
                                key += 13;
                                std::lock_guard lock{mtx};
                                std::cout << "Iteration key: 0x" << std::hex << key << std::dec << std::endl;
                                foundKeys.emplace_back(key);
                            }
                            if (v.key15 == sequence[i + 1]) {
                                uint16_t keyHigh = keyHighH;
                                keyHigh = keyHigh << 8;
                                keyHigh |= keyHighL;
                                uint64_t key = keyHigh;
                                key = key << 24;
                                key |= keyMiddle << 16;
                                key |= keyLow;
                                key += 14;
                                std::lock_guard lock{mtx};
                                std::cout << "Iteration key: 0x" << std::hex << key << std::dec << std::endl;
                                foundKeys.emplace_back(key);
                            }
                            if (v.key16 == sequence[i + 1]) {
                                uint16_t keyHigh = keyHighH;
                                keyHigh = keyHigh << 8;
                                keyHigh |= keyHighL;
                                uint64_t key = keyHigh;
                                key = key << 24;
                                key |= keyMiddle << 16;
                                key |= keyLow;
                                key += 15;
                                std::lock_guard lock{mtx};
                                std::cout << "Iteration key: 0x" << std::hex << key << std::dec << std::endl;
                                foundKeys.emplace_back(key);
                            }
#endif
#endif
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