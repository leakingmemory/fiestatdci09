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
constexpr uint32_t SeedKeyPass1(uint32_t seed, uint32_t keyMostSignificantDW)
{
    auto v0 = ((seed & 0xFF0000) >> 16) | (seed & 0xFF00) | (keyMostSignificantDW) | ((seed & 0xFF) << 16);
    auto state = 0xc541a9;
    for (int i = 0; i < 32; i++) {
        auto bit = ((v0 >> i) & 1 ^ state & 1) << 23;
        auto v2 = bit | (state >> 1);
        state = v2 & 0xEF6FD7 | ((((v2 & 0x100000) >> 20) ^ ((v2 & 0x800000) >> 23)) << 20) | (((((state >> 1) & 0x8000) >> 15) ^ ((v2 & 0x800000) >> 23)) << 15) | (((((state >> 1) & 0x1000) >> 12) ^ ((v2 & 0x800000) >> 23)) << 12) | 32 * ((((state >> 1) & 0x20) >> 5) ^ ((v2 & 0x800000) >> 23)) | 8 * ((((state >> 1) & 8) >> 3) ^ ((v2 & 0x800000) >> 23));
    }
    return state;
}
constexpr uint32_t SeedKeyCompletionPass2(uint32_t state, uint32_t keyLeastSignificant24Bit)
{
    for (int i = 0; i < 32; i++) {
        auto bit = (((((keyLeastSignificant24Bit & 0xFF) << 24) | ((keyLeastSignificant24Bit & 0xFF00) << 8) | ((keyLeastSignificant24Bit >> 24) & 0xff) | (((keyLeastSignificant24Bit >> 16) & 0xff) << 8)) >> i) & 1 ^ state & 1) << 23;
        auto v3 = bit | (state >> 1);
        state = v3 & 0xEF6FD7 | ((((v3 & 0x100000) >> 20) ^ ((v3 & 0x800000) >> 23)) << 20) | (((((state >> 1) & 0x8000) >> 15) ^ ((v3 & 0x800000) >> 23)) << 15) | (((((state >> 1) & 0x1000) >> 12) ^ ((v3 & 0x800000) >> 23)) << 12) | 32 * ((((state >> 1) & 0x20) >> 5) ^ ((v3 & 0x800000) >> 23)) | 8 * ((((state >> 1) & 8) >> 3) ^ ((v3 & 0x800000) >> 23));
    }
    return ((state & 0xF0000) >> 16) | 16 * (state & 0xF) | ((((state & 0xF00000) >> 20) | ((state & 0xF000) >> 8)) << 8) | ((state & 0xFF0) >> 4 << 16);
}
constexpr uint32_t SeedKeyPass2(uint32_t state, uint32_t keyLeastSignificantDW)
{
    for (int i = 0; i < 8; i++) {
        auto bit = ((((keyLeastSignificantDW >> 24) & 0xff) >> i) & 1 ^ state & 1) << 23;
        auto v3 = bit | (state >> 1);
        state = v3 & 0xEF6FD7 | ((((v3 & 0x100000) >> 20) ^ ((v3 & 0x800000) >> 23)) << 20) | (((((state >> 1) & 0x8000) >> 15) ^ ((v3 & 0x800000) >> 23)) << 15) | (((((state >> 1) & 0x1000) >> 12) ^ ((v3 & 0x800000) >> 23)) << 12) | 32 * ((((state >> 1) & 0x20) >> 5) ^ ((v3 & 0x800000) >> 23)) | 8 * ((((state >> 1) & 8) >> 3) ^ ((v3 & 0x800000) >> 23));
    }
    return state;
}
constexpr uint32_t SeedKeyPass3a(uint32_t state, uint32_t keyLeastSignificant24Bit) {
    uint32_t bitConstant = (keyLeastSignificant24Bit & 0xff0000) >> 8;
    for (int i = 8; i < 16; i++) {
        auto bit = ((bitConstant >> i) & 1 ^ state & 1) << 23;
        auto v3 = bit | (state >> 1);
        state = v3 & 0xEF6FD7 | ((((v3 & 0x100000) >> 20) ^ ((v3 & 0x800000) >> 23)) << 20) | (((((state >> 1) & 0x8000) >> 15) ^ ((v3 & 0x800000) >> 23)) << 15) | (((((state >> 1) & 0x1000) >> 12) ^ ((v3 & 0x800000) >> 23)) << 12) | 32 * ((((state >> 1) & 0x20) >> 5) ^ ((v3 & 0x800000) >> 23)) | 8 * ((((state >> 1) & 8) >> 3) ^ ((v3 & 0x800000) >> 23));
    }
    return state;
}
constexpr uint32_t SeedKeyPass3b(uint32_t state, uint32_t keyLeastSignificant24Bit) {
    uint32_t bitConstant = ((keyLeastSignificant24Bit & 0xFF) << 24) | ((keyLeastSignificant24Bit & 0xFF00) << 8);
    for (int i = 16; i < 32; i++) {
        auto bit = ((bitConstant >> i) & 1 ^ state & 1) << 23;
        auto v3 = bit | (state >> 1);
        state = v3 & 0xEF6FD7 | ((((v3 & 0x100000) >> 20) ^ ((v3 & 0x800000) >> 23)) << 20) | (((((state >> 1) & 0x8000) >> 15) ^ ((v3 & 0x800000) >> 23)) << 15) | (((((state >> 1) & 0x1000) >> 12) ^ ((v3 & 0x800000) >> 23)) << 12) | 32 * ((((state >> 1) & 0x20) >> 5) ^ ((v3 & 0x800000) >> 23)) | 8 * ((((state >> 1) & 8) >> 3) ^ ((v3 & 0x800000) >> 23));
    }
    return ((state & 0xF0000) >> 16) | ((state & 0xF) << 4) | ((state & 0xF00000) >> 12) | (state & 0xF000) | ((state & 0xFF0) << 12);
}

struct KeyX4 {
    constexpr KeyX4() : key1(0), key2(0), key3(0), key4(0) {}
    uint32_t key1, key2, key3, key4;
};
constexpr KeyX4 SeedKeyPass3bx4(uint32_t state, uint32_t keyLeastSignificant24Bit) {
    KeyX4 result;
    uint64_t bitConstant = ((((uint64_t) keyLeastSignificant24Bit) & 0xFF) << 8) | (((uint64_t) (keyLeastSignificant24Bit & 0xFF00)) >> 8) |
            (((uint64_t) ((keyLeastSignificant24Bit + 1) & 0xFF)) << 24) | (((uint64_t) ((keyLeastSignificant24Bit + 1) & 0xFF00)) << 8) |
            (((uint64_t) ((keyLeastSignificant24Bit + 2) & 0xFF)) << 40) | (((uint64_t) ((keyLeastSignificant24Bit + 2) & 0xFF00)) << 24) |
            (((uint64_t) ((keyLeastSignificant24Bit + 3) & 0xFF)) << 56) | (((uint64_t) ((keyLeastSignificant24Bit + 3) & 0xFF00)) << 40);
    uint64_t statex2_1 = ((uint64_t) state) | (((uint64_t) state) << 32);
    uint64_t statex2_2 = ((uint64_t) state) | (((uint64_t) state) << 32);
    for (int i = 0; i < 16; i++) {
        uint64_t stateLowBitX4 = (statex2_1 & 0x0000000100000001ull) | ((statex2_2 & 0x0000000100000001ull) << 16);
        auto bitsx4 = ((bitConstant >> i) & 0x0001000100010001ull ^ stateLowBitX4);
        auto bitsx2_1 = (bitsx4 & 0x0000000100000001ull) << 23;
        auto bitsx2_2 = (bitsx4 & 0x0001000000010000ull) << 7;
        auto v3_1 = bitsx2_1 | ((statex2_1 >> 1) & 0x7FFFFFFF7FFFFFFFull);
        auto v3_2 = bitsx2_2 | ((statex2_2 >> 1) & 0x7FFFFFFF7FFFFFFFull);
        statex2_1 = v3_1 & 0x00EF6FD700EF6FD7ull | ((((v3_1 & 0x0010000000100000ull) >> 20) ^ ((v3_1 & 0x0080000000800000ull) >> 23)) << 20) | (((((statex2_1 >> 1) & 0x0000800000008000ull) >> 15) ^ ((v3_1 & 0x0080000000800000ull) >> 23)) << 15) | (((((statex2_1 >> 1) & 0x0000100000001000ull) >> 12) ^ ((v3_1 & 0x0080000000800000ull) >> 23)) << 12) | 32 * ((((statex2_1 >> 1) & 0x0000002000000020ull) >> 5) ^ ((v3_1 & 0x0080000000800000ull) >> 23)) | (((((statex2_1 >> 1) & 0x0000000800000008ull) >> 3) ^ ((v3_1 & 0x0080000000800000ull) >> 23)) << 3);
        statex2_2 = v3_2 & 0x00EF6FD700EF6FD7ull | ((((v3_2 & 0x0010000000100000ull) >> 20) ^ ((v3_2 & 0x0080000000800000ull) >> 23)) << 20) | (((((statex2_2 >> 1) & 0x0000800000008000ull) >> 15) ^ ((v3_2 & 0x0080000000800000ull) >> 23)) << 15) | (((((statex2_2 >> 1) & 0x0000100000001000ull) >> 12) ^ ((v3_2 & 0x0080000000800000ull) >> 23)) << 12) | 32 * ((((statex2_2 >> 1) & 0x0000002000000020ull) >> 5) ^ ((v3_2 & 0x0080000000800000ull) >> 23)) | (((((statex2_2 >> 1) & 0x0000000800000008ull) >> 3) ^ ((v3_2 & 0x0080000000800000ull) >> 23)) << 3);
    }
    result.key1 = statex2_1 & 0xFFFFFFFFull;
    result.key2 = statex2_2 & 0xFFFFFFFFull;
    result.key3 = statex2_1 >> 32;
    result.key4 = statex2_2 >> 32;
    result.key1 = ((result.key1 & 0xF0000) >> 16) | ((result.key1 & 0xF) << 4) | ((result.key1 & 0xF00000) >> 12) | (result.key1 & 0xF000) | ((result.key1 & 0xFF0) << 12);
    result.key2 = ((result.key2 & 0xF0000) >> 16) | ((result.key2 & 0xF) << 4) | ((result.key2 & 0xF00000) >> 12) | (result.key2 & 0xF000) | ((result.key2 & 0xFF0) << 12);
    result.key3 = ((result.key3 & 0xF0000) >> 16) | ((result.key3 & 0xF) << 4) | ((result.key3 & 0xF00000) >> 12) | (result.key3 & 0xF000) | ((result.key3 & 0xFF0) << 12);
    result.key4 = ((result.key4 & 0xF0000) >> 16) | ((result.key4 & 0xF) << 4) | ((result.key4 & 0xF00000) >> 12) | (result.key4 & 0xF000) | ((result.key4 & 0xFF0) << 12);
    return result;
}

#if 0
constexpr uint32_t ReverseSeedKeyPass3(uint32_t result, uint32_t keyLeastSignificant24Bit) {
    uint32_t state = ((result & 0xF) << 16) | ((result & 0xF0) >> 8) | ((result & 0xF00) << 12) | (result & 0xF000) | ((result & 0xFF0000) >> 12);
    uint32_t bitConstant = (((keyLeastSignificant24Bit & 0xFF) << 24) | ((keyLeastSignificant24Bit & 0xFF00) << 8) | (((keyLeastSignificant24Bit >> 16) & 0xff) << 8));
    for (int i = 31; i >= 8; i--) {
        state = state & 0xEF6FD7;
        auto bit = ((bitConstant >> i) & 1 ^ state & 1) << 23;
        auto v3 = bit | (state >> 1);
        // Bit 3
        // Bit 5
        // Bit 12
        // Bit 15
        // Bit 20
        auto xorbit = (v3 & 0x800000) >> 23;
        state = v3 & 0xEF6FD7 | ((((v3 & 0x100000) >> 20) ^ ((v3 & 0x800000) >> 23)) << 20) | (((((state >> 1) & 0x8000) >> 15) ^ ((v3 & 0x800000) >> 23)) << 15) | (((((state >> 1) & 0x1000) >> 12) ^ ((v3 & 0x800000) >> 23)) << 12) | 32 * ((((state >> 1) & 0x20) >> 5) ^ ((v3 & 0x800000) >> 23)) | 8 * ((((state >> 1) & 8) >> 3) ^ ((v3 & 0x800000) >> 23));
    }
}
#endif

constexpr uint32_t  SeedKey(uint32_t seed, uint64_t key) {
    uint32_t keyMostSignificant = (uint32_t) (key >> 32);
    uint32_t keyLeastSignificant = (uint32_t) (key & 0xFFFFFFFF);
    auto state = SeedKeyPass1(seed, keyMostSignificant);
    state = SeedKeyPass2(state, keyLeastSignificant);
    state = SeedKeyPass3a(state, keyLeastSignificant);
    return SeedKeyPass3b(state, keyLeastSignificant);
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

constexpr bool TestKeyX4Func(uint32_t input, uint64_t key, uint32_t expected) {
    auto pass1 = SeedKeyPass1(input, key >> 32);
    auto pass2 = SeedKeyPass2(pass1, key & 0xFF000000);
    auto pass3a = SeedKeyPass3a(pass2, key & 0x00FF0000);
    auto res1 = SeedKeyPass3bx4(pass3a, (key & 0xFFFF) - 3);
    auto res2 = SeedKeyPass3bx4(pass3a, (key & 0xFFFF) - 2);
    auto res3 = SeedKeyPass3bx4(pass3a, (key & 0xFFFF) - 1);
    auto res4 = SeedKeyPass3bx4(pass3a, (key & 0xFFFF));
    return res1.key4 == expected && res2.key3 == expected && res3.key2 == expected && res4.key1 == expected;
}

static_assert(TestKeyX4Func(0x400CAB, 13877892, 0x585AB6));

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
