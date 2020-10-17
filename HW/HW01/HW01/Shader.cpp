#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    ifstream vertexFile;
    ifstream fragmentFile;
    stringstream vertexSStream;
    stringstream fragmentSStream;
    
    vertexFile.open(vertexPath);
    fragmentFile.open(fragmentPath);
    
    vertexFile.exceptions(ifstream::failbit || ifstream::badbit);
    fragmentFile.exceptions(ifstream::failbit || ifstream::badbit);
    
    try
    {
        if(!vertexFile.is_open() || !fragmentFile.is_open())
        {
            logic_error ex("open file error");
            throw exception(ex);
        }
        
        vertexSStream << vertexFile.rdbuf();
        fragmentSStream << fragmentFile.rdbuf();
        
        vertexFile.close();
        fragmentFile.close();
        
        vertexString = vertexSStream.str();
        fragmentString = fragmentSStream.str();
        
        vertexSource = vertexString.c_str();
        fragmentSource = fragmentString.c_str();
        
        unsigned int vertex, fragment;
        
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vertexSource, NULL);
        glCompileShader(vertex);
        checkCompilerErrors(vertex, "VERTEX");
        
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fragmentSource, NULL);
        glCompileShader(fragment);
        checkCompilerErrors(fragment, "FRAGMENT");
        
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompilerErrors(ID, "PROGRAM");
        
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
    catch (const std::exception& ex)
    {
        cout << ex.what() << endl;
    }
}

void Shader::use()
{
    glUseProgram(ID);
}

void Shader::checkCompilerErrors(unsigned int ID, string type)
{
    int success;
    char infoLog[512];
    if (type != "PROGRAM")
    {
        glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(ID, 512, NULL, infoLog);
            cout << "shader compile error: " << infoLog << endl;
        }
    }
    else
    {
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            cout << "program linking error:" << infoLog << endl;
        }
        
    }
}
