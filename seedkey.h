//
// Created by sigsegv on 5/8/25.
//

#ifndef FIESTATDCI09_SEEDKEY_H
#define FIESTATDCI09_SEEDKEY_H

#include <cstdint>
#include <x86intrin.h>
#include <exception>

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

struct KeyX8 {
    uint32_t key1, key2, key3, key4, key5, key6, key7, key8;
};

//#define USE_AVX
#define CHECK_PASS3BX8
#define USE_SSE2
//#define CHECK_PASS3BX4

#ifdef USE_AVX
KeyX8 SeedKeyPass3bx8(uint32_t state, uint32_t keyLeastSignificant24Bit) {
    KeyX8 result;
#ifdef CHECK_PASS3BX8
    KeyX8 expected;
    expected.key1 = SeedKeyPass3b(state, keyLeastSignificant24Bit);
    expected.key2 = SeedKeyPass3b(state, keyLeastSignificant24Bit + 1);
    expected.key3 = SeedKeyPass3b(state, keyLeastSignificant24Bit + 2);
    expected.key4 = SeedKeyPass3b(state, keyLeastSignificant24Bit + 3);
    expected.key5 = SeedKeyPass3b(state, keyLeastSignificant24Bit + 4);
    expected.key6 = SeedKeyPass3b(state, keyLeastSignificant24Bit + 5);
    expected.key7 = SeedKeyPass3b(state, keyLeastSignificant24Bit + 6);
    expected.key8 = SeedKeyPass3b(state, keyLeastSignificant24Bit + 7);
#endif
    __m256i bitConstant;
    __m256i statex4;
    __m256i lowbitsx4;
    __m256i maskx4;
    {
        uint32_t bitValuesRaw[8] = {
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
        uint32_t statex4Raw[8] = {state, state, state, state, state, state, state, state};
        uint32_t lowbitsRaw[8] = {1, 1, 1, 1, 1, 1, 1, 1};
        uint32_t maskRaw[8] = {0x00EF6FD7, 0x00EF6FD7, 0x00EF6FD7, 0x00EF6FD7, 0x00EF6FD7, 0x00EF6FD7, 0x00EF6FD7, 0x00EF6FD7};
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
    result.key1 = vals[0];
    result.key2 = vals[1];
    result.key3 = vals[2];
    result.key4 = vals[3];
    result.key1 = vals[4];
    result.key2 = vals[5];
    result.key3 = vals[6];
    result.key4 = vals[7];
    result.key1 = ((result.key1 & 0xF0000) >> 16) | ((result.key1 & 0xF) << 4) | ((result.key1 & 0xF00000) >> 12) | (result.key1 & 0xF000) | ((result.key1 & 0xFF0) << 12);
    result.key2 = ((result.key2 & 0xF0000) >> 16) | ((result.key2 & 0xF) << 4) | ((result.key2 & 0xF00000) >> 12) | (result.key2 & 0xF000) | ((result.key2 & 0xFF0) << 12);
    result.key3 = ((result.key3 & 0xF0000) >> 16) | ((result.key3 & 0xF) << 4) | ((result.key3 & 0xF00000) >> 12) | (result.key3 & 0xF000) | ((result.key3 & 0xFF0) << 12);
    result.key4 = ((result.key4 & 0xF0000) >> 16) | ((result.key4 & 0xF) << 4) | ((result.key4 & 0xF00000) >> 12) | (result.key4 & 0xF000) | ((result.key4 & 0xFF0) << 12);
    result.key5 = ((result.key5 & 0xF0000) >> 16) | ((result.key5 & 0xF) << 4) | ((result.key5 & 0xF00000) >> 12) | (result.key5 & 0xF000) | ((result.key5 & 0xFF0) << 12);
    result.key6 = ((result.key6 & 0xF0000) >> 16) | ((result.key6 & 0xF) << 4) | ((result.key6 & 0xF00000) >> 12) | (result.key6 & 0xF000) | ((result.key6 & 0xFF0) << 12);
    result.key7 = ((result.key7 & 0xF0000) >> 16) | ((result.key7 & 0xF) << 4) | ((result.key7 & 0xF00000) >> 12) | (result.key7 & 0xF000) | ((result.key7 & 0xFF0) << 12);
    result.key8 = ((result.key8 & 0xF0000) >> 16) | ((result.key8 & 0xF) << 4) | ((result.key8 & 0xF00000) >> 12) | (result.key8 & 0xF000) | ((result.key8 & 0xFF0) << 12);
#ifdef CHECK_PASS3BX8
    if (result.key1 != expected.key1 || result.key2 != expected.key2 || result.key3 != expected.key3 || result.key4 != expected.key4 || result.key5 != expected.key5 || result.key6 != expected.key6 || result.key7 != expected.key7 || result.key8 != expected.key8) {
        std::terminate();
    }
#endif
    return result;
}
#endif

#ifndef USE_SSE2
#ifndef CHECK_PASS3BX4
constexpr
#endif
#endif
KeyX4 SeedKeyPass3bx4(uint32_t state, uint32_t keyLeastSignificant24Bit) {
    KeyX4 result;
#ifdef CHECK_PASS3BX4
    KeyX4 expected;
    expected.key1 = SeedKeyPass3b(state, keyLeastSignificant24Bit);
    expected.key2 = SeedKeyPass3b(state, keyLeastSignificant24Bit + 1);
    expected.key3 = SeedKeyPass3b(state, keyLeastSignificant24Bit + 2);
    expected.key4 = SeedKeyPass3b(state, keyLeastSignificant24Bit + 3);
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
    result.key1 = vals[0];
    result.key2 = vals[1];
    result.key3 = vals[2];
    result.key4 = vals[3];
#else
    result.key1 = statex2_1 & 0xFFFFFFFFull;
    result.key2 = statex2_2 & 0xFFFFFFFFull;
    result.key3 = statex2_1 >> 32;
    result.key4 = statex2_2 >> 32;
#endif
    result.key1 = ((result.key1 & 0xF0000) >> 16) | ((result.key1 & 0xF) << 4) | ((result.key1 & 0xF00000) >> 12) | (result.key1 & 0xF000) | ((result.key1 & 0xFF0) << 12);
    result.key2 = ((result.key2 & 0xF0000) >> 16) | ((result.key2 & 0xF) << 4) | ((result.key2 & 0xF00000) >> 12) | (result.key2 & 0xF000) | ((result.key2 & 0xFF0) << 12);
    result.key3 = ((result.key3 & 0xF0000) >> 16) | ((result.key3 & 0xF) << 4) | ((result.key3 & 0xF00000) >> 12) | (result.key3 & 0xF000) | ((result.key3 & 0xFF0) << 12);
    result.key4 = ((result.key4 & 0xF0000) >> 16) | ((result.key4 & 0xF) << 4) | ((result.key4 & 0xF00000) >> 12) | (result.key4 & 0xF000) | ((result.key4 & 0xFF0) << 12);
#ifdef CHECK_PASS3BX4
    if (result.key1 != expected.key1 || result.key2 != expected.key2 || result.key3 != expected.key3 || result.key4 != expected.key4) {
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
    return res1.key4 == expected && res2.key3 == expected && res3.key2 == expected && res4.key1 == expected;
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
