#ifndef SHADER_H

#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
    // the program ID
    unsigned int ID;
    
    // constructor reads and builds the shader
    int load(const std::string& vertexPath, const std::string& fragmentPath);

    // use/activate the shader
    void use();

    // utility uniform functions
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, glm::mat4 value) const;

private:
    static std::string readFile(const std::string& filepath);
    static unsigned int createShader(const char* shaderSource, GLint shaderType, const std::string& shaderName);
};

#endif //SHADER_H
