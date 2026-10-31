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
GLuint gVertexArrayObject = 0;
GLuint gVertexBufferObject = 0;
GLuint gGraphicsPipelineShaderProgram = 0;
GLuint gIndexBufferObject = 0;
bool gQuit = false;
float g_uOffset = -2.0f;
float g_uRotate = 0.0f;
float g_uScale = 0.5f;
Camera gCamera;

/** 
const std:: string gVertexShaderSource = 
       "#version 410 core\n"
       "in vec4 position;\n"
       "void main()\n"
       "{\n"
       "   gl_Position = vec4(position.x, position.y, position.z, position.w);\n"
       "}\n";

const std:: string gFragmentShaderSource =
       "#version 410 core\n"
       "out vec4 color;\n"
       "void main()\n"
       "{\n"
       "   color = vec4(1.0f, 0.5f, 0.0f, 1.0f);\n"
       "}\n";
*/
std::string LoadShaderAsString(const std:: string& filename){
    std::string result = "";
    std::string line = "";
    std::ifstream myFile(filename.c_str());
    if(myFile.is_open()){
        while(getline(myFile, line)){
            result += line + "\n";
        }
        myFile.close();
    }
    return result;
}

GLuint CompileShader(GLuint type, const std::string& source) {
    GLuint shaderObject;
    if(type == GL_VERTEX_SHADER) {
        shaderObject = glCreateShader(GL_VERTEX_SHADER);
    } else if(type == GL_FRAGMENT_SHADER) {
        shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
    } else {
        std::cout << "ERROR: unknown shader type" << std::endl;
        return 0;
    }

    const char* src = source.c_str();
    glShaderSource(shaderObject, 1, &src, nullptr);
    glCompileShader(shaderObject);

    GLint success;
    char infoLog[512];
    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shaderObject, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED" << infoLog << std::endl;
    }
    return shaderObject;
}

GLuint CreateShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) {
    GLuint programObject = glCreateProgram();
    GLuint myVertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint myFragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    
    glAttachShader(programObject, myVertexShader);
    glAttachShader(programObject, myFragmentShader);
    glLinkProgram(programObject);
    GLint success;
    char infoLog[512];
    glGetProgramiv(programObject, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(programObject, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::LINKING_FAILED" << infoLog << std::endl;
    }

    glValidateProgram(programObject);
    return programObject;
}

void CreateGraphicsPipeline() {
    std::string vertexShaderSource = LoadShaderAsString("shaders/vert.glsl");
    std::string fragmentShaderSource = LoadShaderAsString("shaders/frag.glsl");
    gGraphicsPipelineShaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
}

void GetOpenGLVersionInfo() {
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

void VertexSpecification() {
    const std::vector<GLfloat> vertexData = {
        // 0 vertex
        -0.5f, -0.5f, 0.0f,
         1.0f, 0.0f, 0.0f,
         //1 vertex
         0.5f, -0.5f, 0.0f,
         0.0f, 1.0f, 0.0f,
         //2 vertex
         -0.5f, 0.5f, 0.0f, 
         0.0f, 0.0f, 1.0f,
         // 3 vertex
         0.5f, 0.5f, 0.0f,
         0.0f, 0.0f, 1.0f
    };
    glGenVertexArrays(1, &gVertexArrayObject);
    glBindVertexArray(gVertexArrayObject);

    const std::vector<GLuint> indexBufferData = {2,0,1, 3, 2, 1};
    // start generating vertex buffer object
    glGenBuffers(1, &gVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(GLfloat), vertexData.data(), GL_STATIC_DRAW);


    glGenBuffers(1, &gIndexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferData.size() * sizeof(GLuint), indexBufferData.data(), GL_STATIC_DRAW);
     // now linking up the attributes in our vao
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*)(3 * sizeof(GLfloat)));

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void*)0);
    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
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
    glUseProgram(gGraphicsPipelineShaderProgram);
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, g_uOffset));

    model = glm::rotate(model, glm::radians(g_uRotate), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(g_uScale, g_uScale, g_uScale));
    // projection matrix in perspective

    glm::mat4 view = gCamera.GetViewMatrix();
    GLint u_ViewLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "u_ViewMatrix");
    if(u_ViewLocation >= 0){
        glUniformMatrix4fv(u_ViewLocation, 1, GL_FALSE, &view[0][0]);
    }else{
        std::cout << "u_ViewMatrix not found in shader" << std::endl;
        exit(EXIT_FAILURE);
    }
    glm::mat4 perspective = glm::perspective(glm::radians(45.0f), (float)gScreenWidth / (float)gScreenHeight, 0.1f, 10.0f);

    GLint u_ModelMatrixLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "u_ModelMatrix");
    if(u_ModelMatrixLocation >= 0){
        glUniformMatrix4fv(u_ModelMatrixLocation, 1, GL_FALSE, &model[0][0]);
    }else{
        std::cout << "u_ModelMatrix not found in shader" << std::endl;
        exit(EXIT_FAILURE);
    }
    // retrieve the location of the uniform variable in the shader program
    GLint u_ProjectionLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "u_Projection");
    if(u_ProjectionLocation >= 0){
        glUniformMatrix4fv(u_ProjectionLocation, 1, GL_FALSE, &perspective[0][0]);
    }else{
        std::cout << "u_Projection not found in shader" << std::endl;
        exit(EXIT_FAILURE);
    }
    glUseProgram(gGraphicsPipelineShaderProgram);
}

void Draw(){
    glBindVertexArray(gVertexArrayObject);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glUseProgram(0);
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
    SDL_DestroyWindow(gGraphicsApplicationWindow);
    SDL_Quit();

}

int main()
{
    InitializeProgram();
    VertexSpecification();
    CreateGraphicsPipeline();
    MainLoop();
    Cleanup();
    return 0;
}