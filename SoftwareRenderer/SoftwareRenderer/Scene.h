#pragma once
#include "Camera.h"
#include "Mesh.h"
#include "SoftwareDevice.h"

class Scene
{
private:  
   std::string rootPath;

public:
   Camera cam;

   // Resources
   std::vector<Mesh> mesh;
   std::vector<Material> material;
   std::vector<Texture> texture;

   std::vector<float4> dirLight;

   ~Scene();

   int FindTexture(std::string name);
   void DrawSceneToBuffer(SoftwareDevice* sDevice);
};