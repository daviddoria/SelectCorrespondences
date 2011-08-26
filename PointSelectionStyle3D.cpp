/*=========================================================================
 *
 *  Copyright David Doria 2011 daviddoria@gmail.com
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#include "PointSelectionStyle3D.h"

// VTK
#include <vtkAbstractPicker.h>
#include <vtkCamera.h>
#include <vtkFollower.h>
#include <vtkObjectFactory.h>
#include <vtkPointPicker.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkVectorText.h>

// STL
#include <sstream>

// Custom
#include "Helpers.h"

vtkStandardNewMacro(PointSelectionStyle3D);

PointSelectionStyle3D::PointSelectionStyle3D()
{
  this->MarkerRadius = .05;
  
  // Create a sphere to use as the dot
  this->DotSource = vtkSmartPointer<vtkSphereSource>::New();
  this->DotSource->SetRadius(this->MarkerRadius);
  this->DotSource->Update();
  
}

void PointSelectionStyle3D::OnLeftButtonDown() 
{
  //std::cout << "Picking pixel: " << this->Interactor->GetEventPosition()[0] << " " << this->Interactor->GetEventPosition()[1] << std::endl;
  //int success = vtkPointPicker::SafeDownCast(this->Interactor->GetPicker())->Pick(this->Interactor->GetEventPosition()[0],
  vtkPointPicker::SafeDownCast(this->Interactor->GetPicker())->Pick(this->Interactor->GetEventPosition()[0],
	  this->Interactor->GetEventPosition()[1],
	  0,  // always zero.
	  this->CurrentRenderer);
  //std::cout << "Success? " << success << std::endl;

  if(vtkPointPicker::SafeDownCast(this->Interactor->GetPicker())->GetDataSet() != this->Data)
    {
    std::cerr << "Did not pick from the correct data set!" << std::endl;
    }
  /*
  vtkIdType pointId = vtkPointPicker::SafeDownCast(this->Interactor->GetPicker())->GetPointId();
  double p[3];
  this->Data->GetPoint(pointId, p);
  
  //std::cout << "Picked point: " << pointId << std::endl;
  //std::cout << "Point: " << pointId << " should have coordinate: " << p[0] << " " << p[1] << " " << p[2] << std::endl;
  */
  double picked[3] = {0,0,0};
  
  vtkPointPicker::SafeDownCast(this->Interactor->GetPicker())->GetPickPosition(picked);
  //std::cout << "Picked point with coordinate: " << picked[0] << " " << picked[1] << " " << picked[2] << std::endl;

  
  if(this->Interactor->GetShiftKey())
    {
    this->CurrentRenderer->GetActiveCamera()->SetFocalPoint(picked);
    }

  // Only select the point if control is held
  if(this->Interactor->GetControlKey())
    {
    AddNumber(picked);
    }

  // Forward events
  vtkInteractorStyleTrackballCamera::OnLeftButtonDown();

}

void PointSelectionStyle3D::AddNumber(double p[3])
{
  // Create the text
  std::stringstream ss;
  ss << Coordinates.size();
  
  std::cout << "Added 3D keypoint: " << p[0] << " " << p[1] << " " << p[2] << std::endl;
  Coord3D coord;
  coord.x = p[0];
  coord.y = p[1];
  coord.z = p[2];
  Coordinates.push_back(coord);
  
  vtkSmartPointer<vtkVectorText> textSource = vtkSmartPointer<vtkVectorText>::New();
  textSource->SetText( ss.str().c_str() );

  // Create a mapper
  vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection( textSource->GetOutputPort() );

  // Create a subclass of vtkActor: a vtkFollower that remains facing the camera
  vtkSmartPointer<vtkFollower> follower = vtkSmartPointer<vtkFollower>::New();
  follower->SetMapper( mapper );
  follower->SetCamera(this->CurrentRenderer->GetActiveCamera());
  follower->SetPosition(p);
  follower->GetProperty()->SetColor( 1, 0, 0 ); // red
  follower->SetScale( .1, .1, .1 );

  this->Numbers.push_back(follower);
  this->CurrentRenderer->AddViewProp( follower );
  
  // Create the dot

  // Create a mapper
  vtkSmartPointer<vtkPolyDataMapper> sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  sphereMapper->SetInputConnection( this->DotSource->GetOutputPort() );

  // Create a subclass of vtkActor: a vtkFollower that remains facing the camera
  vtkSmartPointer<vtkActor> sphereActor = vtkSmartPointer<vtkActor>::New();
  sphereActor->SetMapper( sphereMapper );
  sphereActor->SetPosition(p);
  sphereActor->GetProperty()->SetColor( 1, 0, 0 ); // red

  this->Points.push_back(sphereActor);
  this->CurrentRenderer->AddViewProp( sphereActor );
}

void PointSelectionStyle3D::RemoveAll()
{
  Helpers::RemoveAllActors(this->Numbers, this->CurrentRenderer);
  Helpers::RemoveAllActors(this->Points, this->CurrentRenderer);

  this->Numbers.clear();
  this->Points.clear();
  this->Coordinates.clear();
}

void PointSelectionStyle3D::SetCurrentRenderer(vtkRenderer* renderer)
{
  vtkInteractorStyle::SetCurrentRenderer(renderer);
}

void PointSelectionStyle3D::DeleteLastCorrespondence()
{
  this->CurrentRenderer->RemoveViewProp( this->Numbers[this->Numbers.size() - 1]);
  this->CurrentRenderer->RemoveViewProp( this->Points[this->Points.size() - 1]);
  this->Numbers.erase(this->Numbers.end()-1);
  this->Points.erase(this->Points.end()-1);
  this->Coordinates.erase(this->Coordinates.end()-1);
}
