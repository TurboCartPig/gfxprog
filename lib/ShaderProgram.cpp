#include "glove/ShaderProgram.h"
#include <iostream>
#include <fstream>

/**
 * Construct a shader program from multiple shader stages.
 *
 * The shaders stage is determent based on the file extension.
 * Eg. .vert for vertex shader and .geom for geometry shader.
 *
 * @note Does not handle being passed multiple shaders for the same stage.
 * @note Does not handle tessellation or compute shaders
 *
 * @param paths An array of paths to the shader source code.
 */
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
    program = glCreateProgram();
    for (const auto shader : shaders)
        glAttachShader(program, shader);

    glLinkProgram(program);

    // Get the link status
    GLint success;
    char  log[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    glGetProgramInfoLog(program, 512, nullptr, log);

    std::cout << "Linking Program: " << std::endl;
    if (success == GL_FALSE) {
        std::cout << "\tLinking status: " << (bool)success << std::endl;
        std::cout << "\tLinking log:\n" << log << std::endl;

        throw std::runtime_error("GL Error: Failed to link program");
    }

    for (const auto shader : shaders)
        glDeleteShader(shader);
}

ShaderProgram::~ShaderProgram() { glDeleteProgram(program); }

/**
 * Use this shader program for future graphics processing.
 */
void ShaderProgram::use() const { glUseProgram(program); }
