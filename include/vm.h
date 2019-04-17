#ifndef JIECARDVM_VM_H
#define JIECARDVM_VM_H

#include <context.h>

#ifdef __cplusplus
extern "C" {
#endif

extern package_t current_package;

int vm_init(void);

#ifdef __cplusplus
};
#endif

#endif // JIECARDVM_VM_H
