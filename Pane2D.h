#ifndef PANE2D_H
#define PANE2D_H

#include "Pane.h"

#include "Types.h"

class vtkImageSliceMapper;
class vtkImageSlice;

struct Pane2D : public Pane
{
  Pane2D(QVTKWidget* qvtkWidget);
  
  FloatVectorImageType::Pointer Image;
  vtkSmartPointer<vtkImageData> ImageData;
  
  vtkSmartPointer<vtkImageSliceMapper> ImageSliceMapper;
  vtkSmartPointer<vtkImageSlice> ImageSlice;
  
  bool Flipped;
  void SetCameraPosition1();
  void SetCameraPosition2();
  
  void Flip();

};

#endif
