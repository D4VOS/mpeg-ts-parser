#pragma once

#include <cstdint>
#include <cinttypes>
#include <cfloat>
#include <climits>
#include <cstddef>

#define NOT_VALID  -1

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <bitset>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <streambuf>
#include <unordered_map>
#include <cassert>

class buffer_restore {
    std::ostream &os;
    std::streambuf *buf;
public:
    buffer_restore(std::ostream &os) : os(os), buf(os.rdbuf()) {}

    ~buffer_restore() {
        os.rdbuf(buf);
    }
};

#if defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || defined(_M_IX86))
#include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))

#include <x86intrin.h>

#endif

//=============================================================================================================================================================================
// Byte swap
//=============================================================================================================================================================================
#if defined(_MSC_VER)
static inline uint16_t_t xSwapBytes16(uint16_t_t Value) { return _byteswap_ushort(Value); }
static inline  int16_t xSwapBytes16( int16_t Value) { return _byteswap_ushort(Value); }
static inline uint32_t xSwapBytes32(uint32_t Value) { return _byteswap_ulong (Value); }
static inline  int32_t xSwapBytes32( int32_t Value) { return _byteswap_ulong (Value); }
static inline uint64_t xSwapBytes64(uint64_t Value) { return _byteswap_uint64(Value); }
static inline  int64_t xSwapBytes64( int64_t Value) { return _byteswap_uint64(Value); }
#elif defined (__GNUC__)

static inline uint16_t xSwapBytes16(uint16_t Value) { return __builtin_bswap16(Value); }

static inline int16_t xSwapBytes16(int16_t Value) { return __builtin_bswap16(Value); }

static inline uint32_t xSwapBytes32(uint32_t Value) { return __builtin_bswap32(Value); }

static inline int32_t xSwapBytes32(int32_t Value) { return __builtin_bswap32(Value); }

static inline uint64_t xSwapBytes64(uint64_t Value) { return __builtin_bswap64(Value); }

static inline int64_t xSwapBytes64(int64_t Value) { return __builtin_bswap64(Value); }

#else
#error Unrecognized compiler
#endif


