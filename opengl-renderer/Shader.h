#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> 

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	/// <summary>
	/// The shader handle ID in OpenGL
	/// </summary>
	unsigned int ID;
	/// <summary>
	/// Creates a shader on OpenGL
	/// </summary>
	/// <param name="vertPath"></param>
	/// <param name="fragPath"></param>
	Shader(const char* vertPath, const char* fragPath);
	void use();
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string& name, float value) const;
};

#endif
