#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glew/GL/glew.h>
#include <glm/glm.hpp>

class Shader {

public:
	Shader(std::string vertexPath_, std::string fragmentPath_);
	~Shader();
	GLuint GetShaderProgram();
	void Use();
	void setMat4(const std::string &name, const glm::mat4 &mat) const;
	void setVec3(const std::string &name, float x, float y, float z) const;
	void setFloat(const std::string &name, float value) const;
	void setVec3(const std::string &name, const glm::vec3 &value) const;
	void setInt(const std::string &name, int value) const;

private:
	GLuint shaderProgram;
};

#endif // !SHADER_H


