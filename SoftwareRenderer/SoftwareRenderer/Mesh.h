#pragma once
#include "Material.h"

class Mesh
{
public:
   std::string name;

   std::vector<float3> vertices;
   std::vector<float3> vertexNormals;
   std::vector<float2> texCoords;
   std::vector<UINT> indices;

   matrix4x4 world;

   Material* mat;

public:
   Mesh();
};