#include "../include/opengl.h"
#include "../include/io.h"
#include "../include/def.h"
#include "../include/shader.h"
#include <cglm/cam.h>
#include "../include/logs.h"
#include "../include/image.h"

#define DEBUG fprintf(stdout, "[DEBUG] LINE: [%d] FUNCTION : [%s]\n", __LINE__, __func__);

void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods);
void error_callback(int error, const char* description);
int toggleFullscreen(Window window);

static Window window;

// Vertices coordinates
GLfloat vertices[] =
{ //     COORDINATES     /        COLORS      /   TexCoord  //
	-0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // Lower left corner
	-0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,	0.0f, 1.0f, // Upper left corner
	 0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,	1.0f, 1.0f, // Upper right corner
	 0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,	1.0f, 0.0f  // Lower right corner
};

// Indices for vertices order
GLuint indices[] =
{
	0, 2, 1, // Upper triangle
	0, 3, 2 // Lower triangle
};


#if __WIN32
#include <windows.h>
int main(void){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD saved_attributes;
    /* Save current attributes */
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    saved_attributes = consoleInfo.wAttributes;
#else
int main(void){
#endif
    /* Initializing the library  */
    if (!glfwInit()){
        log_log(LOG_ERROR, "Failed initializing GLFW");
        return -1;
    }else{
        log_log(LOG_SUCCESS, "Successfully initialized GLFW");
    }
    /* * * * * * * * * * * * * * */
    
    /* Window hints  */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    
    /* * * * * * * * * * * */

    /* Creating the window */
    window.mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    window.window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, glfwGetPrimaryMonitor(), NULL);
    if (!window.window){
        log_log(LOG_ERROR, "Failed creating window");
        glfwTerminate();
        return -1;
    }else{
        log_log(LOG_SUCCESS, "Successfully created window");
    }
    /* * * * * * * * * * * */

    /* Make the window's context current */
    glfwMakeContextCurrent(window.window);
    /* * * * * * * * * * * * * * * * * * */

    /* OPENGL configuration */
    gladLoadGL();
    glfwSwapInterval(1);
    
    /* 2D texture clamping to border */
    /* * * * * * * * * * * * * * * * */

    /* 2D texture filtering  */
    
    /* * * * * * * * * * * * */

    /* Setting callbacks */
    glfwSetErrorCallback(error_callback);
    glfwSetKeyCallback(window.window, key_callback);
    glfwSetWindowAspectRatio(window.window, 16, 9);
    glfwSetWindowSizeLimits(window.window, (window.mode->width/2), (window.mode->height/2), window.mode->width, window.mode->height);
    /* * * * * * * * * * */
    
    /* 2D orthogonal projection */
    mat4 projection;
    glm_ortho(0.0f, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT, 0.0f, -1.0f, 1.0f, projection);
    /* * * * * * * * */
    /* * * * * * * * * * * * * * * * * */

    /* Images  */
    stbi_set_flip_vertically_on_load(true);
    Image crate = img_load_image("resources/images/container.jpg");

    Shader sprite_shader;
    sprite_shader.vertex_shader = "resources/shaders/image.vert";
    sprite_shader.fragment_shader = "resources/shaders/image.frag";
    sprite_shader.shader_program = create_shader_program(sprite_shader);

    GLuint VAO, VBO, EBO;
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), NULL);
	glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glUseProgram(sprite_shader.shader_program);
    glUniform1i(glGetUniformLocation(sprite_shader.shader_program, "tex0"), 0);

    /* Main loop */
    while (!glfwWindowShouldClose(window.window)){

        glfwPollEvents();
        glClearColor(GLColor32(20), GLColor32(20), GLColor32(20), GLColor32(255));
        glClear(GL_COLOR_BUFFER_BIT);
        
        /* Drawing */
        glUseProgram(sprite_shader.shader_program);
        glUniform1f(glGetUniformLocation(sprite_shader.shader_program, "scale"), 0.5f);
        glBindTexture(GL_TEXTURE_2D, crate.texture);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        /* * * * * */
        
        glfwSwapBuffers(window.window);

    }
    /* * * * * * */

    /* Terminating */
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(sprite_shader.shader_program);
    glDeleteTextures(1, &crate.texture);
    glfwDestroyWindow(window.window);
    glfwTerminate();
    /* * * * * * * */

    return 0;
}

void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods){
    switch(action){
        case GLFW_RELEASE:
            switch(key){
                case GLFW_KEY_ESCAPE:
                    /* Close the game */
                    glfwSetWindowShouldClose(win, GLFW_TRUE);
                    break;
                case GLFW_KEY_F11:
                    /* Fullscreen the game */
                    toggleFullscreen(window);
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

void error_callback(int error, const char* description){
    fprintf(stderr, "[-] Error: %s\n", description);
}
