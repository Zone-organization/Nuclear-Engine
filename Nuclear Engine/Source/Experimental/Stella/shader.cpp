#include "stella/graphics/shader.h"
#include "stella/fileutils.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace stella {
namespace graphics {
Shader::Shader(const char *vsPath, const char *fsPath)
    : VsPath(vsPath), FsPath(fsPath) {
  this->ShaderProgram = this->load();
}

Shader::~Shader() { glDeleteProgram(this->ShaderProgram); }

void Shader::Enable() { glUseProgram(this->ShaderProgram); }

void Shader::Disable() { glUseProgram(0); }

GLuint Shader::load() {
  std::string vertexSourceStr = FileUtils::read_file(this->VsPath);
  const char *vertexSource = vertexSourceStr.c_str();
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSource, nullptr);
  glCompileShader(vertexShader);

  GLint success;
  GLchar infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
    std::cout << "Vertex Shader compilation failed!\n" << infoLog << std::endl;
  }

  std::string fragmentSourceStr = FileUtils::read_file(this->FsPath);
  const char *fragmentSource = fragmentSourceStr.c_str();
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
    std::cout << "Fragment Shader compilation failed!\n"
              << infoLog << std::endl;
  }

  GLuint program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  glGetProgramiv(program, GL_LINK_STATUS, &success);

  if (!success) {
    glGetProgramInfoLog(program, 512, nullptr, infoLog);
    std::cout << "Program linking failed!\n" << infoLog << std::endl;
  }

  glDeleteShader(fragmentShader);
  glDeleteShader(vertexShader);

  return program;
}

// Uniform settings
void Shader::SetFloat(const GLchar *uniformName, const GLfloat &num) {
  glUniform1f(glGetUniformLocation(this->ShaderProgram, uniformName), num);
}

void Shader::SetInt(const GLchar *uniformName, const GLint &num) {
  glUniform1i(glGetUniformLocation(this->ShaderProgram, uniformName), num);
}

void Shader::SetIntv(const GLchar *uniformName, GLint *array, int count) {
  glUniform1iv(glGetUniformLocation(this->ShaderProgram, uniformName), count,
               array);
}

void Shader::SetVec2f(const GLchar *uniformName, const glm::vec2 &vec) {
  glUniform2f(glGetUniformLocation(this->ShaderProgram, uniformName), vec.x,
              vec.y);
}

void Shader::SetVec2f(const GLchar *uniformName, const GLfloat &x,
                      const GLfloat &y) {
  glUniform2f(glGetUniformLocation(this->ShaderProgram, uniformName), x, y);
}

void Shader::SetVec3f(const GLchar *uniformName, const glm::vec3 &vec) {
  glUniform3f(glGetUniformLocation(this->ShaderProgram, uniformName), vec.x,
              vec.y, vec.z);
}

void Shader::SetVec3f(const GLchar *uniformName, const GLfloat &x,
                      const GLfloat &y, const GLfloat &z) {
  glUniform3f(glGetUniformLocation(this->ShaderProgram, uniformName), x, y, z);
}

void Shader::SetVec4f(const GLchar *uniformName, const glm::vec4 &vec) {
  glUniform4f(glGetUniformLocation(this->ShaderProgram, uniformName), vec.x,
              vec.y, vec.z, vec.w);
}

void Shader::SetVec4f(const GLchar *uniformName, const GLfloat &x,
                      const GLfloat &y, const GLfloat &z, const GLfloat &w) {
  glUniform4f(glGetUniformLocation(this->ShaderProgram, uniformName), x, y, z,
              w);
}

void Shader::SetMat4(const GLchar *uniformName, const glm::mat4 &mat) {
  glUniformMatrix4fv(glGetUniformLocation(this->ShaderProgram, uniformName), 1,
                     GL_FALSE, glm::value_ptr(mat));
}

} // namespace graphics
} // namespace stella
