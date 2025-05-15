//
// Created by sigsegv on 5/8/25.
//

#ifndef FIESTATDCI09_SEEDKEY_H
#define FIESTATDCI09_SEEDKEY_H

#include <cstdint>
#include <x86intrin.h>
#include <immintrin.h>
#include <exception>
#include <iostream>
#include <ostream>

//#define CHECK_PASS1X2
//#define CHECK_PASS2X2
//#define CHECK_PASS3AX2
#define CHECK_PASS3BX16
//#define CHECK_PASS3BX8
//#define CHECK_PASS3BX4
//#define CHECK_PASS3AX4
//#define CHECK_PASS3AX8

struct ValueX2 {
    constexpr ValueX2() : value1(0), value2(0) {}
    constexpr ValueX2(uint32_t v1, uint32_t v2) : value1(v1), value2(v2) {}
    uint32_t value1, value2;
};

struct ValueX4 {
    constexpr ValueX4() : value1(0), value2(0), value3(0), value4(0) {}
    constexpr ValueX4(uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4) : value1(v1), value2(v2), value3(v3), value4(v4) {}
    constexpr ValueX4(ValueX2 first, ValueX2 second) : value1(first.value1), value2(first.value2), value3(second.value1), value4(second.value2) {}
    uint32_t value1, value2, value3, value4;
    constexpr ValueX2 First() {
        return {value1, value2};
    }
    constexpr ValueX2 Second() {
        return {value3, value4};
    }
};

struct ValueX8 {
    ValueX8() = default;
    constexpr ValueX8(uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8) : value1(v1), value2(v2), value3(v3), value4(v4), value5(v5), value6(v6), value7(v7), value8(v8) {}
    constexpr ValueX8(ValueX4 first, ValueX4 second) : value1(first.value1), value2(first.value2), value3(first.value3), value4(first.value4), value5(second.value1), value6(second.value2), value7(second.value3), value8(second.value4) {}
    uint32_t value1, value2, value3, value4, value5, value6, value7, value8;
    constexpr ValueX4 First() {
        return {value1, value2, value3, value4};
    }
    constexpr ValueX4 Second() {
        return {value5, value6, value7, value8};
    }
};

struct ValueX16 {
    ValueX16() = default;
    constexpr ValueX16(ValueX8 first, ValueX8 second) : value1(first.value1), value2(first.value2), value3(first.value3), value4(first.value4), value5(first.value5), value6(first.value6), value7(first.value7), value8(first.value8), value9(second.value1), value10(second.value2), value11(second.value3), value12(second.value4), value13(second.value5), value14(second.value6), value15(second.value7), value16(second.value8) {}
    uint32_t value1, value2, value3, value4, value5, value6, value7, value8, value9, value10, value11, value12, value13, value14, value15, value16;
    constexpr ValueX8 First() {
        return {value1, value2, value3, value4, value5, value6, value7, value8};
    }
    constexpr ValueX8 Second() {
        return {value9, value10, value11, value12, value13, value14, value15, value16};
    }
};

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
constexpr ValueX2 SeedKeyPass1(ValueX2 seed, uint32_t keyMostSignificantDW)
{
    uint64_t v0 = ((seed.value1 & 0xFF0000) >> 16) | (seed.value1 & 0xFF00) | (keyMostSignificantDW) | ((seed.value1 & 0xFF) << 16);
    {
        uint64_t v0_2 = ((seed.value2 & 0xFF0000) >> 16) | (seed.value2 & 0xFF00) | (keyMostSignificantDW) | ((seed.value2 & 0xFF) << 16);
        v0 |= v0_2 << 32;
    }
    uint64_t state = 0x00c541a900c541a9ull;
    for (int i = 0; i < 32; i++) {
        uint64_t bit = ((v0 >> i) & 0x100000001ull ^ state & 0x100000001ull) << 23;
        uint64_t v2 = bit | ((state >> 1) & 0x7FFFFFFF7FFFFFFFull);
        state = v2 & 0x00EF6FD700EF6FD7ull | ((((v2 & 0x0010000000100000ull) >> 20) ^ ((v2 & 0x0080000000800000ull) >> 23)) << 20) | (((((state >> 1) & 0x0000800000008000ull) >> 15) ^ ((v2 & 0x0080000000800000ull) >> 23)) << 15) | (((((state >> 1) & 0x0000100000001000ull) >> 12) ^ ((v2 & 0x0080000000800000ull) >> 23)) << 12) | 32 * ((((state >> 1) & 0x0000002000000020ull) >> 5) ^ ((v2 & 0x0080000000800000ull) >> 23)) | 8 * ((((state >> 1) & 0x0000000800000008ull) >> 3) ^ ((v2 & 0x0080000000800000ull) >> 23));
    }
#ifdef CHECK_PASS1X2
    ValueX2 result =
#else
    return
#endif
    {static_cast<uint32_t>(state & 0xFFFFFFFFull), static_cast<uint32_t>(state >> 32)};
#ifdef CHECK_PASS1X2
    auto v1 = SeedKeyPass1(seed.value1, keyMostSignificantDW);
    auto v2 = SeedKeyPass1(seed.value2, keyMostSignificantDW);
    if (result.value1 != v1 || result.value2 != v2) {
        std::terminate();
    }
    return result;
#endif
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
constexpr ValueX2 SeedKeyPass2(ValueX2 seed, uint32_t keyLeastSignificantDW)
{
    uint64_t key = keyLeastSignificantDW + 1;
    key = key << 32;
    key |= keyLeastSignificantDW;
    uint64_t state = seed.value2;
    state = state << 32;
    state |= seed.value1;
    for (int i = 0; i < 8; i++) {
        auto bit = ((((key >> 24) & 0xff000000ffull) >> i) & 0x100000001ull ^ state & 0x100000001ull) << 23;
        auto v3 = bit | (state >> 1);
        state = v3 & 0x00EF6FD700EF6FD7ull | ((((v3 & 0x0010000000100000ull) >> 20) ^ ((v3 & 0x0080000000800000ull) >> 23)) << 20) | (((((state >> 1) & 0x0000800000008000ull) >> 15) ^ ((v3 & 0x0080000000800000ull) >> 23)) << 15) | (((((state >> 1) & 0x0000100000001000ull) >> 12) ^ ((v3 & 0x0080000000800000ull) >> 23)) << 12) | 32 * ((((state >> 1) & 0x0000002000000020ull) >> 5) ^ ((v3 & 0x0080000000800000ull) >> 23)) | 8 * ((((state >> 1) & 0x800000008ull) >> 3) ^ ((v3 & 0x0080000000800000ull) >> 23));
    }
#ifdef CHECK_PASS2X2
    ValueX2 result =
#else
    return
#endif
    {static_cast<uint32_t>(state & 0xFFFFFFFFull), static_cast<uint32_t>(state >> 32)};
#ifdef CHECK_PASS2X2
    auto v1 = SeedKeyPass2(seed.value1, keyLeastSignificantDW);
    auto v2 = SeedKeyPass2(seed.value2, keyLeastSignificantDW + 1);
    if (result.value1 != v1 || result.value2 != v2) {
        std::terminate();
    }
    return result;
#endif
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
constexpr ValueX2 SeedKeyPass3a(ValueX2 seed, uint32_t keyLeastSignificant24Bit) {
    uint64_t bitConstant = ((keyLeastSignificant24Bit + 1) & 0xff0000) >> 8;
    bitConstant = bitConstant << 32;
    bitConstant |= (keyLeastSignificant24Bit & 0xff0000) >> 8;
    uint64_t state = seed.value2;
    state = state << 32;
    state |= seed.value1;
    for (int i = 8; i < 16; i++) {
        auto bit = ((bitConstant >> i) & 0x100000001ull ^ state & 0x100000001) << 23;
        auto v3 = bit | (state >> 1);
        state = v3 & 0x00EF6FD700EF6FD7ull | ((((v3 & 0x0010000000100000ull) >> 20) ^ ((v3 & 0x0080000000800000ull) >> 23)) << 20) | (((((state >> 1) & 0x0000800000008000ull) >> 15) ^ ((v3 & 0x0080000000800000ull) >> 23)) << 15) | (((((state >> 1) & 0x0000100000001000ull) >> 12) ^ ((v3 & 0x0080000000800000ull) >> 23)) << 12) | 32 * ((((state >> 1) & 0x0000002000000020ull) >> 5) ^ ((v3 & 0x0080000000800000ull) >> 23)) | 8 * ((((state >> 1) & 0x0000000800000008ull) >> 3) ^ ((v3 & 0x0080000000800000ull) >> 23));
    }
#ifdef CHECK_PASS3AX2
    ValueX2 result =
#else
    return
#endif
    {static_cast<uint32_t>(state & 0xFFFFFFFFull), static_cast<uint32_t>(state >> 32)};
#ifdef CHECK_PASS3AX2
    auto v1 = SeedKeyPass3a(seed.value1, keyLeastSignificant24Bit);
    auto v2 = SeedKeyPass3a(seed.value2, keyLeastSignificant24Bit + 1);
    if (result.value1 != v1 || result.value2 != v2) {
        std::terminate();
    }
    return result;
#endif
}

#ifdef USE_AVX
ValueX8 SeedKeyPass3a(ValueX8 seed, uint32_t keyLeastSignificant24Bit) {
    ValueX8 result;
#ifdef CHECK_PASS3AX8
    ValueX8 expected;
    expected.value1 = SeedKeyPass3a(seed.value1, keyLeastSignificant24Bit);
    expected.value2 = SeedKeyPass3a(seed.value2, keyLeastSignificant24Bit + 1);
    expected.value3 = SeedKeyPass3a(seed.value3, keyLeastSignificant24Bit + 2);
    expected.value4 = SeedKeyPass3a(seed.value4, keyLeastSignificant24Bit + 3);
    expected.value5 = SeedKeyPass3a(seed.value5, keyLeastSignificant24Bit + 4);
    expected.value6 = SeedKeyPass3a(seed.value6, keyLeastSignificant24Bit + 5);
    expected.value7 = SeedKeyPass3a(seed.value7, keyLeastSignificant24Bit + 6);
    expected.value8 = SeedKeyPass3a(seed.value8, keyLeastSignificant24Bit + 7);
#endif
    __m256i bitConstant;
    __m256i lowBits;
    __m256i mask;
    __m256i state;
    {
        uint32_t bitConstantRaw[8] = {
            (keyLeastSignificant24Bit & 0xff0000) >> 16,
            ((keyLeastSignificant24Bit + 1) & 0xff0000) >> 16,
            ((keyLeastSignificant24Bit + 2) & 0xff0000) >> 16,
            ((keyLeastSignificant24Bit + 3) & 0xff0000) >> 16,
            ((keyLeastSignificant24Bit + 4) & 0xff0000) >> 16,
            ((keyLeastSignificant24Bit + 5) & 0xff0000) >> 16,
            ((keyLeastSignificant24Bit + 6) & 0xff0000) >> 16,
            ((keyLeastSignificant24Bit + 7) & 0xff0000) >> 16
        };
        uint32_t lowBitsRaw[8] = {1, 1, 1, 1, 1, 1, 1, 1};
        uint32_t maskRaw[8] = {0x00EF6FD7, 0x00EF6FD7, 0x00EF6FD7, 0x00EF6FD7, 0x00EF6FD7, 0x00EF6FD7, 0x00EF6FD7, 0x00EF6FD7};
        uint32_t stateRaw[8] = {seed.value1, seed.value2, seed.value3, seed.value4, seed.value5, seed.value6, seed.value7, seed.value8};
        bitConstant = _mm256_load_si256((const __m256i *) bitConstantRaw);
        lowBits = _mm256_load_si256((const __m256i *) lowBitsRaw);
        mask = _mm256_load_si256((const __m256i *) maskRaw);
        state = _mm256_load_si256((const __m256i *) stateRaw);
    }
    for (int i = 0; i < 8; i++) {
        __m256i v3 = _mm256_srli_epi32(state, 1);
        __m256i v3_bit23 = _mm256_xor_si256(bitConstant, state);
        {
            bitConstant = _mm256_srli_epi32(bitConstant, 1);
            v3_bit23 = _mm256_and_si256(v3_bit23, lowBits);
            __m256i bit = _mm256_slli_epi32(v3_bit23, 23);
            v3 = _mm256_or_si256(v3, bit);
        }
        __m256i v3_bit20 = _mm256_srli_epi32(v3, 20);
        __m256i st_bit16 = _mm256_srli_epi32(state, 16); // b15
        __m256i st_bit13 = _mm256_srli_epi32(state, 13); // b13
        __m256i st_bit6 = _mm256_srli_epi32(state, 6); // b5
        __m256i st_bit4 = _mm256_srli_epi32(state, 4); // b3
        v3_bit20 = _mm256_and_si256(_mm256_xor_si256(v3_bit20, v3_bit23), lowBits);
        st_bit16 = _mm256_and_si256(_mm256_xor_si256(st_bit16, v3_bit23), lowBits);
        st_bit13 = _mm256_and_si256(_mm256_xor_si256(st_bit13, v3_bit23), lowBits);
        st_bit6 = _mm256_and_si256(_mm256_xor_si256(st_bit6, v3_bit23), lowBits);
        st_bit4 = _mm256_and_si256(_mm256_xor_si256(st_bit4, v3_bit23), lowBits);

        v3_bit20 = _mm256_slli_epi32(v3_bit20, 20);
        st_bit16 = _mm256_slli_epi32(st_bit16, 15);
        st_bit13 = _mm256_slli_epi32(st_bit13, 12);
        st_bit6 = _mm256_slli_epi32(st_bit6, 5);
        st_bit4 = _mm256_slli_epi32(st_bit4, 3);

        state = _mm256_and_si256(v3, mask);
        state = _mm256_or_si256(state, v3_bit20);
        state = _mm256_or_si256(state, st_bit16);
        state = _mm256_or_si256(state, st_bit13);
        state = _mm256_or_si256(state, st_bit6);
        state = _mm256_or_si256(state, st_bit4);
    }
    uint32_t vals[8];
    _mm256_store_si256(reinterpret_cast<__m256i*>(vals), state);
    result.value1 = vals[0];
    result.value2 = vals[1];
    result.value3 = vals[2];
    result.value4 = vals[3];
    result.value5 = vals[4];
    result.value6 = vals[5];
    result.value7 = vals[6];
    result.value8 = vals[7];
#ifdef CHECK_PASS3AX8
    if (result.value1 != expected.value1 || result.value2 != expected.value2 || result.value3 != expected.value3 || result.value4 != expected.value4 || result.value5 != expected.value5 || result.value6 != expected.value6 || result.value7 != expected.value7 || result.value8 != expected.value8) {
        std::terminate();
    }
#endif
    return result;
}
#endif

#ifdef USE_SSE2
ValueX4 SeedKeyPass3a(ValueX4 seed, uint32_t keyLeastSignificant24Bit) {
    ValueX4 result;
#ifdef CHECK_PASS3AX4
    ValueX4 expected;
    expected.value1 = SeedKeyPass3a(seed.value1, keyLeastSignificant24Bit);
    expected.value2 = SeedKeyPass3a(seed.value2, keyLeastSignificant24Bit + 1);
    expected.value3 = SeedKeyPass3a(seed.value3, keyLeastSignificant24Bit + 2);
    expected.value4 = SeedKeyPass3a(seed.value4, keyLeastSignificant24Bit + 3);
#endif
    __m128i bitConstant;
    __m128i lowBits;
    __m128i mask;
    __m128i state;
    {
        uint32_t bitConstantRaw[4] = {
            (keyLeastSignificant24Bit & 0xff0000) >> 16,
            ((keyLeastSignificant24Bit + 1) & 0xff0000) >> 16,
            ((keyLeastSignificant24Bit + 2) & 0xff0000) >> 16,
            ((keyLeastSignificant24Bit + 3) & 0xff0000) >> 16
        };
        uint32_t lowBitsRaw[4] = {1, 1, 1, 1};
        uint32_t maskRaw[4] = {0x00EF6FD7, 0x00EF6FD7, 0x00EF6FD7, 0x00EF6FD7};
        uint32_t stateRaw[4] = {seed.value1, seed.value2, seed.value3, seed.value4};
        bitConstant = _mm_load_si128((const __m128i *) bitConstantRaw);
        lowBits = _mm_load_si128((const __m128i *) lowBitsRaw);
        mask = _mm_load_si128((const __m128i *) maskRaw);
        state = _mm_load_si128((const __m128i *) stateRaw);
    }
    for (int i = 0; i < 8; i++) {
        __m128i v3 = _mm_srli_epi32(state, 1);
        __m128i v3_bit23 = _mm_xor_si128(bitConstant, state);
        {
            bitConstant = _mm_srli_epi32(bitConstant, 1);
            v3_bit23 = _mm_and_si128(v3_bit23, lowBits);
            __m128i bit = _mm_slli_epi32(v3_bit23, 23);
            v3 = _mm_or_si128(v3, bit);
        }
        __m128i v3_bit20 = _mm_srli_epi32(v3, 20);
        __m128i st_bit16 = _mm_srli_epi32(state, 16); // b15
        __m128i st_bit13 = _mm_srli_epi32(state, 13); // b13
        __m128i st_bit6 = _mm_srli_epi32(state, 6); // b5
        __m128i st_bit4 = _mm_srli_epi32(state, 4); // b3
        v3_bit20 = _mm_and_si128(_mm_xor_si128(v3_bit20, v3_bit23), lowBits);
        st_bit16 = _mm_and_si128(_mm_xor_si128(st_bit16, v3_bit23), lowBits);
        st_bit13 = _mm_and_si128(_mm_xor_si128(st_bit13, v3_bit23), lowBits);
        st_bit6 = _mm_and_si128(_mm_xor_si128(st_bit6, v3_bit23), lowBits);
        st_bit4 = _mm_and_si128(_mm_xor_si128(st_bit4, v3_bit23), lowBits);

        v3_bit20 = _mm_slli_epi32(v3_bit20, 20);
        st_bit16 = _mm_slli_epi32(st_bit16, 15);
        st_bit13 = _mm_slli_epi32(st_bit13, 12);
        st_bit6 = _mm_slli_epi32(st_bit6, 5);
        st_bit4 = _mm_slli_epi32(st_bit4, 3);

        state = _mm_and_si128(v3, mask);
        state = _mm_or_si128(state, v3_bit20);
        state = _mm_or_si128(state, st_bit16);
        state = _mm_or_si128(state, st_bit13);
        state = _mm_or_si128(state, st_bit6);
        state = _mm_or_si128(state, st_bit4);
    }
    uint32_t vals[4];
    _mm_store_si128(reinterpret_cast<__m128i*>(vals), state);
    result.value1 = vals[0];
    result.value2 = vals[1];
    result.value3 = vals[2];
    result.value4 = vals[3];
#ifdef CHECK_PASS3AX4
    if (result.value1 != expected.value1 || result.value2 != expected.value2 || result.value3 != expected.value3 || result.value4 != expected.value4) {
        std::terminate();
    }
#endif
    return result;
}
#endif

constexpr uint32_t SeedKeyPass3b(uint32_t state, uint32_t keyLeastSignificant24Bit) {
    uint32_t bitConstant = ((keyLeastSignificant24Bit & 0xFF) << 24) | ((keyLeastSignificant24Bit & 0xFF00) << 8);
    for (int i = 16; i < 32; i++) {
        auto bit = ((bitConstant >> i) & 1 ^ state & 1) << 23;
        auto v3 = bit | (state >> 1);
        state = v3 & 0xEF6FD7 | ((((v3 & 0x100000) >> 20) ^ ((v3 & 0x800000) >> 23)) << 20) | (((((state >> 1) & 0x8000) >> 15) ^ ((v3 & 0x800000) >> 23)) << 15) | (((((state >> 1) & 0x1000) >> 12) ^ ((v3 & 0x800000) >> 23)) << 12) | 32 * ((((state >> 1) & 0x20) >> 5) ^ ((v3 & 0x800000) >> 23)) | 8 * ((((state >> 1) & 8) >> 3) ^ ((v3 & 0x800000) >> 23));
    }
    return ((state & 0xF0000) >> 16) | ((state & 0xF) << 4) | ((state & 0xF00000) >> 12) | (state & 0xF000) | ((state & 0xFF0) << 12);
}

ValueX8 SeedKeyPass3bx8(uint32_t state, uint32_t keyLeastSignificant24Bit);
#ifndef USE_SSE2
#ifndef CHECK_PASS3BX4
constexpr
#endif
#endif
ValueX4 SeedKeyPass3bx4(uint32_t state, uint32_t keyLeastSignificant24Bit);
#ifndef USE_SSE2
#ifndef CHECK_PASS3BX4
constexpr
#endif
#endif
ValueX4 SeedKeyPass3bx4(ValueX4 seed, uint32_t keyLeastSignificant24Bit);

#ifdef USE_AVX512
inline ValueX16 SeedKeyPass3bx16(uint32_t state, uint32_t keyLeastSignificant24Bit) {
    ValueX16 result;
#ifdef CHECK_PASS3BX16
    ValueX16 expected;
    expected.value1 = SeedKeyPass3b(state, keyLeastSignificant24Bit);
    expected.value2 = SeedKeyPass3b(state, keyLeastSignificant24Bit + 1);
    expected.value3 = SeedKeyPass3b(state, keyLeastSignificant24Bit + 2);
    expected.value4 = SeedKeyPass3b(state, keyLeastSignificant24Bit + 3);
    expected.value5 = SeedKeyPass3b(state, keyLeastSignificant24Bit + 4);
    expected.value6 = SeedKeyPass3b(state, keyLeastSignificant24Bit + 5);
    expected.value7 = SeedKeyPass3b(state, keyLeastSignificant24Bit + 6);
    expected.value8 = SeedKeyPass3b(state, keyLeastSignificant24Bit + 7);
    expected.value9 = SeedKeyPass3b(state, keyLeastSignificant24Bit + 8);
    expected.value10 = SeedKeyPass3b(state, keyLeastSignificant24Bit + 9);
    expected.value11 = SeedKeyPass3b(state, keyLeastSignificant24Bit + 10);
    expected.value12 = SeedKeyPass3b(state, keyLeastSignificant24Bit + 11);
    expected.value13 = SeedKeyPass3b(state, keyLeastSignificant24Bit + 12);
    expected.value14 = SeedKeyPass3b(state, keyLeastSignificant24Bit + 13);
    expected.value15 = SeedKeyPass3b(state, keyLeastSignificant24Bit + 14);
    expected.value16 = SeedKeyPass3b(state, keyLeastSignificant24Bit + 15);
#endif
    __m128i one;
    __m128i twentythree;
    __m128i twenty;
    __m128i sixteen;
    __m128i fifthteen;
    __m128i thirteen;
    __m128i twelve;
    __m128i six;
    __m128i five;
    __m128i four;
    __m128i three;
    {
        alignas(16) uint32_t oneRaw[4] = {1, 0, 0, 0};
        alignas(16) uint32_t twentythreeRaw[4] = {23, 0, 0, 0};
        alignas(16) uint32_t twentyRaw[4] = {20, 0, 0, 0};
        alignas(16) uint32_t sixteenRaw[4] = {16, 0, 0, 0};
        alignas(16) uint32_t fifthteenRaw[4] = {15, 0, 0, 0};
        alignas(16) uint32_t thirteenRaw[4] = {13, 0, 0, 0};
        alignas(16) uint32_t twelveRaw[4] = {12, 0, 0, 0};
        alignas(16) uint32_t sixRaw[4] = {6, 0, 0, 0};
        alignas(16) uint32_t fiveRaw[4] = {5, 0, 0, 0};
        alignas(16) uint32_t fourRaw[4] = {4, 0, 0, 0};
        alignas(16) uint32_t threeRaw[4] = {3, 0, 0, 0};
        one = _mm_load_si128((const __m128i *) oneRaw);
        twentythree = _mm_load_si128((const __m128i *) twentythreeRaw);
        twenty = _mm_load_si128((const __m128i *) twentyRaw);
        sixteen = _mm_load_si128((const __m128i *) sixteenRaw);
        fifthteen = _mm_load_si128((const __m128i *) fifthteenRaw);
        thirteen = _mm_load_si128((const __m128i *) thirteenRaw);
        twelve = _mm_load_si128((const __m128i *) twelveRaw);
        six = _mm_load_si128((const __m128i *) sixRaw);
        five = _mm_load_si128((const __m128i *) fiveRaw);
        four = _mm_load_si128((const __m128i *) fourRaw);
        three = _mm_load_si128((const __m128i *) threeRaw);
    }
    __m512i bitConstant;
    __m512i statex4;
    __m512i lowbitsx4;
    __m512i maskx4;
    {
        alignas(64) uint32_t bitValuesRaw[16] = {
                ((((uint32_t) (keyLeastSignificant24Bit + 0)) & 0xFF) << 8) |
                (((uint32_t) ((keyLeastSignificant24Bit + 0) & 0xFF00)) >> 8),
                ((((uint32_t) (keyLeastSignificant24Bit + 1)) & 0xFF) << 8) |
                (((uint32_t) ((keyLeastSignificant24Bit + 1) & 0xFF00)) >> 8),
                ((((uint32_t) (keyLeastSignificant24Bit + 2)) & 0xFF) << 8) |
                (((uint32_t) ((keyLeastSignificant24Bit + 2) & 0xFF00)) >> 8),
                ((((uint32_t) (keyLeastSignificant24Bit + 3)) & 0xFF) << 8) |
                (((uint32_t) ((keyLeastSignificant24Bit + 3) & 0xFF00)) >> 8),
                ((((uint32_t) (keyLeastSignificant24Bit + 4)) & 0xFF) << 8) |
                (((uint32_t) ((keyLeastSignificant24Bit + 4) & 0xFF00)) >> 8),
                ((((uint32_t) (keyLeastSignificant24Bit + 5)) & 0xFF) << 8) |
                (((uint32_t) ((keyLeastSignificant24Bit + 5) & 0xFF00)) >> 8),
                ((((uint32_t) (keyLeastSignificant24Bit + 6)) & 0xFF) << 8) |
                (((uint32_t) ((keyLeastSignificant24Bit + 6) & 0xFF00)) >> 8),
                ((((uint32_t) (keyLeastSignificant24Bit + 7)) & 0xFF) << 8) |
                (((uint32_t) ((keyLeastSignificant24Bit + 7) & 0xFF00)) >> 8),

                ((((uint32_t) (keyLeastSignificant24Bit + 8)) & 0xFF) << 8) |
                (((uint32_t) ((keyLeastSignificant24Bit + 8) & 0xFF00)) >> 8),
                ((((uint32_t) (keyLeastSignificant24Bit + 9)) & 0xFF) << 8) |
                (((uint32_t) ((keyLeastSignificant24Bit + 9) & 0xFF00)) >> 8),
                ((((uint32_t) (keyLeastSignificant24Bit + 10)) & 0xFF) << 8) |
                (((uint32_t) ((keyLeastSignificant24Bit + 10) & 0xFF00)) >> 8),
                ((((uint32_t) (keyLeastSignificant24Bit + 11)) & 0xFF) << 8) |
                (((uint32_t) ((keyLeastSignificant24Bit + 11) & 0xFF00)) >> 8),
                ((((uint32_t) (keyLeastSignificant24Bit + 12)) & 0xFF) << 8) |
                (((uint32_t) ((keyLeastSignificant24Bit + 12) & 0xFF00)) >> 8),
                ((((uint32_t) (keyLeastSignificant24Bit + 13)) & 0xFF) << 8) |
                (((uint32_t) ((keyLeastSignificant24Bit + 13) & 0xFF00)) >> 8),
                ((((uint32_t) (keyLeastSignificant24Bit + 14)) & 0xFF) << 8) |
                (((uint32_t) ((keyLeastSignificant24Bit + 14) & 0xFF00)) >> 8),
                ((((uint32_t) (keyLeastSignificant24Bit + 15)) & 0xFF) << 8) |
                (((uint32_t) ((keyLeastSignificant24Bit + 15) & 0xFF00)) >> 8)};
        alignas(64) uint32_t statex4Raw[16] = {state, state, state, state, state, state, state, state, state, state, state, state, state, state, state, state};
        alignas(64) uint32_t lowbitsRaw[16] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
        alignas(64) uint32_t maskRaw[16] = {0x00EF6FD7, 0x00EF6FD7, 0x00EF6FD7, 0x00EF6FD7, 0x00EF6FD7, 0x00EF6FD7, 0x00EF6FD7, 0x00EF6FD7, 0x00EF6FD7, 0x00EF6FD7, 0x00EF6FD7, 0x00EF6FD7, 0x00EF6FD7, 0x00EF6FD7, 0x00EF6FD7, 0x00EF6FD7};
        bitConstant = _mm512_load_si512((__m512i const*) bitValuesRaw);
        statex4 = _mm512_load_si512((__m512i const*) statex4Raw);
        lowbitsx4 = _mm512_load_si512((__m512i const*) lowbitsRaw);
        maskx4 = _mm512_load_si512((__m512i const*) maskRaw);
    }
    for (int i = 0; i < 16; i++) {
        __m512i v3;
        __m512i v3_bit23;
        {
            v3_bit23 = _mm512_xor_si512(bitConstant, statex4);
            bitConstant = _mm512_srl_epi32(bitConstant, one);
            v3_bit23 = _mm512_and_si512(v3_bit23, lowbitsx4);
            v3 = _mm512_srl_epi32(statex4, one);
            v3 = _mm512_or_si512(_mm512_sll_epi32(v3_bit23, twentythree), v3);
        }
        __m512i v3_bit20 = _mm512_srl_epi32(v3, twenty);
        __m512i st_bit16 = _mm512_srl_epi32(statex4, sixteen); // b15
        __m512i st_bit13 = _mm512_srl_epi32(statex4, thirteen); // b13
        __m512i st_bit6 = _mm512_srl_epi32(statex4, six); // b5
        __m512i st_bit4 = _mm512_srl_epi32(statex4, four); // b3
        v3_bit20 = _mm512_and_si512(_mm512_xor_si512(v3_bit20, v3_bit23), lowbitsx4);
        st_bit16 = _mm512_and_si512(_mm512_xor_si512(st_bit16, v3_bit23), lowbitsx4);
        st_bit13 = _mm512_and_si512(_mm512_xor_si512(st_bit13, v3_bit23), lowbitsx4);
        st_bit6 = _mm512_and_si512(_mm512_xor_si512(st_bit6, v3_bit23), lowbitsx4);
        st_bit4 = _mm512_and_si512(_mm512_xor_si512(st_bit4, v3_bit23), lowbitsx4);

        v3_bit20 = _mm512_sll_epi32(v3_bit20, twenty);
        st_bit16 = _mm512_sll_epi32(st_bit16, fifthteen);
        st_bit13 = _mm512_sll_epi32(st_bit13, twelve);
        st_bit6 = _mm512_sll_epi32(st_bit6, five);
        st_bit4 = _mm512_sll_epi32(st_bit4, three);

        statex4 = _mm512_and_si512(v3, maskx4);
        statex4 = _mm512_or_si512(statex4, v3_bit20);
        statex4 = _mm512_or_si512(statex4, st_bit16);
        statex4 = _mm512_or_si512(statex4, st_bit13);
        statex4 = _mm512_or_si512(statex4, st_bit6);
        statex4 = _mm512_or_si512(statex4, st_bit4);
    }
    uint32_t vals[16];
    _mm512_store_si512(reinterpret_cast<__m512i*>(vals), statex4);
    result.value1 = vals[0];
    result.value2 = vals[1];
    result.value3 = vals[2];
    result.value4 = vals[3];
    result.value5 = vals[4];
    result.value6 = vals[5];
    result.value7 = vals[6];
    result.value8 = vals[7];
    result.value9 = vals[8];
    result.value10 = vals[9];
    result.value11 = vals[10];
    result.value12 = vals[11];
    result.value13 = vals[12];
    result.value14 = vals[13];
    result.value15 = vals[14];
    result.value16 = vals[15];
    result.value1 = ((result.value1 & 0xF0000) >> 16) | ((result.value1 & 0xF) << 4) | ((result.value1 & 0xF00000) >> 12) | (result.value1 & 0xF000) | ((result.value1 & 0xFF0) << 12);
    result.value2 = ((result.value2 & 0xF0000) >> 16) | ((result.value2 & 0xF) << 4) | ((result.value2 & 0xF00000) >> 12) | (result.value2 & 0xF000) | ((result.value2 & 0xFF0) << 12);
    result.value3 = ((result.value3 & 0xF0000) >> 16) | ((result.value3 & 0xF) << 4) | ((result.value3 & 0xF00000) >> 12) | (result.value3 & 0xF000) | ((result.value3 & 0xFF0) << 12);
    result.value4 = ((result.value4 & 0xF0000) >> 16) | ((result.value4 & 0xF) << 4) | ((result.value4 & 0xF00000) >> 12) | (result.value4 & 0xF000) | ((result.value4 & 0xFF0) << 12);
    result.value5 = ((result.value5 & 0xF0000) >> 16) | ((result.value5 & 0xF) << 4) | ((result.value5 & 0xF00000) >> 12) | (result.value5 & 0xF000) | ((result.value5 & 0xFF0) << 12);
    result.value6 = ((result.value6 & 0xF0000) >> 16) | ((result.value6 & 0xF) << 4) | ((result.value6 & 0xF00000) >> 12) | (result.value6 & 0xF000) | ((result.value6 & 0xFF0) << 12);
    result.value7 = ((result.value7 & 0xF0000) >> 16) | ((result.value7 & 0xF) << 4) | ((result.value7 & 0xF00000) >> 12) | (result.value7 & 0xF000) | ((result.value7 & 0xFF0) << 12);
    result.value8 = ((result.value8 & 0xF0000) >> 16) | ((result.value8 & 0xF) << 4) | ((result.value8 & 0xF00000) >> 12) | (result.value8 & 0xF000) | ((result.value8 & 0xFF0) << 12);
    result.value9 = ((result.value9 & 0xF0000) >> 16) | ((result.value9 & 0xF) << 4) | ((result.value9 & 0xF00000) >> 12) | (result.value9 & 0xF000) | ((result.value9 & 0xFF0) << 12);
    result.value10 = ((result.value10 & 0xF0000) >> 16) | ((result.value10 & 0xF) << 4) | ((result.value10 & 0xF00000) >> 12) | (result.value10 & 0xF000) | ((result.value10 & 0xFF0) << 12);
    result.value11 = ((result.value11 & 0xF0000) >> 16) | ((result.value11 & 0xF) << 4) | ((result.value11 & 0xF00000) >> 12) | (result.value11 & 0xF000) | ((result.value11 & 0xFF0) << 12);
    result.value12 = ((result.value12 & 0xF0000) >> 16) | ((result.value12 & 0xF) << 4) | ((result.value12 & 0xF00000) >> 12) | (result.value12 & 0xF000) | ((result.value12 & 0xFF0) << 12);
    result.value13 = ((result.value13 & 0xF0000) >> 16) | ((result.value13 & 0xF) << 4) | ((result.value13 & 0xF00000) >> 12) | (result.value13 & 0xF000) | ((result.value13 & 0xFF0) << 12);
    result.value14 = ((result.value14 & 0xF0000) >> 16) | ((result.value14 & 0xF) << 4) | ((result.value14 & 0xF00000) >> 12) | (result.value14 & 0xF000) | ((result.value14 & 0xFF0) << 12);
    result.value15 = ((result.value15 & 0xF0000) >> 16) | ((result.value15 & 0xF) << 4) | ((result.value15 & 0xF00000) >> 12) | (result.value15 & 0xF000) | ((result.value15 & 0xFF0) << 12);
    result.value16 = ((result.value16 & 0xF0000) >> 16) | ((result.value16 & 0xF) << 4) | ((result.value16 & 0xF00000) >> 12) | (result.value16 & 0xF000) | ((result.value16 & 0xFF0) << 12);
#ifdef CHECK_PASS3BX16
    if (result.value1 != expected.value1 || result.value2 != expected.value2 || result.value3 != expected.value3 || result.value4 != expected.value4 || result.value5 != expected.value5 || result.value6 != expected.value6 || result.value7 != expected.value7 || result.value8 != expected.value8) {
        std::terminate();
    }
    if (result.value9 != expected.value9 || result.value10 != expected.value10 || result.value11 != expected.value11 || result.value12 != expected.value12 || result.value13 != expected.value13 || result.value14 != expected.value14 || result.value15 != expected.value15 || result.value16 != expected.value16) {
        std::terminate();
    }
#endif
    return result;
}
#else
inline ValueX16 SeedKeyPass3bx16(uint32_t state, uint32_t keyLeastSignificant24Bit) {
    return {SeedKeyPass3bx8(state, keyLeastSignificant24Bit), SeedKeyPass3bx8(state, keyLeastSignificant24Bit + 8)};
}
#endif

#ifdef USE_AVX
ValueX8 SeedKeyPass3bx8(uint32_t state, uint32_t keyLeastSignificant24Bit) {
    ValueX8 result;
#ifdef CHECK_PASS3BX8
    ValueX8 expected;
    expected.value1 = SeedKeyPass3b(state, keyLeastSignificant24Bit);
    expected.value2 = SeedKeyPass3b(state, keyLeastSignificant24Bit + 1);
    expected.value3 = SeedKeyPass3b(state, keyLeastSignificant24Bit + 2);
    expected.value4 = SeedKeyPass3b(state, keyLeastSignificant24Bit + 3);
    expected.value5 = SeedKeyPass3b(state, keyLeastSignificant24Bit + 4);
    expected.value6 = SeedKeyPass3b(state, keyLeastSignificant24Bit + 5);
    expected.value7 = SeedKeyPass3b(state, keyLeastSignificant24Bit + 6);
    expected.value8 = SeedKeyPass3b(state, keyLeastSignificant24Bit + 7);
#endif
    __m256i bitConstant;
    __m256i statex4;
    __m256i lowbitsx4;
    __m256i maskx4;
    {
        alignas(32) uint32_t bitValuesRaw[8] = {
                ((((uint32_t) (keyLeastSignificant24Bit + 0)) & 0xFF) << 8) |
                (((uint32_t) ((keyLeastSignificant24Bit + 0) & 0xFF00)) >> 8),
                ((((uint32_t) (keyLeastSignificant24Bit + 1)) & 0xFF) << 8) |
                (((uint32_t) ((keyLeastSignificant24Bit + 1) & 0xFF00)) >> 8),
                ((((uint32_t) (keyLeastSignificant24Bit + 2)) & 0xFF) << 8) |
                (((uint32_t) ((keyLeastSignificant24Bit + 2) & 0xFF00)) >> 8),
                ((((uint32_t) (keyLeastSignificant24Bit + 3)) & 0xFF) << 8) |
                (((uint32_t) ((keyLeastSignificant24Bit + 3) & 0xFF00)) >> 8),
                ((((uint32_t) (keyLeastSignificant24Bit + 4)) & 0xFF) << 8) |
                (((uint32_t) ((keyLeastSignificant24Bit + 4) & 0xFF00)) >> 8),
                ((((uint32_t) (keyLeastSignificant24Bit + 5)) & 0xFF) << 8) |
                (((uint32_t) ((keyLeastSignificant24Bit + 5) & 0xFF00)) >> 8),
                ((((uint32_t) (keyLeastSignificant24Bit + 6)) & 0xFF) << 8) |
                (((uint32_t) ((keyLeastSignificant24Bit + 6) & 0xFF00)) >> 8),
                ((((uint32_t) (keyLeastSignificant24Bit + 7)) & 0xFF) << 8) |
                (((uint32_t) ((keyLeastSignificant24Bit + 7) & 0xFF00)) >> 8)};
        alignas(32) uint32_t statex4Raw[8] = {state, state, state, state, state, state, state, state};
        alignas(32) uint32_t lowbitsRaw[8] = {1, 1, 1, 1, 1, 1, 1, 1};
        alignas(32) uint32_t maskRaw[8] = {0x00EF6FD7, 0x00EF6FD7, 0x00EF6FD7, 0x00EF6FD7, 0x00EF6FD7, 0x00EF6FD7, 0x00EF6FD7, 0x00EF6FD7};
        bitConstant = _mm256_load_si256((__m256i const*) bitValuesRaw);
        statex4 = _mm256_load_si256((__m256i const*) statex4Raw);
        lowbitsx4 = _mm256_load_si256((__m256i const*) lowbitsRaw);
        maskx4 = _mm256_load_si256((__m256i const*) maskRaw);
    }
    for (int i = 0; i < 16; i++) {
        __m256i v3;
        __m256i v3_bit23;
        {
            v3_bit23 = _mm256_xor_si256(bitConstant, statex4);
            bitConstant = _mm256_srli_epi32(bitConstant, 1);
            v3_bit23 = _mm256_and_si256(v3_bit23, lowbitsx4);
            v3 = _mm256_srli_epi32(statex4, 1);
            v3 = _mm256_or_si256(_mm256_slli_epi32(v3_bit23, 23), v3);
        }
        __m256i v3_bit20 = _mm256_srli_epi32(v3, 20);
        __m256i st_bit16 = _mm256_srli_epi32(statex4, 16); // b15
        __m256i st_bit13 = _mm256_srli_epi32(statex4, 13); // b13
        __m256i st_bit6 = _mm256_srli_epi32(statex4, 6); // b5
        __m256i st_bit4 = _mm256_srli_epi32(statex4, 4); // b3
        v3_bit20 = _mm256_and_si256(_mm256_xor_si256(v3_bit20, v3_bit23), lowbitsx4);
        st_bit16 = _mm256_and_si256(_mm256_xor_si256(st_bit16, v3_bit23), lowbitsx4);
        st_bit13 = _mm256_and_si256(_mm256_xor_si256(st_bit13, v3_bit23), lowbitsx4);
        st_bit6 = _mm256_and_si256(_mm256_xor_si256(st_bit6, v3_bit23), lowbitsx4);
        st_bit4 = _mm256_and_si256(_mm256_xor_si256(st_bit4, v3_bit23), lowbitsx4);

        v3_bit20 = _mm256_slli_epi32(v3_bit20, 20);
        st_bit16 = _mm256_slli_epi32(st_bit16, 15);
        st_bit13 = _mm256_slli_epi32(st_bit13, 12);
        st_bit6 = _mm256_slli_epi32(st_bit6, 5);
        st_bit4 = _mm256_slli_epi32(st_bit4, 3);

        statex4 = _mm256_and_si256(v3, maskx4);
        statex4 = _mm256_or_si256(statex4, v3_bit20);
        statex4 = _mm256_or_si256(statex4, st_bit16);
        statex4 = _mm256_or_si256(statex4, st_bit13);
        statex4 = _mm256_or_si256(statex4, st_bit6);
        statex4 = _mm256_or_si256(statex4, st_bit4);
    }
    uint32_t vals[8];
    _mm256_store_si256(reinterpret_cast<__m256i*>(vals), statex4);
    result.value1 = vals[0];
    result.value2 = vals[1];
    result.value3 = vals[2];
    result.value4 = vals[3];
    result.value5 = vals[4];
    result.value6 = vals[5];
    result.value7 = vals[6];
    result.value8 = vals[7];
    result.value1 = ((result.value1 & 0xF0000) >> 16) | ((result.value1 & 0xF) << 4) | ((result.value1 & 0xF00000) >> 12) | (result.value1 & 0xF000) | ((result.value1 & 0xFF0) << 12);
    result.value2 = ((result.value2 & 0xF0000) >> 16) | ((result.value2 & 0xF) << 4) | ((result.value2 & 0xF00000) >> 12) | (result.value2 & 0xF000) | ((result.value2 & 0xFF0) << 12);
    result.value3 = ((result.value3 & 0xF0000) >> 16) | ((result.value3 & 0xF) << 4) | ((result.value3 & 0xF00000) >> 12) | (result.value3 & 0xF000) | ((result.value3 & 0xFF0) << 12);
    result.value4 = ((result.value4 & 0xF0000) >> 16) | ((result.value4 & 0xF) << 4) | ((result.value4 & 0xF00000) >> 12) | (result.value4 & 0xF000) | ((result.value4 & 0xFF0) << 12);
    result.value5 = ((result.value5 & 0xF0000) >> 16) | ((result.value5 & 0xF) << 4) | ((result.value5 & 0xF00000) >> 12) | (result.value5 & 0xF000) | ((result.value5 & 0xFF0) << 12);
    result.value6 = ((result.value6 & 0xF0000) >> 16) | ((result.value6 & 0xF) << 4) | ((result.value6 & 0xF00000) >> 12) | (result.value6 & 0xF000) | ((result.value6 & 0xFF0) << 12);
    result.value7 = ((result.value7 & 0xF0000) >> 16) | ((result.value7 & 0xF) << 4) | ((result.value7 & 0xF00000) >> 12) | (result.value7 & 0xF000) | ((result.value7 & 0xFF0) << 12);
    result.value8 = ((result.value8 & 0xF0000) >> 16) | ((result.value8 & 0xF) << 4) | ((result.value8 & 0xF00000) >> 12) | (result.value8 & 0xF000) | ((result.value8 & 0xFF0) << 12);
#ifdef CHECK_PASS3BX8
    if (result.value1 != expected.value1 || result.value2 != expected.value2 || result.value3 != expected.value3 || result.value4 != expected.value4 || result.value5 != expected.value5 || result.value6 != expected.value6 || result.value7 != expected.value7 || result.value8 != expected.value8) {
        std::terminate();
    }
#endif
    return result;
}
#else
ValueX8 SeedKeyPass3bx8(uint32_t state, uint32_t keyLeastSignificant24Bit) {
    return {SeedKeyPass3bx4(state, keyLeastSignificant24Bit), SeedKeyPass3bx4(state, keyLeastSignificant24Bit + 4)};
}
#endif

#ifdef USE_AVX
ValueX8 SeedKeyPass3bx8(ValueX8 seed, uint32_t keyLeastSignificant24Bit) {
    ValueX8 result;
#ifdef CHECK_PASS3BX8
    ValueX8 expected;
    expected.value1 = SeedKeyPass3b(seed.value1, keyLeastSignificant24Bit);
    expected.value2 = SeedKeyPass3b(seed.value2, keyLeastSignificant24Bit + 1);
    expected.value3 = SeedKeyPass3b(seed.value3, keyLeastSignificant24Bit + 2);
    expected.value4 = SeedKeyPass3b(seed.value4, keyLeastSignificant24Bit + 3);
    expected.value5 = SeedKeyPass3b(seed.value5, keyLeastSignificant24Bit + 4);
    expected.value6 = SeedKeyPass3b(seed.value6, keyLeastSignificant24Bit + 5);
    expected.value7 = SeedKeyPass3b(seed.value7, keyLeastSignificant24Bit + 6);
    expected.value8 = SeedKeyPass3b(seed.value8, keyLeastSignificant24Bit + 7);
#endif
    __m256i bitConstant;
    __m256i statex4;
    __m256i lowbitsx4;
    __m256i maskx4;
    {
        alignas(32) uint32_t bitValuesRaw[8] = {
                ((((uint32_t) (keyLeastSignificant24Bit + 0)) & 0xFF) << 8) |
                (((uint32_t) ((keyLeastSignificant24Bit + 0) & 0xFF00)) >> 8),
                ((((uint32_t) (keyLeastSignificant24Bit + 1)) & 0xFF) << 8) |
                (((uint32_t) ((keyLeastSignificant24Bit + 1) & 0xFF00)) >> 8),
                ((((uint32_t) (keyLeastSignificant24Bit + 2)) & 0xFF) << 8) |
                (((uint32_t) ((keyLeastSignificant24Bit + 2) & 0xFF00)) >> 8),
                ((((uint32_t) (keyLeastSignificant24Bit + 3)) & 0xFF) << 8) |
                (((uint32_t) ((keyLeastSignificant24Bit + 3) & 0xFF00)) >> 8),
                ((((uint32_t) (keyLeastSignificant24Bit + 4)) & 0xFF) << 8) |
                (((uint32_t) ((keyLeastSignificant24Bit + 4) & 0xFF00)) >> 8),
                ((((uint32_t) (keyLeastSignificant24Bit + 5)) & 0xFF) << 8) |
                (((uint32_t) ((keyLeastSignificant24Bit + 5) & 0xFF00)) >> 8),
                ((((uint32_t) (keyLeastSignificant24Bit + 6)) & 0xFF) << 8) |
                (((uint32_t) ((keyLeastSignificant24Bit + 6) & 0xFF00)) >> 8),
                ((((uint32_t) (keyLeastSignificant24Bit + 7)) & 0xFF) << 8) |
                (((uint32_t) ((keyLeastSignificant24Bit + 7) & 0xFF00)) >> 8)};
        alignas(32) uint32_t statex4Raw[8] = {seed.value1, seed.value2, seed.value3, seed.value4, seed.value5, seed.value6, seed.value7, seed.value8};
        alignas(32) uint32_t lowbitsRaw[8] = {1, 1, 1, 1, 1, 1, 1, 1};
        alignas(32) uint32_t maskRaw[8] = {0x00EF6FD7, 0x00EF6FD7, 0x00EF6FD7, 0x00EF6FD7, 0x00EF6FD7, 0x00EF6FD7, 0x00EF6FD7, 0x00EF6FD7};
        bitConstant = _mm256_load_si256((__m256i const*) bitValuesRaw);
        statex4 = _mm256_load_si256((__m256i const*) statex4Raw);
        lowbitsx4 = _mm256_load_si256((__m256i const*) lowbitsRaw);
        maskx4 = _mm256_load_si256((__m256i const*) maskRaw);
    }
    for (int i = 0; i < 16; i++) {
        __m256i v3;
        __m256i v3_bit23;
        {
            v3_bit23 = _mm256_xor_si256(bitConstant, statex4);
            bitConstant = _mm256_srli_epi32(bitConstant, 1);
            v3_bit23 = _mm256_and_si256(v3_bit23, lowbitsx4);
            v3 = _mm256_srli_epi32(statex4, 1);
            v3 = _mm256_or_si256(_mm256_slli_epi32(v3_bit23, 23), v3);
        }
        __m256i v3_bit20 = _mm256_srli_epi32(v3, 20);
        __m256i st_bit16 = _mm256_srli_epi32(statex4, 16); // b15
        __m256i st_bit13 = _mm256_srli_epi32(statex4, 13); // b13
        __m256i st_bit6 = _mm256_srli_epi32(statex4, 6); // b5
        __m256i st_bit4 = _mm256_srli_epi32(statex4, 4); // b3
        v3_bit20 = _mm256_and_si256(_mm256_xor_si256(v3_bit20, v3_bit23), lowbitsx4);
        st_bit16 = _mm256_and_si256(_mm256_xor_si256(st_bit16, v3_bit23), lowbitsx4);
        st_bit13 = _mm256_and_si256(_mm256_xor_si256(st_bit13, v3_bit23), lowbitsx4);
        st_bit6 = _mm256_and_si256(_mm256_xor_si256(st_bit6, v3_bit23), lowbitsx4);
        st_bit4 = _mm256_and_si256(_mm256_xor_si256(st_bit4, v3_bit23), lowbitsx4);

        v3_bit20 = _mm256_slli_epi32(v3_bit20, 20);
        st_bit16 = _mm256_slli_epi32(st_bit16, 15);
        st_bit13 = _mm256_slli_epi32(st_bit13, 12);
        st_bit6 = _mm256_slli_epi32(st_bit6, 5);
        st_bit4 = _mm256_slli_epi32(st_bit4, 3);

        statex4 = _mm256_and_si256(v3, maskx4);
        statex4 = _mm256_or_si256(statex4, v3_bit20);
        statex4 = _mm256_or_si256(statex4, st_bit16);
        statex4 = _mm256_or_si256(statex4, st_bit13);
        statex4 = _mm256_or_si256(statex4, st_bit6);
        statex4 = _mm256_or_si256(statex4, st_bit4);
    }
    uint32_t vals[8];
    _mm256_store_si256(reinterpret_cast<__m256i*>(vals), statex4);
    result.value1 = vals[0];
    result.value2 = vals[1];
    result.value3 = vals[2];
    result.value4 = vals[3];
    result.value5 = vals[4];
    result.value6 = vals[5];
    result.value7 = vals[6];
    result.value8 = vals[7];
    result.value1 = ((result.value1 & 0xF0000) >> 16) | ((result.value1 & 0xF) << 4) | ((result.value1 & 0xF00000) >> 12) | (result.value1 & 0xF000) | ((result.value1 & 0xFF0) << 12);
    result.value2 = ((result.value2 & 0xF0000) >> 16) | ((result.value2 & 0xF) << 4) | ((result.value2 & 0xF00000) >> 12) | (result.value2 & 0xF000) | ((result.value2 & 0xFF0) << 12);
    result.value3 = ((result.value3 & 0xF0000) >> 16) | ((result.value3 & 0xF) << 4) | ((result.value3 & 0xF00000) >> 12) | (result.value3 & 0xF000) | ((result.value3 & 0xFF0) << 12);
    result.value4 = ((result.value4 & 0xF0000) >> 16) | ((result.value4 & 0xF) << 4) | ((result.value4 & 0xF00000) >> 12) | (result.value4 & 0xF000) | ((result.value4 & 0xFF0) << 12);
    result.value5 = ((result.value5 & 0xF0000) >> 16) | ((result.value5 & 0xF) << 4) | ((result.value5 & 0xF00000) >> 12) | (result.value5 & 0xF000) | ((result.value5 & 0xFF0) << 12);
    result.value6 = ((result.value6 & 0xF0000) >> 16) | ((result.value6 & 0xF) << 4) | ((result.value6 & 0xF00000) >> 12) | (result.value6 & 0xF000) | ((result.value6 & 0xFF0) << 12);
    result.value7 = ((result.value7 & 0xF0000) >> 16) | ((result.value7 & 0xF) << 4) | ((result.value7 & 0xF00000) >> 12) | (result.value7 & 0xF000) | ((result.value7 & 0xFF0) << 12);
    result.value8 = ((result.value8 & 0xF0000) >> 16) | ((result.value8 & 0xF) << 4) | ((result.value8 & 0xF00000) >> 12) | (result.value8 & 0xF000) | ((result.value8 & 0xFF0) << 12);
#ifdef CHECK_PASS3BX8
    if (result.value1 != expected.value1 || result.value2 != expected.value2 || result.value3 != expected.value3 || result.value4 != expected.value4 || result.value5 != expected.value5 || result.value6 != expected.value6 || result.value7 != expected.value7 || result.value8 != expected.value8) {
        std::terminate();
    }
#endif
    return result;
}
#else
ValueX8 SeedKeyPass3bx8(ValueX8 state, uint32_t keyLeastSignificant24Bit) {
    return {SeedKeyPass3bx4(state.First(), keyLeastSignificant24Bit), SeedKeyPass3bx4(state.Second(), keyLeastSignificant24Bit + 4)};
}
#endif

#ifndef USE_SSE2
#ifndef CHECK_PASS3BX4
constexpr
#endif
#endif
ValueX4 SeedKeyPass3bx4(uint32_t state, uint32_t keyLeastSignificant24Bit) {
    ValueX4 result;
#ifdef CHECK_PASS3BX4
    ValueX4 expected;
    expected.value1 = SeedKeyPass3b(state, keyLeastSignificant24Bit);
    expected.value2 = SeedKeyPass3b(state, keyLeastSignificant24Bit + 1);
    expected.value3 = SeedKeyPass3b(state, keyLeastSignificant24Bit + 2);
    expected.value4 = SeedKeyPass3b(state, keyLeastSignificant24Bit + 3);
#endif
#ifdef USE_SSE2
    __m128i bitConstant;
    __m128i statex4;
    __m128i lowbitsx4;
    __m128i maskx4;
    {
        uint32_t bitValuesRaw[4] = {
                ((((uint32_t) (keyLeastSignificant24Bit + 0)) & 0xFF) << 8) |
                (((uint32_t) ((keyLeastSignificant24Bit + 0) & 0xFF00)) >> 8),
                ((((uint32_t) (keyLeastSignificant24Bit + 1)) & 0xFF) << 8) |
                (((uint32_t) ((keyLeastSignificant24Bit + 1) & 0xFF00)) >> 8),
                ((((uint32_t) (keyLeastSignificant24Bit + 2)) & 0xFF) << 8) |
                (((uint32_t) ((keyLeastSignificant24Bit + 2) & 0xFF00)) >> 8),
                ((((uint32_t) (keyLeastSignificant24Bit + 3)) & 0xFF) << 8) |
                (((uint32_t) ((keyLeastSignificant24Bit + 3) & 0xFF00)) >> 8)};
        uint32_t statex4Raw[4] = {state, state, state, state};
        uint32_t lowbitsRaw[4] = {1, 1, 1, 1};
        uint32_t maskRaw[4] = {0x00EF6FD7, 0x00EF6FD7, 0x00EF6FD7, 0x00EF6FD7};
        bitConstant = _mm_load_si128((__m128i const*) bitValuesRaw);
        statex4 = _mm_load_si128((__m128i const*) statex4Raw);
        lowbitsx4 = _mm_load_si128((__m128i const*) lowbitsRaw);
        maskx4 = _mm_load_si128((__m128i const*) maskRaw);
    }
#else
    uint64_t bitConstant = ((((uint64_t) keyLeastSignificant24Bit) & 0xFF) << 8) | (((uint64_t) (keyLeastSignificant24Bit & 0xFF00)) >> 8) |
            (((uint64_t) ((keyLeastSignificant24Bit + 1) & 0xFF)) << 24) | (((uint64_t) ((keyLeastSignificant24Bit + 1) & 0xFF00)) << 8) |
            (((uint64_t) ((keyLeastSignificant24Bit + 2) & 0xFF)) << 40) | (((uint64_t) ((keyLeastSignificant24Bit + 2) & 0xFF00)) << 24) |
            (((uint64_t) ((keyLeastSignificant24Bit + 3) & 0xFF)) << 56) | (((uint64_t) ((keyLeastSignificant24Bit + 3) & 0xFF00)) << 40);
    uint64_t statex2_1 = ((uint64_t) state) | (((uint64_t) state) << 32);
    uint64_t statex2_2 = ((uint64_t) state) | (((uint64_t) state) << 32);
#endif
    for (int i = 0; i < 16; i++) {
#ifdef USE_SSE2
        __m128i v3;
        __m128i v3_bit23;
        {
            v3_bit23 = _mm_xor_si128(bitConstant, statex4);
            bitConstant = _mm_srli_epi32(bitConstant, 1);
            v3_bit23 = _mm_and_si128(v3_bit23, lowbitsx4);
            v3 = _mm_srli_epi32(statex4, 1);
            v3 = _mm_or_si128(_mm_slli_epi32(v3_bit23, 23), v3);
        }
        __m128i v3_bit20 = _mm_srli_epi32(v3, 20);
        __m128i st_bit16 = _mm_srli_epi32(statex4, 16); // b15
        __m128i st_bit13 = _mm_srli_epi32(statex4, 13); // b13
        __m128i st_bit6 = _mm_srli_epi32(statex4, 6); // b5
        __m128i st_bit4 = _mm_srli_epi32(statex4, 4); // b3
        v3_bit20 = _mm_and_si128(_mm_xor_si128(v3_bit20, v3_bit23), lowbitsx4);
        st_bit16 = _mm_and_si128(_mm_xor_si128(st_bit16, v3_bit23), lowbitsx4);
        st_bit13 = _mm_and_si128(_mm_xor_si128(st_bit13, v3_bit23), lowbitsx4);
        st_bit6 = _mm_and_si128(_mm_xor_si128(st_bit6, v3_bit23), lowbitsx4);
        st_bit4 = _mm_and_si128(_mm_xor_si128(st_bit4, v3_bit23), lowbitsx4);

        v3_bit20 = _mm_slli_epi32(v3_bit20, 20);
        st_bit16 = _mm_slli_epi32(st_bit16, 15);
        st_bit13 = _mm_slli_epi32(st_bit13, 12);
        st_bit6 = _mm_slli_epi32(st_bit6, 5);
        st_bit4 = _mm_slli_epi32(st_bit4, 3);

        statex4 = _mm_and_si128(v3, maskx4);
        statex4 = _mm_or_si128(statex4, v3_bit20);
        statex4 = _mm_or_si128(statex4, st_bit16);
        statex4 = _mm_or_si128(statex4, st_bit13);
        statex4 = _mm_or_si128(statex4, st_bit6);
        statex4 = _mm_or_si128(statex4, st_bit4);
#else
        uint64_t stateLowBitX4 = (statex2_1 & 0x0000000100000001ull) | ((statex2_2 & 0x0000000100000001ull) << 16);
        auto bitsx4 = ((bitConstant >> i) & 0x0001000100010001ull ^ stateLowBitX4);
        auto bitsx2_1 = (bitsx4 & 0x0000000100000001ull) << 23;
        auto bitsx2_2 = (bitsx4 & 0x0001000000010000ull) << 7;
        auto v3_1 = bitsx2_1 | ((statex2_1 >> 1) & 0x7FFFFFFF7FFFFFFFull);
        auto v3_2 = bitsx2_2 | ((statex2_2 >> 1) & 0x7FFFFFFF7FFFFFFFull);
        statex2_1 = v3_1 & 0x00EF6FD700EF6FD7ull | ((((v3_1 & 0x0010000000100000ull) >> 20) ^ ((v3_1 & 0x0080000000800000ull) >> 23)) << 20) | (((((statex2_1 >> 1) & 0x0000800000008000ull) >> 15) ^ ((v3_1 & 0x0080000000800000ull) >> 23)) << 15) | (((((statex2_1 >> 1) & 0x0000100000001000ull) >> 12) ^ ((v3_1 & 0x0080000000800000ull) >> 23)) << 12) | 32 * ((((statex2_1 >> 1) & 0x0000002000000020ull) >> 5) ^ ((v3_1 & 0x0080000000800000ull) >> 23)) | (((((statex2_1 >> 1) & 0x0000000800000008ull) >> 3) ^ ((v3_1 & 0x0080000000800000ull) >> 23)) << 3);
        statex2_2 = v3_2 & 0x00EF6FD700EF6FD7ull | ((((v3_2 & 0x0010000000100000ull) >> 20) ^ ((v3_2 & 0x0080000000800000ull) >> 23)) << 20) | (((((statex2_2 >> 1) & 0x0000800000008000ull) >> 15) ^ ((v3_2 & 0x0080000000800000ull) >> 23)) << 15) | (((((statex2_2 >> 1) & 0x0000100000001000ull) >> 12) ^ ((v3_2 & 0x0080000000800000ull) >> 23)) << 12) | 32 * ((((statex2_2 >> 1) & 0x0000002000000020ull) >> 5) ^ ((v3_2 & 0x0080000000800000ull) >> 23)) | (((((statex2_2 >> 1) & 0x0000000800000008ull) >> 3) ^ ((v3_2 & 0x0080000000800000ull) >> 23)) << 3);
#endif
    }
#ifdef USE_SSE2
    uint32_t vals[4];
    _mm_store_si128(reinterpret_cast<__m128i*>(vals), statex4);
    result.value1 = vals[0];
    result.value2 = vals[1];
    result.value3 = vals[2];
    result.value4 = vals[3];
#else
    result.value1 = statex2_1 & 0xFFFFFFFFull;
    result.value2 = statex2_2 & 0xFFFFFFFFull;
    result.value3 = statex2_1 >> 32;
    result.value4 = statex2_2 >> 32;
#endif
    result.value1 = ((result.value1 & 0xF0000) >> 16) | ((result.value1 & 0xF) << 4) | ((result.value1 & 0xF00000) >> 12) | (result.value1 & 0xF000) | ((result.value1 & 0xFF0) << 12);
    result.value2 = ((result.value2 & 0xF0000) >> 16) | ((result.value2 & 0xF) << 4) | ((result.value2 & 0xF00000) >> 12) | (result.value2 & 0xF000) | ((result.value2 & 0xFF0) << 12);
    result.value3 = ((result.value3 & 0xF0000) >> 16) | ((result.value3 & 0xF) << 4) | ((result.value3 & 0xF00000) >> 12) | (result.value3 & 0xF000) | ((result.value3 & 0xFF0) << 12);
    result.value4 = ((result.value4 & 0xF0000) >> 16) | ((result.value4 & 0xF) << 4) | ((result.value4 & 0xF00000) >> 12) | (result.value4 & 0xF000) | ((result.value4 & 0xFF0) << 12);
#ifdef CHECK_PASS3BX4
    if (result.value1 != expected.value1 || result.value2 != expected.value2 || result.value3 != expected.value3 || result.value4 != expected.value4) {
        std::terminate();
    }
#endif
    return result;
}

#ifndef USE_SSE2
#ifndef CHECK_PASS3BX4
constexpr
#endif
#endif
ValueX4 SeedKeyPass3bx4(ValueX4 seed, uint32_t keyLeastSignificant24Bit) {
    ValueX4 result;
#ifdef CHECK_PASS3BX4
    ValueX4 expected;
    expected.value1 = SeedKeyPass3b(seed.value1, keyLeastSignificant24Bit);
    expected.value2 = SeedKeyPass3b(seed.value2, keyLeastSignificant24Bit + 1);
    expected.value3 = SeedKeyPass3b(seed.value3, keyLeastSignificant24Bit + 2);
    expected.value4 = SeedKeyPass3b(seed.value4, keyLeastSignificant24Bit + 3);
#endif
#ifdef USE_SSE2
    __m128i bitConstant;
    __m128i statex4;
    __m128i lowbitsx4;
    __m128i maskx4;
    {
        uint32_t bitValuesRaw[4] = {
                ((((uint32_t) (keyLeastSignificant24Bit + 0)) & 0xFF) << 8) |
                (((uint32_t) ((keyLeastSignificant24Bit + 0) & 0xFF00)) >> 8),
                ((((uint32_t) (keyLeastSignificant24Bit + 1)) & 0xFF) << 8) |
                (((uint32_t) ((keyLeastSignificant24Bit + 1) & 0xFF00)) >> 8),
                ((((uint32_t) (keyLeastSignificant24Bit + 2)) & 0xFF) << 8) |
                (((uint32_t) ((keyLeastSignificant24Bit + 2) & 0xFF00)) >> 8),
                ((((uint32_t) (keyLeastSignificant24Bit + 3)) & 0xFF) << 8) |
                (((uint32_t) ((keyLeastSignificant24Bit + 3) & 0xFF00)) >> 8)};
        uint32_t statex4Raw[4] = {seed.value1, seed.value2, seed.value3, seed.value4};
        uint32_t lowbitsRaw[4] = {1, 1, 1, 1};
        uint32_t maskRaw[4] = {0x00EF6FD7, 0x00EF6FD7, 0x00EF6FD7, 0x00EF6FD7};
        bitConstant = _mm_load_si128((__m128i const*) bitValuesRaw);
        statex4 = _mm_load_si128((__m128i const*) statex4Raw);
        lowbitsx4 = _mm_load_si128((__m128i const*) lowbitsRaw);
        maskx4 = _mm_load_si128((__m128i const*) maskRaw);
    }
#else
    uint64_t bitConstant = ((((uint64_t) keyLeastSignificant24Bit) & 0xFF) << 8) | (((uint64_t) (keyLeastSignificant24Bit & 0xFF00)) >> 8) |
            (((uint64_t) ((keyLeastSignificant24Bit + 1) & 0xFF)) << 24) | (((uint64_t) ((keyLeastSignificant24Bit + 1) & 0xFF00)) << 8) |
            (((uint64_t) ((keyLeastSignificant24Bit + 2) & 0xFF)) << 40) | (((uint64_t) ((keyLeastSignificant24Bit + 2) & 0xFF00)) << 24) |
            (((uint64_t) ((keyLeastSignificant24Bit + 3) & 0xFF)) << 56) | (((uint64_t) ((keyLeastSignificant24Bit + 3) & 0xFF00)) << 40);
    uint64_t statex2_1 = ((uint64_t) seed.value1) | (((uint64_t) seed.value3) << 32);
    uint64_t statex2_2 = ((uint64_t) seed.value2) | (((uint64_t) seed.value4) << 32);
#endif
    for (int i = 0; i < 16; i++) {
#ifdef USE_SSE2
        __m128i v3;
        __m128i v3_bit23;
        {
            v3_bit23 = _mm_xor_si128(bitConstant, statex4);
            bitConstant = _mm_srli_epi32(bitConstant, 1);
            v3_bit23 = _mm_and_si128(v3_bit23, lowbitsx4);
            v3 = _mm_srli_epi32(statex4, 1);
            v3 = _mm_or_si128(_mm_slli_epi32(v3_bit23, 23), v3);
        }
        __m128i v3_bit20 = _mm_srli_epi32(v3, 20);
        __m128i st_bit16 = _mm_srli_epi32(statex4, 16); // b15
        __m128i st_bit13 = _mm_srli_epi32(statex4, 13); // b13
        __m128i st_bit6 = _mm_srli_epi32(statex4, 6); // b5
        __m128i st_bit4 = _mm_srli_epi32(statex4, 4); // b3
        v3_bit20 = _mm_and_si128(_mm_xor_si128(v3_bit20, v3_bit23), lowbitsx4);
        st_bit16 = _mm_and_si128(_mm_xor_si128(st_bit16, v3_bit23), lowbitsx4);
        st_bit13 = _mm_and_si128(_mm_xor_si128(st_bit13, v3_bit23), lowbitsx4);
        st_bit6 = _mm_and_si128(_mm_xor_si128(st_bit6, v3_bit23), lowbitsx4);
        st_bit4 = _mm_and_si128(_mm_xor_si128(st_bit4, v3_bit23), lowbitsx4);

        v3_bit20 = _mm_slli_epi32(v3_bit20, 20);
        st_bit16 = _mm_slli_epi32(st_bit16, 15);
        st_bit13 = _mm_slli_epi32(st_bit13, 12);
        st_bit6 = _mm_slli_epi32(st_bit6, 5);
        st_bit4 = _mm_slli_epi32(st_bit4, 3);

        statex4 = _mm_and_si128(v3, maskx4);
        statex4 = _mm_or_si128(statex4, v3_bit20);
        statex4 = _mm_or_si128(statex4, st_bit16);
        statex4 = _mm_or_si128(statex4, st_bit13);
        statex4 = _mm_or_si128(statex4, st_bit6);
        statex4 = _mm_or_si128(statex4, st_bit4);
#else
        uint64_t stateLowBitX4 = (statex2_1 & 0x0000000100000001ull) | ((statex2_2 & 0x0000000100000001ull) << 16);
        auto bitsx4 = ((bitConstant >> i) & 0x0001000100010001ull ^ stateLowBitX4);
        auto bitsx2_1 = (bitsx4 & 0x0000000100000001ull) << 23;
        auto bitsx2_2 = (bitsx4 & 0x0001000000010000ull) << 7;
        auto v3_1 = bitsx2_1 | ((statex2_1 >> 1) & 0x7FFFFFFF7FFFFFFFull);
        auto v3_2 = bitsx2_2 | ((statex2_2 >> 1) & 0x7FFFFFFF7FFFFFFFull);
        statex2_1 = v3_1 & 0x00EF6FD700EF6FD7ull | ((((v3_1 & 0x0010000000100000ull) >> 20) ^ ((v3_1 & 0x0080000000800000ull) >> 23)) << 20) | (((((statex2_1 >> 1) & 0x0000800000008000ull) >> 15) ^ ((v3_1 & 0x0080000000800000ull) >> 23)) << 15) | (((((statex2_1 >> 1) & 0x0000100000001000ull) >> 12) ^ ((v3_1 & 0x0080000000800000ull) >> 23)) << 12) | 32 * ((((statex2_1 >> 1) & 0x0000002000000020ull) >> 5) ^ ((v3_1 & 0x0080000000800000ull) >> 23)) | (((((statex2_1 >> 1) & 0x0000000800000008ull) >> 3) ^ ((v3_1 & 0x0080000000800000ull) >> 23)) << 3);
        statex2_2 = v3_2 & 0x00EF6FD700EF6FD7ull | ((((v3_2 & 0x0010000000100000ull) >> 20) ^ ((v3_2 & 0x0080000000800000ull) >> 23)) << 20) | (((((statex2_2 >> 1) & 0x0000800000008000ull) >> 15) ^ ((v3_2 & 0x0080000000800000ull) >> 23)) << 15) | (((((statex2_2 >> 1) & 0x0000100000001000ull) >> 12) ^ ((v3_2 & 0x0080000000800000ull) >> 23)) << 12) | 32 * ((((statex2_2 >> 1) & 0x0000002000000020ull) >> 5) ^ ((v3_2 & 0x0080000000800000ull) >> 23)) | (((((statex2_2 >> 1) & 0x0000000800000008ull) >> 3) ^ ((v3_2 & 0x0080000000800000ull) >> 23)) << 3);
#endif
    }
#ifdef USE_SSE2
    uint32_t vals[4];
    _mm_store_si128(reinterpret_cast<__m128i*>(vals), statex4);
    result.value1 = vals[0];
    result.value2 = vals[1];
    result.value3 = vals[2];
    result.value4 = vals[3];
#else
    result.value1 = statex2_1 & 0xFFFFFFFFull;
    result.value2 = statex2_2 & 0xFFFFFFFFull;
    result.value3 = statex2_1 >> 32;
    result.value4 = statex2_2 >> 32;
#endif
    result.value1 = ((result.value1 & 0xF0000) >> 16) | ((result.value1 & 0xF) << 4) | ((result.value1 & 0xF00000) >> 12) | (result.value1 & 0xF000) | ((result.value1 & 0xFF0) << 12);
    result.value2 = ((result.value2 & 0xF0000) >> 16) | ((result.value2 & 0xF) << 4) | ((result.value2 & 0xF00000) >> 12) | (result.value2 & 0xF000) | ((result.value2 & 0xFF0) << 12);
    result.value3 = ((result.value3 & 0xF0000) >> 16) | ((result.value3 & 0xF) << 4) | ((result.value3 & 0xF00000) >> 12) | (result.value3 & 0xF000) | ((result.value3 & 0xFF0) << 12);
    result.value4 = ((result.value4 & 0xF0000) >> 16) | ((result.value4 & 0xF) << 4) | ((result.value4 & 0xF00000) >> 12) | (result.value4 & 0xF000) | ((result.value4 & 0xFF0) << 12);
#ifdef CHECK_PASS3BX4
    if (result.value1 != expected.value1 || result.value2 != expected.value2 || result.value3 != expected.value3 || result.value4 != expected.value4) {
        std::terminate();
    }
#endif
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

constexpr ValueX2 SeedKeyX2Pass1To2(ValueX2 seed, uint64_t key) {
    uint32_t keyMostSignificant = (uint32_t) (key >> 32);
    uint32_t keyLeastSignificant = (uint32_t) (key & 0xFFFFFFFF);
#ifdef CHECK_PASS1X2
    if (keyLeastSignificant == 0xFFFFFFFF) {
        std::terminate();
    }
#endif
    ValueX2 state = SeedKeyPass1(seed, keyMostSignificant);
    return SeedKeyPass2(state, keyLeastSignificant);
}

constexpr ValueX2 SeedKeyX2Pass1To3a(ValueX2 seed, uint64_t key) {
    ValueX2 state = SeedKeyX2Pass1To2(seed, key);
    return SeedKeyPass3a(state, (uint32_t) (key & 0xFFFFFFFF));
}

constexpr ValueX2 SeedKeyX2(ValueX2 seed, uint64_t key) {
    auto state = SeedKeyX2Pass1To3a(seed, key);
    state.value1 = SeedKeyPass3b(state.value1, (uint32_t) (key & 0xFFFFFFFF));
    state.value2 = SeedKeyPass3b(state.value2, (uint32_t) ((key + 1) & 0xFFFFFFFF));
    return state;
}

#ifndef USE_SSE2
constexpr
#endif
ValueX4 SeedKeyX4Pass1To3a(ValueX4 seed, uint64_t key) {
#ifdef USE_SSE2
    ValueX4 state{SeedKeyX2Pass1To2(seed.First(), key), SeedKeyX2Pass1To2(seed.Second(), key + 2)};
    return SeedKeyPass3a(state, (uint32_t) (key & 0xFFFFFFFF));
#else
    return {SeedKeyX2Pass1To3a(seed.First(), key), SeedKeyX2Pass1To3a(seed.Second(), key + 2)};
#endif
}

#ifndef USE_SSE2
constexpr
#endif
ValueX4 SeedKeyX4(ValueX4 seed, uint64_t key) {
    auto state = SeedKeyX4Pass1To3a(seed, key);
    return SeedKeyPass3bx4(state, (uint32_t) (key & 0xFFFFFFFF));
}

#ifndef USE_SSE2
constexpr
#endif
ValueX8 SeedKeyX8Pass1To3a(ValueX8 seed, uint64_t key) {
#ifdef USE_AVX
    ValueX8 state{{SeedKeyX2Pass1To2(seed.First().First(), key), SeedKeyX2Pass1To2(seed.First().Second(), key + 2)},
                  {SeedKeyX2Pass1To2(seed.Second().First(), key + 4), SeedKeyX2Pass1To2(seed.Second().Second(), key + 6)}};
    return SeedKeyPass3a(state, (uint32_t) (key & 0xFFFFFFFF));
#else
    return {SeedKeyX4Pass1To3a(seed.First(), key), SeedKeyX4Pass1To3a(seed.Second(), key + 4)};
#endif
}

#ifndef USE_SSE2
constexpr
#endif
ValueX8 SeedKeyX8(ValueX8 seed, uint64_t key) {
    return SeedKeyPass3bx8(SeedKeyX8Pass1To3a(seed, key), key);
}

#ifndef USE_SSE2
constexpr
#endif
ValueX16 SeedKeyX16(ValueX16 seed, uint64_t key) {
    return {SeedKeyX8(seed.First(), key), SeedKeyX8(seed.Second(), key + 8)};
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

#ifndef USE_SSE2
#ifndef CHECK_PASS3BX4
constexpr bool TestKeyX4Func(uint32_t input, uint64_t key, uint32_t expected) {
    auto pass1 = SeedKeyPass1(input, key >> 32);
    auto pass2 = SeedKeyPass2(pass1, key & 0xFF000000);
    auto pass3a = SeedKeyPass3a(pass2, key & 0x00FF0000);
    auto res1 = SeedKeyPass3bx4(pass3a, (key & 0xFFFF) - 3);
    auto res2 = SeedKeyPass3bx4(pass3a, (key & 0xFFFF) - 2);
    auto res3 = SeedKeyPass3bx4(pass3a, (key & 0xFFFF) - 1);
    auto res4 = SeedKeyPass3bx4(pass3a, (key & 0xFFFF));
    return res1.value4 == expected && res2.value3 == expected && res3.value2 == expected && res4.value1 == expected;
}

static_assert(TestKeyX4Func(0x400CAB, 13877892, 0x585AB6));
#endif
#endif

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
