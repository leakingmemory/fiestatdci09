//
// Created by sigsegv on 5/8/25.
//

#include "seedkey.h"
#include <iostream>

int main() {
    auto seed = 0x400CAB;
    auto exp = 0x585AB6;
    for (uint64_t key = 0; key < 0x10000000000; key++) {
        auto v = SeedKey(seed, key);
        if (v == exp) {
            std::cout << key << std::endl;
        }
    }
}