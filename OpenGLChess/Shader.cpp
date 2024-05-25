
#pragma once

#include"glew.h"
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include "Shader.h"

#include <vector>
Shader::Shader(const char* vertexFilePath, const char* fragmentFilePath) {
        std::string vertexSource, fragmentSource;
        std::ifstream vertexFile, fragmentFile;
        vertexFile.exceptions(std::ifstream::failbit | std::fstream::badbit);
        fragmentFile.exceptions(std::ifstream::failbit | std::fstream::badbit);
        try {
            std::stringstream vertexStream, fragmentStream;

            unsigned int vertexID = CompileShader(vertexFilePath, GL_VERTEX_SHADER);
            unsigned int fragmentID = CompileShader(fragmentFilePath, GL_FRAGMENT_SHADER);

            shaderProgramID = LinkShader(vertexID, fragmentID);
        }
        catch (std::ifstream::failure& e) {
            std::cout << "ERROR::FILE_NOT_READ" << std::endl;
        }
}


 void Shader::Use()const {
    glUseProgram(shaderProgramID);
}

inline void Shader::ShaderError(unsigned int ID, GLenum shaderType) {
    int success;
    char infoLog[512];
    glGetShaderiv(shaderProgramID, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderProgramID, 512, nullptr, infoLog);
        std::cout
            << "ERROR::SHADER::COMPILATION of type: "
            << (shaderType == GL_VERTEX_SHADER ? "VERTEX: " : "FRAGMENT: ")
            << infoLog << std::endl;
    }
}

inline void Shader::ProgramError(unsigned int ID) {
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgramID, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::LINKING : " << infoLog;
    }
}

inline unsigned int Shader::CompileShader(const char* shaderPath, GLenum shaderType) {
    std::string shaderSource;
    std::ifstream shaderFile;
    std::stringstream shaderStream;
    
    shaderFile.open(shaderPath);
    shaderStream << shaderFile.rdbuf();
    shaderFile.close();

    shaderSource = shaderStream.str();
    const char* shaderCode = shaderSource.c_str();

    unsigned int shaderID = glCreateShader(shaderType);
    glShaderSource(shaderID, 1, &shaderCode, nullptr);
    glCompileShader(shaderID);
    ShaderError(shaderID, shaderType);
    return shaderID;
}

inline unsigned int Shader::LinkShader(unsigned int vertexID, unsigned int fragmentID) {
    unsigned int programID = glCreateProgram();
    glAttachShader(programID, vertexID);
    glAttachShader(programID, fragmentID);
    glLinkProgram(programID);
    ProgramError(programID);

    glDeleteShader(vertexID);
    glDeleteShader(fragmentID);
    return programID;
}
  void Shader::setBool(const std::string &name, bool value) const
    {         
        glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), (int)value); 
    }
    // ------------------------------------------------------------------------
    void Shader::setInt(const std::string &name, int value) const
    { 
        glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    void Shader::setFloat(const std::string &name, float value) const
    { 
        glUniform1f(glGetUniformLocation(shaderProgramID, name.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
    { 
        glUniform2fv(glGetUniformLocation(shaderProgramID, name.c_str()), 1, &value[0]); 
    }
    void Shader::setVec2(const std::string &name, float x, float y) const
    { 
        glUniform2f(glGetUniformLocation(shaderProgramID, name.c_str()), x, y); 
    }
    // ------------------------------------------------------------------------
    void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
    { 
        glUniform3fv(glGetUniformLocation(shaderProgramID, name.c_str()), 1, &value[0]); 
    }
    void Shader::setVec3(const std::string &name, float x, float y, float z) const
    { 
        glUniform3f(glGetUniformLocation(shaderProgramID, name.c_str()), x, y, z); 
    }
    // ------------------------------------------------------------------------
    void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
    { 
        glUniform4fv(glGetUniformLocation(shaderProgramID, name.c_str()), 1, &value[0]); 
    }
    void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const
    { 
        glUniform4f(glGetUniformLocation(shaderProgramID, name.c_str()), x, y, z, w); 
    }
    // ------------------------------------------------------------------------
    void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(shaderProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(shaderProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    void Shader::setLights(std::vector<Light> lights ) const
    {

        GLuint lightn = glGetUniformLocation(shaderProgramID, "numberOfLights");
        glUniform1i(lightn,lights.size());
        int i = 0;
        for (auto const&  light : lights)
        {
            std::string prop= "lights[";
            prop = prop + std::to_string(i);
            prop = prop + "].intensity";
            GLuint loc = glGetUniformLocation(shaderProgramID,(prop).c_str());
            glUniform1f(loc, light.intensity);

            prop = "lights[";
            prop = prop + std::to_string(i);
            prop = prop + "].ambient";
            loc = glGetUniformLocation(shaderProgramID, (prop).c_str());
            glUniform1f(loc, light.ambient);


            prop = "lights[";
            prop = prop + std::to_string(i);
            prop = prop + "].pos";
            loc = glGetUniformLocation(shaderProgramID, (prop).c_str());
            glUniform3f(loc, light.pos.x, light.pos.y, light.pos.z);



             prop = "lights[";
            prop = prop + std::to_string(i);
            prop = prop + "].color";
            loc = glGetUniformLocation(shaderProgramID, (prop).c_str());
            glUniform3f(loc, light.color.x, light.color.y, light.color.z);

           
            i++;
        }
       
    }
