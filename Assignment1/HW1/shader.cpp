/*
**** DO NOT MODIFY THIS FILE ****

- Contains shader class which handles shader compilation, and attaching uniforms
- You will not need to look here for the homework1.
*/

#include "shader.h"
template<>
void Shader::attachToLoc(const GLint & loc, const int & value)
{
	glUniform1i(loc, value);
	getError("attachToUniform<int>");
}

template<>
void Shader::attachToLoc(const GLint & loc, const float & value)
{
	glUniform1f(loc, value);
	getError("attachToUniform<int>");
}

template<>
void Shader::attachToLoc(const GLint & loc, const glm::mat4 & value)
{
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
	getError("attachToUniform<glm::mat>");
}

template<>
void Shader::attachToLoc(const GLint & loc, const glm::vec3 & value)
{
	glUniform3f(loc, value.x, value.y, value.z);
	getError("attachToUniform<glm::vec3>");
}
