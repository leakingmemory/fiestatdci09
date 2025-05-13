//
// Created by sigsegv on 5/12/25.
//

#include "seedkey.h"
#include <iostream>
#include <vector>

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
        std::vector<uint64_t> foundKeys{};
        uint32_t pass1Table[256];
        for (int i = 0; i < 256; i++) {
            pass1Table[i] = SeedKeyPass1(sequence[i], i >> 8);
        }
        uint32_t pass2Table[65536];
        for (int i = 0; i < 65536; i++) {
            pass2Table[i] = SeedKeyPass2(pass1Table[i >> 8], ((uint32_t) (i & 0xFF)) << 24);
        }
        for (uint64_t keyLow = 0; keyLow < 0x1000000; keyLow++) {
            for (int keyHigh = 0; keyHigh < 65536; keyHigh++) {
                auto v = SeedKeyPass3(pass2Table[keyHigh], (uint32_t) keyLow);
                if (v == sequence[i + 1]) {
                    uint64_t key = keyHigh;
                    key = key << 24;
                    key |= keyLow;
                    std::cout << "Iteration key: 0x" << std::hex << key << std::dec << std::endl;
                    foundKeys.emplace_back(key);
                }
            }
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