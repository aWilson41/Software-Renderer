#include "RayTracer.h"

void RayTracer::Trace(Screen* screen, Camera* cam, std::vector<Mesh>* mesh, float3 dirLight)
{
	dirLight = dirLight.Normalized();

	// For every pixel in the screen
	for (UINT y = 0; y < screen->GetScreenHeight(); y++)
	{
		for (UINT x = 0; x < screen->GetScreenWidth(); x++)
		{
			// Compute projection view and ray	
			Ray ray = MathHelper::ComputeEyeRay(float2((float)x, (float)y), screen->GetScreenWidth(), screen->GetScreenHeight(), cam->GetFOV(), cam->GetNearZ());
			
			// For every mesh
			for (UINT j = 0; j < mesh->size(); j++)
			{
				// Create mesh handle and compute wvp
				Mesh* tmpMesh = &mesh[0][j];
				matrix4x4 worldView = cam->GetView() * tmpMesh->world;

				// For all the meshes triangles
				for (UINT k = 0; k < tmpMesh->indices.size(); k += 3)
				{
					// Get the triangle
					Triangle4 triangle;

					triangle.p1 = worldView * MathHelper::ToFloat4(tmpMesh->vertices[tmpMesh->indices[k]]);
					triangle.p2 = worldView * MathHelper::ToFloat4(tmpMesh->vertices[tmpMesh->indices[k + 1]]);
					triangle.p3 = worldView * MathHelper::ToFloat4(tmpMesh->vertices[tmpMesh->indices[k + 2]]);

					float3 p1 = MathHelper::ToFloat3(triangle.p1);
					float3 p2 = MathHelper::ToFloat3(triangle.p2);
					float3 p3 = MathHelper::ToFloat3(triangle.p3);

					// Get the triangle faces normal (could optionally store this in memory)
					float3 n = MathHelper::Cross(p2 - p1, p3 - p1).Normalized();

					// Test ray/triangle intersection check against current pixel depth
					float4 bCoords = MathHelper::TriangleRayIntersection(ray, p1, p2, p3, n);
					if (bCoords.w < screen->GetZ(x, y))
					{
						screen->SetZ(x, y, bCoords.w);

						float k = Utility::Saturate(MathHelper::Dot(dirLight, n));

						// Monte Carlo approx
						//float3 n1 = MathHelper::ToFloat3((tmpMesh->world * MathHelper::ToFloat4(tmpMesh->vertexNormals[tmpMesh->indices[k]]))).Normalized();
						//float3 n2 = MathHelper::ToFloat3((tmpMesh->world * MathHelper::ToFloat4(tmpMesh->vertexNormals[tmpMesh->indices[k + 1]]))).Normalized();
						//float3 n3 = MathHelper::ToFloat3((tmpMesh->world * MathHelper::ToFloat4(tmpMesh->vertexNormals[tmpMesh->indices[k + 2]]))).Normalized();
						//
						//float3 k = float3(Utility::Saturate(MathHelper::Dot(dirLight, n1)), 
						//	Utility::Saturate(MathHelper::Dot(dirLight, n2)),
						//	Utility::Saturate(MathHelper::Dot(dirLight, n3)));

						//Color col1 = tmpMesh->mat.diffuse * k.x;// Color(tmpMesh->mat.diffuse.r * kVec.x, tmpMesh->mat.diffuse.g * kVec.x, tmpMesh->mat.diffuse.b * kVec.x);
						//Color col2 = tmpMesh->mat.diffuse * k.y;// Color(tmpMesh->mat.diffuse.r * kVec.y, tmpMesh->mat.diffuse.g * kVec.y, tmpMesh->mat.diffuse.b * kVec.y);
						//Color col3 = tmpMesh->mat.diffuse * k.z;// Color(tmpMesh->mat.diffuse.r * kVec.z, tmpMesh->mat.diffuse.g * kVec.z, tmpMesh->mat.diffuse.b * kVec.z);

						//screen->SetPixel(x, y, col1 * bCoords.x + col2 * bCoords.y + col3 * bCoords.z);
						screen->SetPixel(x, y, tmpMesh->mat->diffuse * k);
					}
				}
			}
		}
	}
}