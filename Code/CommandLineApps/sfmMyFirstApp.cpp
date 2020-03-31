/*=============================================================================

  PHAS0100ASSIGNMENT2: PHAS0100 Assignment 2 Social Force Model

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

#include <sfmMyFunctions.h>
#include <sfmExceptionMacro.h>
#include <iostream>

// Example, header-only library, included in project for simplicity's sake.
#include <Eigen/Dense>
#include <unsupported/Eigen/NonLinearOptimization>

/**
 * \brief Demo file to check that includes and library linkage is correct.
 */
int main(int argc, char** argv)
{

  int returnStatus = EXIT_FAILURE;

  try
  {

    Eigen::MatrixXd m(2,2);
    std::cout << "Printing 2x2 Eigen::MatrixXd ..." << std::endl << m << std::endl;

    std::cout << "Calculating ... " << sfm::MyFirstAddFunction(1, 2) << std::endl;

    returnStatus = EXIT_SUCCESS;
  }
  catch (sfm::Exception& e)
  {
    std::cerr << "Caught sfm::Exception: " << e.GetDescription() << std::endl;
  }
  catch (std::exception& e)
  {
    std::cerr << "Caught std::exception: " << e.what() << std::endl;
  }

  return returnStatus;
}
