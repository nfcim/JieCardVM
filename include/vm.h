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
 * Load Import.cap
 *
 * @param data Content of mport.cap
 * @param length Length of data
 * @return VM_ERR_OK on success
 */
int vm_load_import(u1 *data, u4 length);

/**
 * Run install(byte[],short,byte) method of the applet
 *
 * @param aid AID of applet in binary
 * @param aid_length Length of aid
 * @param params Copied to first argument of install()
 * @param length Total length of params
 * @param offset_arg Second argument of install()
 * @param length_arg Third argument of install()
 * @return VM_ERR_OK on success
 */
int vm_install_applet(u1 *aid, u2 aid_length, u1 *params, u2 length,
                      u2 offset_arg, u1 length_arg);

/**
 * Run process(APDU) method of the applet
 *
 * @param obj_ref Object ref of Applet
 * @param cmd_apdu Command APDU
 * @param cmd_len Length of Command APDU
 * @param resp_apdu Response APDU
 * @param resp_len Length of Resp APDU
 * @return Actual Resp APDU length
 */
int vm_process_apdu(u2 ref, u1 *cmd_apdu, u2 cmd_len, u1 *resp_apdu,
                    u2 resp_len);

#ifdef __cplusplus
};
#endif

#endif // JIECARDVM_VM_H
