#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_SWIZZLE

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>

glm::mat4 camera(float Translate, glm::vec2 const& Rotate)
{
	glm::mat4 Projection = glm::perspective(glm::pi<float>() * 0.25f, 4.0f / 3.0f, 0.1f, 100.f);
	glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
	View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
	View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
	return Projection * View * Model;
}

int main(){
    glm::vec3 A(3.0f, 4.0f, 0.0f);
    glm::vec3 B(0.0f, 7.0f, 0.0f);
    glm::vec4 vertex(1.0f, 5.0f, 1.0f, 1.0f);
    glm::mat4 model(1.0f);
    glm::mat4 s = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f));
    glm::mat4 r = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 1, 0));
    glm::mat4 t = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, -2.0f)); 
    model = t*r*s;
   std::cout << glm::to_string(model[0]) << std::endl;
    std::cout << glm::to_string(model[1]) << std::endl;
    std::cout << glm::to_string(model[2]) << std::endl;
    std::cout << glm::to_string(model[3]) << std::endl;
    std::cout << '\n' << "our vertex in world space\n";
    std::cout << glm::to_string(model * vertex) << std::endl;
    return 0;
}