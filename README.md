PHAS0100Assignment2
------------------

[![Build Status](https://travis-ci.com/MattClarkson/PHAS0100Assignment2.svg?branch=master)](https://travis-ci.com/MattClarkson/PHAS0100Assignment2)
[![Build Status](https://ci.appveyor.com/api/projects/status/5pm89ej732c1ekf0/branch/master)](https://ci.appveyor.com/project/MattClarkson/cmakecatch2)


Purpose
-------

Extended version of CMakeCatch2 for PHAS0100 Assignment 2 that includes vec2d, pos2d and dir2d classes with wrapping functionality for periodic boundary conditions. As with CMakeCatch2 this can be used a starting point for a reasonable folder structure for [CMake](https://cmake.org/) based projects,
that use [CTest](https://cmake.org/) to run unit tests via [Catch](https://github.com/catchorg/Catch2).

Now includes a Visualisation Toolkit (VTK) based visualiser class `Code/Lib/sfmVisualiser` that can be used to plot pedestrian positions and velocities as they are simulated. This requires VTK 7 to be installed, see below. An example of its use in application given in `Code/CommandLineApps/sfmVisualiserDemo.cpp`. 

Credits
-------

This project was developed as a teaching aid for UCL's ["Research Computing with C++"](http://rits.github-pages.ucl.ac.uk/research-computing-with-cpp/)
course developed by [Dr. James Hetherington](http://www.ucl.ac.uk/research-it-services/people/james)
and [Dr. Matt Clarkson](https://iris.ucl.ac.uk/iris/browse/profile?upi=MJCLA42) and then extended by Dr. Jim Dobson based on code by Dr Tim Spain.

Build Instructions
------------------

If you want to use the VTK visualiser you need the VTK development libraries. On Ubuntu these can be installed with:
``` shell
apt-get install libvtk7-dev
```

This project itself can be built if you just want to test it. In Linux terms that
would be:
``` cmake
git clone https://github.com/jeydobson/PHAS0100Assignment2
cd PHAS0100Assignment2
mkdir build
cd build
cmake .. 
make
```
You can either use this project with the current naming convention or you can use it as a 
template to create your own project with a different naming convention. To do so,
please refer to the [CMakeTemplateRenamer](https://github.com/MattClarkson/CMakeTemplateRenamer)
which will show you how to clone this repository, and rename all the variables to names of your choice.
Then you would simply build your new project, using cmake, as shown above.
