/*=============================================================================

  PHAS0100ASSIGNMENT2: PHAS0100 Assignment 2 Social Force Model

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

  Based on prototype version: 9 Mar 2020
      Author: Tim Spain
  Included in this project: 1 Apr 2020
      Author: Jim Dobson

=============================================================================*/

#include "catch.hpp"
#include "sfmCatchMain.h"
#include "sfmBasicTypes.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>

bool generic_direction_test(
                double x1, double y1,
                double x2, double y2,
                double xt, double yt);
bool generic_displacement_test(
                double x1, double y1,
                double x2, double y2,
                double xt, double yt);
double mean_reldiff(double a, double b);

bool closely_equal(sfm::vec2d &a, sfm::vec2d &b);

bool ctor_test(double xin, double yin, double xt, double yt) {
        sfm::pos2d construct(xin, yin);
        sfm::pos2d target(xt, yt);
	return closely_equal(construct, target);
}

// as POS2D_XWRAP and POS2D_YWRAP can change define some test
// points relative to these for convenience
double near_xwrap = POS2D_XWRAP - POS2D_XWRAP/100.0;
double near_xzero = POS2D_XWRAP/100.0;
double middle_xwrap = POS2D_XWRAP/2.0;
double near_ywrap = POS2D_YWRAP - POS2D_YWRAP/100.0;
double near_yzero = POS2D_YWRAP/100.0;
double middle_ywrap = POS2D_YWRAP/2.0;

TEST_CASE("Test the wrap values", "[Tests]") {
	REQUIRE(sfm::pos2d::get_x_wrap() == POS2D_XWRAP);
	REQUIRE(sfm::pos2d::get_y_wrap() == POS2D_YWRAP);
}

TEST_CASE("Test the constructor", "[Tests]") {
	// Centre of the field
	REQUIRE(ctor_test(middle_xwrap, middle_ywrap, 
			  middle_xwrap, middle_ywrap));
        // To guarantee failure need:
	// xtest != ytest && xtest and ytest within xwrap and ywrap
	double xtest = std::min(middle_xwrap, middle_ywrap);
        double ytest = xtest + std::min(near_xzero, near_yzero); 
	REQUIRE(!ctor_test(xtest, ytest, 
			   ytest, xtest));
	// Near the wrap-around
	REQUIRE(ctor_test(near_xwrap, near_ywrap, 
		          near_xwrap, near_ywrap)); 
	// On the edge
	REQUIRE(ctor_test(POS2D_XWRAP, POS2D_YWRAP,
			  0, 0));
	// Over the edge
	REQUIRE(ctor_test(POS2D_XWRAP+near_xzero, POS2D_YWRAP+near_yzero, 
			  near_xzero, near_yzero));
	// Check the coordinates have genuinely wrapped
	sfm::pos2d p(POS2D_XWRAP+near_xzero, 
		     POS2D_YWRAP+near_yzero);
	REQUIRE(p.x() != POS2D_XWRAP+near_xzero);
	REQUIRE(p.y() != POS2D_YWRAP+near_yzero);
}

TEST_CASE("Direction calculation", "[Tests]") {

	// No wrap: direction from (ax, ay) to (bx, by) should be (bx-ax, by-ax)
	REQUIRE(generic_direction_test(
			near_xzero, near_yzero,
			middle_xwrap, middle_ywrap,
			middle_xwrap-near_xzero, middle_ywrap-near_yzero));

	// With bx wrapped: direction from (ax, ay) to (bx, by) should be (wrap(bx)-ax, by-ay)
	REQUIRE(generic_direction_test(
			near_xzero, near_yzero,
			POS2D_XWRAP+0.5*near_xzero, middle_ywrap,
			0.5*near_xzero-near_xzero, middle_ywrap-near_yzero));

	// As above now for wrap in y
	REQUIRE(generic_direction_test(
			near_xzero, near_yzero,
			middle_xwrap, POS2D_YWRAP+0.5*near_yzero,
			middle_xwrap-near_xzero, 0.5*near_yzero-near_yzero));

	// As above but wrap in both x and y (choose different factors) 
	REQUIRE(generic_direction_test(
			near_xzero, near_yzero,
			POS2D_XWRAP+0.2*near_xzero, POS2D_YWRAP+0.3*near_yzero,
			0.2*near_xzero-near_xzero, 0.3*near_yzero-near_yzero));

}

bool generic_direction_test(
		double x1, double y1,
		double x2, double y2,
		double xt, double yt) {
	sfm::pos2d xy1(x1, y1);
	sfm::pos2d xy2(x2, y2);
	sfm::dir2d xyt(xt, yt);

	sfm::dir2d xyr = xy1.direction(xy2);
	return closely_equal(xyr, xyt);
}

double mean_reldiff(double a, double b) {
	if ((a == 0.) && (b == 0.))
		return 0.;
	return (b - a)/(0.5 * (b + a));
}

bool closely_equal(sfm::vec2d &a, sfm::vec2d &b) {
  const double tol = 100*std::numeric_limits<double>::epsilon(); // tolerance for floating point comparisons

  double mean_rel_diff_x = std::abs(mean_reldiff(a.x(), b.x()));
  double mean_rel_diff_y = std::abs(mean_reldiff(a.y(), b.y()));

  return (mean_rel_diff_x < tol
            && mean_rel_diff_y < tol);
}

TEST_CASE("Addition test", "[Tests]") {
	// No wrapping
	REQUIRE(generic_displacement_test(
			near_xzero, near_yzero,
			middle_xwrap, middle_ywrap,
			near_xzero+middle_xwrap,  near_yzero+middle_ywrap));
	// Wrapping in x
	REQUIRE(generic_displacement_test(
			near_xzero, near_yzero,
			POS2D_XWRAP+near_xzero, middle_ywrap, 
			near_xzero+near_xzero, near_yzero+middle_ywrap));
	// Wrapping in y
	REQUIRE(generic_displacement_test(
			near_xzero, near_yzero,
			middle_xwrap, POS2D_YWRAP+near_yzero,
			near_xzero+middle_xwrap, near_yzero+near_yzero));
	// Wrapping in -x and -y 
	REQUIRE(generic_displacement_test(
			near_xzero, near_yzero,
			-middle_xwrap, -middle_ywrap,
			POS2D_XWRAP-middle_xwrap+near_xzero, 
			POS2D_YWRAP-middle_ywrap+near_yzero));
}

bool generic_displacement_test(
		double x1, double y1,
		double x2, double y2,
		double xt, double yt) {
	sfm::pos2d xy1(x1, y1);
	sfm::dir2d xy2(x2, y2);
	sfm::pos2d xyt(xt, yt);

	sfm::pos2d xyr = xy2.displace(xy1);
	return closely_equal(xyr, xyt);
}

bool generic_subtraction_test(
		double x1, double y1,
		double x2, double y2,
		double xt, double yt) {
	sfm::pos2d xy1(x1, y1);
	sfm::pos2d xy2(x2, y2);
	sfm::dir2d xyt(xt, yt);

	sfm::dir2d xyr = xy1 - xy2;
	return closely_equal(xyr, xyt);
}

// Subtraction, operator-()
TEST_CASE("operator- tests", "[Tests]") {
	// The direction from (ax,ay) to (bx,by) should be (bx-ax,by-ax)
	REQUIRE(generic_subtraction_test(
			middle_xwrap, middle_ywrap,
			near_xzero, near_yzero,
			middle_xwrap-near_xzero, middle_ywrap-near_yzero));

	// Now with wrap in x
	REQUIRE(generic_subtraction_test(
			POS2D_XWRAP+0.4*middle_xwrap, middle_ywrap,
			near_xzero, near_yzero,
			0.4*middle_xwrap-near_xzero, middle_ywrap-near_yzero));

	// Now with wrap in y
	REQUIRE(generic_subtraction_test(
			middle_xwrap, POS2D_YWRAP+0.6*middle_ywrap,
			near_xzero, near_yzero,
			middle_xwrap-near_xzero, 0.6*middle_ywrap-near_yzero));

	// Wrap in x and y 
	REQUIRE(generic_subtraction_test(
			POS2D_XWRAP+0.6*middle_xwrap, near_yzero,
		       	near_xzero, POS2D_YWRAP+0.4*middle_ywrap,
			0.6*middle_xwrap-near_xzero, near_yzero-0.4*middle_ywrap));

}

// Use distance squared for easier, more accurate data entry
bool generic_distance_test(
		double x1, double y1,
		double x2, double y2,
		double dt2) {
	sfm::pos2d xy1(x1, y1);
	sfm::pos2d xy2(x2, y2);

	double dr = xy1.distance(xy2);
	const double tol = 1e-16;
	return (mean_reldiff(dr, sqrt(dt2)) < tol);
}

TEST_CASE("Distance tests", "[Tests]") {
  REQUIRE(generic_distance_test(1, 1, 3, 2, 5));
  REQUIRE(generic_distance_test(1, 1, 4, 2, 5));
  REQUIRE(generic_distance_test(1, 0.5, 3, 2.5, 5));
  REQUIRE(generic_distance_test(1, 0.5, 4 ,2.5, 5));
  // Test a distance that isn't sqrt 5
  REQUIRE(generic_distance_test(4.5, 2.5, 0.5, 0.5, 2));
}

