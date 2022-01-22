#ifndef _MEL_THREAD_H
#define _MEL_THREAD_H

#if 0
MEL_THREAD_FN test_function(void * args){
	/* Do something related to threading */
	printf("Hello\n");
	return 0;
}
MEL_THREAD_FN test_function2(void * args){
	/* Do something related to threading */
	printf("World\n");
	return 0;
}

int main(){
    MEL_Thread threads[2] = {0};
    while (<gameloop>){
    [...]
	MEL_thread_create(threads[0], test_function, NULL);
	MEL_thread_create(threads[1], test_function2, NULL);
    [...]
	for (size_t i = 0; i < (sizeof(threads)/sizeof(threads[0])); ++i){
    	MEL_thread_join(threads[i]);
	}
    <swapbuffers>
    }
    return 0;
}
#endif

#if _WIN32
#include <processthreadsapi.h>
#include <synchapi.h>

#define MEL_THREAD_FN DWORD WINAPI

#define MEL_Thread LPDWORD
#define MEL_thread_create(thread_obj, function, args) CreateThread(NULL, 0, function, args, 0, thread_obj)
#define MEL_thread_join(thread_obj) WaitForSingleObject(thread_obj, INFINITE)

#define MEL_Mutex HANDLE
#define MEL_MUTEX_INIT CreateMutex(NULL, FALSE, NULL)
#define MEL_mutex_lock(lock) WaitForSingleObject(lock, INFINITE)
#define MEL_mutex_unlock(lock)

#else
#include <pthread.h>

#define MEL_THREAD_FN void *

#define MEL_Thread pthread_t
#define MEL_thread_create(thread_obj, function, args) pthread_create(&thread_obj, NULL, function, args)
#define MEL_thread_join(thread_obj) pthread_join(thread_obj, NULL)

#define MEL_Mutex pthread_mutex_t
#define MEL_MUTEX_INIT PTHREAD_MUTEX_INITIALIZER
#define MEL_mutex_lock(lock) pthread_mutex_lock(lock)
#define MEL_mutex_unlock(lock) pthread_mutex_unlock(lock)
#endif

#endif
