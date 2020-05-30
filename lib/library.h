#ifndef JIECARDVM_LIBRARY_H
#define JIECARDVM_LIBRARY_H

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*library_function_handler)();

struct library_function {
  const u2 aid_length;
  const u1 *aid;
  const u2 class_token;
  const u2 token;
  const library_function_handler handler;
};

void javacard_framework_applet_init();
void javacard_framework_applet_register();

extern const struct library_function LIBRARY_FUNCTIONS[];
extern const int LIBRARY_FUNCTION_COUNT;

struct library_virtual_function {
  const u2 aid_length;
  const u1 *aid;
  const u2 class_token;
  const u2 token;
};

extern const int JAVACARD_FRAMEWORK_APPLET_PROCESS_TOKEN;
extern const struct library_virtual_function LIBRARY_VIRTUAL_FUNCTIONS[];

#ifdef __cplusplus
};
#endif

#endif
