#ifndef JIECARDVM_VM_H
#define JIECARDVM_VM_H

#include <context.h>

#ifdef __cplusplus
extern "C" {
#endif

extern package_t current_package;

int vm_init(void);
int vm_execute_static_method(package_t *package, char *class_name, char *method_name);

#ifdef __cplusplus
};
#endif

#endif // JIECARDVM_VM_H
