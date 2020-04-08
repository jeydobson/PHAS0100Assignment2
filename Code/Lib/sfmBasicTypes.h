/*=============================================================================

  PHAS0100ASSIGNMENT2: PHAS0100 Assignment 2 Social Force Model

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

  Based on prototype vec2d, pos2d, dir2d: 9 Mar 2020
      Author: Tim Spain
  Included in this project: 1 Apr 2020
      Author: Jim Dobson

=============================================================================*/

#ifndef sfmBasicTypes_h
#define sfmBasicTypes_h

#include "sfmWin32ExportHeader.h"

/**
* \defgroup internal internal
* \brief Internal stuff, not for end-users.
*/

/**
* \defgroup types types
* \brief Package-wide data types.
*/

/**
* \defgroup utilities utilities
* \brief Groups of c-style functions.
*/

/**
* \defgroup applications applications
* \brief Small, end-user applications, most likely command line.
*/

/**
* \file sfmBasicTypes.h
* \brief Defines types and typedefs used in this library.
* \ingroup types
*/

#include <ctgmath>

#ifndef POS2D_XWRAP
#define POS2D_XWRAP 50.
#endif

#ifndef POS2D_YWRAP
#define POS2D_YWRAP 10.
#endif

//! Single namespace for all code in this package
namespace sfm
{

/**
* \class vec2d 
* \brief 2D vector base class 
* \ingroup types
*/
class vec2d {
  private:
    double xx;
    double yy;
  public:
    vec2d(double x, double y) 
    : xx(x), yy(y){ }
    vec2d operator+(vec2d &b); // vector addition
    vec2d operator-(vec2d &b); // vector subtraction
    double operator*(vec2d &b); // scalar product
    vec2d operator*(double &c); // scaling
    double operator[](int i) // Get first or second component
    {return i ? xx : yy;}
    double x() {return xx;} // Access the x component
    double y() {return yy;} // Access the y component
};

/**
* \class pos2d 
* \brief 2D vector position class that wraps
* \ingroup types
*/
class dir2d; // forward declaration as required by pos2d

class pos2d : public vec2d {
  private:
    static constexpr double x_wrap = POS2D_XWRAP;
    static constexpr double y_wrap = POS2D_YWRAP;

    double fmodulo(double x, double q) {
      double osign = copysign(1.0, x);
      double qa = osign * q;
      double xa = osign * x;
      double q2 = 0.5*qa;
      double f = fmod(xa, qa) + q2 - copysign(q2, xa);
      return osign * f;
    }

  public:
    pos2d(double x, double y)
    : vec2d(fmodulo(x, x_wrap), fmodulo(y, y_wrap)) {};
    pos2d( )  // position at the origin
    : vec2d(0., 0.) {};
    pos2d operator+(dir2d &b);  // Move in a given direction from this position
    dir2d operator-(pos2d &b); // direction vector from there to here a-b, 'this' is a
    double distance(pos2d &); // distance between two positions
    dir2d direction(pos2d &b); // direction vector of b from 'this'
    pos2d displace(dir2d&); // position in the given direction from this initial position

    static double get_x_wrap() {return x_wrap;};
    static double get_y_wrap() {return y_wrap;};

    static dir2d direction(pos2d &a, pos2d &b); // direction vector from a to b
};

/**
* \class pos2d 
* \brief 2D vector position class that wraps
* \ingroup types
*/
class dir2d : public vec2d {
  public:
    dir2d(double x, double y)
    : vec2d(x, y) { };

    pos2d operator+(pos2d&); // move the given position in this direction
    dir2d operator+(dir2d&); // vector addition
    dir2d operator-(dir2d &b); // vector subtraction
    double operator*(dir2d &b); // Scalar product between this and b
  	dir2d operator*(double); // Scaling vector * scalar
    dir2d operator-(); // unary negation operator

    pos2d displace(pos2d&); // Move a position in this direction
    dir2d add(dir2d&); // vector addition
    dir2d subtract(dir2d &b); // vector subtraction
    double scalar_product(dir2d &b); // Scalar product between this and b
    double length(); // length of this direction vector
    double cos_angle(dir2d &b); // Cosine of the angle between this and b
    void negate(); // negative vector, in place
    dir2d negative(); // returns the negative vector, leaving the original
};

dir2d operator*(double, dir2d); // Scaling scalar * vector

} // end namespace

#endif
