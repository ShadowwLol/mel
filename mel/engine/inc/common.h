#ifndef _COMMON_H
#define _COMMON_H

//#define PRINT_DEBUG fprintf(stdout, "[DEBUG] LINE: [%d] FUNCTION : [%s]\n", __LINE__, __func__);
//#define TICK(X) clock_t X = clock()
//#define TOCK(X) printf("time %s: %g sec.\n", (#X), (double)(clock() - (X)) / CLOCKS_PER_SEC)

#include <stdio.h>
#include <stdlib.h>

#include "ext/glad.h"
#include <GLFW/glfw3.h>
#include "ext/cglm/cglm.h"

#include "types.h"
#include "logs.h"
#include "IO.h"

#endif /* _COMMON_H */
