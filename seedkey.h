//
// Created by sigsegv on 5/8/25.
//

#ifndef FIESTATDCI09_SEEDKEY_H
#define FIESTATDCI09_SEEDKEY_H

#include <cstdint>

/*
 * Based on https://github.com/andrewraharjo/CAN-Bus-Hack_Prius_Focus
 *
 * MIT License

Copyright (c) 2016 andrewraharjo

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */
constexpr uint32_t  SeedKey(uint32_t seed, uint64_t key) {
    auto v0 = ((seed & 0xFF0000) >> 16) | (seed & 0xFF00) | ((uint32_t) (key >> 32)) | ((seed & 0xFF) << 16);
    auto v1 = 0xc541a9;
    for (int i = 0; i < 32; i++) {
        auto bit = ((v0 >> i) & 1 ^ v1 & 1) << 23;
        auto v2 = bit | (v1 >> 1);
        v1 = v2 & 0xEF6FD7 | ((((v2 & 0x100000) >> 20) ^ ((v2 & 0x800000) >> 23)) << 20) | (((((v1 >> 1) & 0x8000) >> 15) ^ ((v2 & 0x800000) >> 23)) << 15) | (((((v1 >> 1) & 0x1000) >> 12) ^ ((v2 & 0x800000) >> 23)) << 12) | 32 * ((((v1 >> 1) & 0x20) >> 5) ^ ((v2 & 0x800000) >> 23)) | 8 * ((((v1 >> 1) & 8) >> 3) ^ ((v2 & 0x800000) >> 23));
    }
    for (int i = 0; i < 32; i++) {
        auto bit = (((((key & 0xFF) << 24) | ((key & 0xFF00) << 8) | ((key >> 24) & 0xff) | (((key >> 16) & 0xff) << 8)) >> i) & 1 ^ v1 & 1) << 23;
        auto v3 = bit | (v1 >> 1);
        v1 = v3 & 0xEF6FD7 | ((((v3 & 0x100000) >> 20) ^ ((v3 & 0x800000) >> 23)) << 20) | (((((v1 >> 1) & 0x8000) >> 15) ^ ((v3 & 0x800000) >> 23)) << 15) | (((((v1 >> 1) & 0x1000) >> 12) ^ ((v3 & 0x800000) >> 23)) << 12) | 32 * ((((v1 >> 1) & 0x20) >> 5) ^ ((v3 & 0x800000) >> 23)) | 8 * ((((v1 >> 1) & 8) >> 3) ^ ((v3 & 0x800000) >> 23));
    }
    return ((v1 & 0xF0000) >> 16) | 16 * (v1 & 0xF) | ((((v1 & 0xF00000) >> 20) | ((v1 & 0xF000) >> 8)) << 8) | ((v1 & 0xFF0) >> 4 << 16);
}

static_assert(SeedKey(0x400CAB, 13877892) == 0x585AB6);
static_assert(SeedKey(0x7B4164, 13877892) == 0x551CB4);
static_assert(SeedKey(0xF05B0C, 13877892) == 0x65CC8C);
static_assert(SeedKey(0xE17924, 13877892) == 0xF4CEDF);
static_assert(SeedKey(0x4BF5E2, 13877892) == 0xC2331F);
static_assert(SeedKey(0x877290, 13877892) == 0x0FDEE7);
static_assert(SeedKey(0x483565, 13877892) == 0xDA3971);
static_assert(SeedKey(0xC1BC45, 13877892) == 0x770470);
static_assert(SeedKey(0x45361E, 13877892) == 0x06C92D);
static_assert(SeedKey(0xD23AFF, 13877892) == 0xF9C873);


/*SEED : D23AFF
        KEY  : F9C873

        SEED : A83C08
        KEY  : AA8F2B

        SEED : 138CEC
KEY  : 0896BF

SEED : D93923
        KEY  : 245764

SEED : 02614D
KEY  : 0A884C

SEED : 1C39BC
KEY  : 522BD5

SEED : E59005
        KEY  : 606925

SEED : A089F0
        KEY  : E593C4

        SEED : 6F7227
KEY  : FFF94F

        SEED : CFFAAB
        KEY  : F4D5D0

        SEED : 04E5B1
KEY  : BCD128

        SEED : 029DE1
KEY  : F3B24F*/
#endif //FIESTATDCI09_SEEDKEY_H
