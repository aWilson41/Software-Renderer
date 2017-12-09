#pragma once
#include "Utility.h"

enum TextureType
{
	DIFFUSEMAP,
	NORMALMAP
};

class Texture
{
public:
	std::string name;
	TextureType type;
	unsigned char* buffer;
	UINT width;
	UINT height;

public:
	Texture();
	Texture(std::string name, UINT width, UINT height, TextureType type);
	Texture(std::string name, UINT width, UINT height, unsigned char* data, TextureType type);

	void SetDim(UINT width, UINT height);
	void SetPixel(int x, int y, Color color);

	float2 GetDim() { return float2((float)width, (float)height); }
	Color GetPixel(int x, int y);
	Color GetPixel(UINT i);

	void Fill(unsigned char grayScaleColor);
	void SetBuffer(UINT width, UINT height, unsigned char* buffer);
};