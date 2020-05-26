#ifndef JIECARDVM_VM_H
#define JIECARDVM_VM_H

#include <context.h>

#ifdef __cplusplus
extern "C" {
#endif

enum vm_error {
  VM_ERR_OK = 0,           // No error
  VM_ERR_INVALID_ARG = -1, // Invalid argument
  VM_ERR_UNKNOWN = -2,     // Unknown error
  VM_ERR_NO_ENT = -3,      // No entry found
};

extern package_t current_package;

/**
 * Initialize JieCardVM
 *
 * @return VM_ERR_OK on success
 */
int vm_init(void);

/**
 * Switch current package
 *
 * @param aid AID of the CAP package in hex string without spaces
 * @return VM_ERR_OK on success
 */
int vm_set_current_package(char *aid);

/**
 * Load Method.cap
 *
 * @param data Content of Method.cap
 * @param length Length of data
 * @return VM_ERR_OK on success
 */
int vm_load_method(u1 *data, u4 length);

/**
 * Load ConstantPool.cap
 *
 * @param data Content of ConstantPool.cap
 * @param length Length of data
 * @return VM_ERR_OK on success
 */
int vm_load_constant_pool(u1 *data, u4 length);

/**
 * Load Applet.cap
 *
 * @param data Content of Applet.cap
 * @param length Length of data
 * @return VM_ERR_OK on success
 */
int vm_load_applet(u1 *data, u4 length);

/**
 * Load Class.cap
 *
 * @param data Content of Class.cap
 * @param length Length of data
 * @return VM_ERR_OK on success
 */
int vm_load_class(u1 *data, u4 length);

/**
 * Run install(byte[],short,byte) method of the applet
 *
 * @param aid AID of applet in binary
 * @param length Length of aid
 * @return VM_ERR_OK on success
 */
int vm_install_applet(u1 *aid, u2 length);

#ifdef __cplusplus
};
#endif

#endif // JIECARDVM_VM_H
