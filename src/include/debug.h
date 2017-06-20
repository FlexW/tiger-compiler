/**
 * @file debug.h Debugging functions.
 */

#ifndef _DEBUG_H_
#define _DEBUG_H_

#define DEBUG 1

#define debug_print(fmt, ...)                                           \
  do { if (DEBUG) fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__,         \
                          __LINE__, __func__, __VA_ARGS__); } while (0)

#endif /* _DEBUG_H_ */
