template<typename T>
void RemoveAllActors(std::vector<T*>& actors, vtkRenderer* renderer)
{
  for(unsigned int i = 0; i < actors.size(); ++i)
    {
    renderer->RemoveViewProp( actors[i]);
    }
}
