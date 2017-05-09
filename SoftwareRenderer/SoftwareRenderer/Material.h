#pragma once
#include "Utility.h"
#include "Texture.h"

class Material
{
public:
	Color diffuse;
	Color specular;
	Color ambient;
    float specularShine;
	Texture* diffuseMap;
	bool hasDiffuseMap;

public:
	Material();
};