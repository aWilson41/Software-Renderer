#pragma once
#include "Utility.h"
#include "Texture.h"

class Material
{
public:
	Color diffuse;
	Color specular;
   float specularShine;
	Color ambient;
	Texture* diffuseMap;
	bool hasDiffuseMap;

public:
	Material();
};