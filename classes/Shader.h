#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader
{
public:
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath);
	void use();
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const; 
	void setFloat(const std::string& name, float value) const;
	void setMatrix2(const std::string& name, const glm::mat2 &mat) const;
	void setMatrix3(const std::string &name, const glm::mat3 &mat) const;
	void setMatrix4(const std::string &name, const glm::mat4 &mat) const;
	void setVec3(const std::string& name, const glm::vec3& vec) const;
	void setVec3(const std::string& name, float x, float y, float z) const;
};