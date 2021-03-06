#ifndef UA_UTIL_H_
#define UA_UTIL_H_

#include "ua_config.h"

/* Subtract from nodeids to get from the encoding to the content */
#define UA_ENCODINGOFFSET_XML 1
#define UA_ENCODINGOFFSET_BINARY 2

#include <assert.h> // assert
#define UA_assert(ignore) assert(ignore)

/*********************/
/* Memory Management */
/*********************/

/* Replace the macros with functions for custom allocators if necessary */
#include <stdlib.h> // malloc, free
#ifdef _WIN32
# include <malloc.h>
#endif

#ifndef UA_free
# define UA_free(ptr) free(ptr)
#endif
#ifndef UA_malloc
# define UA_malloc(size) malloc(size)
#endif
#ifndef UA_calloc
# define UA_calloc(num, size) calloc(num, size)
#endif
#ifndef UA_realloc
# define UA_realloc(ptr, size) realloc(ptr, size)
#endif

#ifndef NO_ALLOCA
# ifdef __GNUC__
#  define UA_alloca(size) __builtin_alloca (size)
# elif defined(_WIN32)
#  define UA_alloca(SIZE) _alloca(SIZE)
# else
#  include <alloca.h>
#  define UA_alloca(SIZE) alloca(SIZE)
# endif
#endif

/************************/
/* Thread Local Storage */
/************************/

#ifdef UA_MULTITHREADING
# ifdef __GNUC__
#  define UA_THREAD_LOCAL __thread
# elif defined(_MSC_VER)
#  define UA_THREAD_LOCAL __declspec(thread)
# else
#  error No thread local storage keyword defined for this compiler
# endif
#else
# define UA_THREAD_LOCAL
#endif

/********************/
/* System Libraries */
/********************/

#include <time.h>
#ifdef _WIN32
# include <winsock2.h> //needed for amalgation
# include <windows.h>
# undef SLIST_ENTRY
#else
# include <sys/time.h>
#endif

/*************************/
/* External Dependencies */
/*************************/

#include "queue.h"

#ifdef UA_MULTITHREADING
# define _LGPL_SOURCE
# include <urcu.h>
# include <urcu/wfcqueue.h>
# include <urcu/uatomic.h>
# include <urcu/rculfhash.h>
# include <urcu/lfstack.h>
# ifdef NDEBUG
# define UA_RCU_LOCK() rcu_read_lock()
# define UA_RCU_UNLOCK() rcu_read_unlock()
# else
extern UA_THREAD_LOCAL bool rcu_locked;
# define UA_RCU_LOCK() do {     \
        assert(!rcu_locked);    \
        rcu_locked = UA_TRUE;   \
        rcu_read_lock(); } while(0)
# define UA_RCU_UNLOCK() do { \
        assert(rcu_locked);   \
        rcu_locked = UA_FALSE;    \
        rcu_read_lock(); } while(0)
# endif
#else
# define UA_RCU_LOCK()
# define UA_RCU_UNLOCK()
#endif

#endif /* UA_UTIL_H_ */
