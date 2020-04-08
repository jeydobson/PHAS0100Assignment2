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

#include "sfmBasicTypes.h"

namespace sfm {

//=============================================================================*/
// vec2d definitions
//-----------------------------------------------------------------------------
vec2d vec2d::operator+(vec2d &b) {
  return vec2d(xx + b.xx, yy + b.yy);
}

//-----------------------------------------------------------------------------
vec2d vec2d::operator-(vec2d &b) {
  return vec2d(xx - b.xx, yy - b.yy);
}

//-----------------------------------------------------------------------------
double vec2d::operator*(vec2d &b) {
  return xx * b.xx + yy * b.yy;
}

//-----------------------------------------------------------------------------
vec2d vec2d::operator*(double &c) {
  return vec2d(c * xx, c * yy);
}

//=============================================================================*/
// pos2d definitions
//-----------------------------------------------------------------------------
// Move in a given direction from this position
pos2d pos2d::operator+(dir2d &b){
  return this->displace(b);
}

//-----------------------------------------------------------------------------
dir2d pos2d::operator-(pos2d &b) {
  return pos2d::direction(b, *this);
}       
 
//-----------------------------------------------------------------------------
double pos2d::distance(pos2d &b) {
  dir2d d = this->direction(b);
  return sqrt(d.x() * d.x() + d.y() * d.y());
}       
        
//-----------------------------------------------------------------------------
// direction vector a - b, where 'this' is a
dir2d pos2d::direction(pos2d& b) {
  return direction(*this, b);
}

//-----------------------------------------------------------------------------
// direction vector from a to b
dir2d pos2d::direction(pos2d &a, pos2d &b) {
  double dx = b.x() - a.x();
  double dy = b.y() - a.y();
  if (fabs(dx) > x_wrap/2) {
          dx -= copysign(x_wrap, dx);
  }
  if (fabs(dy) > y_wrap/2) {
          dy -= copysign(y_wrap, dy);
  }
  return dir2d(dx, dy);
}

//-----------------------------------------------------------------------------
pos2d pos2d::displace(dir2d &b) {
  double nx = x() + b.x();
  double ny = y() + b.y();
  return pos2d(nx, ny);
}


//=============================================================================*/
// dir2d definitions
//-----------------------------------------------------------------------------
pos2d dir2d::operator+(pos2d &b) {
  return displace(b);
}

//-----------------------------------------------------------------------------
dir2d dir2d::operator+(dir2d &b) {
  return dir2d(x() + b.x(), y() + b.y());
}

//-----------------------------------------------------------------------------
dir2d dir2d::operator-(dir2d &b) {
  return dir2d(x() - b.x(), y() - b.y());
}

//-----------------------------------------------------------------------------
double dir2d::operator*(dir2d &b) {
  return x() * b.x() + y() * b.y();
}

//-----------------------------------------------------------------------------
dir2d dir2d::operator*(double s) {
	return dir2d(x() * s, y() * s);
}

//-----------------------------------------------------------------------------
dir2d dir2d::operator-() {
  return dir2d(-x(), -y());
}

//-----------------------------------------------------------------------------
pos2d dir2d::displace(pos2d &b) {
  return b.displace(*this);
}

//-----------------------------------------------------------------------------
dir2d dir2d::add(dir2d &b) {
  return *this + b;
}

//-----------------------------------------------------------------------------
dir2d dir2d::subtract(dir2d &b) {
  return *this - b;
}

//-----------------------------------------------------------------------------
double dir2d::scalar_product(dir2d &b) {
  return *this * b;
}

//-----------------------------------------------------------------------------
double dir2d::length() {
  return sqrt(scalar_product(*this));
}

//-----------------------------------------------------------------------------
double dir2d::cos_angle(dir2d &b) {
  return scalar_product(b)/(length() * b.length());
}

//-----------------------------------------------------------------------------
void dir2d::negate( ) {
  *this = this->negative();
}

//-----------------------------------------------------------------------------
dir2d dir2d::negative( ) {
  return dir2d(-x(), -y());
}

//-----------------------------------------------------------------------------
dir2d operator*(double s, dir2d v) {
	return v * s;
}
  
} // end namespace
