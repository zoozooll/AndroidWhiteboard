#include <jni.h>
#include <string>
#include <GLES3/gl32.h>

#include "AssetHelper.h"
#include "logutil.h"
#include <atomic>

static GLuint paintProgram = 0;
static GLuint eraserProgram = 0;
static GLuint prevFbo = 0;
static GLuint boundFboTexture = 0;
static GLuint canvasTexture = 0;

static int scrWidth;
static int scrHeight;
static unsigned int VAO = 0;
static std::atomic_bool isEraser(false);

extern "C"
JNIEXPORT void JNICALL
Java_com_mouselee_androidwhiteboard_Renderer_init(JNIEnv *env, jobject thiz) {
    char* source;
    size_t length;
    loadDataFromAsset("shaders/canvas_editor.vert", (void**)&source, &length);
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    if (vertShader != 0)
    {
        int srcLength = (int)length;
        glShaderSource(vertShader, 1, &source, &srcLength);
        glCompileShader(vertShader);
        int compiled = 0;
        glGetShaderiv(vertShader, GL_COMPILE_STATUS, &compiled);
        if (compiled == 0)
        {
            char logString[1024];
            int logLength;
            glGetShaderInfoLog(vertShader, 1024, &logLength, logString);
            LOGE("aaron", "error in canvas_editor.vert : %s", logString);
            glDeleteShader(vertShader);
            vertShader = 0;
        }
    }
    delete[] source;

    loadDataFromAsset("shaders/canvas_editor_paint.frag", (void**)&source, &length);
    GLuint paintShader = glCreateShader(GL_FRAGMENT_SHADER);
    if (paintShader != 0)
    {
        int srcLength = (int)length;
        glShaderSource(paintShader, 1, &source, &srcLength);
        glCompileShader(paintShader);
        int compiled = 0;
        glGetShaderiv(paintShader, GL_COMPILE_STATUS, &compiled);
        if (compiled == 0)
        {
            char logString[1024];
            int logLength;
            glGetShaderInfoLog(vertShader, 1024, &logLength, logString);
            LOGE("aaron", "error in canvas_editor_paint.frag : %s", logString);
            glDeleteShader(paintShader);
            paintShader = 0;
        }
    }
    delete[] source;

    loadDataFromAsset("shaders/canvas_editor_eraser.frag", (void**)&source, &length);
    GLuint eraserShader = glCreateShader(GL_FRAGMENT_SHADER);
    if (eraserShader != 0)
    {
        int srcLength = (int)length;
        glShaderSource(eraserShader, 1, &source, &srcLength);
        glCompileShader(eraserShader);
        int compiled = 0;
        glGetShaderiv(eraserShader, GL_COMPILE_STATUS, &compiled);
        if (compiled == 0)
        {
            char logString[1024];
            int logLength;
            glGetShaderInfoLog(vertShader, 1024, &logLength, logString);
            LOGE("aaron", "error in canvas_editor_paint.frag : %s", logString);
            glDeleteShader(eraserShader);
            eraserShader = 0;
        }
    }
    delete[] source;

    paintProgram = glCreateProgram();
    if (paintProgram) {
        glAttachShader(paintProgram, vertShader);
        glAttachShader(paintProgram, paintShader);
        glLinkProgram(paintProgram);
        int linkStatus = 0;
        glGetProgramiv(paintProgram, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE)
        {
            glDeleteProgram(paintProgram);
            paintProgram = 0;
        }
    }

    eraserProgram = glCreateProgram();
    if (eraserProgram) {
        glAttachShader(eraserProgram, vertShader);
        glAttachShader(eraserProgram, eraserShader);
        glLinkProgram(eraserProgram);
        int linkStatus = 0;
        glGetProgramiv(eraserProgram, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE)
        {
            glDeleteProgram(eraserProgram);
            eraserProgram = 0;
        }
    }

    glDeleteShader(vertShader);
    glDeleteShader(paintShader);
    glDeleteShader(eraserShader);

    glGenFramebuffers(1, &prevFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, prevFbo);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        LOGE("aaron", "ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
        glDeleteFramebuffers(0, &prevFbo);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
            // positions                   // texture coords
            1.f,  1.f, 0.f,    1.f, 1.f, // top right
            1.f, -1.f, 0.f,    1.f, 0.f, // bottom right
            -1.f, -1.f, 0.f,    0.f, 0.f, // bottom left
            -1.f,  1.f, 0.f,    0.f, 1.f  // top left
    };
    unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
    };
    unsigned int VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
//    glUseProgram(paintProgram); // don't forget to activate/use the shader before setting uniforms!
    // either set it manually like so:
//    glUniform1i(glGetUniformLocation(paintProgram, "canvasTexture"), 0);
    // or set it via the texture class
//    glUniform1i(glGetUniformLocation(paintProgram, "prevTexture"), 1);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_mouselee_androidwhiteboard_Renderer_sizeChanged(JNIEnv *env, jobject thiz, jint width,
                                                         jint height) {
    scrWidth = width;
    scrHeight = height;
    glBindFramebuffer(GL_FRAMEBUFFER, prevFbo);
    glGenTextures(1, &boundFboTexture);
    glBindTexture(GL_TEXTURE_2D, boundFboTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, boundFboTexture, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_mouselee_androidwhiteboard_Renderer_update(JNIEnv *env, jobject thiz) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//    glClearColor(0.0F, 1.0f, 0.0f, 1.0f);
//    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, scrWidth, scrHeight);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, canvasTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, boundFboTexture);

    // render container
    GLuint curProgram;
    if (!isEraser) {
        curProgram = paintProgram;
    } else {
        curProgram = eraserProgram;
    }
    glUseProgram(curProgram);
    // either set it manually like so:
    glUniform1i(glGetUniformLocation(curProgram, "canvasTexture"), 0);
    // or set it via the texture class
    glUniform1i(glGetUniformLocation(curProgram, "prevTexture"), 1);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

//    glBindFramebuffer(GL_FRAMEBUFFER, prevFbo);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, prevFbo);
    glBlitFramebuffer(0, 0, scrWidth, scrHeight, 0, 0, scrWidth, scrHeight,
                      GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_mouselee_androidwhiteboard_Renderer_finish(JNIEnv *env, jobject thiz) {
    glDeleteProgram(paintProgram);
    glDeleteProgram(eraserProgram);
    glDeleteFramebuffers(1, &prevFbo);
    glDeleteTextures(1, &boundFboTexture);
    glDeleteVertexArrays(1, &VAO);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_mouselee_androidwhiteboard_Renderer_updateCanvasTexture(JNIEnv *env, jobject thiz,
    jint texture, jboolean eraserMode) {
    if (canvasTexture) {
        glDeleteTextures(1, &canvasTexture);
    }
    canvasTexture = texture;
    isEraser = eraserMode;
}