#include "glove/ShaderProgram.h"
#include <iostream>
#include <fstream>

ShaderProgram::ShaderProgram(const std::vector<std::string> &paths) {
    // Load all shaders from disk and compile them
    std::vector<GLuint> shaders;

    for (const auto &path : paths) {
        // Open the file for reading
        std::ifstream file(path, std::ifstream::in);
        if (file.bad() || !file.is_open()) {
            throw std::runtime_error("Error: Failed to open file.");
        }

        // Read in the file line by line
        std::string source;
        std::string line;
        while (getline(file, line)) {
            source.append(line + "\n");
        }
        // Why can I not do this?
        // file >> source;
        file.close();

        // Find the shader type
        GLenum type;
        if (path.find(".vert") != std::string::npos)
            type = GL_VERTEX_SHADER;
        else if (path.find(".geom") != std::string::npos)
            type = GL_GEOMETRY_SHADER;
        else if (path.find(".frag") != std::string::npos)
            type = GL_FRAGMENT_SHADER;
        else {
            std::cout << "Error: Shader with unknown file extension: " << path
                      << std::endl;
            throw std::runtime_error("GL Error: Unknown file extension.");
        }

        // Compile the shader
        const char *const cstring = source.c_str();
        GLuint            shader  = glCreateShader(type);
        glShaderSource(shader, 1, &cstring, nullptr);
        glCompileShader(shader);

        // Get the compile status
        GLint success;
        char  log[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        glGetShaderInfoLog(shader, 512, nullptr, log);

        std::cout << "Compiling shader: " << path << std::endl;
        if (success == GL_FALSE) {
            std::cout << "\tShader type: " << (int)type << std::endl;
            std::cout << "\tSource code:\n" << source << std::endl;
            std::cout << "\tCompilation status: " << (bool)success << std::endl;
            std::cout << "\tCompilation log:\n" << log << std::endl;

            throw std::runtime_error("GL Error: Shader failed to compile");
        }

        shaders.push_back(shader);
    }

    // Build and link a shader program
    m_program = glCreateProgram();
    for (const auto shader : shaders)
        glAttachShader(m_program, shader);

    glLinkProgram(m_program);

    // Get the link status
    GLint success;
    char  log[512];
    glGetProgramiv(m_program, GL_LINK_STATUS, &success);
    glGetProgramInfoLog(m_program, 512, nullptr, log);

    std::cout << "Linking Program: " << std::endl;
    if (success == GL_FALSE) {
        std::cout << "\tLinking status: " << (bool)success << std::endl;
        std::cout << "\tLinking log:\n" << log << std::endl;

        throw std::runtime_error("GL Error: Failed to link program");
    }

    for (const auto shader : shaders)
        glDeleteShader(shader);
}

ShaderProgram::~ShaderProgram() { glDeleteProgram(m_program); }

void ShaderProgram::use() { glUseProgram(m_program); }

void ShaderProgram::setUniform(const std::string &name, const float x) {
	auto loc = glGetUniformLocation(m_program, name.c_str());
	glUniform1f(loc, x);
}

void ShaderProgram::setUniform(const std::string &name, const glm::vec2 v) {
    auto loc = glGetUniformLocation(m_program, name.c_str());
    glUniform2f(loc, v.x, v.y);
}

void ShaderProgram::setUniform(const std::string &name, const glm::vec3 v) {
    auto loc = glGetUniformLocation(m_program, name.c_str());
    glUniform3f(loc, v.x, v.y, v.z);
}

void ShaderProgram::setUniform(const std::string &name, const glm::vec4 v) {
    auto loc = glGetUniformLocation(m_program, name.c_str());
    glUniform4f(loc, v.x, v.y, v.z, v.w);
}
