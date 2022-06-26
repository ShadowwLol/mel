#ifndef _COMMON_H
#define _COMMON_H

#define PRINT_DEBUG fprintf(stdout, "[DEBUG] LINE: [%d] FUNCTION : [%s]\n", __LINE__, __func__);
#define TICK(X) clock_t X = clock()
#define TOCK(X) printf("time %s: %g sec.\n", (#X), (double)(clock() - (X)) / CLOCKS_PER_SEC)

#include <stdio.h>
#include <stdlib.h>

#include "ext/glad.h"
#include <GLFW/glfw3.h>
#include "ext/cglm/cglm.h"

#include "types.h"
#include "logs.h"
#include "IO.h"

#define MEL_FN uint8_t
#define EX_S (0)
#define EX_F (255)

#define MEL_KEY uint8_t

#define MEL_SINGLETON(t, inst, init) t* MEL_S_##t_##inst() { static t inst = init; return &inst; }

#define MEL_set_clear_color(r, g, b, a) glClearColor(r/255.0f, g/255.0f, b/255.0f, a/255.0f);
#define MEL_clear(buffer) glClear(buffer);


#endif /* _COMMON_H */
