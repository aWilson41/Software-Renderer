#include "SoftwareDevice.h"

void SoftwareDevice::InitDevice(UINT windowSizeX, UINT windowSizeY)
{
	screen.SetDim(windowSizeX, windowSizeY);
	SetTechnique(RenderTechnique::POINTGRAPH);
	SetShader(Shader::NOSHADER);
	SetSampler(Sampler::NOSAMPLE);
}

void SoftwareDevice::SetTech(RenderTechnique tech, Shader shader, Sampler sampler)
{
	SetTechnique(tech);
	SetShader(shader);
	SetSampler(sampler);
}