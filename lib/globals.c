#include "globals.h"

lfs_t g_lfs = {};
frame_t frames[TOTAL_FRAMES];
int current_frame;
int running;
package_t current_package;
u1 *current_resp_apdu;
u2 current_resp_apdu_len;
