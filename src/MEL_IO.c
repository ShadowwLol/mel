#if 0
#include "../include/MEL_IO.h"

/* Return length of `src` string */
size_t MEL_strlen(const char * src){
	size_t i = 0;
	for (; src[i] != '\0'; ++i){}
	return i;
}

/* Copy string `src` to buffer `dest` which has size `n` */
char * MEL_strlcpy(char * dest, char * src, size_t n){
	size_t i;
	for (i = 0; i < n && src[i] != '\0'; ++i){
		dest[i] = src[i];
	}
	for (; i < n; ++i){
		dest[i] = '\0';
	}
	dest[--n] = '\0';
	return dest;
}

/* Append string `src` to buffer `dest` which has size `n` */
char * MEL_strlcat(char * dest, char * src, size_t n){
	size_t dest_len = MEL_strlen(dest);
	size_t src_len = MEL_strlen(src);
	size_t i;
	for (i = 0; i < n && src[i] != '\0'; ++i){
		dest[dest_len+i] = src[i];
	}
	dest[1+(dest_len+src_len)] = '\0';
	dest[--n] = '\0';
	return dest;
}

void * MEL_memcpy(void *dst, const void *src, size_t len){
	size_t i;

	/*
	 * memcpy does not support overlapping buffers, so always do it
	 * forwards. (Don't change this without adjusting memmove.)
	 *
	 * For speedy copying, optimize the common case where both pointers
	 * and the length are word-aligned, and copy word-at-a-time instead
	 * of byte-at-a-time. Otherwise, copy by bytes.
	 *
	 * The alignment logic below should be portable. We rely on
	 * the compiler to be reasonably intelligent about optimizing
	 * the divides and modulos out. Fortunately, it is.
	 */

	if ((uintptr_t)dst % sizeof(long) == 0 &&
	    (uintptr_t)src % sizeof(long) == 0 &&
	    len % sizeof(long) == 0) {

	        long *d = dst;
	        const long *s = src;

	        for (i=0; i<len/sizeof(long); i++) {
	                d[i] = s[i];
	        }
	}
	else {
	        char *d = dst;
	        const char *s = src;

	        for (i=0; i<len; i++) {
	                d[i] = s[i];
	        }
	}

	return dst;
}

int MEL_strcmp(const char * p1, const char * p2){
	const unsigned char * s1 = (const unsigned char *) p1;
	const unsigned char * s2 = (const unsigned char *) p2;
	unsigned char c1, c2;
	do{
			c1 = (unsigned char) *++s1;
			c2 = (unsigned char) *++s2;
			if (c1 == '\0')
				break;
	}while (c1 == c2);
	return c1 - c2;
}
#endif
