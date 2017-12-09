#pragma once
#include "Screen.h"
#include "Camera.h"
#include "Mesh.h"

class RayTracer
{
public:
	void Trace(Screen* screen, Camera* cam, std::vector<Mesh>* mesh, float3 dirLight);
};