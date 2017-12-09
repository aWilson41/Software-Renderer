#include "Camera.h"

// We use column major matrices

Camera::Camera()
{
	// The Projection Identity
	proj.m00 = 1.0f; proj.m01 = 0.0f; proj.m02 = 0.0f; proj.m03 = 0.0f;

	proj.m10 = 0.0f; proj.m11 = 1.0f; proj.m12 = 0.0f; proj.m13 = 0.0f;

	proj.m20 = 0.0f; proj.m21 = 0.0f; proj.m22 = 1.0f; proj.m23 = 0.0f;

	proj.m30 = 0.0f; proj.m31 = 0.0f; proj.m32 = 1.0f; proj.m33 = 0.0f;

	view = MathHelper::MatrixIdentity();
   dir = float3(0.0f, 0.0f, 1.0f);
}

// Sets Perspective Projection
void Camera::SetPerspective(float fov, float aspectRatio, float nearZ, float farZ)
{
   // Set camera specifications
	Camera::fov = fov;
	Camera::nearZ = nearZ;
	Camera::farZ = farZ;
	Camera::aspectRatio = aspectRatio;

	float tanHalfFov = tanf(fov * 0.5f * 3.14f / 180.0f);
	float s = 1.0f / (tanHalfFov * aspectRatio);
	float zRange = farZ - nearZ;

	// Set the projection matrix
	proj.m00 = s;		proj.m01 = 0.0f;			    proj.m02 = 0.0f;						    proj.m03 = 0.0f;

   proj.m10 = 0.0f;	proj.m11 = aspectRatio * s; proj.m12 = 0.0f;						    proj.m13 = 0.0f;

	proj.m20 = 0.0f;	proj.m21 = 0.0f;            proj.m22 = (farZ + nearZ) / zRange; proj.m23 = 2.0f * farZ * nearZ / zRange;

	proj.m30 = 0.0f;	proj.m31 = 0.0f;            proj.m32 = 1.0f;						    proj.m33 = 0.0f;
}

void Camera::SetCameraView(matrix4x4 dM)
{
   view = dM * view;
   dir = MathHelper::ToFloat3(dM * MathHelper::ToFloat4w0(dir));
}