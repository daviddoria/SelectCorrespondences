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

#ifndef FORM_H
#define FORM_H

#include "ui_Form.h"

// VTK
#include <vtkSmartPointer.h>
#include <vtkSeedWidget.h>
#include <vtkPointHandleRepresentation2D.h>

// ITK
#include "itkImage.h"

// Qt
#include <QMainWindow>

// Custom
#include "Pane.h"
#include "Pane2D.h"
#include "Pane3D.h"
#include "PointSelectionStyle.h"
#include "Types.h"

// Forward declarations
class vtkActor;
class vtkBorderWidget;
class vtkImageData;
class vtkImageActor;
class vtkPolyData;
class vtkPolyDataMapper;
class vtkRenderer;

class Form : public QMainWindow, public Ui::Form
{
  Q_OBJECT
public:

  // Constructor/Destructor
  Form();
  ~Form() {};

public slots:
  void on_actionOpenImageLeft_activated();
  void on_actionOpenPointCloudLeft_activated();
  void on_actionSavePointsLeft_activated();
  void on_actionLoadPointsLeft_activated();
  
  void on_actionOpenImageRight_activated();
  void on_actionOpenPointCloudRight_activated();
  void on_actionSavePointsRight_activated();
  void on_actionLoadPointsRight_activated();
  
  void on_actionHelp_activated();
  void on_actionQuit_activated();
  
  void on_btnDeleteLastCorrespondenceLeft_clicked();
  void on_btnDeleteAllCorrespondencesLeft_clicked();
  void on_btnDeleteLastCorrespondenceRight_clicked();
  void on_btnDeleteAllCorrespondencesRight_clicked();
  
protected:

  void LoadPoints(Pane* pane);
  void LoadPoints2D(Pane2D* pane, const std::string& filename);
  void LoadPoints3D(Pane3D* pane, const std::string& filename);

  void SavePoints(Pane* pane);
  void SavePoints2D(Pane2D* pane, const std::string& filename);
  void SavePoints3D(Pane3D* pane, const std::string& filename);
  
  void Load(Pane* pane);
  void LoadImage(Pane* pane, const std::string& filename);
  void LoadPointCloud(Pane* pane, const std::string& filename);
  
  Pane* LeftPane;
  Pane* RightPane;
  
};

#endif // Form_H
