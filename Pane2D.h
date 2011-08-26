#ifndef PANE2D_H
#define PANE2D_H

#include "Pane.h"

#include "Types.h"

class vtkImageActor;

struct Pane2D : public Pane
{
  Pane2D(QVTKWidget* qvtkWidget);
  
  FloatVectorImageType::Pointer Image;
  vtkSmartPointer<vtkImageActor> ImageActor;
  vtkSmartPointer<vtkImageData> ImageData;

};

#endif
