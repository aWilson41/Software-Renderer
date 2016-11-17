#include "Rasterizer.h"

Rasterizer::Rasterizer() : rTech(BARYCENTRIC)
{

}


void Rasterizer::DrawBhmLineColor(Screen* screen, float2 p1, float2 p2, Color color)
{
	int dx = (int)(p2.x - p1.x);
	int dy = (int)(p2.y - p1.y);
	int dx1 = abs(dx);
	int dy1 = abs(dy);
	int px = 2 * dy1 - dx1;
	int py = 2 * dx1 - dy1;

	if (dy1 <= dx1)
	{
		int x = (int)p2.x;
		int y = (int)p2.y;
		int eX = (int)p1.x;
		if (dx >= 0)
		{
			x = (int)p1.x;
			y = (int)p1.y;
			eX = (int)p2.x;
		}

		screen->SetPixel(x, y, color);
		for (int i = 0; x < eX; i++)
		{
			x++;
			if (px < 0)
				px += 2 * dy1;
			else
			{
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
					y++;
				else
					y--;

				px += 2 * (dy1 - dx1);
			}
			screen->SetPixel(x, y, color);
		}
	}
	else
	{
		int x = (int)p2.x;
		int y = (int)p2.y;
		int eY = (int)p1.y;
		if (dy >= 0)
		{
			x = (int)p1.x;
			y = (int)p1.y;
			eY = (int)p2.y;
		}

		screen->SetPixel(x, y, color);
		for (int i = 0; y < eY; i++)
		{
			y++;
			if (py <= 0)
				py += 2 * dx1;
			else
			{
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
					x++;
				else
					x--;
				py += 2 * (dx1 - dy1);
			}
			screen->SetPixel(x, y, color);
		}
	}
}

// Does not sort x1 or x2 (Req: x1 < x2)
void Rasterizer::DrawRightScanLine(Screen* screen, int x1, int x2, int y, Color color)
{
	for (int i = x1; i <= x2; i++)
	{
		screen->SetPixel(i, y, color);
	}
}

// Sorts x1 and x2
void Rasterizer::DrawSortedScanLine(Screen* screen, int x1, int x2, int y, Color color)
{
	int max = x1;
	int min = x2;
	if (x2 > max)
	{
		max = x2;
		min = x1;
	}

	for (int i = min; i <= max; i++)
	{
		screen->SetPixel(i, y, color);
	}
}

void Rasterizer::DrawBhmEllipse(Screen* screen, float2 center, float xRadius, float yRadius, Color color)
{
	int cX = (int)center.x;
	int cY = (int)center.y;
	int a2 = (int)(xRadius * xRadius);
	int b2 = (int)(yRadius * yRadius);
	int fa2 = 4 * a2;
	int fb2 = 4 * b2;

	// First half
	for (int x = 0, y = (int)yRadius, sigma = 2 * b2 + a2 * (1 - 2 * (int)yRadius); b2 * x <= a2 * y; x++)
	{
		screen->SetPixel(cX + x, cY + y, color);
		screen->SetPixel(cX - x, cY + y, color);
		screen->SetPixel(cX + x, cY - y, color);
		screen->SetPixel(cX - x, cY - y, color);

		if (sigma >= 0)
		{
			sigma += fa2 * (1 - y);
			y--;
		}
		sigma += b2 * ((4 * x) + 6);
	}

	// Second half
	for (int x = (int)xRadius, y = 0, sigma = 2 * a2 + b2 * (1 - 2 * (int)xRadius); a2 * y <= b2 * x; y++)
	{
		screen->SetPixel(cX + x, cY + y, color);
		screen->SetPixel(cX - x, cY + y, color);
		screen->SetPixel(cX + x, cY - y, color);
		screen->SetPixel(cX - x, cY - y, color);

		if (sigma >= 0)
		{
			sigma += fb2 * (1 - x);
			x--;
		}
		sigma += a2 * ((4 * y) + 6);
	}
}


// Draw Triangles
void Rasterizer::DrawWireTriangle(Screen* screen, Triangle2 triangle, Color color)
{
	DrawBhmLineColor(screen, triangle.p1, triangle.p2, color);
	DrawBhmLineColor(screen, triangle.p2, triangle.p3, color);
	DrawBhmLineColor(screen, triangle.p3, triangle.p1, color);
}

// Incomplete needs z interpolation
void Rasterizer::DrawScanlineTriangle(Screen* screen, Triangle3 triangle, Color color) 
{
	// Sort triangle points
	float2 p1 = float2(triangle.p1.x, triangle.p1.y); // Greatest y value
	float2 p2 = float2(triangle.p2.x, triangle.p2.y); // Second greatest y value
	float2 p3 = float2(triangle.p3.x, triangle.p3.y); // Least greatest y valu

	if (p2.y > p1.y)
	{
		float2 tmp = p1;
		p1 = p2;
		p2 = tmp;
	}

	if (p3.y > p2.y)
	{
		float2 tmp = p2;
		p2 = p3;
		p3 = tmp;
	}

	if (p2.y > p1.y)
	{
		float2 tmp = p1;
		p1 = p2;
		p2 = tmp;
	}

	// Triangle information
	float dy1 = p3.y - p1.y;
	float dx1 = p3.x - p1.x;
	float invM1 = dx1 / dy1;

	float dy2 = p2.y - p1.y;
	float dx2 = p2.x - p1.x;
	float invM2 = dx2 / dy2;

	float dy3 = p3.y - p2.y;
	float dx3 = p3.x - p2.x;
	float invM3 = dx3 / dy3;

	if (p2.y == p1.y)
	{
		// Flat Top
		float curX1 = p3.x;
		float curX2 = p3.x;
		for (int y = (int)p3.y; y < (int)p2.y; y++)
		{
			DrawSortedScanLine(screen, (int)curX1, (int)curX2, y, color);
			curX2 += invM3;
			curX1 += invM1;
		}
	}
	else if (p2.y == p3.y)
	{
		// Bottom Flat
		float curX1 = p1.x;
		float curX2 = p1.x;
		for (int y = (int)p1.y; y > (int)p2.y; y--)
		{
			DrawSortedScanLine(screen, (int)curX1, (int)curX2, y, color);
			curX1 -= invM2;
			curX2 -= invM1;
		}
	}
	else
	{
		// Bottom Flat
		float curX1 = p1.x;
		float curX2 = p1.x;
		for (int y = (int)p1.y; y > (int)p2.y; y--)
		{
			DrawSortedScanLine(screen, (int)curX1, (int)curX2, y, color);
			curX1 -= invM2;
			curX2 -= invM1;
		}

		float2 p4 = float2((int)(p1.x + dy2 * (p3.x - p1.x) / (p3.y - p1.y)), p2.y);
      invM3 = dx3 / dy3;
      invM1 = (p3.x - p4.x) / (p3.y - p4.y);
		// Top Flat (buggy when y <= p2.y?)
		curX1 = p2.x;
		curX2 = p4.x;
		for (int y = (int)p2.y; y > (int)p3.y; y--)
		{
			DrawSortedScanLine(screen, (int)curX1, (int)curX2, y, color);
			curX2 -= invM1;
			curX1 -= invM3;
		}
	}
}

// Bary Centric Triangle Drawing
void Rasterizer::DrawBaryTriangle(Screen* screen, Triangle3 triangle, Color color)
{
	// Find the extrema
	float maxX = triangle.p1.x;
	float minX = triangle.p1.x;
	float maxY = triangle.p1.y;
	float minY = triangle.p1.y;

	// MaxX
	if (triangle.p2.x > maxX)
		maxX = triangle.p2.x;
	if (triangle.p3.x > maxX)
		maxX = triangle.p3.x;

	// MinX
	if (triangle.p2.x < minX)
		minX = triangle.p2.x;
	if (triangle.p3.x < minX)
		minX = triangle.p3.x;

	// MaxY
	if (triangle.p2.y > maxY)
		maxY = triangle.p2.y;
	if (triangle.p3.y > maxY)
		maxY = triangle.p3.y;

	// MinY
	if (triangle.p2.y < minY)
		minY = triangle.p2.y;
	if (triangle.p3.y < minY)
		minY = triangle.p3.y;

	float2 p1 = MathHelper::ToFloat2(triangle.p1);
	float2 v0 = MathHelper::ToFloat2(triangle.p2) - p1;
	float2 v1 = MathHelper::ToFloat2(triangle.p3) - p1;

	float d00 = MathHelper::Dot(v0, v0);
	float d01 = MathHelper::Dot(v0, v1);
	float d11 = MathHelper::Dot(v1, v1);
	float denom = d00 * d11 - d01 * d01;

	for (int x = (int)minX; x <= (int)maxX; x++)
	{
		for (int y = (int)minY; y <= (int)maxY; y++)
		{
			float2 v2 = float2((float)x, (float)y) - p1;
			float d20 = MathHelper::Dot(v2, v0);
			float d21 = MathHelper::Dot(v2, v1);

			float v = (d11 * d20 - d01 * d21) / denom;
			float w = (d00 * d21 - d01 * d20) / denom;
			float u = 1.0f - v - w;

			// Find the z value and check the bounds
			if (u >= 0 && v >= 0 && w >= 0 && x >= 0 && y >= 0  && x < (int)screen->GetScreenWidth() && y < (int)screen->GetScreenHeight())
			{
				// Interpolate z value
				float z = triangle.p1.z * u + triangle.p2.z * v + triangle.p3.z * w;

				int i = (int)Utility::Index2DTo1D(x, y, screen->GetScreenWidth());
				// Depth test
				if (z < screen->GetZ(i))
					screen->SetPixelZ(i, z, color);
			}
		}
	}
}

void Rasterizer::DrawBaryTriangle(Screen* screen, Triangle3 triangle, Color color, float diffuse1, float diffuse2, float diffuse3)
{
	// Find the extrema
	float maxX = triangle.p1.x;
	float minX = triangle.p1.x;
	float maxY = triangle.p1.y;
	float minY = triangle.p1.y;

	// MaxX
	if (triangle.p2.x > maxX)
		maxX = triangle.p2.x;
	if (triangle.p3.x > maxX)
		maxX = triangle.p3.x;

	// MinX
	if (triangle.p2.x < minX)
		minX = triangle.p2.x;
	if (triangle.p3.x < minX)
		minX = triangle.p3.x;

	// MaxY
	if (triangle.p2.y > maxY)
		maxY = triangle.p2.y;
	if (triangle.p3.y > maxY)
		maxY = triangle.p3.y;

	// MinY
	if (triangle.p2.y < minY)
		minY = triangle.p2.y;
	if (triangle.p3.y < minY)
		minY = triangle.p3.y;

	float2 p1 = MathHelper::ToFloat2(triangle.p1);
	float2 v0 = MathHelper::ToFloat2(triangle.p2) - p1;
	float2 v1 = MathHelper::ToFloat2(triangle.p3) - p1;

	float d00 = MathHelper::Dot(v0, v0);
	float d01 = MathHelper::Dot(v0, v1);
	float d11 = MathHelper::Dot(v1, v1);
	float denom = d00 * d11 - d01 * d01;

	for (int x = (int)minX; x <= (int)maxX; x++)
	{
		for (int y = (int)minY; y <= (int)maxY; y++)
		{
			float2 v2 = float2((float)x, (float)y) - p1;
			float d20 = MathHelper::Dot(v2, v0);
			float d21 = MathHelper::Dot(v2, v1);

			float v = (d11 * d20 - d01 * d21) / denom;
			float w = (d00 * d21 - d01 * d20) / denom;
			float u = 1.0f - v - w;

			// Find the z value and check the bounds
			if (u >= 0 && v >= 0 && w >= 0 && x >= 0 && y >= 0 && x < (int)screen->GetScreenWidth() && y < (int)screen->GetScreenHeight())
			{
				// Interpolate z value
				float z = triangle.p1.z * u + triangle.p2.z * v + triangle.p3.z * w;

				int i = (int)Utility::Index2DTo1D(x, y, screen->GetScreenWidth());
				// Depth test
				if (z < screen->GetZ(i))
					screen->SetPixelZ(i, z, color * (diffuse1 * u + diffuse2 * v + diffuse3 * w));
			}
		}
	}
}

void Rasterizer::DrawBaryTriangle(Screen* screen, Triangle3 triangle, Color color1, Color color2, Color color3)
{
   // Find the extrema
   float maxX = triangle.p1.x;
   float minX = triangle.p1.x;
   float maxY = triangle.p1.y;
   float minY = triangle.p1.y;

   // MaxX
   if (triangle.p2.x > maxX)
      maxX = triangle.p2.x;
   if (triangle.p3.x > maxX)
      maxX = triangle.p3.x;

   // MinX
   if (triangle.p2.x < minX)
      minX = triangle.p2.x;
   if (triangle.p3.x < minX)
      minX = triangle.p3.x;

   // MaxY
   if (triangle.p2.y > maxY)
      maxY = triangle.p2.y;
   if (triangle.p3.y > maxY)
      maxY = triangle.p3.y;

   // MinY
   if (triangle.p2.y < minY)
      minY = triangle.p2.y;
   if (triangle.p3.y < minY)
      minY = triangle.p3.y;

   float2 p1 = MathHelper::ToFloat2(triangle.p1);
   float2 v0 = MathHelper::ToFloat2(triangle.p2) - p1;
   float2 v1 = MathHelper::ToFloat2(triangle.p3) - p1;

   float d00 = MathHelper::Dot(v0, v0);
   float d01 = MathHelper::Dot(v0, v1);
   float d11 = MathHelper::Dot(v1, v1);
   float denom = d00 * d11 - d01 * d01;

   for (int x = (int)minX; x <= (int)maxX; x++)
   {
      for (int y = (int)minY; y <= (int)maxY; y++)
      {
         float2 v2 = float2((float)x, (float)y) - p1;
         float d20 = MathHelper::Dot(v2, v0);
         float d21 = MathHelper::Dot(v2, v1);

         float v = (d11 * d20 - d01 * d21) / denom;
         float w = (d00 * d21 - d01 * d20) / denom;
         float u = 1.0f - v - w;

         // Find the z value and check the bounds
         if (u >= 0 && v >= 0 && w >= 0 && x >= 0 && y >= 0 && x < (int)screen->GetScreenWidth() && y < (int)screen->GetScreenHeight())
         {
            // Interpolate z value
            float z = triangle.p1.z * u + triangle.p2.z * v + triangle.p3.z * w;

            int i = (int)Utility::Index2DTo1D(x, y, screen->GetScreenWidth());
            // Depth test
            if (z < screen->GetZ(i))
            {
               Color color = color1 * u + color2 * v + color3 * w;
               screen->SetPixelZ(i, z, color);
            }
         }
      }
   }
}

void Rasterizer::DrawBaryTriangle(Screen* screen, Triangle3 triangle, Texture* tex, float2 texCoord1, float2 texCoord2, float2 texCoord3)
{
	// Find the extrema
	float maxX = triangle.p1.x;
	float minX = triangle.p1.x;
	float maxY = triangle.p1.y;
	float minY = triangle.p1.y;

	// MaxX
	if (triangle.p2.x > maxX)
		maxX = triangle.p2.x;
	if (triangle.p3.x > maxX)
		maxX = triangle.p3.x;

	// MinX
	if (triangle.p2.x < minX)
		minX = triangle.p2.x;
	if (triangle.p3.x < minX)
		minX = triangle.p3.x;

	// MaxY
	if (triangle.p2.y > maxY)
		maxY = triangle.p2.y;
	if (triangle.p3.y > maxY)
		maxY = triangle.p3.y;

	// MinY
	if (triangle.p2.y < minY)
		minY = triangle.p2.y;
	if (triangle.p3.y < minY)
		minY = triangle.p3.y;

	float2 p1 = MathHelper::ToFloat2(triangle.p1);
	float2 v0 = MathHelper::ToFloat2(triangle.p2) - p1;
	float2 v1 = MathHelper::ToFloat2(triangle.p3) - p1;

	float d00 = MathHelper::Dot(v0, v0);
	float d01 = MathHelper::Dot(v0, v1);
	float d11 = MathHelper::Dot(v1, v1);
	float denom = d00 * d11 - d01 * d01;

	for (int x = (int)minX; x <= (int)maxX; x++)
	{
		for (int y = (int)minY; y <= (int)maxY; y++)
		{
			float2 v2 = float2((float)x, (float)y) - p1;
			float d20 = MathHelper::Dot(v2, v0);
			float d21 = MathHelper::Dot(v2, v1);

			float v = (d11 * d20 - d01 * d21) / denom;
			float w = (d00 * d21 - d01 * d20) / denom;
			float u = 1.0f - v - w;

			// Find the z value and check the bounds
			if (u >= 0 && v >= 0 && w >= 0 && x >= 0 && y >= 0 && x < (int)screen->GetScreenWidth() && y < (int)screen->GetScreenHeight())
			{
				// Interpolate z value
				float zInv = triangle.p1.z * u + triangle.p2.z * v + triangle.p3.z * w;
				float z = 1 / zInv;

				int i = (int)Utility::Index2DTo1D(x, y, screen->GetScreenWidth());
				// Depth test
				if (z < screen->GetZ(i))
				{
               float2 texCoord = (texCoord1 * u + texCoord2 * v + texCoord3 * w) / zInv;
               texCoord.x = fmod(texCoord.x + (float)tex->width * 8.0f, (float)tex->width);
               texCoord.y = fmod(texCoord.y + (float)tex->height * 8.0f, (float)tex->height);
					screen->SetPixelZ(i, z, tex->GetPixel((int)texCoord.x, (int)texCoord.y));
				}
			}
		}
	}
}

void Rasterizer::DrawBaryTriangle(Screen* screen, Triangle3 triangle, Texture* tex, float2 texCoord1, float2 texCoord2, float2 texCoord3, float diffuse)
{
	// Find the extrema
	float maxX = triangle.p1.x;
	float minX = triangle.p1.x;
	float maxY = triangle.p1.y;
	float minY = triangle.p1.y;

	// MaxX
	if (triangle.p2.x > maxX)
		maxX = triangle.p2.x;
	if (triangle.p3.x > maxX)
		maxX = triangle.p3.x;

	// MinX
	if (triangle.p2.x < minX)
		minX = triangle.p2.x;
	if (triangle.p3.x < minX)
		minX = triangle.p3.x;

	// MaxY
	if (triangle.p2.y > maxY)
		maxY = triangle.p2.y;
	if (triangle.p3.y > maxY)
		maxY = triangle.p3.y;

	// MinY
	if (triangle.p2.y < minY)
		minY = triangle.p2.y;
	if (triangle.p3.y < minY)
		minY = triangle.p3.y;

	float2 p1 = MathHelper::ToFloat2(triangle.p1);
	float2 v0 = MathHelper::ToFloat2(triangle.p2) - p1;
	float2 v1 = MathHelper::ToFloat2(triangle.p3) - p1;

	float d00 = MathHelper::Dot(v0, v0);
	float d01 = MathHelper::Dot(v0, v1);
	float d11 = MathHelper::Dot(v1, v1);
	float denom = d00 * d11 - d01 * d01;

	for (int x = (int)minX; x <= (int)maxX; x++)
	{
		for (int y = (int)minY; y <= (int)maxY; y++)
		{
			float2 v2 = float2((float)x, (float)y) - p1;
			float d20 = MathHelper::Dot(v2, v0);
			float d21 = MathHelper::Dot(v2, v1);

			float v = (d11 * d20 - d01 * d21) / denom;
			float w = (d00 * d21 - d01 * d20) / denom;
			float u = 1.0f - v - w;

			// Find the z value and check the bounds
			if (u >= 0 && v >= 0 && w >= 0 && x >= 0 && y >= 0 && x < (int)screen->GetScreenWidth() && y < (int)screen->GetScreenHeight())
			{
				// Interpolate z value
				float zInv = triangle.p1.z * u + triangle.p2.z * v + triangle.p3.z * w;
				float z = 1 / zInv;
				int i = (int)Utility::Index2DTo1D(x, y, screen->GetScreenWidth());
				// Depth test
				if (z < screen->GetZ(i))
				{
               float2 texCoord = (texCoord1 * u + texCoord2 * v + texCoord3 * w) / zInv;
               texCoord.x = fmod(texCoord.x + (float)tex->width * 8.0f, (float)tex->width);
               texCoord.y = fmod(texCoord.y + (float)tex->height * 8.0f, (float)tex->height);
					screen->SetPixelZ(i, z, tex->GetPixel((int)texCoord.x, (int)texCoord.y) * diffuse);
				}
			}
		}
	}
}

void Rasterizer::DrawBaryTriangle(Screen* screen, Triangle3 triangle, Texture* tex, float2 texCoord1, float2 texCoord2, float2 texCoord3, float diffuse1, float diffuse2, float diffuse3)
{
	// Find the extrema
	float maxX = triangle.p1.x;
	float minX = triangle.p1.x;
	float maxY = triangle.p1.y;
	float minY = triangle.p1.y;

	// MaxX
	if (triangle.p2.x > maxX)
		maxX = triangle.p2.x;
	if (triangle.p3.x > maxX)
		maxX = triangle.p3.x;

	// MinX
	if (triangle.p2.x < minX)
		minX = triangle.p2.x;
	if (triangle.p3.x < minX)
		minX = triangle.p3.x;

	// MaxY
	if (triangle.p2.y > maxY)
		maxY = triangle.p2.y;
	if (triangle.p3.y > maxY)
		maxY = triangle.p3.y;

	// MinY
	if (triangle.p2.y < minY)
		minY = triangle.p2.y;
	if (triangle.p3.y < minY)
		minY = triangle.p3.y;

	float2 p1 = MathHelper::ToFloat2(triangle.p1);
	float2 v0 = MathHelper::ToFloat2(triangle.p2) - p1;
	float2 v1 = MathHelper::ToFloat2(triangle.p3) - p1;
	
	float d00 = MathHelper::Dot(v0, v0);
	float d01 = MathHelper::Dot(v0, v1);
	float d11 = MathHelper::Dot(v1, v1);
	float denom = d00 * d11 - d01 * d01;

	for (int x = (int)minX; x <= (int)maxX; x++)
	{
		for (int y = (int)minY; y <= (int)maxY; y++)
		{
			float2 v2 = float2((float)x, (float)y) - p1;
			float d20 = MathHelper::Dot(v2, v0);
			float d21 = MathHelper::Dot(v2, v1);

			float v = (d11 * d20 - d01 * d21) / denom;
			float w = (d00 * d21 - d01 * d20) / denom;
			float u = 1.0f - v - w;

			// Find the z value and check the bounds
			if (u >= 0 && v >= 0 && w >= 0 && x >= 0 && y >= 0 && x < (int)screen->GetScreenWidth() && y < (int)screen->GetScreenHeight())
			{
				// Interpolate z value
				float zInv = triangle.p1.z * u + triangle.p2.z * v + triangle.p3.z * w;
				float z = 1 / zInv;
				int i = (int)Utility::Index2DTo1D(x, y, screen->GetScreenWidth());
				// Depth test
				if (z < screen->GetZ(i))
				{
					float2 texCoord = (texCoord1 * u + texCoord2 * v + texCoord3 * w) / zInv;
               texCoord.x = fmod(texCoord.x + (float)tex->width * 8.0f, (float)tex->width);
               texCoord.y = fmod(texCoord.y + (float)tex->height * 8.0f, (float)tex->height);
					Color color = tex->GetPixel((int)texCoord.x, (int)texCoord.y) * (diffuse1 * u + diffuse2 * v + diffuse3 * w);
					screen->SetPixelZ(i, z, color);
				}
			}
		}
	}
}


// Draw Meshes
void Rasterizer::DrawMeshPoints(Screen* screen, Mesh* mesh, Camera* cam, UINT pointSize)
{
   matrix4x4 worldViewProj = cam->GetProj() * cam->GetView() * mesh->world;

   // For every vertex of the mesh
   for (UINT i = 0; i < mesh->vertices.size(); i++)
   {
      float4 pt1 = worldViewProj * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i]]);
      if (pt1.w > cam->GetNearZ() && pt1.w < cam->GetFarZ())
         screen->SetSquare((int)((pt1.x / pt1.w + 0.5f) * screen->GetScreenWidth()), (int)((pt1.y / pt1.w + 0.5f) * screen->GetScreenHeight()), mesh->mat->diffuse, pointSize);
   }
}

void Rasterizer::DrawMeshWireFrame(Screen* screen, Mesh* mesh, Camera* cam)
{
   matrix4x4 worldViewProj = cam->GetProj() * cam->GetView() * mesh->world;

	for (UINT i = 0; i < mesh->indices.size(); i += 3)
	{
		Triangle2 triangle;
		float4 pt1 = worldViewProj * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i]]);
		triangle.p1 = float2((pt1.x / pt1.w + 0.5f) * screen->GetScreenWidth(), (pt1.y / pt1.w + 0.5f) * screen->GetScreenHeight());

		float4 pt2 = worldViewProj * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i + 1]]);
		triangle.p2 = float2((pt2.x / pt2.w + 0.5f) * screen->GetScreenWidth(), (pt2.y / pt2.w + 0.5f) * screen->GetScreenHeight());

		float4 pt3 = worldViewProj * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i + 2]]);
		triangle.p3 = float2((pt3.x / pt3.w + 0.5f) * screen->GetScreenWidth(), (pt3.y / pt3.w + 0.5f) * screen->GetScreenHeight());

      // Per triangle clipping
      if(pt1.w > cam->GetNearZ() && pt2.w > cam->GetNearZ() && pt3.w > cam->GetNearZ() &&
         pt1.w < cam->GetFarZ() && pt2.w < cam->GetFarZ() && pt3.w < cam->GetFarZ())
		   DrawWireTriangle(screen, triangle, mesh->mat->diffuse);
	}
}


void Rasterizer::DrawScanlineSurfaceMesh(Screen* screen, Mesh* mesh, matrix4x4 view, matrix4x4 proj)
{
	matrix4x4 worldViewProj = proj * view * mesh->world;

	for (UINT i = 0; i < mesh->indices.size(); i += 3)
	{
		Triangle3 triangle;

		float4 pt1 = worldViewProj * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i]]);
		triangle.p1 = float3((pt1.x / pt1.w + 0.5f) * screen->GetScreenWidth(), (pt1.y / pt1.w + 0.5f) * screen->GetScreenHeight(), pt1.w);

		float4 pt2 = worldViewProj * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i + 1]]);
		triangle.p2 = float3((pt2.x / pt2.w + 0.5f) * screen->GetScreenWidth(), (pt2.y / pt2.w + 0.5f) * screen->GetScreenHeight(), pt1.w);

		float4 pt3 = worldViewProj * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i + 2]]);
		triangle.p3 = float3((pt3.x / pt3.w + 0.5f) * screen->GetScreenWidth(), (pt3.y / pt3.w + 0.5f) * screen->GetScreenHeight(), pt1.w);

		DrawScanlineTriangle(screen, triangle, mesh->mat->diffuse + mesh->mat->ambient);
	}
}

void Rasterizer::DrawScanlineFlatMesh(Screen* screen, Mesh* mesh, matrix4x4 view, matrix4x4 proj, float3 dirLight)
{
   dirLight = dirLight.Normalized();
   matrix4x4 worldView = view * mesh->world;
   matrix4x4 worldViewProj = proj * view * mesh->world;

   for (UINT i = 0; i < mesh->indices.size(); i += 3)
   {
      Triangle3 triangle;

      float4 pt1 = worldView * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i]]);
      float4 pt4 = worldViewProj * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i]]);
      triangle.p1 = float3((pt4.x / pt4.w + 0.5f) * screen->GetScreenWidth(), (pt4.y / pt4.w + 0.5f) * screen->GetScreenHeight(), pt4.w);

      float4 pt2 = worldView * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i + 1]]);
      pt4 = worldViewProj * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i + 1]]);
      triangle.p2 = float3((pt4.x / pt4.w + 0.5f) * screen->GetScreenWidth(), (pt4.y / pt4.w + 0.5f) * screen->GetScreenHeight(), pt4.w);

      float4 pt3 = worldView * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i + 2]]);
      pt4 = worldViewProj * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i + 2]]);
      triangle.p3 = float3((pt4.x / pt4.w + 0.5f) * screen->GetScreenWidth(), (pt4.y / pt4.w + 0.5f) * screen->GetScreenHeight(), pt4.w);

      // Get the shade
      float3 n = MathHelper::Cross(MathHelper::ToFloat3(pt2) - MathHelper::ToFloat3(pt1), MathHelper::ToFloat3(pt3) - MathHelper::ToFloat3(pt1)).Normalized();
      float k = Utility::Saturate(MathHelper::Dot(n, dirLight));

      DrawScanlineTriangle(screen, triangle, mesh->mat->diffuse * k + mesh->mat->ambient);
   }
}


// Non Textured
void Rasterizer::DrawBarySurfaceMesh(Screen* screen, Mesh* mesh, Camera* cam)
{
	matrix4x4 worldViewProj = cam->GetProj() * cam->GetView() * mesh->world;

	for (UINT i = 0; i < mesh->indices.size(); i += 3)
	{
		Triangle3 triangle;

		float4 pt1 = worldViewProj * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i]]);
		triangle.p1 = float3((pt1.x / pt1.w + 0.5f) * screen->GetScreenWidth(), (pt1.y / pt1.w + 0.5f) * screen->GetScreenHeight(), pt1.w);

		float4 pt2 = worldViewProj * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i + 1]]);
		triangle.p2 = float3((pt2.x / pt2.w + 0.5f) * screen->GetScreenWidth(), (pt2.y / pt2.w + 0.5f) * screen->GetScreenHeight(), pt2.w);

		float4 pt3 = worldViewProj * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i + 2]]);
		triangle.p3 = float3((pt3.x / pt3.w + 0.5f) * screen->GetScreenWidth(), (pt3.y / pt3.w + 0.5f) * screen->GetScreenHeight(), pt3.w);

      // Clip per triangle
      if (pt1.w > cam->GetNearZ() && pt2.w > cam->GetNearZ() && pt3.w > cam->GetNearZ() &&
         pt1.w < cam->GetFarZ() && pt2.w < cam->GetFarZ() && pt3.w < cam->GetFarZ())
		   DrawBaryTriangle(screen, triangle, mesh->mat->diffuse + mesh->mat->ambient);
	}
}

void Rasterizer::DrawBarySmoothMesh(Screen* screen, Mesh* mesh, Camera* cam, float3 dirLight)
{
	dirLight = dirLight.Normalized();
	matrix4x4 worldViewProj = cam->GetProj() * cam->GetView() * mesh->world;

	for (UINT i = 0; i < mesh->indices.size(); i += 3)
	{
		Triangle3 triangle;

		float4 pt1 = worldViewProj * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i]]);
		triangle.p1 = float3((pt1.x / pt1.w + 0.5f) * screen->GetScreenWidth(), (pt1.y / pt1.w + 0.5f) * screen->GetScreenHeight(), pt1.w);

		float4 pt2 = worldViewProj * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i + 1]]);
		triangle.p2 = float3((pt2.x / pt2.w + 0.5f) * screen->GetScreenWidth(), (pt2.y / pt2.w + 0.5f) * screen->GetScreenHeight(), pt2.w);

		float4 pt3 = worldViewProj * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i + 2]]);
		triangle.p3 = float3((pt3.x / pt3.w + 0.5f) * screen->GetScreenWidth(), (pt3.y / pt3.w + 0.5f) * screen->GetScreenHeight(), pt3.w);

		float k1 = Utility::Saturate(MathHelper::Dot(MathHelper::ToFloat3(mesh->world * MathHelper::ToFloat4w0(mesh->vertexNormals[mesh->indices[i]])).Normalized(), dirLight));
		float k2 = Utility::Saturate(MathHelper::Dot(MathHelper::ToFloat3(mesh->world * MathHelper::ToFloat4w0(mesh->vertexNormals[mesh->indices[i + 1]])).Normalized(), dirLight));
		float k3 = Utility::Saturate(MathHelper::Dot(MathHelper::ToFloat3(mesh->world * MathHelper::ToFloat4w0(mesh->vertexNormals[mesh->indices[i + 2]])).Normalized(), dirLight));

      // Clip per triangle
      if (pt1.w > cam->GetNearZ() && pt2.w > cam->GetNearZ() && pt3.w > cam->GetNearZ() &&
         pt1.w < cam->GetFarZ() && pt2.w < cam->GetFarZ() && pt3.w < cam->GetFarZ())
		   DrawBaryTriangle(screen, triangle, mesh->mat->diffuse + mesh->mat->ambient, k1, k2, k3);
	}
}

void Rasterizer::DrawBaryFlatMesh(Screen* screen, Mesh* mesh, Camera* cam, float3 dirLight)
{
	dirLight = dirLight.Normalized();
	matrix4x4 worldViewProj = cam->GetProj() * cam->GetView() * mesh->world;

	for (UINT i = 0; i < mesh->indices.size(); i += 3)
	{
		Triangle3 triangle;

		float4 pt1 = worldViewProj * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i]]);
		triangle.p1 = float3((pt1.x / pt1.w + 0.5f) * screen->GetScreenWidth(), (pt1.y / pt1.w + 0.5f) * screen->GetScreenHeight(), pt1.w);

		float4 pt2 = worldViewProj * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i + 1]]);
		triangle.p2 = float3((pt2.x / pt2.w + 0.5f) * screen->GetScreenWidth(), (pt2.y / pt2.w + 0.5f) * screen->GetScreenHeight(), pt2.w);

		float4 pt3 = worldViewProj * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i + 2]]);
		triangle.p3 = float3((pt3.x / pt3.w + 0.5f) * screen->GetScreenWidth(), (pt3.y / pt3.w + 0.5f) * screen->GetScreenHeight(), pt3.w);

      if (pt1.w > cam->GetNearZ() && pt2.w > cam->GetNearZ() && pt3.w > cam->GetNearZ() &&
         pt1.w < cam->GetFarZ() && pt2.w < cam->GetFarZ() && pt3.w < cam->GetFarZ())
      {
         // Get the shade
         float3 n = MathHelper::Cross(mesh->vertices[mesh->indices[i + 1]] - mesh->vertices[mesh->indices[i]], mesh->vertices[mesh->indices[i + 2]] - mesh->vertices[mesh->indices[i]]).Normalized();
         float3 nTransformed = MathHelper::ToFloat3(cam->GetView() * mesh->world * MathHelper::ToFloat4w0(n)).Normalized();
         float3 dirLightTransformed = MathHelper::ToFloat3(cam->GetView() * MathHelper::ToFloat4w0(dirLight)).Normalized();
         float k = Utility::Saturate(MathHelper::Dot(nTransformed, dirLightTransformed));

         // Clip per triangle
         if (pt1.w > cam->GetNearZ() && pt2.w > cam->GetNearZ() && pt3.w > cam->GetNearZ() &&
            pt1.w < cam->GetFarZ() && pt2.w < cam->GetFarZ() && pt3.w < cam->GetFarZ())
            DrawBaryTriangle(screen, triangle, (mesh->mat->diffuse + mesh->mat->ambient) * k);
      }
	}
}

void Rasterizer::DrawBaryPhongFlatMesh(Screen* screen, Mesh* mesh, Camera* cam, float3 dirLight)
{
   dirLight = dirLight.Normalized();
   matrix4x4 worldView = cam->GetView() * mesh->world;
   matrix4x4 worldViewProj = cam->GetProj() * worldView;

   for (UINT i = 0; i < mesh->indices.size(); i += 3)
   {
      Triangle3 triangle;

      float4 pt1 = worldView * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i]]);
      float4 pt4 = worldViewProj * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i]]);
      triangle.p1 = float3((pt4.x / pt4.w + 0.5f) * screen->GetScreenWidth(), (pt4.y / pt4.w + 0.5f) * screen->GetScreenHeight(), pt4.w);

      float4 pt2 = worldView * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i + 1]]);
      pt4 = worldViewProj * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i + 1]]);
      triangle.p2 = float3((pt4.x / pt4.w + 0.5f) * screen->GetScreenWidth(), (pt4.y / pt4.w + 0.5f) * screen->GetScreenHeight(), pt4.w);

      float4 pt3 = worldView * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i + 2]]);
      pt4 = worldViewProj * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i + 2]]);
      triangle.p3 = float3((pt4.x / pt4.w + 0.5f) * screen->GetScreenWidth(), (pt4.y / pt4.w + 0.5f) * screen->GetScreenHeight(), pt4.w);

      // Get the shade
      float3 n = MathHelper::Cross(MathHelper::ToFloat3(pt2) - MathHelper::ToFloat3(pt1), MathHelper::ToFloat3(pt3) - MathHelper::ToFloat3(pt1)).Normalized();
      float diff = MathHelper::Dot(n, dirLight);

      // Reflect the light across the normal
      float3 r = dirLight - n * 2 * diff;

      // Get the specular shade
      float spec = pow(MathHelper::Dot(r, cam->GetViewDir()), mesh->mat->specularShine);

      if (pt1.w > cam->GetNearZ() && pt2.w > cam->GetNearZ() && pt3.w > cam->GetNearZ() &&
         pt1.w < cam->GetFarZ() && pt2.w < cam->GetFarZ() && pt3.w < cam->GetFarZ())
         DrawBaryTriangle(screen, triangle, mesh->mat->diffuse * Utility::Saturate(diff) + mesh->mat->specular * Utility::Saturate(diff) * Utility::Saturate(spec) + mesh->mat->ambient);
   }
}

void Rasterizer::DrawBaryPhongSmoothMesh(Screen* screen, Mesh* mesh, Camera* cam, float3 dirLight)
{
   dirLight = dirLight.Normalized();
   matrix4x4 worldViewProj = cam->GetProj() * cam->GetView() * mesh->world;

   for (UINT i = 0; i < mesh->indices.size(); i += 3)
   {
      Triangle3 triangle;

      float4 pt1 = worldViewProj * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i]]);
      triangle.p1 = float3((pt1.x / pt1.w + 0.5f) * screen->GetScreenWidth(), (pt1.y / pt1.w + 0.5f) * screen->GetScreenHeight(), pt1.w);

      float4 pt2 = worldViewProj * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i + 1]]);
      triangle.p2 = float3((pt2.x / pt2.w + 0.5f) * screen->GetScreenWidth(), (pt2.y / pt2.w + 0.5f) * screen->GetScreenHeight(), pt2.w);

      float4 pt3 = worldViewProj * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i + 2]]);
      triangle.p3 = float3((pt3.x / pt3.w + 0.5f) * screen->GetScreenWidth(), (pt3.y / pt3.w + 0.5f) * screen->GetScreenHeight(), pt3.w);

      float3 n1 = MathHelper::ToFloat3(mesh->world * MathHelper::ToFloat4w0(mesh->vertexNormals[mesh->indices[i]])).Normalized();
      float3 n2 = MathHelper::ToFloat3(mesh->world * MathHelper::ToFloat4w0(mesh->vertexNormals[mesh->indices[i + 1]])).Normalized();
      float3 n3 = MathHelper::ToFloat3(mesh->world * MathHelper::ToFloat4w0(mesh->vertexNormals[mesh->indices[i + 2]])).Normalized();

      // diffuse cosines
      float diff1 = MathHelper::Dot(n1, dirLight);
      float diff2 = MathHelper::Dot(n2, dirLight);
      float diff3 = MathHelper::Dot(n3, dirLight);

      // Get the specular shade
      // Reflectance vectors
      float3 r1 = dirLight - n1 * 2 * diff1;
      float spec1 = pow(MathHelper::Dot(r1, cam->GetViewDir()), mesh->mat->specularShine);

      // Reflectance vectors
      float3 r2 = dirLight - n2 * 2 * diff2;
      float spec2 = pow(MathHelper::Dot(r2, cam->GetViewDir()), mesh->mat->specularShine);

      // Reflectance vectors
      float3 r3 = dirLight - n3 * 2 * diff3;
      float spec3 = pow(MathHelper::Dot(r3, cam->GetViewDir()), mesh->mat->specularShine);

      DrawBaryTriangle(screen, triangle, mesh->mat->diffuse * Utility::Saturate(diff1) + mesh->mat->specular * Utility::Saturate(diff1) * Utility::Saturate(spec1) + mesh->mat->ambient,
         mesh->mat->diffuse * Utility::Saturate(diff2) + mesh->mat->specular * Utility::Saturate(diff2) * Utility::Saturate(spec2) + mesh->mat->ambient,
         mesh->mat->diffuse * Utility::Saturate(diff3) + mesh->mat->specular * Utility::Saturate(diff3) * Utility::Saturate(spec3) + mesh->mat->ambient);
   }
}


// Textured
void Rasterizer::DrawBaryTexturedSurfaceMesh(Screen* screen, Mesh* mesh, matrix4x4 view, matrix4x4 proj)
{
	matrix4x4 worldViewProj = proj * view * mesh->world;

	for (UINT i = 0; i < mesh->indices.size(); i += 3)
	{
		Triangle3 triangle;

		float4 pt1 = worldViewProj * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i]]);
		float2 uv1 = mesh->texCoords[mesh->indices[i]] / pt1.w;
		triangle.p1 = float3((pt1.x / pt1.w + 0.5f) * screen->GetScreenWidth(), (pt1.y / pt1.w + 0.5f) * screen->GetScreenHeight(), 1 / pt1.w);

		float4 pt2 = worldViewProj * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i + 1]]);
		float2 uv2 = mesh->texCoords[mesh->indices[i + 1]] / pt2.w;
		triangle.p2 = float3((pt2.x / pt2.w + 0.5f) * screen->GetScreenWidth(), (pt2.y / pt2.w + 0.5f) * screen->GetScreenHeight(), 1 / pt2.w);

		float4 pt3 = worldViewProj * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i + 2]]);
		float2 uv3 = mesh->texCoords[mesh->indices[i + 2]] / pt3.w;
		triangle.p3 = float3((pt3.x / pt3.w + 0.5f) * screen->GetScreenWidth(), (pt3.y / pt3.w + 0.5f) * screen->GetScreenHeight(), 1 / pt3.w);

		DrawBaryTriangle(screen, triangle, mesh->mat->diffuseMap, uv1, uv2, uv3);
	}
}

void Rasterizer::DrawBaryTexturedFlatMesh(Screen* screen, Mesh* mesh, matrix4x4 view, matrix4x4 proj, float3 dirLight)
{
	dirLight = dirLight.Normalized();
	matrix4x4 worldView = view * mesh->world;
	matrix4x4 worldViewProj = proj * view * mesh->world;

	for (UINT i = 0; i < mesh->indices.size(); i += 3)
	{
		Triangle3 triangle;

		float4 pt1 = worldView * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i]]);
		float4 pt4 = worldViewProj * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i]]);
		float2 uv1 = mesh->texCoords[mesh->indices[i]] / pt4.w;
		triangle.p1 = float3((pt4.x / pt4.w + 0.5f) * screen->GetScreenWidth(), (pt4.y / pt4.w + 0.5f) * screen->GetScreenHeight(), 1 / pt4.w);

		float4 pt2 = worldView * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i + 1]]);
		pt4 = worldViewProj * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i + 1]]);
		float2 uv2 = mesh->texCoords[mesh->indices[i + 1]] / pt4.w;
		triangle.p2 = float3((pt4.x / pt4.w + 0.5f) * screen->GetScreenWidth(), (pt4.y / pt4.w + 0.5f) * screen->GetScreenHeight(), 1 / pt4.w);

		float4 pt3 = worldView * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i + 2]]);
		pt4 = worldViewProj * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i + 2]]);
		float2 uv3 = mesh->texCoords[mesh->indices[i + 2]] / pt4.w;
		triangle.p3 = float3((pt4.x / pt4.w + 0.5f) * screen->GetScreenWidth(), (pt4.y / pt4.w + 0.5f) * screen->GetScreenHeight(), 1 / pt4.w);

		// Get the shade
		float3 n = MathHelper::Cross(MathHelper::ToFloat3(pt2) - MathHelper::ToFloat3(pt1), MathHelper::ToFloat3(pt3) - MathHelper::ToFloat3(pt1)).Normalized();
		float k = Utility::Saturate(MathHelper::Dot(n, dirLight));

		DrawBaryTriangle(screen, triangle, mesh->mat->diffuseMap, uv1, uv2, uv3, k);
	}
}

void Rasterizer::DrawBaryTexturedSmoothMesh(Screen* screen, Mesh* mesh, matrix4x4 view, matrix4x4 proj, float3 dirLight)
{
	dirLight = dirLight.Normalized();
	matrix4x4 worldViewProj = proj * view * mesh->world;

	for (UINT i = 0; i < mesh->indices.size(); i += 3)
	{
		Triangle3 triangle;

		float4 pt1 = worldViewProj * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i]]);
		float2 uv1 = mesh->texCoords[mesh->indices[i]] / pt1.w;
		triangle.p1 = float3((pt1.x / pt1.w + 0.5f) * screen->GetScreenWidth(), (pt1.y / pt1.w + 0.5f) * screen->GetScreenHeight(), 1 / pt1.w);

		float4 pt2 = worldViewProj * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i + 1]]);
		float2 uv2 = mesh->texCoords[mesh->indices[i + 1]] / pt2.w;
		triangle.p2 = float3((pt2.x / pt2.w + 0.5f) * screen->GetScreenWidth(), (pt2.y / pt2.w + 0.5f) * screen->GetScreenHeight(), 1 / pt2.w);

		float4 pt3 = worldViewProj * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i + 2]]);
		float2 uv3 = mesh->texCoords[mesh->indices[i + 2]] / pt3.w;
		triangle.p3 = float3((pt3.x / pt3.w + 0.5f) * screen->GetScreenWidth(), (pt3.y / pt3.w + 0.5f) * screen->GetScreenHeight(), 1 / pt3.w);

		// Get the shade
		float k1 = Utility::Saturate(MathHelper::Dot(MathHelper::ToFloat3(mesh->world * MathHelper::ToFloat4w0(mesh->vertexNormals[mesh->indices[i]])).Normalized(), dirLight));
		float k2 = Utility::Saturate(MathHelper::Dot(MathHelper::ToFloat3(mesh->world * MathHelper::ToFloat4w0(mesh->vertexNormals[mesh->indices[i + 1]])).Normalized(), dirLight));
		float k3 = Utility::Saturate(MathHelper::Dot(MathHelper::ToFloat3(mesh->world * MathHelper::ToFloat4w0(mesh->vertexNormals[mesh->indices[i + 2]])).Normalized(), dirLight));

		DrawBaryTriangle(screen, triangle, mesh->mat->diffuseMap, uv1, uv2, uv3, k1, k2, k3);
	}
}

void Rasterizer::DrawBaryPhongTexturedSmoothMesh(Screen* screen, Mesh* mesh, Camera* cam, float3 dirLight)
{

}

void Rasterizer::DrawBaryPhongFlatTexturedMesh(Screen* screen, Mesh* mesh, Camera* cam, float3 dirLight)
{
   dirLight = dirLight.Normalized();
   matrix4x4 worldView = cam->GetView() * mesh->world;
   matrix4x4 worldViewProj = cam->GetProj() * worldView;

   for (UINT i = 0; i < mesh->indices.size(); i += 3)
   {
      Triangle3 triangle;

      float4 pt1 = worldView * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i]]);
      float4 pt4 = worldViewProj * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i]]);
      float2 uv1 = mesh->texCoords[mesh->indices[i]] / pt4.w;
      triangle.p1 = float3((pt4.x / pt4.w + 0.5f) * screen->GetScreenWidth(), (pt4.y / pt4.w + 0.5f) * screen->GetScreenHeight(), 1 / pt4.w);

      float4 pt2 = worldView * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i + 1]]);
      pt4 = worldViewProj * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i + 1]]);
      float2 uv2 = mesh->texCoords[mesh->indices[i + 1]] / pt4.w;
      triangle.p2 = float3((pt4.x / pt4.w + 0.5f) * screen->GetScreenWidth(), (pt4.y / pt4.w + 0.5f) * screen->GetScreenHeight(), 1 / pt4.w);

      float4 pt3 = worldView * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i + 2]]);
      pt4 = worldViewProj * MathHelper::ToFloat4(mesh->vertices[mesh->indices[i + 2]]);
      float2 uv3 = mesh->texCoords[mesh->indices[i + 2]] / pt4.w;
      triangle.p3 = float3((pt4.x / pt4.w + 0.5f) * screen->GetScreenWidth(), (pt4.y / pt4.w + 0.5f) * screen->GetScreenHeight(), 1 / pt4.w);

      // Get the shade
      float3 n = MathHelper::Cross(MathHelper::ToFloat3(pt2) - MathHelper::ToFloat3(pt1), MathHelper::ToFloat3(pt3) - MathHelper::ToFloat3(pt1)).Normalized();
      float k = MathHelper::Dot(n, dirLight);
      float kSat = Utility::Saturate(k);

      float spec = 0;
      if (k > 0)
      {
         // Reflect the light across the normal
         float3 r = dirLight - n * 2 * k;

         // Get the specular shade
         spec = Utility::Saturate(/*pow(*/MathHelper::Dot(r, cam->GetViewDir())/*, mesh->mat->specularShine)*/);
      }

      // Triangle Clipping
      if (pt1.w > cam->GetNearZ() && pt2.w > cam->GetNearZ() && pt3.w > cam->GetNearZ() &&
         pt1.w < cam->GetFarZ() && pt2.w < cam->GetFarZ() && pt3.w < cam->GetFarZ())
         DrawBaryTriangle(screen, triangle, mesh->mat->diffuse * kSat + mesh->mat->ambient + mesh->mat->specular * spec);
   }
}