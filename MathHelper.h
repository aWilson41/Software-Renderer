#pragma once
#include <math.h>
#include <vector>
#include <limits>

typedef unsigned int UINT;

class float2
{
public:
   float x = 0.0f;
   float y = 0.0f;

   float2()
   {

   }

   float2(float x, float y)
   {
      float2::x = x;
      float2::y = y;
   }

   float2 operator*(const float& a)
   {
	   return float2(x * a, y * a);
   }

   float2 operator/(const float& a)
   {
	   return float2(x / a, y / a);
   }

   float2 operator*(const float2& a)
   {
	   return float2(x * a.x, y * a.y);
   }

   float2 operator+(const float2& a)
   {
      return float2(x + a.x, y + a.y);
   }

   float2 operator-(const float2& a)
   {
      return float2(x - a.x, y - a.y);
   }

   float Magnitude()
   {
	   return sqrt(x * x + y * y);
   }
};

class float3
{
public:
   float x;
   float y;
   float z;

   float3()
   {

   }

   float3(float x, float y, float z)
   {
      float3::x = x;
      float3::y = y;
      float3::z = z;
   }

   float3 operator+(const float3& a)
   {
      return float3(x + a.x, y + a.y, z + a.z);
   }

   float3 operator-(const float3& a)
   {
      return float3(x - a.x, y - a.y, z - a.z);
   }

   float3 operator*(const float& a)
   {
      return float3(x * a, y * a, z * a);
   }

   float3 operator*(const float3& a)
   {
	   return float3(x * a.x, y * a.y, z * a.z);
   }

   float3 operator/(const float& a)
   {
      return float3(x / a, y / a, z / a);
   }

   float3 Normalized()
   {
	   return float3(x, y, z) / Magnitude();
   }

   float Magnitude()
   {
	   return sqrt(x * x + y * y + z * z);
   }
};

class float4
{
public:
   float x;
   float y;
   float z;
   float w;

   float4()
   {

   }

   float4(float x, float y, float z, float w)
   {
      float4::x = x;
      float4::y = y;
      float4::z = z;
      float4::w = w;
   }

   float4 operator+(const float4& a)
   {
      return float4(x + a.x, y + a.y, z + a.z, w + a.w);
   }

   float4 operator-(const float4& a)
   {
      return float4(x - a.x, y - a.y, z - a.z, w + a.w);
   }

   float4 operator*(const float& a)
   {
      return float4(x * a, y * a, z * a, w * a);
   }

   float4 operator/(const float& a)
   {
      return float4(x / a, y / a, z / a, w / a);
   }

   float4 Normalized()
   {
	   return float4(x, y, z, w) / Magnitude();
   }

   float Magnitude()
   {
	   return sqrt(x * x + y * y + z * z); // w?
   }
};

class matrix4x4
{
public:
   float m00;
   float m01;
   float m02;
   float m03;

   float m10;
   float m11;
   float m12;
   float m13;

   float m20;
   float m21;
   float m22;
   float m23;

   float m30;
   float m31;
   float m32;
   float m33;

   // Should probably move this out of inline
   matrix4x4 operator*(const matrix4x4& a)
   {
      matrix4x4 results;
      results.m00 = m00 * a.m00 + m01 * a.m10 + m02 * a.m20 + m03 * a.m30;
      results.m01 = m00 * a.m01 + m01 * a.m11 + m02 * a.m21 + m03 * a.m31;
      results.m02 = m00 * a.m02 + m01 * a.m12 + m02 * a.m22 + m03 * a.m32;
      results.m03 = m00 * a.m03 + m01 * a.m13 + m02 * a.m23 + m03 * a.m33;

      results.m10 = m10 * a.m00 + m11 * a.m10 + m12 * a.m20 + m13 * a.m30;
      results.m11 = m10 * a.m01 + m11 * a.m11 + m12 * a.m21 + m13 * a.m31;
      results.m12 = m10 * a.m02 + m11 * a.m12 + m12 * a.m22 + m13 * a.m32;
      results.m13 = m10 * a.m03 + m11 * a.m13 + m12 * a.m23 + m13 * a.m33;

      results.m20 = m20 * a.m00 + m21 * a.m10 + m22 * a.m20 + m23 * a.m30;
      results.m21 = m20 * a.m01 + m21 * a.m11 + m22 * a.m21 + m23 * a.m31;
      results.m22 = m20 * a.m02 + m21 * a.m12 + m22 * a.m22 + m23 * a.m32;
      results.m23 = m20 * a.m03 + m21 * a.m13 + m22 * a.m23 + m23 * a.m33;

      results.m30 = m30 * a.m00 + m31 * a.m10 + m32 * a.m20 + m33 * a.m30;
      results.m31 = m30 * a.m01 + m31 * a.m11 + m32 * a.m21 + m33 * a.m31;
      results.m32 = m30 * a.m02 + m31 * a.m12 + m32 * a.m22 + m33 * a.m32;
      results.m33 = m30 * a.m03 + m31 * a.m13 + m32 * a.m23 + m33 * a.m33;
      return results;
   }

   float4 operator*(const float4& a)
   {
      return float4(m00 * a.x + m01 * a.y + m02 * a.z + m03 * a.w, m10 * a.x + m11 * a.y + m12 * a.z + m13 * a.w, m20 * a.x + m21 * a.y + m22 * a.z + m23 * a.w, m30 * a.x + m31 * a.y + m32 * a.z + m33 * a.w);
   }

   matrix4x4 Tranpose()
   {
	   matrix4x4 results;
	   results.m00 = m00;
	   results.m01 = m10;
	   results.m02 = m20;
	   results.m03 = m30;

	   results.m10 = m01;
	   results.m11 = m11;
	   results.m12 = m21;
	   results.m13 = m31;
	   
	   results.m20 = m02;
	   results.m21 = m12;
	   results.m22 = m22;
	   results.m23 = m32;

	   results.m30 = m03;
	   results.m31 = m13;
	   results.m32 = m23;
	   results.m33 = m33;
	   return results;
   }

   float Det()
   {
	   return m03 * m12 * m21 * m30 - m02 * m13 * m21 * m30 - m03 * m11 * m22 * m30 + m01 * m13 * m22 * m30 +
		   m02 * m11 * m23 * m30 - m01 * m12 * m23 * m30 - m03 * m12 * m20 * m31 + m02 * m13 * m20 * m31 +
		   m03 * m10 * m22 * m31 - m00 * m13 * m22 * m31 - m02 * m10 * m23 * m31 + m00 * m12 * m23 * m31 +
		   m03 * m11 * m20 * m32 - m01 * m13 * m20 * m32 - m03 * m10 * m21 * m32 + m00 * m13 * m21 * m32 +
		   m01 * m10 * m23 * m32 - m00 * m11 * m23 * m32 - m02 * m11 * m20 * m33 + m01 * m12 * m20 * m33 +
		   m02 * m10 * m21 * m33 - m00 * m12 * m21 * m33 - m01 * m10 * m22 * m33 + m00 * m11 * m22 * m33;
   }

   // Not optimized for transformation matrices
   matrix4x4 Invert()
   {
	   matrix4x4 results = matrix4x4();
	   results.m00 = results.m12 * results.m23 * results.m31 - results.m13 * results.m22 * results.m31 + results.m13 * results.m21 * results.m32 - results.m11 * results.m23 * results.m32 - results.m12 * results.m21 * results.m33 + results.m11 * results.m22 * results.m33;
	   results.m01 = results.m03 * results.m22 * results.m31 - results.m02 * results.m23 * results.m31 - results.m03 * results.m21 * results.m32 + results.m01 * results.m23 * results.m32 + results.m02 * results.m21 * results.m33 - results.m01 * results.m22 * results.m33;
	   results.m02 = results.m02 * results.m13 * results.m31 - results.m03 * results.m12 * results.m31 + results.m03 * results.m11 * results.m32 - results.m01 * results.m13 * results.m32 - results.m02 * results.m11 * results.m33 + results.m01 * results.m12 * results.m33;
	   results.m03 = results.m03 * results.m12 * results.m21 - results.m02 * results.m13 * results.m21 - results.m03 * results.m11 * results.m22 + results.m01 * results.m13 * results.m22 + results.m02 * results.m11 * results.m23 - results.m01 * results.m12 * results.m23;
	   results.m10 = results.m13 * results.m22 * results.m30 - results.m12 * results.m23 * results.m30 - results.m13 * results.m20 * results.m32 + results.m10 * results.m23 * results.m32 + results.m12 * results.m20 * results.m33 - results.m10 * results.m22 * results.m33;
	   results.m11 = results.m02 * results.m23 * results.m30 - results.m03 * results.m22 * results.m30 + results.m03 * results.m20 * results.m32 - results.m00 * results.m23 * results.m32 - results.m02 * results.m20 * results.m33 + results.m00 * results.m22 * results.m33;
	   results.m12 = results.m03 * results.m12 * results.m30 - results.m02 * results.m13 * results.m30 - results.m03 * results.m10 * results.m32 + results.m00 * results.m13 * results.m32 + results.m02 * results.m10 * results.m33 - results.m00 * results.m12 * results.m33;
	   results.m13 = results.m02 * results.m13 * results.m20 - results.m03 * results.m12 * results.m20 + results.m03 * results.m10 * results.m22 - results.m00 * results.m13 * results.m22 - results.m02 * results.m10 * results.m23 + results.m00 * results.m12 * results.m23;
	   results.m20 = results.m11 * results.m23 * results.m30 - results.m13 * results.m21 * results.m30 + results.m13 * results.m20 * results.m31 - results.m10 * results.m23 * results.m31 - results.m11 * results.m20 * results.m33 + results.m10 * results.m21 * results.m33;
	   results.m21 = results.m03 * results.m21 * results.m30 - results.m01 * results.m23 * results.m30 - results.m03 * results.m20 * results.m31 + results.m00 * results.m23 * results.m31 + results.m01 * results.m20 * results.m33 - results.m00 * results.m21 * results.m33;
	   results.m22 = results.m01 * results.m13 * results.m30 - results.m03 * results.m11 * results.m30 + results.m03 * results.m10 * results.m31 - results.m00 * results.m13 * results.m31 - results.m01 * results.m10 * results.m33 + results.m00 * results.m11 * results.m33;
	   results.m23 = results.m03 * results.m11 * results.m20 - results.m01 * results.m13 * results.m20 - results.m03 * results.m10 * results.m21 + results.m00 * results.m13 * results.m21 + results.m01 * results.m10 * results.m23 - results.m00 * results.m11 * results.m23;
	   results.m30 = results.m12 * results.m21 * results.m30 - results.m11 * results.m22 * results.m30 - results.m12 * results.m20 * results.m31 + results.m10 * results.m22 * results.m31 + results.m11 * results.m20 * results.m32 - results.m10 * results.m21 * results.m32;
	   results.m31 = results.m01 * results.m22 * results.m30 - results.m02 * results.m21 * results.m30 + results.m02 * results.m20 * results.m31 - results.m00 * results.m22 * results.m31 - results.m01 * results.m20 * results.m32 + results.m00 * results.m21 * results.m32;
	   results.m32 = results.m02 * results.m11 * results.m30 - results.m01 * results.m12 * results.m30 - results.m02 * results.m10 * results.m31 + results.m00 * results.m12 * results.m31 + results.m01 * results.m10 * results.m32 - results.m00 * results.m11 * results.m32;
	   results.m33 = results.m01 * results.m12 * results.m20 - results.m02 * results.m11 * results.m20 + results.m02 * results.m10 * results.m21 - results.m00 * results.m12 * results.m21 - results.m01 * results.m10 * results.m22 + results.m00 * results.m11 * results.m22;
	   
	   float invDet = results.Det();
	   results.m00 *= invDet;
	   results.m01 *= invDet;
	   results.m02 *= invDet;
	   results.m03 *= invDet;

	   results.m10 *= invDet;
	   results.m11 *= invDet;
	   results.m12 *= invDet;
	   results.m13 *= invDet;

	   results.m20 *= invDet;
	   results.m21 *= invDet;
	   results.m22 *= invDet;
	   results.m23 *= invDet;

	   results.m30 *= invDet;
	   results.m31 *= invDet;
	   results.m32 *= invDet;
	   results.m33 *= invDet;
	   return results;
   }
};

class Triangle2
{
public:
	float2 p1;
	float2 p2;
	float2 p3;

	Triangle2()
	{
		
	}

	Triangle2(float2 p1, float2 p2, float2 p3)
	{
		Triangle2::p1 = p1;
		Triangle2::p2 = p2;
		Triangle2::p3 = p3;
	}
};

class Triangle3
{
public:
	float3 p1;
	float3 p2;
	float3 p3;

	Triangle3()
	{

	}

	Triangle3(float3 p1, float3 p2, float3 p3)
	{
		Triangle3::p1 = p1;
		Triangle3::p2 = p2;
		Triangle3::p3 = p3;
	}
};

class Triangle4
{
public:
	float4 p1;
	float4 p2;
	float4 p3;

	Triangle4()
	{

	}

	Triangle4(float4 p1, float4 p2, float4 p3)
	{
		Triangle4::p1 = p1;
		Triangle4::p2 = p2;
		Triangle4::p3 = p3;
	}
};

class Ray
{
public:
	float3 start;
	float3 direction;

	Ray(float3 start, float3 direction)
	{
		Ray::start = start;
		Ray::direction = direction;
	}
};

class MathHelper
{
public:
	// Matrices
   static matrix4x4 MatrixIdentity()
   {
      matrix4x4 results;
      results.m00 = 1.0f;
      results.m01 = 0.0f;
      results.m02 = 0.0f;
      results.m03 = 0.0f;

      results.m10 = 0.0f;
      results.m11 = 1.0f;
      results.m12 = 0.0f;
      results.m13 = 0.0f;

      results.m20 = 0.0f;
      results.m21 = 0.0f;
      results.m22 = 1.0f;
      results.m23 = 0.0f;

      results.m30 = 0.0f;
      results.m31 = 0.0f;
      results.m32 = 0.0f;
      results.m33 = 1.0f;
      return results;
   }

   static matrix4x4 MatrixRotateX(float radians)
   {
      matrix4x4 results;
      results.m00 = 1.0f;
      results.m01 = 0.0f;
      results.m02 = 0.0f;
      results.m03 = 0.0f;

      results.m10 = 0.0f;
      results.m11 = cos(radians);
      results.m12 = -sin(radians);
      results.m13 = 0.0f;

      results.m20 = 0.0f;
      results.m21 = sin(radians);
      results.m22 = cos(radians);
      results.m23 = 0.0f;

      results.m30 = 0.0f;
      results.m31 = 0.0f;
      results.m32 = 0.0f;
      results.m33 = 1.0f;
      return results;
   }

   static matrix4x4 MatrixRotateY(float radians)
   {
      matrix4x4 results;
      results.m00 = cos(radians);
      results.m01 = 0.0f;
      results.m02 = sin(radians);
      results.m03 = 0.0f;

      results.m10 = 0.0f;
      results.m11 = 1.0f;
      results.m12 = 0.0f;
      results.m13 = 0.0f;

      results.m20 = -sin(radians);
      results.m21 = 0.0f;
      results.m22 = cos(radians);
      results.m23 = 0.0f;

      results.m30 = 0.0f;
      results.m31 = 0.0f;
      results.m32 = 0.0f;
      results.m33 = 1.0f;
      return results;
   }

   static matrix4x4 MatrixRotateZ(float radians)
   {
      matrix4x4 results;
      results.m00 = cos(radians);
      results.m01 = -sin(radians);
      results.m02 = 0.0f;
      results.m03 = 0.0f;

      results.m10 = sin(radians);
      results.m11 = cos(radians);
      results.m12 = 0.0f;
      results.m13 = 0.0f;

      results.m20 = 0.0f;
      results.m21 = 0.0f;
      results.m22 = 1.0f;
      results.m23 = 0.0f;

      results.m30 = 0.0f;
      results.m31 = 0.0f;
      results.m32 = 0.0f;
      results.m33 = 1.0f;
      return results;
   }

   static matrix4x4 MatrixTranslate(float x, float y, float z)
   {
      matrix4x4 results;
      results.m00 = 1.0f;
      results.m01 = 0.0f;
      results.m02 = 0.0f;
      results.m03 = x;

      results.m10 = 0.0f;
      results.m11 = 1.0f;
      results.m12 = 0.0f;
      results.m13 = y;

      results.m20 = 0.0f;
      results.m21 = 0.0f;
      results.m22 = 1.0f;
      results.m23 = z;

      results.m30 = 0.0f;
      results.m31 = 0.0f;
      results.m32 = 0.0f;
      results.m33 = 1.0f;
      return results;
   }

   static matrix4x4 MatrixScale(float x, float y, float z)
   {
      matrix4x4 results;
      results.m00 = x;
      results.m01 = 0.0f;
      results.m02 = 0.0f;
      results.m03 = 0.0f;

      results.m10 = 0.0f;
      results.m11 = y;
      results.m12 = 0.0f;
      results.m13 = 0.0f;

      results.m20 = 0.0f;
      results.m21 = 0.0f;
      results.m22 = z;
      results.m23 = 0.0f;

      results.m30 = 0.0f;
      results.m31 = 0.0f;
      results.m32 = 0.0f;
      results.m33 = 1.0f;
      return results;
   }

   static matrix4x4 MatrixScale(float s)
   {
	   return MatrixScale(s, s, s);
   }


   // Vector Operations
   static float Cross(float2 a, float2 b)
   {
	   return a.x * b.y - a.y * b.x;
   }

   static float3 Cross(float3 a, float3 b)
   {
	   return float3(a.y * b.z - a.z * b.y,
		   a.z * b.x - a.x * b.z,
		   a.x * b.y - a.y * b.x);
   }

   static float2 ProjAToB(float2 a, float2 b)
   {
	   float m = b.Magnitude();
	   return b * (Dot(a, b) / (m * m));
   }

   static float3 ProjAToB(float3 a, float3 b)
   {
	   float m = b.Magnitude();
	   return b * (Dot(a, b) / (m * m));
   }

   static float Dot(float4 a, float4 b)
   {
	   return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
   }

   static float Dot(float3 a, float3 b)
   {
	   return a.x * b.x + a.y * b.y + a.z * b.z;
   }

   static float Dot(float2 a, float2 b)
   {
	   return a.x * b.x + a.y * b.y;
   }

   // Converts float3 to float4 with w as 1
   static float4 ToFloat4(float3 a)
   {
	   return float4(a.x, a.y, a.z, 1.0f);
   }

   // Converts float3 to float4 with w as 0
   static float4 ToFloat4w0(float3 a)
   {
	   return float4(a.x, a.y, a.z, 0.0f);
   }

   static float3 ToFloat3(float4 a)
   {
	   return float3(a.x, a.y, a.z);
   }

   static float2 ToFloat2(float3 a)
   {
	   return float2(a.x, a.y);
   }


   // Triangles
   static float TriangleArea(float2 p1, float2 p2, float2 p3)
   {
	   return abs(Cross(p2 - p1, p3 - p1)) / 2;
   }

   // Computes bary centric of point p
   static float3 BaryCentric(float2 p, float2 a, float2 b, float2 c)
   {
	   float2 v0 = b - a;
	   float2 v1 = c - a;
	   float2 v2 = p - a;
	   float d00 = MathHelper::Dot(v0, v0);
	   float d01 = MathHelper::Dot(v0, v1);
	   float d11 = MathHelper::Dot(v1, v1);
	   float d20 = MathHelper::Dot(v2, v0);
	   float d21 = MathHelper::Dot(v2, v1);
	   float denom = d00 * d11 - d01 * d01;
	   float v = (d11 * d20 - d01 * d21) / denom;
	   float w = (d00 * d21 - d01 * d20) / denom;
	   float u = 1.0f - v - w;

	   return float3(u, v, w);
   }

   static float3 BaryCentric(float3 p, float3 a, float3 b, float3 c)
   {
	   float3 v0 = b - a;
	   float3 v1 = c - a;
	   float3 v2 = p - a;
	   float d00 = MathHelper::Dot(v0, v0);
	   float d01 = MathHelper::Dot(v0, v1);
	   float d11 = MathHelper::Dot(v1, v1);
	   float d20 = MathHelper::Dot(v2, v0);
	   float d21 = MathHelper::Dot(v2, v1);
	   float denom = d00 * d11 - d01 * d01;
	   float v = (d11 * d20 - d01 * d21) / denom;
	   float w = (d00 * d21 - d01 * d20) / denom;
	   float u = 1.0f - v - w;

	   return float3(u, v, w);
   }


   // Etc
   static Ray ComputeEyeRay(float2 pos, UINT width, UINT height, float fov, float nearZ)
   {
	   float aspectRatio = (float)(width / height);
	   float s = 2.0f * tan(fov * 0.5f);
	   float3 start = float3((pos.x / width - 0.5f) * s /** aspectRatio*/, (pos.y / height - 0.5f) * s, 1.0f) * nearZ;
	   return Ray(start, start.Normalized());
   }

   // Returns float4(bCoords1, bCoords2, bCoords3, zDepth)
   // If failed returns float4(0.0f, 0.0f, 0.0f, maxFloat)
   static float4 TriangleRayIntersection(Ray ray, float3 p1, float3 p2, float3 p3, float3 n)
   {
	   // Use plane/ray intersection to get the point on the plane
	   
	   // Ray plane intersection
	   float denom = MathHelper::Dot(ray.direction, n);
	   if (abs(denom) > 0.0001f)
	   {
			float t = MathHelper::Dot(p1 - ray.start, n) / denom;
			float3 p = ray.start + ray.direction * t;

			float3 bCoords = BaryCentric(p, p1, p2, p3);

			if(bCoords.x >= 0.0f && bCoords.y >= 0.0f && bCoords.z >= 0.0f)
				return float4(bCoords.x, bCoords.y, bCoords.z, p.z);
	   }
	
	   return float4(0.0f, 0.0f, 0.0f, std::numeric_limits<float>::max());
   }
};