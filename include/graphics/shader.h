#pragma once

#include <glad/glad.h>
#include <glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;
using namespace glm;

class Shader
{
public:
    unsigned int ID;

    // Constructor
    Shader(const char *vertexPath, const char *fragmentPath);

    // Destructor
    ~Shader();

    // Public methods
    void Use();
    void SetBool(const string &name, bool value) const;
    void SetInt(const string &name, int value) const;
    void SetFloat(const string &name, float value) const;
    void SetVec2(const string &name, const vec2 &value) const;
    void SetVec2(const string &name, float x, float y) const;
    void SetVec3(const string &name, const vec3 &value) const;
    void SetVec3(const string &name, float x, float y, float z) const;
    void SetVec4(const string &name, const vec4 &value) const;
    void SetVec4(const string &name, float x, float y, float z, float w) const;
    void SetMat2(const string &name, const mat2 &mat) const;
    void SetMat3(const string &name, const mat3 &mat) const;
    void SetMat4(const string &name, const mat4 &mat) const;

private:
    // Private methods
    void CheckCompileErrors(unsigned int shader, string type);
};
