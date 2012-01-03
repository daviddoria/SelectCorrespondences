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

#ifndef SelectCorrespondencesWidget_H
#define SelectCorrespondencesWidget_H

#include "ui_SelectCorrespondencesWidget.h"

// VTK
#include <vtkSmartPointer.h>
#include <vtkSeedWidget.h>
#include <vtkPointHandleRepresentation2D.h>

// ITK
#include "itkImage.h"

// Qt
#include <QMainWindow>
#include <QFutureWatcher>
class QProgressDialog;

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

class SelectCorrespondencesWidget : public QMainWindow, public Ui::SelectCorrespondencesWidget
{
  Q_OBJECT
public:

  // Constructor/Destructor
  SelectCorrespondencesWidget();
  ~SelectCorrespondencesWidget() {};

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
  
  void on_actionFlipLeftHorizontally_activated();
  void on_actionFlipLeftVertically_activated();
  void on_actionFlipRightHorizontally_activated();
  void on_actionFlipRightVertically_activated();
  
private:

  QFutureWatcher<void> FutureWatcher;
  QProgressDialog* ProgressDialog;

  void LoadPoints(Pane* const pane);
  void LoadPoints2D(Pane2D* const pane, const std::string& filename);
  void LoadPoints3D(Pane3D* const pane, const std::string& filename);

  void SavePoints(Pane* const pane);
  void SavePoints2D(Pane2D* const pane, const std::string& filename);
  void SavePoints3D(Pane3D* const pane, const std::string& filename);
  
  void LoadImage(Pane* const pane);
  void LoadPointCloud(Pane* const pane);
  
  Pane* LeftPane;
  Pane* RightPane;
  
};

#endif
