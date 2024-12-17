
#ifndef HASH_H
#define HASH_H

#include <cstdint>

namespace Forge {

#if defined(_MSC_VER)

#    define FORCE_INLINE __forceinline
#    include <stdlib.h>
#    define ROTL32(x, y) _rotl(x, y)
#    define ROTL64(x, y) _rotl64(x, y)

#else // For non-MS compilers (e.g., GCC, Clang)
#    define FORCE_INLINE inline __attribute__((always_inline))
inline uint32_t rotl32(uint32_t x, int8_t r) {
    return (x << r) | (x >> (32 - r));
}

inline uint64_t rotl64(uint64_t x, int8_t r) {
    return (x << r) | (x >> (64 - r));
}

#    define ROTL32(x, y) rotl32(x, y)
#    define ROTL64(x, y) rotl64(x, y)

#endif // !defined(_MSC_VER)

class HashFast {
public:
    HashFast() = delete;
    HashFast(const HashFast&) = delete;
    HashFast(HashFast&&) = delete;
    HashFast& operator=(const HashFast&) = delete;
    HashFast& operator=(HashFast&&) = delete;
    ~HashFast() = delete;

    FORCE_INLINE static uint32_t GenerateU32BaseHash(const void* key, int len, uint32_t seed = 0x42424242) {
        // Use provided seed or a good default
        const uint8_t* data = static_cast<const uint8_t*>(key);
        int nblocks = len / 4;

        uint32_t h1 = seed;
        const uint32_t c1 = 0xcc9e2d51;
        const uint32_t c2 = 0x1b873593;

// Potential optimization: Prefetch data if supported
#ifdef __builtin_prefetch
        __builtin_prefetch(data, 0, 0);
#endif

        const uint32_t* blocks = reinterpret_cast<const uint32_t*>(data + nblocks * 4);

        // Potential SIMD optimization opportunity
        for (int i = -nblocks; i; i++) {
            uint32_t k1 = blocks[i];
            k1 *= c1;
            k1 = ROTL32(k1, 15);
            k1 *= c2;
            h1 ^= k1;
            h1 = ROTL32(h1, 13);
            h1 = h1 * 5 + 0xe6546b64;
        }

        // Tail handling remains the same
        const uint8_t* tail = data + nblocks * 4;
        uint32_t k1 = 0;

        switch (len & 3) {
        case 3:
            k1 ^= tail[2] << 16;
            [[fallthrough]];
        case 2:
            k1 ^= tail[1] << 8;
            [[fallthrough]];
        case 1:
            k1 ^= tail[0];
            k1 *= c1;
            k1 = ROTL32(k1, 15);
            k1 *= c2;
            h1 ^= k1;
            break;
        }

        // Finalization mix (similar to MurmurHash3 fmix32)
        h1 ^= len;
        h1 ^= h1 >> 16;
        h1 *= 0x85ebca6b;
        h1 ^= h1 >> 13;
        h1 *= 0xc2b2ae35;
        h1 ^= h1 >> 16;

        return h1;
    }

private:
};

} // namespace Forge

#endif
