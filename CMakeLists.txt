cmake_minimum_required(VERSION 2.6)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

PROJECT(SelectCorrespondences)

FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})

if(${VTK_MAJOR_VERSION} LESS 5 OR ${VTK_MINOR_VERSION} LESS 8)
  message(FATAL_ERROR "Your VTK version (${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}) is too old! At least 5.8 is required.")
endif()

if(NOT VTK_USE_QVTK)
  message(FATAL_ERROR "You must compile VTK with VTK_USE_QT=ON. To do this, you must also use VTK_USE_GUISUPPORT=ON and BUILD_SHARED_LIBS=ON.")
endif()

FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})

FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})

QT4_WRAP_UI(UISrcs SelectCorrespondencesWidget.ui)
QT4_WRAP_CPP(MOCSrcs SelectCorrespondencesWidget.h)

ADD_EXECUTABLE(SelectCorrespondences
SelectCorrespondences.cpp
SelectCorrespondencesWidget.cpp
Helpers.cpp
Pane.cpp
Pane2D.cpp
Pane3D.cpp
PointSelectionStyle.cpp
PointSelectionStyle2D.cpp
PointSelectionStyle3D.cpp
${UISrcs} ${MOCSrcs} ${ResourceSrcs})
TARGET_LINK_LIBRARIES(SelectCorrespondences QVTK ${VTK_LIBRARIES} ${ITK_LIBRARIES})
