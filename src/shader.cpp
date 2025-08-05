#include "vox.h"
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sys/stat.h>

Shader::Shader(GLuint type) { this->type = type; }

Shader &Shader::from_file(std::string path) {

    FILE *f = fopen(path.c_str(), "r");
    if (!f) {
        std::cerr << "Failed to open shader file: \"" << path << "\""
                  << std::endl;
        throw ShaderCreationException();
    }

    struct stat stats;
    if (stat(path.c_str(), &stats)) {

        std::cerr << "Cannot access file information: \"" << path << "\""
                  << std::endl;
        perror("stat()");
        throw ShaderCreationException();
    }

    char *buff = (char *)calloc(1, stats.st_size + 1);

    if (fread(buff, stats.st_size, 1, f) == -1) {
        std::cerr << "Cannot read shader file" << path << "\"" << std::endl;
        throw ShaderCreationException();
    }

    this->source = std::string(buff);
    free(buff);
    this->path = path;
    return *this;
}

Shader &Shader::compile() {

    this->id = glCreateShader(this->type);
    char *source = this->source.data();
    glShaderSource(this->id, 1, &source, NULL);
    glCompileShader(this->id);

    int success;
    char infoLog[512];
    glGetShaderiv(this->id, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(this->id, 512, NULL, infoLog);
        std::cerr << "Shader Compilation failed: " << this->path << std::endl;
        std::cerr << infoLog << std::endl;
        throw ShaderCreationException();
    }
    return *this;
}

Shader::~Shader() { 
}

// Shader program utility functions

ShaderProgram::ShaderProgram() { this->id = glCreateProgram(); }
void ShaderProgram::attach(Shader &s) { this->shaders.push_back(s); }
void ShaderProgram::use() { glUseProgram(this->id); }

int ShaderProgram::link() {
    for (auto it = this->shaders.begin(); it != this->shaders.end(); it++) {
        glAttachShader(this->id, it->id);
    }
    glLinkProgram(this->id);

    int success;
    char infoLog[512];
    glGetProgramiv(this->id, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(this->id, 512, NULL, infoLog);
        std::cout << "Shader program linking failed" << std::endl
                  << infoLog << std::endl;
        return ERROR;
    }
    return OK;
}
