#include "graphics/shader.h"

using namespace std;
using namespace glm;

// Constructor
Shader::Shader(const char *vertexPath, const char *fragmentPath)
    : ID(0)
{
    // Retrieve the vertex/fragment source code from filePath
    string vertexCode;
    string fragmentCode;
    ifstream vShaderFile;
    ifstream fShaderFile;

    // Ensure ifstream objects can throw exceptions
    vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
    fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);

    try
    {
        // Open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        stringstream vShaderStream, fShaderStream;

        // Read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        // Close file handlers
        vShaderFile.close();
        fShaderFile.close();

        // Convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (ifstream::failure &e)
    {
        cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << endl;
    }

    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();

    // Compile shaders
    unsigned int vertex, fragment;

    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    CheckCompileErrors(vertex, "VERTEX");

    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    CheckCompileErrors(fragment, "FRAGMENT");

    // Shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    CheckCompileErrors(ID, "PROGRAM");

    // Delete no longer needed shaders
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

// Destructor
Shader::~Shader()
{
    glDeleteProgram(ID);
}

// Activates the Shader
void Shader::Use()
{
    glUseProgram(ID);
}

// Utility uniform functions
void Shader::SetBool(const string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::SetInt(const string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(const string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetVec2(const string &name, const vec2 &value) const
{
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::SetVec2(const string &name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

void Shader::SetVec3(const string &name, const vec3 &value) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::SetVec3(const string &name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::SetVec4(const string &name, const vec4 &value) const
{
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::SetVec4(const string &name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

void Shader::SetMat2(const string &name, const mat2 &mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat3(const string &name, const mat3 &mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat4(const string &name, const mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::CheckCompileErrors(unsigned int shader, string type)
{
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                 << infoLog << "\n -- --------------------------------------------------- -- " << endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                 << infoLog << "\n -- --------------------------------------------------- -- " << endl;
        }
    }
}
