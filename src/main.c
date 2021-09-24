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
void window_size_callback(GLFWwindow * window, int width, int height);
int toggleFullscreen(Window window);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

static Window window;
static Image crate;
static int has_rotated = 0;

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
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, 1);
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

	/* Setting callbacks */
	glfwSetErrorCallback(error_callback);
	glfwSetKeyCallback(window.window, key_callback);
	glfwSetWindowSizeCallback(window.window, window_size_callback);
	glfwSetWindowAspectRatio(window.window, 16, 9);
	glfwSetWindowSizeLimits(window.window, (window.mode->width/2), (window.mode->height/2), window.mode->width, window.mode->height);

	glfwSetScrollCallback (window.window, scroll_callback);
	/* * * * * * * * * * */

	/* 2D orthogonal projection */
	mat4 projection;
	glm_ortho(0.0f, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT, 0.0f, -1.0f, 1.0f, projection);
	/* * * * * * * * */
	/* * * * * * * * * * * * * * * * * */

	/* Images  */
	stbi_set_flip_vertically_on_load(true);
	crate = img_load_image("resources/images/smiley.png", GL_RGBA, 640.0f, 360.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f);

    glGenVertexArrays(1, &crate.VAO);
    glGenBuffers(1, &crate.VBO);
    glGenBuffers(1, &crate.EBO);

    glBindVertexArray(crate.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, crate.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(crate.rect.vertices), crate.rect.vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, crate.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(crate.indices), crate.indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

	//glUniform1i(glGetUniformLocation(sprite_shader.shader_program, "tex0"), 0);

	glBindTexture(GL_TEXTURE_2D, crate.texture);
	//glBindVertexArray(VAO);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* Main loop */
	while (!glfwWindowShouldClose(window.window)){
		crate.rect = image_update_image(crate);
    	glBufferData(GL_ARRAY_BUFFER, sizeof(crate.rect.vertices), crate.rect.vertices, GL_STATIC_DRAW);
		vec3 rotation_axis = {0.0f, 0.0f, 1.0f};
		if (has_rotated){
			has_rotated = 0;
			vec3 pivot = {crate.rect.x + crate.width/2, crate.rect.y + crate.height/2, 0.0f};
			glm_ortho(0.0f, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT, 0.0f, -1.0f, 1.0f, projection);
			glm_rotate_at(projection, pivot, glm_rad(crate.rect.rotation), rotation_axis);
		}
		// position attribute
    	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    	//glEnableVertexAttribArray(0);
    	// color attribute
    	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    	//glEnableVertexAttribArray(1);
    	// texture coord attribute
    	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    	//glEnableVertexAttribArray(2);
/*		crate.vertices[0] += 1.0f;
		crate.vertices[8] += 1.0f;
		crate.vertices[16] += 1.0f;
		crate.vertices[24] += 1.0f;
*/
		glfwPollEvents();
		glClearColor(GLColor32(20), GLColor32(20), GLColor32(20), GLColor32(255));
		glClear(GL_COLOR_BUFFER_BIT);

		/* Drawing */
		glUseProgram(crate.image_shader.shader_program);
		glUniform1i(glGetUniformLocation(crate.image_shader.shader_program, "texture1"), 0);
		glUniformMatrix4fv(glGetUniformLocation(crate.image_shader.shader_program, "projection"), 1, GL_FALSE, (const GLfloat *)projection);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		/* * * * * */

		glfwSwapBuffers(window.window);

	}
	/* * * * * * */

	/* Terminating */
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteVertexArrays(1, &crate.VAO);
	glDeleteBuffers(1, &crate.VBO);
	glDeleteBuffers(1, &crate.EBO);
	glDeleteProgram(crate.image_shader.shader_program);
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
				case GLFW_KEY_SPACE:
					if (crate.rect.R == 1.0f){
						crate.rect.R = 0.0f;
					}else{
						crate.rect.R = 1.0;
					}
					break;
				default:
					break;
			}
			break;
		case GLFW_REPEAT:
			switch(key){
				case GLFW_KEY_W:
					crate.rect.y -= 10;
					break;
				case GLFW_KEY_A:
					crate.rect.x -= 10;
					break;
				case GLFW_KEY_S:
					crate.rect.y += 10;
					break;
				case GLFW_KEY_D:
					crate.rect.x += 10;
					break;
				case GLFW_KEY_RIGHT:
					has_rotated = 1;
					crate.rect.rotation += 1.0f;
					break;
				case GLFW_KEY_LEFT:
					has_rotated = 1;
					crate.rect.rotation -= 1.0f;
					break;
				default:
					break;
			}
		default:
			break;
	}
}

void error_callback(int error, const char* description){
	fprintf(stderr, "[-] Error: %s\n", description);
}

void window_size_callback(GLFWwindow * window, int width, int height){
	glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
	//printf("%lf, %lf", xoffset, yoffset);
	switch((int)yoffset){
		case -1:
			crate.width -= 10;
			crate.height -= 10;
			break;
		case 1:
			crate.width += 10;
			crate.height += 10;
			break;
		default:
			break;
	}
}
