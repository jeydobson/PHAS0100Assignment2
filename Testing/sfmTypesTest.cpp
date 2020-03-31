/*=============================================================================

  PHAS0100ASSIGNMENT2: PHAS0100 Assignment 2 Social Force Model

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

#include "catch.hpp"
#include "sfmCatchMain.h"
#include "sfmBasicTypes.h"
#include <iostream>
#include <vector>

#include <cmath>

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

TEST_CASE("Test the wrap values", "[Tests]") {
	REQUIRE(sfm::pos2d::get_x_wrap() == POS2D_XWRAP);
	REQUIRE(sfm::pos2d::get_y_wrap() == POS2D_YWRAP);
}

TEST_CASE("Test the constructor", "[Tests]") {
	// Centre of the field
	REQUIRE(ctor_test(2, 1, 2, 1));
	REQUIRE(!ctor_test(1, 2, 2, 1));
	// Near the wrap-around
	REQUIRE(ctor_test(POS2D_XWRAP-0.5, POS2D_YWRAP-0.5, 
			  POS2D_XWRAP-0.5, POS2D_YWRAP-0.5));
	// On the edge
	REQUIRE(ctor_test(POS2D_XWRAP, POS2D_YWRAP, 0, 0));
	// Over the edge
	REQUIRE(ctor_test(POS2D_XWRAP+1, POS2D_YWRAP+1, 1, 1));
	// Check the coordinates have genuinely wrapped
	sfm::pos2d p(POS2D_XWRAP+1, POS2D_YWRAP+1);
	REQUIRE(p.x() != POS2D_XWRAP+1);
	REQUIRE(p.y() != POS2D_YWRAP+1);
}

TEST_CASE("Direction calculation", "[Tests]") {

	// The direction from (1,1) to (3,2) should be (2,1)
	REQUIRE(generic_direction_test(
			1, 1,
			3, 2,
			2, 1));

	// The direction from (2,1) to (POS2D_XWRAP+1,2) should be (-1,1), due to the
	// wrap around in x
	REQUIRE(generic_direction_test(
			2, 1,
			POS2D_XWRAP+1, 2,
			-1, 1));

	// The direction from (1, 2.5) to (3, POS2D_YWRAP+0.5) should be (2,-2), due
	// to the wrap around in y
	REQUIRE(generic_direction_test(
			1, 2.5,
			3, POS2D_YWRAP+0.5,
			2, -2));

	// The direction from (1, 0.5) to (POS2D_XWRAP+0.25, POS2D_YWRAP+1.0) should be (-0.75,0.5), due
	// to the wrap around in both x and y
	REQUIRE(generic_direction_test(
			1, 0.5,
			POS2D_XWRAP+0.25, POS2D_YWRAP+1.0,
			-0.75, 0.5));

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
  const double tol = 1e-16; // tolerance for floating point comparisons

  double mean_rel_diff_x = std::abs(mean_reldiff(a.x(), b.x()));
  double mean_rel_diff_y = std::abs(mean_reldiff(a.y(), b.y()));

  return (mean_rel_diff_x < tol
            && mean_rel_diff_y < tol);
}

TEST_CASE("Addition test", "[Tests]") {
	// No wrapping
	REQUIRE(generic_displacement_test(
			1, 1,
			2, 1,
			3, 2));
	// Wrapping in x
	REQUIRE(generic_displacement_test(
			2, 1,
			POS2D_XWRAP+1, 1,
			3, 2));
	// Wrapping in y
	REQUIRE(generic_displacement_test(
			1, 1,
			2, POS2D_YWRAP+1,
			3, 2));
	// Wrapping in -x and -y 
	REQUIRE(generic_displacement_test(
			2, 1,
			-POS2D_XWRAP+1, -POS2D_YWRAP+1,
			3, 2));
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
	// The direction from (1,1) to (3,2) should be (2,1)
	REQUIRE(generic_subtraction_test(
			3, 2,
			1, 1,
			2, 1));

	// The direction from (1,1) to (POS2D_XWRAP+2,2) should be (1,1), due to the
	// wrap around in x
	REQUIRE(generic_subtraction_test(
			POS2D_XWRAP+2, 2,
			1, 1,
			1, 1));

	// The direction from (1, 0.5) to (3, POS2D_YWRAP+1.5) should be (2,1), due
	// to the wrap around in y
	REQUIRE(generic_subtraction_test(
			3, POS2D_YWRAP+1.5,
			1, 0.5,
			2, 1));

	// The direction from (POS2D_XWRAP+2.0, POS2D_YWRAP+1.5) to (1.5, 2.5) should be (-0.5,1), due
	// to the wrap around in both x and y
	REQUIRE(generic_subtraction_test(
			1.5, 2.5,
			POS2D_XWRAP+2.0, POS2D_YWRAP+1.5,
			-0.5, 1));

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

