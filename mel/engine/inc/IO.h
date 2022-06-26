#ifndef _MEL_IO_H
#define _MEL_IO_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "logs.h"

#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

#if __WIN32
#define mkdir(A, B) mkdir(A)
#endif

#define FN uint8_t
#define EX_S (0)
#define EX_F (255)

typedef struct {
  char* buffer;
  size_t length;
  size_t cap;
} String;

typedef struct {
  char* buffer;
  size_t length;
} String_View;

/* char * operations */
static char* MEL_strlcpy(char* , const char* , size_t);
static char* MEL_strlcat(char* , const char* , size_t);
/* * * * * * * * * * */

/* String operations */
static String init_str(const char* );
static FN realloc_str(String *, uint64_t);
static FN set_str(String *, const char* );
static FN append_str(String *, const char* );
static FN remove_str(String *);
/* * * * * * * * * * */

/* String_View operations  */
static String_View set_strv(char* );
static String_View substr_to_strv(char* , size_t, size_t);
inline static char* MEL_read_file(const char* f_name);
/* * * * * * * * * * * * * */

/* Copy string `src` to buffer `dest` with length `n` */
inline static char* MEL_strlcpy(char* dest, const char* src, size_t n) {
  size_t i;
  for (i = 0; i < n && src[i] != '\0'; ++i) {
    dest[i] = src[i];
  }
  for (; i < n; ++i) {
    dest[i] = '\0';
  }
  dest[n - 1] = '\0';
  return dest;
}

/* Append string `src` to buffer `dest` with length`n` */
inline static char* MEL_strlcat(char* dest, const char* src, size_t n) {
  size_t dest_len = strlen(dest);
  size_t src_len = strlen(src);
  for (size_t i = 0; i < n && src[i] != '\0'; ++i) {
    dest[dest_len + i] = src[i];
  }
  dest[dest_len + src_len] = '\0';
  dest[n - 1] = '\0';
  return dest;
}

/* Initialize a String */
inline static String init_str(const char* s) {
  String str;
  str.length = strlen(s);
  str.cap = sizeof(char) * str.length + 1;
  str.buffer = (char *) calloc(str.cap, sizeof(char));
  MEL_strlcpy(str.buffer, s, str.cap);
  return str;
}

/* Reallocate a String */
inline static FN realloc_str(String * s, uint64_t bytes) {
  s->buffer = (char *) realloc(s->buffer, sizeof(char) * bytes);
  s->cap = sizeof(char) * bytes;
  return 0;
}

/* Set String `str`s buffer to `src` and update String */
inline static FN set_str(String * str, const char* src) {
  if (!str->length) {
    *str = init_str(src);
    return 0;
  }

  size_t src_len = strlen(src);
  str->length = src_len;
  if (str->cap < str->length) {
    str->cap = str->length + 1;
    str->buffer = (char *) realloc(str->buffer, sizeof(char) * str->cap);
  }
  MEL_strlcpy(str->buffer, src, str->cap);
  return 0;
}

/* Append String `str`s buffer with `src` and update String */
inline static FN append_str(String * str, const char* src) {
  size_t src_len = strlen(src);
  str->length += src_len;
  if (str->cap < str->length) {
    str->cap = str->length + 1;
    str->buffer = (char *) realloc(str->buffer, sizeof(char) * str->cap);
  }
  MEL_strlcat(str->buffer, src, str->cap);
  return 0;
}

/* Remove String */
inline static FN remove_str(String * str) {
  free(str->buffer);
  str->length = 0;
  str->cap = 0;
  return 0;
}

/* Set String_View `src`s buffer and update String_View */
inline static String_View set_strv(char* src) {
  String_View sv = {
    .buffer = src,
    .length = strlen(src)
  };
  return sv;
}

/* Return substring of `src`s buffer with offset `offset` and length `len` */
inline static String_View substr_to_strv(char* src, size_t offset, size_t len) {
  size_t src_len = strlen(src);
  if (src_len < (offset + len)) {
    return (String_View) {
    NULL, 0};
  }
  String_View sv = {
    .buffer = (src + offset),
    .length = len,
  };
  sv.buffer[len] = '\0';
  return sv;
}

/* Return String with contents of file `f_name` */
inline static char* MEL_read_file(const char* f_name) {
  char* str = (char *) calloc(1, sizeof(char));
  FILE* f = fopen(f_name, "rb");

  if (f) {
    fseek(f, 0, SEEK_END);
    size_t length = ftell(f);
    fseek(f, 0, SEEK_SET);

    /* 1 GiB; best not to load a whole large file in one string */
    if (length > 1073741824) {
      free(str);
      mlog(LOG_WARNING, "File {%s} too large", f_name);
      return NULL;
    }

    str = (char *) realloc(str, sizeof(char) * (length + 1));

    if (length) {
      size_t read_length = fread(str, 1, length, f);

      if (length != read_length) {
        free(str);
        mlog(LOG_WARNING, "Failed reading file {%s}", f_name);
        return NULL;
      }
    }

    fclose(f);
    str[length] = '\0';
  } else {
    free(str);
    mlog(LOG_WARNING, "File does not exist {%s}", f_name);
    return NULL;
  }
  return str;
}

static FN mkpath(char* file_path, mode_t mode);
inline static FN mkpath(char* file_path, mode_t mode) {
  if (!file_path || !*file_path) {
    return EX_F;
  }

  for (char *p = strchr(file_path + 1, '/'); p; p = strchr(p + 1, '/')) {
    *p = '\0';
    if (mkdir(file_path, mode) == -1) {
      if (errno != EEXIST) {
        *p = '/';
        mlog(LOG_ERROR, "Failed creating path");
        return EX_F;
      }
    }
    *p = '/';
  }
  return EX_S;
}

static FN get_input(char* str, size_t sz, FILE * fd);
inline static FN get_input(char* str, size_t sz, FILE * fd) {
  if (!fgets(str, sz, fd)) {
    mlog(LOG_ERROR, "Failed getting input");
    return EX_F;
  }
  str[strlen(str) - 1] = '\0';
  return EX_S;
}

#ifndef _WIN32

/* itoa implementation */
static char* itoa(int32_t value, char* result, int8_t base);
inline static char* itoa(int32_t value, char* result, int8_t base) {
  /* making sure the base is valid */
  if (base < 2 || base > 36) {
    *result = '\0';
    return result;
  }
  /* * * * * * * * * * * * * * * * */

  char* ptr = result, *ptr1 = result, tmp_char;
  int32_t tmp_val;

  do {
    tmp_val = value;
    value /= base;
    *ptr++ =
      "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"
      [35 + (tmp_val - value * base)];
  } while (value);

  if (tmp_val < 0)
    *ptr++ = '-';
  *ptr-- = '\0';
  while (ptr1 < ptr) {
    tmp_char = *ptr;
    *ptr-- = *ptr1;
    *ptr1++ = tmp_char;
  }

  return result;
}

#endif

#endif
