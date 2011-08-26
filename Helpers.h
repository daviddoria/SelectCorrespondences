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

#ifndef HELPERS_H
#define HELPERS_H

// ITK
#include "itkImage.h"
#include "itkIndex.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"

// VTK
#include <vtkImageData.h>
#include <vtkPoints.h>
#include <vtkProp.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>

// Custom
#include "Types.h"

namespace Helpers
{
template<typename T>
void RemoveAllActors(std::vector<T*>& actors, vtkRenderer* renderer);

void ITKImagetoVTKImage(FloatVectorImageType::Pointer image, vtkImageData* outputImage); // This function simply drives ITKImagetoVTKRGBImage or ITKImagetoVTKMagnitudeImage
void ITKImagetoVTKRGBImage(FloatVectorImageType::Pointer image, vtkImageData* outputImage);
void ITKImagetoVTKMagnitudeImage(FloatVectorImageType::Pointer image, vtkImageData* outputImage);
float ComputeAverageSpacing(vtkPoints* points);

template<typename TImage>
void DeepCopyScalarImage(typename TImage::Pointer input, typename TImage::Pointer output)
{
  output->SetRegions(input->GetLargestPossibleRegion());
  output->Allocate();
 
  itk::ImageRegionConstIterator<TImage> inputIterator(input, input->GetLargestPossibleRegion());
  itk::ImageRegionIterator<TImage> outputIterator(output, output->GetLargestPossibleRegion());
 
  while(!inputIterator.IsAtEnd())
    {
    outputIterator.Set(inputIterator.Get());
    ++inputIterator;
    ++outputIterator;
    }
}

template<typename TImage>
void DeepCopyVectorImage(typename TImage::Pointer input, typename TImage::Pointer output)
{
  output->SetRegions(input->GetLargestPossibleRegion());
  output->SetNumberOfComponentsPerPixel(input->GetNumberOfComponentsPerPixel());
  output->Allocate();
 
  itk::ImageRegionConstIterator<TImage> inputIterator(input, input->GetLargestPossibleRegion());
  itk::ImageRegionIterator<TImage> outputIterator(output, output->GetLargestPossibleRegion());
 
  while(!inputIterator.IsAtEnd())
    {
    outputIterator.Set(inputIterator.Get());
    ++inputIterator;
    ++outputIterator;
    }
}

#include "Helpers.hxx"

}

#endif
