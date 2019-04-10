#ifndef JIECARDVM_CONTEXT_H
#define JIECARDVM_CONTEXT_H

#include <stdbool.h>

#include "lfs.h"
#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

void CTXInit(const struct lfs_config *cfg);
bool CTXCreateCAP(u1 *aid, u1 aidLength);

#ifdef __cplusplus
}
#endif

#endif // JIECARDVM_CONTEXT_H
