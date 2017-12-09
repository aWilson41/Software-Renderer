#pragma once
#include "Rasterizer.h"
#include "RayTracer.h"

class SoftwareDevice
{
private:
	RenderTechnique tech;
	Shader shader;
	Sampler sampler;

public:
	Screen screen;
	Rasterizer rasterizer;
	RayTracer rayTracer;

public:
	void InitDevice(UINT windowSizeX, UINT windowSizeY);

	void SetTech(RenderTechnique tech, Shader shader, Sampler sampler);

	void SetTechnique(RenderTechnique tech) { SoftwareDevice::tech = tech; }
	RenderTechnique GetTechnique() { return tech; }

	void SetShader(Shader shader) { SoftwareDevice::shader = shader; }
	Shader GetShader() { return shader; }

	void SetSampler(Sampler sampler) { SoftwareDevice::sampler = sampler; }
	Sampler GetSampler() { return sampler; }
};