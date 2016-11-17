#pragma once
#include "MathHelper.h"

class Camera
{
private:
	// Camera View
	matrix4x4 view;
	// Camera Projection
	matrix4x4 proj;

   // Direction
   float3 dir;
   
	float fov;
	float width;
	float height;
	float aspectRatio;
	float nearZ;
	float farZ;

public:
	Camera();

	// Sets the Camera's Projection
	void SetPerspective(float fov, float aspectRatio, float nearZ, float farZ);
   void SetCameraView(matrix4x4 dM); // Delta transform

	float GetFOV() { return fov; }
	float GetAspectRatio() { return aspectRatio; }
	float GetNearZ() { return nearZ; }
	float GetFarZ() { return farZ; }

	matrix4x4 GetProj() { return proj; }
	matrix4x4 GetView() { return view; }

   float3 GetViewDir() { return dir; }
};