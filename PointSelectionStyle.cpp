#include "PointSelectionStyle.h"

unsigned int PointSelectionStyle::GetNumberOfCorrespondences()
{
  return this->Numbers.size();
}

Coord3D PointSelectionStyle::GetCorrespondence(unsigned int i)
{
  return this->Coordinates[i];
}

void PointSelectionStyle::SetMarkerRadius(float radius)
{
  this->MarkerRadius = radius;
  //this->DotSource->SetRadius(this->MarkerRadius);
  //this->DotSource->Update();
}

