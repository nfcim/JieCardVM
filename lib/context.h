#ifndef JIECARDVM_CONTEXT_H
#define JIECARDVM_CONTEXT_H

#include <stdbool.h>
#include <stdio.h>

#include "lfs.h"
#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

void CTXInit(const struct lfs_config *cfg);
bool CTXCreateCAP(u1 *aid, u1 aidLength);
bool CTXDeleteCAP(u1 *aid, u1 aidLength);
bool CTXAppendMethods(u1 *aid, u1 aidLength, u1 *bytecodes, u2 bytecodesLength);
ssize_t CTXReadMethods(u1 *aid, u1 aidLength, u1 *target, u2 offset, u2 length);

#ifdef __cplusplus
}
#endif

#endif // JIECARDVM_CONTEXT_H
