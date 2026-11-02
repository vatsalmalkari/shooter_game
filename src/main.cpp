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
#include "Texture.hpp"
#include <chrono>
#include "player.hpp"
#include "Game.hpp"
#include "enemy.hpp"


//global variables
int gScreenHeight = 640;
int gScreenWidth = 480;
SDL_Window*  gGraphicsApplicationWindow = nullptr;
SDL_GLContext gOpenGLContext = nullptr;
bool gQuit = false;
std::unique_ptr<Shader> gShader;
std::unique_ptr<Texture> gTexture;
std::unique_ptr<Game> gGame;
std::unique_ptr<Camera> gCamera;

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
}

void PreDraw() {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glViewport(0, 0, gScreenWidth, gScreenHeight);
    glClearColor(1.f, 1.f, 0.1f, 1.f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    gShader->Use();
    glm::mat4 view = gCamera->GetViewMatrix();
    glUniformMatrix4fv(gShader->GetUniformLocation("u_ViewMatrix"), 1, GL_FALSE, &view[0][0]);
    glUniform1i(gShader->GetUniformLocation("u_Texture"), 0);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f),(float)gScreenWidth / (float)gScreenHeight, 0.1f, 10.0f);
    glUniformMatrix4fv(gShader->GetUniformLocation("u_Projection"), 1, GL_FALSE, &projection[0][0]);
}

void Draw(){
    gGame->Draw();
}

void Update(float deltaTime) {
    gGame->Update(deltaTime);
}

void MainLoop() {
    using Clock = std::chrono::high_resolution_clock;
    const float fixedTimeStep = 1.0f / 60.0f;
    auto lastFrameTime = Clock::now();
    float accumulator = 0.0f;
    while (!gQuit) {
        auto currentTime = Clock::now();
        float frameTime = std::chrono::duration<float>(currentTime - lastFrameTime).count();
        lastFrameTime = currentTime;
        if (frameTime > 0.25f) {
            frameTime = 0.25f;
        }
        accumulator += frameTime;
        Input();
        while (accumulator >= fixedTimeStep) {
            Update(fixedTimeStep);
            accumulator -= fixedTimeStep;
        }
        PreDraw();
        Draw();
        SDL_GL_SwapWindow(gGraphicsApplicationWindow);
    }
}

  void Cleanup() {
    gGame.reset();
    gTexture.reset();
    gShader.reset();
    gCamera.reset();
    SDL_GL_DeleteContext(gOpenGLContext);
    SDL_DestroyWindow(gGraphicsApplicationWindow);
    SDL_Quit();
  }

int main() {
    InitializeProgram();

    gCamera = std::make_unique<Camera>();
    gShader = std::make_unique<Shader>("shaders/vert.glsl", "shaders/frag.glsl");
    gTexture = std::make_unique<Texture>("assets/screenshot.png");

    gGame = std::make_unique<Game>();
    gGame->SetShader(*gShader);
    auto player = std::make_unique<Player>(*gShader, *gTexture);
    gGame->SetPlayer(player.get());
    gGame->AddEntity(std::move(player));

    MainLoop();
    Cleanup();
    return 0;
}