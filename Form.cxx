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

#include "ui_Form.h"
#include "Form.h"

// ITK
#include "itkCastImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRegionOfInterestImageFilter.h"
#include "itkVector.h"

// Qt
#include <QFileDialog>
#include <QIcon>
#include <QTextEdit>

// VTK
#include <vtkActor.h>
#include <vtkActor2D.h>
#include <vtkCamera.h>
#include <vtkCommand.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkFloatArray.h>
#include <vtkImageActor.h>
#include <vtkImageData.h>
#include <vtkInteractorStyleImage.h>
#include <vtkLookupTable.h>
#include <vtkMath.h>
#include <vtkPointData.h>
#include <vtkPointPicker.h>
#include <vtkProperty2D.h>
#include <vtkPolyDataMapper.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkXMLPolyDataReader.h>

// Custom
#include "Helpers.h"
#include "Types.h"
#include "PointSelectionStyle2D.h"
#include "PointSelectionStyle3D.h"
#include "Pane.h"
#include "Pane2D.h"
#include "Pane3D.h"

void Form::on_actionHelp_activated()
{
  QTextEdit* help=new QTextEdit();
  
  help->setReadOnly(true);
  help->append("<h1>Image correspondences</h1>\
  Hold the right mouse button and drag to zoom in and out. <br/>\
  Hold the middle mouse button and drag to pan the image. <br/>\
  Click the left mouse button to select a keypoint.<br/> <p/>\
  <h1>Point cloud correspondences</h1>\
  Hold the left mouse button and drag to rotate the scene.<br/>\
  Hold the right mouse button and drag to zoom in and out. Hold the middle mouse button and drag to pan the scene. While holding control (CTRL), click the left mouse button to select a keypoint.<br/>\
  If you need to zoom in farther, hold shift while left clicking a point to change the camera's focal point to that point. You can reset the focal point by pressing 'r'.\
  <h1>Saving keypoints</h1>\
  The same number of keypoints must be selected in both the left and right panels before the points can be saved."
  );
  help->show();
}

void Form::on_actionQuit_activated()
{
  exit(0);
}

// Constructor
Form::Form()
{
  this->setupUi(this);

  // Setup icons
  QIcon openIcon = QIcon::fromTheme("document-open");
  QIcon saveIcon = QIcon::fromTheme("document-save");
  
  // Setup left toolbar
  actionOpenImageLeft->setIcon(openIcon);
  this->toolBar_left->addAction(actionOpenImageLeft);

  actionOpenPointCloudLeft->setIcon(openIcon);
  this->toolBar_left->addAction(actionOpenPointCloudLeft);

  actionSavePointsLeft->setIcon(saveIcon);
  this->toolBar_left->addAction(actionSavePointsLeft);

  actionLoadPointsLeft->setIcon(openIcon);
  this->toolBar_left->addAction(actionLoadPointsLeft);
  
  // Setup right toolbar
  actionOpenImageRight->setIcon(openIcon);
  this->toolBar_right->addAction(actionOpenImageRight);

  actionOpenPointCloudRight->setIcon(openIcon);
  this->toolBar_right->addAction(actionOpenPointCloudRight);

  actionSavePointsRight->setIcon(saveIcon);
  this->toolBar_right->addAction(actionSavePointsRight);

  actionLoadPointsRight->setIcon(openIcon);
  this->toolBar_right->addAction(actionLoadPointsRight);

  // Initialize
  this->LeftPane = NULL;
  this->RightPane = NULL;
};

void Form::LoadPoints(Pane* pane)
{
  // This function reads existing correspondences from a plain text file and displays them in the left pane.

  if(!this->LeftPane)
    {
    std::cerr << "Cannot load points unless an image or point cloud is loaded!" << std::endl;
    return;
    }
    
  // Get a filename to open
  QString fileName = QFileDialog::getOpenFileName(this, "Open File", ".", "Text Files (*.txt)");

  std::cout << "Got filename: " << fileName.toStdString() << std::endl;
  if(fileName.toStdString().empty())
    {
    std::cout << "Filename was empty." << std::endl;
    return;
    }

  if(dynamic_cast<Pane2D*>(pane))
    {
    LoadPoints2D(dynamic_cast<Pane2D*>(pane), fileName.toStdString());
    }
  else if(dynamic_cast<Pane3D*>(pane))
    {
    LoadPoints3D(dynamic_cast<Pane3D*>(pane), fileName.toStdString());
    }
}


void Form::LoadPoints2D(Pane2D* pane, const std::string& filename)
{
  std::string line;
  std::ifstream fin(filename.c_str());

  if(fin == NULL)
    {
    std::cout << "Cannot open file." << std::endl;
    }

  pane->SelectionStyle->RemoveAll();

  while(getline(fin, line))
    {
    std::stringstream ss;
    ss << line;
    double p[3];
    ss >> p[0] >> p[1];
    p[2] = 0;

    pane->SelectionStyle->AddNumber(p);
    }
}

void Form::LoadPoints3D(Pane3D* pane, const std::string& filename)
{
  std::string line;
  std::ifstream fin(filename.c_str());

  if(fin == NULL)
    {
    std::cout << "Cannot open file." << std::endl;
    }

  pane->SelectionStyle->RemoveAll();

  while(getline(fin, line))
    {
    std::stringstream ss;
    ss << line;
    double p[3];
    ss >> p[0] >> p[1] >> p[2];

    pane->SelectionStyle->AddNumber(p);
    }
}

void Form::SavePoints(Pane* pane)
{
  if(!pane)
    {
    std::cerr << "You must have loaded and selected points from this pane!" << std::endl;
    return;
    }

  QString fileName = QFileDialog::getSaveFileName(this, "Save File", ".", "Text Files (*.txt)");
  std::cout << "Got filename: " << fileName.toStdString() << std::endl;
  if(fileName.toStdString().empty())
    {
    std::cout << "Filename was empty." << std::endl;
    return;
    }

  if(dynamic_cast<Pane2D*>(pane))
    {
    SavePoints2D(dynamic_cast<Pane2D*>(pane), fileName.toStdString());
    }
  else if(dynamic_cast<Pane3D*>(pane))
    {
    SavePoints3D(dynamic_cast<Pane3D*>(pane), fileName.toStdString());
    }
  
}

void Form::SavePoints2D(Pane2D* pane, const std::string& filename)
{
  std::ofstream fout(filename.c_str());

  for(unsigned int i = 0; i < pane->SelectionStyle->GetNumberOfCorrespondences(); i++)
    {
    fout << pane->SelectionStyle->GetCorrespondence(i).x << " "
         << pane->SelectionStyle->GetCorrespondence(i).y << std::endl;

    }
  fout.close();
}

void Form::SavePoints3D(Pane3D* pane, const std::string& filename)
{
  std::ofstream fout(filename.c_str());

  for(unsigned int i = 0; i < pane->SelectionStyle->GetNumberOfCorrespondences(); i++)
    {
    fout << pane->SelectionStyle->GetCorrespondence(i).x << " "
         << pane->SelectionStyle->GetCorrespondence(i).y << " "
         << pane->SelectionStyle->GetCorrespondence(i).z << std::endl;
    }
  fout.close();
}

void Form::Load(Pane* pane)
{
  // Open an image or point cloud in the left pane.

  // Get a filename to open
  QString fileName = QFileDialog::getOpenFileName(this, "Open File", ".", "Image Files (*.png *.mha *.vtp)");

  std::cout << "Got filename: " << fileName.toStdString() << std::endl;
  if(fileName.toStdString().empty())
    {
    std::cout << "Filename was empty." << std::endl;
    return;
    }

  QFileInfo fileInfo(fileName.toStdString().c_str());
  std::string extension = fileInfo.suffix().toStdString();
  std::cout << "extension: " << extension << std::endl;

  if(extension.compare("png") == 0 || extension.compare("mha") == 0)
    {
    LoadImage(pane, fileName.toStdString());
    }
  else if(extension.compare("vtp") == 0)
    {
    LoadPointCloud(pane, fileName.toStdString());
    }
  else
    {
    std::cerr << "The extension " << extension << " is not known!" << std::endl;
    return;
    }
}

void Form::LoadImage(Pane* inputPane, const std::string& filename)
{
  Pane2D* pane = static_cast<Pane2D*>(inputPane);

  typedef itk::ImageFileReader<FloatVectorImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(filename);
  reader->Update();

  //static_cast<Pane2D*>(pane)->Image = reader->GetOutput();
  pane->Image = reader->GetOutput();

  if(this->chkRGB->isChecked())
    {
    Helpers::ITKImagetoVTKRGBImage(pane->Image, pane->ImageData);
    }
  else
    {
    Helpers::ITKImagetoVTKMagnitudeImage(pane->Image, pane->ImageData);
    }

  pane->ImageActor->SetInput(pane->ImageData);
  pane->ImageActor->InterpolateOff();

  // Add Actor to renderer
  pane->Renderer->AddActor(pane->ImageActor);
  pane->Renderer->ResetCamera();

  vtkSmartPointer<vtkPointPicker> pointPicker = vtkSmartPointer<vtkPointPicker>::New();
  pane->qvtkWidget->GetRenderWindow()->GetInteractor()->SetPicker(pointPicker);
  pane->SelectionStyle = PointSelectionStyle2D::New();
  //this->LeftPointSelectionStyle->SetCurrentRenderer(this->LeftRenderer);
  pane->SelectionStyle->SetCurrentRenderer(pane->Renderer);
  pane->qvtkWidget->GetRenderWindow()->GetInteractor()->SetInteractorStyle(static_cast<PointSelectionStyle2D*>(pane->SelectionStyle));

  pane->Renderer->ResetCamera();

  // Flip the image by changing the camera view up because of the conflicting conventions used by ITK and VTK
  //this->LeftRenderer = vtkSmartPointer<vtkRenderer>::New();
  //this->LeftRenderer->GradientBackgroundOn();
  //this->LeftRenderer->SetBackground(this->BackgroundColor);
  //this->LeftRenderer->SetBackground2(1,1,1);

  double cameraUp[3];
  cameraUp[0] = 0;
  cameraUp[1] = -1;
  cameraUp[2] = 0;
  pane->Renderer->GetActiveCamera()->SetViewUp(cameraUp);

  double cameraPosition[3];
  pane->Renderer->GetActiveCamera()->GetPosition(cameraPosition);
  //std::cout << cameraPosition[0] << " " << cameraPosition[1] << " " << cameraPosition[2] << std::endl;
  //cameraPosition[0] *= -1;
  //cameraPosition[1] *= -1;
  cameraPosition[2] *= -1;
  pane->Renderer->GetActiveCamera()->SetPosition(cameraPosition);

  // Verify
  pane->Renderer->GetActiveCamera()->GetPosition(cameraPosition);
  //std::cout << cameraPosition[0] << " " << cameraPosition[1] << " " << cameraPosition[2] << std::endl;

  pane->qvtkWidget->GetRenderWindow()->Render();
}

void Form::LoadPointCloud(Pane* inputPane, const std::string& filename)
{
  Pane3D* pane = static_cast<Pane3D*>(inputPane);

  vtkSmartPointer<vtkXMLPolyDataReader> reader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
  reader->SetFileName(filename.c_str());
  reader->Update();
  reader->GetOutput()->GetPointData()->SetActiveScalars("Intensity");

  float range[2];
  vtkFloatArray::SafeDownCast(reader->GetOutput()->GetPointData()->GetArray("Intensity"))->GetValueRange(range);

  vtkSmartPointer<vtkLookupTable> lookupTable = vtkSmartPointer<vtkLookupTable>::New();
  //lookupTable->SetTableRange(0.0, 10.0);
  lookupTable->SetTableRange(range[0], range[1]);
  //lookupTable->SetHueRange(0, .5);
  //lookupTable->SetHueRange(.5, 1);
  lookupTable->SetHueRange(0, 1);

  pane->PointCloudMapper->SetInputConnection(reader->GetOutputPort());
  pane->PointCloudMapper->SetLookupTable(lookupTable);
  pane->PointCloudActor->SetMapper(pane->PointCloudMapper);
  pane->PointCloudActor->GetProperty()->SetRepresentationToPoints();

  // Add Actor to renderer
  pane->Renderer->AddActor(pane->PointCloudActor);
  pane->Renderer->ResetCamera();

  vtkSmartPointer<vtkPointPicker> pointPicker = vtkSmartPointer<vtkPointPicker>::New();

  pointPicker->PickFromListOn();
  pointPicker->AddPickList(pane->PointCloudActor);
  pane->qvtkWidget->GetRenderWindow()->GetInteractor()->SetPicker(pointPicker);
  pane->SelectionStyle = PointSelectionStyle3D::New();
  pane->SelectionStyle->SetCurrentRenderer(pane->Renderer);
  static_cast<PointSelectionStyle3D*>(pane->SelectionStyle)->Data = reader->GetOutput();
  pane->qvtkWidget->GetRenderWindow()->GetInteractor()->SetInteractorStyle(static_cast<PointSelectionStyle3D*>(pane->SelectionStyle));

  pane->Renderer->ResetCamera();

  float averageSpacing = Helpers::ComputeAverageSpacing(reader->GetOutput()->GetPoints());
  static_cast<PointSelectionStyle3D*>(pane->SelectionStyle)->SetMarkerRadius(averageSpacing);
}

void Form::on_actionLoadPointsLeft_activated()
{
  LoadPoints(this->LeftPane);
}

void Form::on_actionLoadPointsRight_activated()
{
  LoadPoints(this->RightPane);
}

void Form::on_actionOpenImageLeft_activated()
{
  if(this->LeftPane)
    {
    delete this->LeftPane;
    }
  this->LeftPane = new Pane2D(this->qvtkWidgetLeft);
  Load(this->LeftPane);
}

void Form::on_actionOpenImageRight_activated()
{
  if(this->RightPane)
    {
    delete this->RightPane;
    }
  this->RightPane = new Pane2D(this->qvtkWidgetRight);
  Load(this->RightPane);
}


void Form::on_actionOpenPointCloudLeft_activated()
{
  if(this->LeftPane)
    {
    delete this->LeftPane;
    }
  this->LeftPane = new Pane3D(this->qvtkWidgetLeft);
  Load(this->LeftPane);
}

void Form::on_actionOpenPointCloudRight_activated()
{
  if(this->RightPane)
    {
    delete this->RightPane;
    }
  this->RightPane = new Pane3D(this->qvtkWidgetRight);
  Load(this->RightPane);
}

void Form::on_actionSavePointsLeft_activated()
{
  SavePoints(this->LeftPane);
}

void Form::on_actionSavePointsRight_activated()
{
  SavePoints(this->RightPane);
}

void Form::on_btnDeleteLastCorrespondenceLeft_clicked()
{
  this->LeftPane->SelectionStyle->DeleteLastCorrespondence();
  this->LeftPane->Refresh();
}

void Form::on_btnDeleteAllCorrespondencesLeft_clicked()
{
  this->LeftPane->SelectionStyle->RemoveAll();
  this->LeftPane->Refresh();
  //static_cast<PointSelectionStyle2D*>(pane->SelectionStyle)->RemoveAll();
  //this->qvtkWidgetLeft->GetRenderWindow()->Render();
}

void Form::on_btnDeleteLastCorrespondenceRight_clicked()
{
  this->RightPane->SelectionStyle->DeleteLastCorrespondence();
  this->RightPane->Refresh();
}

void Form::on_btnDeleteAllCorrespondencesRight_clicked()
{
  this->RightPane->SelectionStyle->RemoveAll();
  this->RightPane->Refresh();
  //static_cast<PointSelectionStyle2D*>(pane->SelectionStyle)->RemoveAll();
  //this->qvtkWidgetRight->GetRenderWindow()->Render();
}
