#include "../include/logs.h"
#include "../include/image.h"
#include <cglm/affine.h>
#include <cglm/util.h>

#if __WIN32
Image image_load_image(HANDLE hConsole, WORD saved_attributes, GLchar * path){
#else
Image image_load_image(GLchar * path){
#endif
    Image img;
    img.path = path;
    img.data = stbi_load(img.path, &img.width, &img.height, &img.channels, 0);
    if (img.data){
        glGenTextures(1, &img.texture);
        glBindTexture(GL_TEXTURE_2D, img.texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.width, img.height, 0, GL_RGB, GL_UNSIGNED_BYTE, img.data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        GLfloat border_color[] = {0.0f, 0.0f, 0.0f, 1.0f};
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);
        glBindTexture(GL_TEXTURE_2D, 0);
        log_log(LOG_SUCCESS, "Successfully loaded image : {%s}", img.path);
    }else{
        log_log(LOG_ERROR, "Failed loading image : {%s}", img.path);
    }
    stbi_image_free(img.data);
    return img;
}


void image_init(GLuint VAO){
    // configure VAO/VBO
    unsigned int VBO;
    float vertices[] = { 
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 
    
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), NULL);
    glBindBuffer(GL_ARRAY_BUFFER, 0);  
    glBindVertexArray(0);
}

#define DEBUG fprintf(stdout, "[DEBUG] LINE: [%d] FUNCTION : [%s]\n", __LINE__, __func__);


void draw_image(GLuint * VAO, GLuint * shader_program, GLuint * texture, vec2 position, vec2 size, float rotate, vec3 color){
    // prepare transformations
    glUseProgram(*shader_program);
    mat4 model = {1.0f, 1.0f, 1.0f, 1.0f};

    vec3 model_pos = {position[0], position[1], 0.0f};
    DEBUG;

    glm_translate(model, model_pos);

    vec3 new_model_pos = {0.5f * size[0], 0.5f * size[1], 0.0f};
    glm_translate(model, new_model_pos);

    vec3 model_rotation = {0.0f, 0.0f, 1.0f};
    glm_rotate(model, glm_rad(rotate), model_rotation);
    DEBUG;
    
    vec3 new_new_model_pos = {-0.5f * size[0], -0.5f * size[1], 0.0f};
    glm_translate(model, new_new_model_pos);

    vec3 model_scale = {size[0], size[1], 1.0f};
    glm_scale(model, model_scale);
    DEBUG;

    glUniformMatrix4fv(glGetUniformLocation(*shader_program, "model"), 1, GL_FALSE, (const GLfloat *)&model);
    DEBUG;
    glUniform3fv(glGetUniformLocation(*shader_program, "spriteColor"), 1, (const GLfloat *)&color);
    DEBUG;
  
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, *texture);

    glBindVertexArray(*VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    DEBUG;
    glBindVertexArray(0);
    DEBUG;

}