#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public :
	Shader(std::string vertexPath, std::string fragmentPath);

	void use();

	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMat4(const std::string& name, glm::mat4 value) const;
	void setVec3(const std::string& name, const glm::vec3 value) const;
private:
	unsigned int ID;
	void checkCompileErrors(unsigned int shader, std::string type);
};