// -----------------------------------------------------------------------------
// Copyright  : (C) 2014 Andreas-C. Bernstein
// License    : MIT (see the file LICENSE)
// Maintainer : Andreas-C. Bernstein <andreas.bernstein@uni-weimar.de>
// Stability  : experimental
//
// utils
// -----------------------------------------------------------------------------

#include "utils.hpp"
#include <stdexcept>

GLuint loadShader(GLenum type, std::string const& s)
{
  GLuint id = glCreateShader(type);
  const char* source = s.c_str();
  glShaderSource(id, 1, &source, nullptr);
  glCompileShader(id);

  GLint successful;
  glGetShaderiv(id, GL_COMPILE_STATUS, &successful);

  if (!successful) {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    std::string info(length, ' ');

    glGetShaderInfoLog(id, length, &length, &info[0]);
    throw std::logic_error(info);
  }
  return id;
}

GLuint createProgram(std::string const& v, std::string const& f)
{
  GLuint id = glCreateProgram();

  GLuint vsHandle = loadShader(GL_VERTEX_SHADER, v);
  GLuint fsHandle = loadShader(GL_FRAGMENT_SHADER, f);
  glAttachShader(id, vsHandle);
  glAttachShader(id, fsHandle);
  // schedule for deletion
  glDeleteShader(vsHandle);
  glDeleteShader(fsHandle);

  glLinkProgram(id);
  GLint successful;

  glGetProgramiv(id, GL_LINK_STATUS, &successful);
  if (!successful) {
    int length;
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
    std::string info(length, ' ');

    glGetProgramInfoLog(id, length, &length, &info[0]);
    throw std::logic_error(info);
  }
  return id;
}

GLuint updateTexture2D(GLuint tex, unsigned const& width, unsigned const& height,
    const char* data)
{
    GLuint error = glGetError();

    if (error != GL_NO_ERROR)
    {
        std::cout << "OpenGL Error entry updateTexture2D: " << error << std::endl;
    }
    //GLuint tex;
    //glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cout << "OpenGL Error glTexParameteri: " << error << std::endl;
    }

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
    error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cout << "OpenGL Error glTexImage2D: " << error << std::endl;
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    return tex;
}

GLuint createTexture2D(unsigned const& width, unsigned const& height,
    const char* data)
{
  GLuint tex;
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
      GL_UNSIGNED_BYTE, data);
  
  //glBindTexture(GL_TEXTURE_2D, 0);

  return tex;
}

GLuint createTexture3D(unsigned const& width, unsigned const& height,
    unsigned const& depth, unsigned const channel_size,
    unsigned const channel_count, const char* data)
{
  GLuint tex;
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_3D, tex);

  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  //glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  //glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  if (channel_size == 1)
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, width, height, depth, 0, GL_RED,
        GL_UNSIGNED_BYTE, data);

  if (channel_size == 2)
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, width, height, depth, 0, GL_RED,
        GL_UNSIGNED_SHORT, data);
  
  if (channel_size == 4)
      glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, width, height, depth, 0, GL_RED,
      GL_FLOAT, data);

  //glBindTexture(GL_TEXTURE_3D, 0);

  return tex;
}

GLuint createDepthBuffer(unsigned const& width, unsigned const& height,
	unsigned const channel_size)
{
	GLuint texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	GLenum internalFormat = GL_DEPTH_COMPONENT24;
	GLenum type = GL_UNSIGNED_INT;
	//if (GLE_ARB_depth_buffer_float)
	//{
	//	internalFormat = GL_DEPTH_COMPONENT32F;
	//	type = GL_FLOAT;
	//}

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, GL_DEPTH_COMPONENT, type, NULL);

	return texId;
}

