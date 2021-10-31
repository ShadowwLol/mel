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
	size_t i;
	for (i = 0; i < n && src[i] != '\0'; ++i){
		dest[dest_len+i] = src[i];
	}
	dest[++dest_len] = '\0';
	dest[--n] = '\0';
	return dest;
}
