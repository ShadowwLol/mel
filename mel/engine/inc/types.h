#ifndef _TYPES_H
#define _TYPES_H

#include <inttypes.h>
#include <stdbool.h>

typedef unsigned char uchar;

typedef uint_least8_t u8l;
typedef uint_fast8_t u8f;
typedef uint8_t u8;

typedef uint_least16_t u16l;
typedef uint_fast16_t u16f;
typedef uint16_t u16;

typedef uint_least32_t u32l;
typedef uint_fast32_t u32f;
typedef uint32_t u32;

typedef uint_least64_t u64l;
typedef uint_fast64_t u64f;
typedef uint64_t u64;

typedef int_least8_t i8l;
typedef int_fast8_t i8f;
typedef int8_t i8;

typedef int_least16_t i16l;
typedef int_fast16_t i16f;
typedef int16_t i16;

typedef int_least32_t i32l;
typedef int_fast32_t i32f;
typedef int32_t i32;

typedef int_least64_t i64l;
typedef int_fast64_t i64f;
typedef int64_t i64;

typedef float f32;
typedef double f64;

typedef u8l fn;
#define xs (0)
#define xf (255)

typedef u8 flag;


#endif /* _TYPES_H */
