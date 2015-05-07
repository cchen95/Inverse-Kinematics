#ifndef COLOR_H
#define COLOR_H

#ifdef WIN32
#define NOMINMAX
#include <Windows.h>
#endif

#include <GL/gl.h>

/*! \file color.h
 *  \class Color
 *  \brief Color structure for applying colors through OpenGL
 *  \author Alexandros Dermenakis
 *  \date    2011
 *
 */
struct Color
{
  float r;
  float g;
  float b;

  inline void apply() { glColor3f(r, g, b); }
};

#endif // COLOR_H
