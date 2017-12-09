#pragma once
#include "MathHelper.h"
#include "Utility.h"

class Screen
{
public:
	Screen();
	Screen(UINT width, UINT height);
	~Screen();

	unsigned char* GetBuffer();
	float* GetZBuffer();
	float GetZ(int x, int y);
	float GetZ(int i);
	UINT GetScreenWidth() { return mScreenWidth; }
	UINT GetScreenHeight() { return mScreenHeight; }

	void SetPixel(int x, int y, Color color);
	void SetPixel(int x, int y, unsigned char grayScaleColor);
	void SetSquare(int x, int y, Color color, UINT size);
	void SetDim(UINT width, UINT height);
	void SetZ(int x, int y, float z);
	void SetPixelZ(int x, int y, float z, Color color);
	void SetPixelZ(int i, float z, Color color);
	void Cls();
	void ClsColor(Color color);

private:
	UINT mScreenWidth;
	UINT mScreenHeight;
	unsigned char* mBuffer = nullptr;
	float* mZBuffer = nullptr;
};