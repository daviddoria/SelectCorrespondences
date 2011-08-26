#include "Pane2D.h"

#include <vtkImageActor.h>
#include <vtkImageData.h>

Pane2D::Pane2D(QVTKWidget* qvtkWidget) : Pane(qvtkWidget)
{
  this->ImageActor = vtkSmartPointer<vtkImageActor>::New();
  this->ImageData = vtkSmartPointer<vtkImageData>::New();

  
}
