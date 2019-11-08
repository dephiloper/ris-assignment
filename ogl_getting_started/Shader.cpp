#include "Shader.h"

/**
* https://stackoverflow.com/a/2602258
*/
std::string Shader::readFile(const std::string& filepath) {
    std::ifstream stream;
    stream.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    std::stringstream buffer;
    try {
        stream.open(filepath);
        buffer << stream.rdbuf();
        stream.close();
    } catch(std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << filepath << std::endl;
    }
    return buffer.str();
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexSource = readFile(vertexPath);
    std::string fragmentSource = readFile(fragmentPath);
    const char* vShaderSource = vertexSource.c_str(); // https://stackoverflow.com/a/30804288
    const char* fShaderSource= fragmentSource.c_str();

    // compile the shader
    unsigned int vertex, fragment;

    // shader creation and compilation
    vertex = createShader(vShaderSource, GL_VERTEX_SHADER, "VERTEX");
    fragment = createShader(fShaderSource, GL_FRAGMENT_SHADER, "FRAGMENT");

    // shader program creation and linking
    ID = glCreateProgram();

    if (ID == 0) std::cout << "Error occurred while creating shader program." << std::endl;

    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    int success;
    char infoLog[512];

    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(ID, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

}

void Shader::use() {
    glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

unsigned int Shader::createShader(const char* shaderSource, GLint shaderType, const std::string& shaderName) {
    int success;
    char infoLog[512];

    unsigned int shaderId;
    // vertex shader creation and compilation
    shaderId = glCreateShader(shaderType);
    glShaderSource(shaderId, 1, &shaderSource, nullptr);
    glCompileShader(shaderId);

    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shaderId, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::" << shaderName << "::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return shaderId;
}
