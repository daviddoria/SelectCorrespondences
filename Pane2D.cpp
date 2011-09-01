#include "Pane2D.h"

#include <vtkImageData.h>
#include <vtkImageSliceMapper.h>
#include <vtkImageSlice.h>

#include "PointSelectionStyle2D.h"

Pane2D::Pane2D(QVTKWidget* qvtkWidget) : Pane(qvtkWidget)
{
  this->ImageData = vtkSmartPointer<vtkImageData>::New();

  this->ImageSliceMapper = vtkSmartPointer<vtkImageSliceMapper>::New();
  
  this->ImageSlice = vtkSmartPointer<vtkImageSlice>::New();
  
  this->Flipped = false;
}

void Pane2D::SetCameraPosition1()
{
  double leftToRight[3] = {-1,0,0};
  double bottomToTop[3] = {0,1,0};
  static_cast<PointSelectionStyle2D*>(this->SelectionStyle)->SetImageOrientation(leftToRight, bottomToTop);
  
  static_cast<PointSelectionStyle2D*>(this->SelectionStyle)->GetCurrentRenderer()->ResetCamera();
  static_cast<PointSelectionStyle2D*>(this->SelectionStyle)->GetCurrentRenderer()->ResetCameraClippingRange();
}

void Pane2D::SetCameraPosition2()
{
  double leftToRight[3] = {-1,0,0};
  double bottomToTop[3] = {0,-1,0};
  static_cast<PointSelectionStyle2D*>(this->SelectionStyle)->SetImageOrientation(leftToRight, bottomToTop);
  
  static_cast<PointSelectionStyle2D*>(this->SelectionStyle)->GetCurrentRenderer()->ResetCamera();
  static_cast<PointSelectionStyle2D*>(this->SelectionStyle)->GetCurrentRenderer()->ResetCameraClippingRange();
}

void Pane2D::Flip()
{
  if(this->Flipped)
    {
    SetCameraPosition1();
    }
  else
    {
    SetCameraPosition2();
    }
    
  this->Flipped = !this->Flipped;
  
  this->Refresh();
}
