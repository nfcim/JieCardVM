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


#ifdef __cplusplus
};
#endif

#endif // JIECARDVM_GLOBALS_H
