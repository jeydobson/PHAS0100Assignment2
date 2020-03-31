/*=============================================================================

  PHAS0100ASSIGNMENT2: PHAS0100 Assignment 2 Social Force Model

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

#ifndef sfmExceptionMacro_h
#define sfmExceptionMacro_h

#include "sfmException.h"

#define sfmExceptionThrow() throw sfm::Exception(__FILE__,__LINE__)

#endif
