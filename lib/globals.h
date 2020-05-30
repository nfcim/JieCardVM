#ifndef JIECARDVM_GLOBALS_H
#define JIECARDVM_GLOBALS_H

#include "lfs.h"
#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

// little fs
extern lfs_t g_lfs;

// index of current frame
extern int current_frame;

// whether vm is running
extern int running;

// frames
extern frame_t frames[TOTAL_FRAMES];

// current package
extern package_t current_package;

#define current_operand_stack (frames[current_frame].operand_stack)
#define current_variable_table (frames[current_frame].variable_table)


#ifdef __cplusplus
};
#endif

#endif // JIECARDVM_GLOBALS_H
