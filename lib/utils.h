#ifndef JIECARDVM_UTILS_H
#define JIECARDVM_UTILS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
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
