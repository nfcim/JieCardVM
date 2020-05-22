#ifndef JIECARDVM_UTILS_H
#define JIECARDVM_UTILS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef DEBUG_OUTPUT
#include <stdio.h>
#define DBG_MSG(format, ...)                                                   \
  printf("[DBG] %s(%dL): " format, __func__, __LINE__, ##__VA_ARGS__)
#define ERR_MSG(format, ...)                                                   \
  printf("[ERR] %s(%dL): " format, __func__, __LINE__, ##__VA_ARGS__)
#define PRINT_HEX(...) print_hex(__VA_ARGS__)
#else
#define DBG_MSG(...)
#define ERR_MSG(...)
#define PRINT_HEX(...)
#endif

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define htobe32(x) (x)
#define htobe16(x) (x)
#define htole32(x) __builtin_bswap32(x)
#else
#define htobe32(x) __builtin_bswap32(x)
#define htobe16(x) __builtin_bswap16(x)
#define htole32(x) (x)
#endif

/**
 * Convert bytes to hex string.
 *
 * @param xp Pointer to output buffer
 * @param bb Pointer to input bytes
 * @param n  Input length in bytes
 */
void bytes_to_hexstring(char *xp, const uint8_t *bb, int n);

#ifdef __cplusplus
}
#endif

#endif // JIECARDVM_UTILS_H
