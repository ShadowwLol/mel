#ifndef _MEL_THREAD_H
#define _MEL_THREAD_H

#if _WIN32
#include <processthreadsapi.h>
#include <synchapi.h>
#define MEL_Thread LPDWORD
#define MEL_thread_create(thread_obj_ptr, function, args) CreateThread(NULL, 0, function, args, 0, thread_obj_ptr)
#define MEL_thread_join(thread_obj) WaitForSingleObject(thread_obj, INFINITE)
#else
#include <pthread.h>
#define MEL_Thread pthread_t
#define MEL_thread_create(thread_obj_ptr, function, args) pthread_create(thread_obj_ptr, NULL, function, args)
#define MEL_thread_join(thread_obj) pthread_join(thread_obj, NULL)
#endif

#endif
