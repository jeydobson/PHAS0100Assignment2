/*=============================================================================

  PHAS0100ASSIGNMENT2: PHAS0100 Assignment 2 Social Force Model

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

  Example use of VTK based viewer for Social Force Model simulation: 12 Apr 2020
      Author: Jim Dobson

=============================================================================*/

#include <iostream>
#include <thread>
#include <chrono>
#include <memory>
#include <array>
#include <vector>
#include <random>

#include "sfmVisualiser.h"

int main(int argc, char** argv){

  // Needed these to define viewer world	
  double world_width_x = 50.0;
  double world_height_y = 10.0;
  const unsigned int n_pedestrians = 10;

  // Create viewer and initialise with required number of pedestrians and world dimensions
  sfm::Visualiser viewer(n_pedestrians, world_width_x, world_height_y);
 
  // Minimal structure to store dummy x,y,vx,vy 
  std::array<std::array<double, 4>, n_pedestrians> pedestrians;
 
  std::default_random_engine generator;
  std::uniform_real_distribution<double> ranx(0.0, world_width_x);
  std::uniform_real_distribution<double> rany(0.0, world_height_y);

  // Dummy loop over time
  int n_steps = 1000;
  for(int i = 0; i < n_steps; i++){ 

    // Simulate some pedestrian movement 
    for(auto& pedestrian : pedestrians){
      pedestrian[0] = ranx(generator);
      pedestrian[1] = rany(generator);
      pedestrian[2] = 0.1*pedestrian[0];
      pedestrian[3] = 0.1*pedestrian[1];
    }

    // Send updated pedestrian positions to viewer 
    for(unsigned int i = 0; i < n_pedestrians; i++){
      viewer.SetPedestrian(i, pedestrians[i][0],  pedestrians[i][1], 
		              pedestrians[i][2],  pedestrians[i][3]);
    }

    // Tell viewer to redraw scene
    viewer.UpdateScene();

    // Sleep for a bit so can see visualiser updating 
    std::this_thread::sleep_for (std::chrono::milliseconds(200));

  } 

  return 0;
}
