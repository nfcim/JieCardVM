#ifndef JIECARDVM_CONTEXT_H
#define JIECARDVM_CONTEXT_H

#include <lfs.h>
#include <rtda.h>
#include <types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LFS_ATTR_METADATA 0

enum data_size {
  ELEM_SIZE_1 = 1,
  ELEM_SIZE_2 = 2,
};

enum context_error {
  CONTEXT_ERR_OK = 0,         // No error
  CONTEXT_ERR_EXIST = -1,     // Package already exists
  CONTEXT_ERR_NOENT = -2,     // Entry does not exist
  CONTEXT_ERR_UNKNOWN = -128, // Unknown error
};

/**
 * Initialize the context
 *
 * @param lfs_cfg Config of little fs
 */
void context_init(const struct lfs_config *lfs_cfg);

/**
 * Create a new cap
 *
 * @param pkg Package Info
 * @return CONTEXT_ERR_EXIST if the AID exists;
 * CONTEXT_ERR_OK if the package is created.
 */
int context_create_cap(package_t *pkg);

/**
 * Delete a cap
 *
 * @param pkg Package info
 * @return CONTEXT_ERR_NOENT if the AID does not exist;
 * CONTEXT_ERR_OK if the package is created.
 */
int context_delete_cap(package_t *pkg);

/**
 * Write Method.cap into current package
 *
 * @param pkg Package info
 * @param data Method info
 * @param length Length of method info
 * @return CONTEXT_ERR_OK on success
 */
int context_write_methods(package_t *pkg, u1 *data, u2 length);

/**
 * Read method info from current package
 *
 * @param pkg Package info
 * @param target Buffer to store method info
 * @param offset Offset into Method.cap
 * @param length Length to read
 * @return bytes read
 */
int context_read_method(package_t *pkg, u1 *target, u2 offset, u2 length);

/**
 * Write Constant.cp into current package
 *
 * @param pkg Package info
 * @param data One or more constant info
 * @param length Length of constant info
 * @return CONTEXT_ERR_OK on success
 */
int context_write_constants(package_t *pkg, u1 *data, u2 length);

/**
 * Read constant info from a cap
 *
 * @param pkg Package info
 * @param target Buffer to store constant info
 * @param index Index of constant
 * @param length Length to read
 * @return bytes read
 */
int context_read_constant(package_t *pkg, u2 index, u1 *info, u2 length);

/**
 * Get current constant count of a cap
 *
 * @param pkg Package info
 * @return count of constants
 */
int context_count_constant(package_t *pkg);

/**
 * Write Applet.cap into current package
 *
 * @param pkg Package info
 * @param data Applet info
 * @param length Length of applet info
 * @return CONTEXT_ERR_OK on success
 */
int context_write_applets(package_t *pkg, u1 *data, u2 length);

/**
 * Read applet info from current package
 *
 * @param pkg Applet info
 * @param target Buffer to store applet info
 * @param offset Offset into Applet.cap
 * @param length Length to read
 * @return bytes read
 */
int context_read_applet(package_t *pkg, u1 *target, u2 offset, u2 length);

/**
 * Create an array
 *
 * @param pkg Package info
 * @param type Array type
 * @param class_ref If the array is of type reference, the class_ref should be
 * provided
 * @param length Length of the array
 * @return Array ref starting from 1
 */
int context_create_array(package_t *pkg, u1 type, u2 class_ref, u2 length);

/** 
 * Read an element of array
 * 
 * @param pkg Package info
 * @param ref Array ref
 * @param type Element type
 * @param index Index of element in array
 * @param val Output buffer
 * @return CONTEXT_ERR_OK on success
*/
int context_read_array(package_t *pkg, u2 ref, u1 type, u2 index, u1 *val);

/** 
 * Write an element of array
 * 
 * @param pkg Package info
 * @param ref Array ref
 * @param type Element type
 * @param index Index of element in array
 * @param val Element data
 * @return CONTEXT_ERR_OK on success
*/
int context_write_array(package_t *pkg, u2 ref, u1 type, u2 index, u2 val);

/** 
 * Get the metadata of an array
 * 
 * @param pkg Package info
 * @param ref Array ref
 * @param metadata Metadata
 * @return CONTEXT_ERR_OK on success
*/
int context_array_meta(package_t *pkg, u2 ref, array_metadata_t *metadata);

/**
 * Write Class.cap into current package
 *
 * @param pkg Package info
 * @param data Class info
 * @param length Length of class info
 * @return CONTEXT_ERR_OK on success
 */
int context_write_classes(package_t *pkg, u1 *data, u2 length);

/**
 * Read class info from current package
 *
 * @param pkg Applet info
 * @param target Buffer to store class info
 * @param offset Offset into Class.cap
 * @param length Length to read
 * @return bytes read
 */
int context_read_class(package_t *pkg, u1 *target, u2 offset, u2 length);

/**
 * Create an class object
 *
 * @param pkg Package info
 * @param class_ref Class index in Class.cap
 * @return Object ref starting from 1
 */
int context_create_object(package_t *pkg, u2 class_index);

int context_create_static_image(package_t *pkg, u1 *data, u2 length);

int context_read_static_image(package_t *pkg, u2 offset, u1 size, u1 *val);

int context_write_static_image(package_t *pkg, u2 offset, u1 size, u2 val);

#ifdef __cplusplus
}
#endif

#endif // JIECARDVM_CONTEXT_H
