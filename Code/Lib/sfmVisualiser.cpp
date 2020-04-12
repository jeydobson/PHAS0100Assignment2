/*=============================================================================

  PHAS0100ASSIGNMENT2: PHAS0100 Assignment 2 Social Force Model

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

  VTK based viewer for Social Force Model simulation: 12 Apr 2020
      Author: Jim Dobson

=============================================================================*/

#include "sfmVisualiser.h"

#include <iostream>

#include <vtkChartXY.h>
#include <vtkContextScene.h>
#include <vtkContextView.h>
#include <vtkFloatArray.h>
#include <vtkPlotPoints.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkTable.h>
#include <vtkAxis.h>

namespace sfm
{

//=============================================================================*/
// Visualiser definitions
//-----------------------------------------------------------------------------
Visualiser::Visualiser(unsigned int n_pedestrians, 
		             double world_x, double world_y):
	m_window_scale(15.0) {

  CreateWorld(n_pedestrians, world_x, world_y);

}

//-----------------------------------------------------------------------------
Visualiser::~Visualiser(){
  // cleanup handled by smart pointers
}

//-----------------------------------------------------------------------------
void Visualiser::CreateWorld(unsigned int n_pedestrians, double world_x, double world_y){

  m_world_x = world_x;
  m_world_y = world_y;
  m_n_pedestrians = n_pedestrians;

  // Create view
  m_view = vtkSmartPointer<vtkContextView>::New();
  m_view->GetRenderer()->SetBackground(1.0, 1.0, 1.0);
  m_view->GetRenderWindow()->SetSize( m_world_x*m_window_scale+35, 
		                      m_world_y*m_window_scale+35);     

  // Add 2D chart
  m_chart = vtkSmartPointer<vtkChartXY>::New();
  m_view->GetScene()->AddItem(m_chart);

  m_chart->GetAxis(0)->SetBehavior(vtkAxis::FIXED);
  m_chart->GetAxis(1)->SetBehavior(vtkAxis::FIXED);
  m_chart->GetAxis(0)->SetRange(0.0, world_y);
  m_chart->GetAxis(1)->SetRange(0.0, world_x);

  // Initialise view in non-blocking mode
  m_view->GetRenderWindow()->SetMultiSamples(0);
  m_view->GetInteractor()->Initialize();

  // To store pedestrian data
  CreateTable();  
}

//-----------------------------------------------------------------------------
void Visualiser::CreateTable(){

  m_table = vtkSmartPointer<vtkTable>::New();
  
  vtkSmartPointer<vtkFloatArray> arrX =
    vtkSmartPointer<vtkFloatArray>::New();
  arrX->SetName("X Axis [m]");
  m_table->AddColumn(arrX);

  vtkSmartPointer<vtkFloatArray> arrY =
    vtkSmartPointer<vtkFloatArray>::New();
  arrY->SetName("Y Axis [m]");
  m_table->AddColumn(arrY);

  vtkSmartPointer<vtkFloatArray> arrVX =
    vtkSmartPointer<vtkFloatArray>::New();
  arrVX->SetName("Velocity X [m/s]");
  m_table->AddColumn(arrVX);
 
  vtkSmartPointer<vtkFloatArray> arrVY =
    vtkSmartPointer<vtkFloatArray>::New();
  arrVY->SetName("Velocity Y [m/s]");
  m_table->AddColumn(arrVY);

  m_table->SetNumberOfRows(m_n_pedestrians);

}

//-----------------------------------------------------------------------------
bool Visualiser::SetPedestrian(unsigned int i, 
		                  double xpos, double ypos,
				  double xspeed, double yspeed){

  if(i < m_n_pedestrians)
  {
    if(xpos >= 0.0 && xpos < m_world_x &&
       ypos >= 0.0 && ypos < m_world_y)
    {
      m_table->SetValue(i, 0, xpos);
      m_table->SetValue(i, 1, ypos);
      m_table->SetValue(i, 2, xspeed);
      m_table->SetValue(i, 3, yspeed);

      return true;
    }
    else
    {
      std::cerr << "Pedestrian position x = "<< xpos 
	        << ", y = "<< ypos << " as outside of world\n";
    }
  }
  else
  {
    std::cerr << "Unable to update pedestrian "<< i+1 
	      << "as there are only " << m_n_pedestrians 
	      << " pedestrians in world\n";
  }

  return false;
}

//-----------------------------------------------------------------------------
void Visualiser::UpdateScene(){

  // Brute force just replace all plots
  m_chart->ClearPlots();
  m_points = m_chart->AddPlot(vtkChart::POINTS);
  m_points->SetColor(0, 0, 0, 255);
  m_points->SetWidth(0.1);

  m_points->SetInputData(m_table, 0, 1); 
  m_points->Modified();

  m_view->GetRenderWindow()->Render();

}

//-----------------------------------------------------------------------------
void Visualiser::SetWindowScale(double scale){
  m_window_scale = scale;
}

} // end namespace

