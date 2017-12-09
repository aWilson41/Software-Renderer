#include "Screen.h"

Screen::Screen() : mScreenWidth(0), mScreenHeight(0)
{

}

Screen::Screen(UINT width, UINT height)
{
	SetDim(width, height);
}

Screen::~Screen()
{
	if(mBuffer)
		delete[] mBuffer;
	if(mZBuffer)
		delete[] mZBuffer;
}

unsigned char* Screen::GetBuffer()
{
	return mBuffer;
}

float* Screen::GetZBuffer()
{
	return mZBuffer;
}

float Screen::GetZ(int x, int y)
{
	if (x >= 0 && y >= 0 && x < (int)mScreenWidth && y < (int)mScreenHeight)
		return mZBuffer[Utility::Index2DTo1D(x, y, mScreenWidth)];
	else
		return 0.0f;
}

// Bounds aren't checked
float Screen::GetZ(int i)
{
	return mZBuffer[i];
}

void Screen::SetPixel(int x, int y, Color color)
{
	if (x >= 0 && y >= 0 && x < (int)mScreenWidth && y < (int)mScreenHeight)
    {
        // Multiplied by 3 for the 3 color channels
		UINT index = Utility::Index2DTo1D(x, y, mScreenWidth) * 3;
	    mBuffer[index] = color.r;
	    mBuffer[index + 1] = color.g;
	    mBuffer[index + 2] = color.b;
    }
}

void Screen::SetPixel(int x, int y, unsigned char grayScaleColor)
{
	if (x >= 0 && y >= 0 && x < (int)mScreenWidth && y < (int)mScreenHeight)
	{
		// Multiplied by 3 for the 3 color channels
		int index = Utility::Index2DTo1D(x, y, mScreenWidth) * 3;
		mBuffer[index] = grayScaleColor;
		mBuffer[index + 1] = grayScaleColor;
		mBuffer[index + 2] = grayScaleColor;
	}
}

void Screen::SetPixelZ(int x, int y, float z, Color color)
{
	if (x >= 0 && y >= 0 && x < (int)mScreenWidth && y < (int)mScreenHeight)
	{
		// Multiplied by 3 for the 3 color channels
		UINT i = Utility::Index2DTo1D(x, y, mScreenWidth);
		UINT j = i * 3;
		mBuffer[j] = color.r;
		mBuffer[j + 1] = color.g;
		mBuffer[j + 2] = color.b;
		mZBuffer[i] = z;
	}
}

// Bounds aren't checked
void Screen::SetPixelZ(int i, float z, Color color)
{
	UINT j = i * 3;
	mBuffer[j] = color.r;
	mBuffer[j + 1] = color.g;
	mBuffer[j + 2] = color.b;
	mZBuffer[i] = z;
}

void Screen::SetZ(int x, int y, float z)
{
	if (x >= 0 && y >= 0 && x < (int)mScreenWidth && y < (int)mScreenHeight)
		mZBuffer[Utility::Index2DTo1D(x, y, mScreenWidth)] = z;
}

// Sets a square of pixels of size * size
// If size is not even it floors size / 2
// Size represents 1 pixel
void Screen::SetSquare(int x, int y, Color color, UINT size)
{
	// Top left
	int xS = x - (int)(size * 0.5f);
	int yS = y - (int)(size * 0.5f);

	for (int i = 0; i < (int)size; i++)
	{
		for (int j = 0; j < (int)size; j++)
		{
			SetPixel(xS + i, yS + j, color);
		}
	}
}

void Screen::SetDim(UINT width, UINT height)
{
	if (mBuffer != nullptr)
		delete[] mBuffer;
	if (mZBuffer != nullptr)
		delete[] mZBuffer;

	mScreenWidth = width;
	mScreenHeight = height;

	// 3 rgb channels so we multiply 3
	mBuffer = new unsigned char[mScreenWidth * mScreenHeight * 3];
	std::fill_n(mBuffer, mScreenWidth * mScreenHeight * 3, 0);
	mZBuffer = new float[mScreenWidth * mScreenHeight];
	std::fill_n(mZBuffer, mScreenWidth * mScreenHeight, std::numeric_limits<float>::max());
}


void Screen::Cls()
{
	std::fill_n(mBuffer, mScreenWidth * mScreenHeight * 3, 255);
    std::fill_n(mZBuffer, mScreenWidth * mScreenHeight, std::numeric_limits<float>::max());
}

void Screen::ClsColor(Color color)
{
	for (UINT i = 0; i < mScreenWidth * mScreenHeight * 3; i += 3)
	{
		mBuffer[i] = color.r;
		mBuffer[i + 1] = color.g;
		mBuffer[i + 2] = color.b;
	}

	std::fill_n(mZBuffer, mScreenWidth * mScreenHeight, std::numeric_limits<float>::max());
}