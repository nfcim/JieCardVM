#ifndef JIECARDVM_CONTEXT_H
#define JIECARDVM_CONTEXT_H

#include "types.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void CTXInit(void);
bool CTXCreateCAP(u1 *aid, u1 aidLength);

#ifdef __cplusplus
}
#endif

#endif // JIECARDVM_CONTEXT_H
