#include <SDL2/SDL.h>
#include <iostream>
#include <glad/glad.h>
#include <vector>
#include <fstream>
#include <string>
#include <glm/glm.hpp>
#include <glm/mat4x2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "camera.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include <memory>

static void GLClearError() {
    while(glGetError() != GL_NO_ERROR);
}

static bool GLCheckErrorStatus(const char* function, const char* file, int line) {
    while(GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << "): " << function <<
        " " << file << ":" << line << std::endl;
        return true;
    }
    return false;
}

//global variables
int gScreenHeight = 640;
int gScreenWidth = 480;
SDL_Window*  gGraphicsApplicationWindow = nullptr;
SDL_GLContext gOpenGLContext = nullptr;
bool gQuit = false;
float g_uOffset = -2.0f;
float g_uRotate = 0.0f;
float g_uScale = 0.5f;
Camera gCamera;
std::unique_ptr<Shader> gShader;
std::unique_ptr<Mesh> gMesh;

void GetOpenGLVersionInfo() {
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

void InitializeProgram() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    gGraphicsApplicationWindow = SDL_CreateWindow("OpenGL Window", 0, 0, gScreenWidth, gScreenHeight, SDL_WINDOW_OPENGL);
    if(!gGraphicsApplicationWindow) {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    gOpenGLContext = SDL_GL_CreateContext(gGraphicsApplicationWindow);
    if(!gOpenGLContext) {
        std::cout << "OpenGL context could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    if(!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(1);
    }
    GetOpenGLVersionInfo();
}

void Input(){
    SDL_Event e;
    while(SDL_PollEvent(&e) != 0) {
        if(e.type == SDL_QUIT) {
            std::cout << "Goodbye!" << std::endl;
            gQuit = true;
        }
    }
// retrieve keyboard state
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    if(state[SDL_SCANCODE_UP]){
      g_uOffset += 0.001f;
      std::cout << "g_uOffset: " << g_uOffset << std::endl;
    }
    if(state[SDL_SCANCODE_DOWN]){
      g_uOffset -= 0.001f;
      std::cout << "g_uOffset: " << g_uOffset << std::endl;
    }
    if(state[SDL_SCANCODE_LEFT]){
      g_uRotate -= 0.01f;
      std::cout << "g_uRotate: " << g_uRotate << std::endl;
    }
    if(state[SDL_SCANCODE_RIGHT]){
      g_uRotate += 0.01f;
      std::cout << "g_uRotate: " << g_uRotate << std::endl;
    }
}

void PreDraw(){
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glViewport(0, 0, gScreenWidth, gScreenHeight);
    glClearColor(1.f, 1.f, 0.1f, 1.f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    gShader->Use();
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, g_uOffset));

    model = glm::rotate(model, glm::radians(g_uRotate), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(g_uScale, g_uScale, g_uScale));
    // projection matrix in perspective

    glm::mat4 view = gCamera.GetViewMatrix();
    GLint u_ViewLocation = gShader->GetUniformLocation("u_ViewMatrix");
    if(u_ViewLocation >= 0){
        glUniformMatrix4fv(u_ViewLocation, 1, GL_FALSE, &view[0][0]);
    }else{
        std::cout << "u_ViewMatrix not found in shader" << std::endl;
        exit(EXIT_FAILURE);
    }
    glm::mat4 perspective = glm::perspective(glm::radians(45.0f), (float)gScreenWidth / (float)gScreenHeight, 0.1f, 10.0f);

    GLint u_ModelMatrixLocation = gShader->GetUniformLocation("u_ModelMatrix");
    if(u_ModelMatrixLocation >= 0){
        glUniformMatrix4fv(u_ModelMatrixLocation, 1, GL_FALSE, &model[0][0]);
    }else{
        std::cout << "u_ModelMatrix not found in shader" << std::endl;
        exit(EXIT_FAILURE);
    }
    // retrieve the location of the uniform variable in the shader program
    GLint u_ProjectionLocation = gShader->GetUniformLocation("u_Projection");
    if(u_ProjectionLocation >= 0){
        glUniformMatrix4fv(u_ProjectionLocation, 1, GL_FALSE, &perspective[0][0]);
    }else{
        std::cout << "u_Projection not found in shader" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Draw(){
    gMesh->Draw();
}


void MainLoop() {
    while(!gQuit){
        Input();
        PreDraw();
        Draw();
        SDL_GL_SwapWindow(gGraphicsApplicationWindow);
    }

}

void Cleanup() {
    gShader.reset();
    gMesh.reset();
    SDL_GL_DeleteContext(gOpenGLContext);
    SDL_DestroyWindow(gGraphicsApplicationWindow);
    SDL_Quit();
}

int main() {
    InitializeProgram();
    gShader = std::make_unique<Shader>("shaders/vert.glsl", "shaders/frag.glsl");
    gMesh = std::make_unique<Mesh>(
        std::vector<float>{
            -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
            -0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
            0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f
        },
        std::vector<unsigned int>{2, 0, 1, 3, 2, 1}
    );
    MainLoop();
    Cleanup();
    return 0;
}