#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <stdnoreturn.h>
#include <sys/types.h>
#include <uchar.h>

#if __STDC_VERSION__ >= 202311L
#define NULLPTR nullptr
#define NORETURN [[noreturn]]
#else
#define NULLPTR NULL
#define NORETURN noreturn
#endif

#ifndef _PORT
#error Provide port as -D_PORT=<port>
#endif

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef char c8;
typedef unsigned char uc8;
typedef signed char sc8;
typedef char16_t c16;
typedef char32_t c32;

typedef float f32;
typedef double f64;

typedef size_t size;
typedef ssize_t ssize;

enum { BUFFER_SIZE = 1024 };
enum { PORT = _PORT };

#endif
