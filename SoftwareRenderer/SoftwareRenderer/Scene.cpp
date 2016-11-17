#include "Scene.h"

// Returns index of first occurence texture or -1 if not found
int Scene::FindTexture(std::string name)
{
	for (UINT i = 0; i < texture.size(); i++)
	{
		if (texture[i].name == name)
			return i;
	}

	return -1;
}

void Scene::DrawSceneToBuffer(SoftwareDevice* sDevice)
{
	switch (sDevice->GetTechnique())
	{
	case RenderTechnique::POINTGRAPH:
		// For every mesh in the scene
		for (UINT i = 0; i < mesh.size(); i++)
		{
			sDevice->rasterizer.DrawMeshPoints(&sDevice->screen, &mesh[i], &cam, 4);
		}
		break;

	case RenderTechnique::WIREFRAME:
		// For every mesh in the scene
		for (UINT i = 0; i < mesh.size(); i++)
		{
			sDevice->rasterizer.DrawMeshWireFrame(&sDevice->screen, &mesh[i], &cam);
		}
		break;

	case RenderTechnique::RASTER:
		// For every mesh in the scene
		for (UINT i = 0; i < mesh.size(); i++)
		{
			if (sDevice->rasterizer.rTech == BARYCENTRIC)
			{
				Shader shader = sDevice->GetShader();
				Sampler sampler = sDevice->GetSampler();
				if (shader == Shader::NOSHADER)
				{
					if(sampler == Sampler::NOSAMPLE)
						sDevice->rasterizer.DrawBarySurfaceMesh(&sDevice->screen, &mesh[i], &cam);
					else if(sampler == Sampler::POINTSAMPLE && mesh[i].mat->hasDiffuseMap)
						sDevice->rasterizer.DrawBaryTexturedSurfaceMesh(&sDevice->screen, &mesh[i], cam.GetView(), cam.GetProj());
				}
				else if (shader == Shader::FLAT)
				{
					if (sampler == Sampler::NOSAMPLE)
						sDevice->rasterizer.DrawBaryFlatMesh(&sDevice->screen, &mesh[i], &cam, MathHelper::ToFloat3(dirLight[0]));
					else if (sampler == Sampler::POINTSAMPLE && mesh[i].mat->hasDiffuseMap)
						sDevice->rasterizer.DrawBaryTexturedFlatMesh(&sDevice->screen, &mesh[i], cam.GetView(), cam.GetProj(), MathHelper::ToFloat3(dirLight[0]));
				}
				else if (shader == Shader::SMOOTH)
				{
					if (sampler == Sampler::NOSAMPLE)
						sDevice->rasterizer.DrawBarySmoothMesh(&sDevice->screen, &mesh[i], &cam, MathHelper::ToFloat3(dirLight[0]));
					else if (sampler == Sampler::POINTSAMPLE && mesh[i].mat->hasDiffuseMap)
						sDevice->rasterizer.DrawBaryTexturedSmoothMesh(&sDevice->screen, &mesh[i], cam.GetView(), cam.GetProj(), MathHelper::ToFloat3(dirLight[0]));
				}
            else if (shader == Shader::PHONGFLAT)
               sDevice->rasterizer.DrawBaryPhongFlatMesh(&sDevice->screen, &mesh[i], &cam, MathHelper::ToFloat3(dirLight[0]));
            else if (shader == Shader::PHONGSMOOTH)
               sDevice->rasterizer.DrawBaryPhongSmoothMesh(&sDevice->screen, &mesh[i], &cam, MathHelper::ToFloat3(dirLight[0]));
			}
         else if (sDevice->rasterizer.rTech == DDA)
         {
            Shader shader = sDevice->GetShader();
            Sampler sampler = sDevice->GetSampler();
            if(shader == Shader::NOSHADER)
               sDevice->rasterizer.DrawScanlineSurfaceMesh(&sDevice->screen, &mesh[i], cam.GetView(), cam.GetProj());
            else if(shader == Shader::FLAT)
               sDevice->rasterizer.DrawScanlineFlatMesh(&sDevice->screen, &mesh[i], cam.GetView(), cam.GetProj(), MathHelper::ToFloat3(dirLight[0]));
         }
		}
		break;

	case RenderTechnique::RAYTRACE:
		sDevice->rayTracer.Trace(&sDevice->screen, &cam, &mesh, MathHelper::ToFloat3(dirLight[0]));
		break;

	default:
		break;
	}
}

Scene::~Scene()
{
	// Cleanup the textures
	for (UINT i = 0; i < texture.size(); i++)
	{
		delete[] texture[i].buffer;
	}
}