#ifndef JIECARDVM_GLOBALS_H
#define JIECARDVM_GLOBALS_H

#include "common.h"
#include "lfs.h"

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

// current processing apdu
extern u1 *current_resp_apdu;

// apdu resp length
extern u2 current_resp_apdu_len;

#ifdef __cplusplus
};
#endif

#endif // JIECARDVM_GLOBALS_H
