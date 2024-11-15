#pragma once
#include"glew.h"
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat3x3.hpp"
#include "Light.h"
#include <vector>
class Shader {
public:
    unsigned int shaderProgramID;

    void Use() const;

    Shader(const char* vertexFilePath, const char* fragmentFilePath);
    
    Shader(const char* vertexFilePath, const char* geomtryFilePath, const char* fragmentFilePath);

    inline void ShaderError(unsigned int ID, GLenum shaderType);
    inline void ProgramError(unsigned int ID);

    inline unsigned int CompileShader(const char* shaderPath, GLenum shaderType);

    inline unsigned int LinkShader(unsigned int vertexID, unsigned int fragmentID);

    inline unsigned int LinkShader(unsigned int vertexID, unsigned int geomtryID, unsigned int fragmentID);
    void setBool(const std::string& name, bool value) const;
    // ------------------------------------------------------------------------
    void setInt(const std::string& name, int value) const;
    // ------------------------------------------------------------------------
    void setFloat(const std::string& name, float value) const;
    // ------------------------------------------------------------------------
    void setVec2(const std::string& name, const glm::vec2& value) const;
    void setVec2(const std::string& name, float x, float y) const;
    // ------------------------------------------------------------------------
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec3(const std::string& name, float x, float y, float z) const;
    // ------------------------------------------------------------------------
    void setVec4(const std::string& name, const glm::vec4& value) const;
    void setVec4(const std::string& name, float x, float y, float z, float w) const;
    // ------------------------------------------------------------------------
    void setMat2(const std::string& name, const glm::mat2& mat) const;
    // ------------------------------------------------------------------------
    void setMat3(const std::string& name, const glm::mat3& mat) const;
    // ------------------------------------------------------------------------
    void setMat4(const std::string& name, const glm::mat4& mat) const;

    void setLights(std::vector<Light> lights)const;

};



