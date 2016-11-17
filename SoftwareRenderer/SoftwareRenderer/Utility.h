#pragma once
#include <algorithm>
#include "MathHelper.h"
#include "LodePNG\lodepng.h"

enum RenderTechnique
{
	POINTGRAPH,
	WIREFRAME,
	RASTER,
	RAYTRACE
};

enum Shader
{
	NOSHADER,
	FLAT,
	SMOOTH,
   PHONGFLAT,
   PHONGSMOOTH
};

enum Sampler
{
	NOSAMPLE,
	POINTSAMPLE
};

enum RasterizationTechnique
{
	BARYCENTRIC,
	DDA
};

class Color
{
public:
	unsigned char r;
	unsigned char g;
	unsigned char b;

	Color() : r(0), g(0), b(0)
	{

	}

	Color(unsigned char r, unsigned char g, unsigned char b)
	{
		Color::r = r;
		Color::g = g;
		Color::b = b;
	}

	Color operator+(const Color& a)
	{
		return Color(r + a.r, g + a.g, b + a.b);
	}

   Color operator-(const Color& a)
   {
      return Color(r - a.r, g - a.g, b - a.b);
   }

	Color operator*(const float& a)
	{
		return Color((char)(r * a), (char)(g * a), (char)(b * a));
	}

	Color operator*(const Color& a)
	{
		return Color(r * a.r, g * a.g, b * a.b);
	}
};

class Utility
{
public:
	// Gets the index in 1 dimensional array of size w * h
	static int Index2DTo1D(int x, int y, UINT w)
	{
		return y * w + x;
	}

	static float Saturate(float a)
	{
		if (a < 0)
			a = 0;

		return a;
	}

   static Color Saturate(Color color)
   {
      Color ans = Color(0, 0, 0);

      if (color.r > 255)
         color.r = 255;
      if (color.g > 255)
         color.g = 255;
      if (color.b > 255)
         color.b = 255;

      return ans;
   }

	// Returns true if successful
	static bool SaveScreenshot(unsigned char* image, UINT width, UINT height, std::string fileName = "output.png")
	{
		// Convert Screen Buffer to LodePng compatiable format
		std::vector<unsigned char> convertedImage = std::vector<unsigned char>(width * height * 4u);
		// image starts from the bottom left going right, up
		// convertedImage starts from the top left going right, down
		// So we need to flip the y coordinate (hence height - y)
		for (UINT y = 0; y < height; y++)
		{
			for (UINT x = 0; x < width; x++)
			{
				convertedImage[4 * (width * y + x)] = image[3 * (width * (height - y - 1) + x)];
				convertedImage[4 * (width * y + x) + 1] = image[3 * (width * (height - y - 1) + x) + 1];
				convertedImage[4 * (width * y + x) + 2] = image[3 * (width * (height - y - 1) + x) + 2];
				convertedImage[4 * (width * y + x) + 3] = 255;
			}
		}

		// Saves the image
		UINT error = lodepng::encode(fileName.c_str(), convertedImage, width, height);

		if (error)
			return false;
		else
			return true;
	}
};