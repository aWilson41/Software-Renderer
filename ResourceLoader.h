#pragma once
#include "Scene.h"
#define NOMINMAX
#include <Windows.h>
#include "LodePNG\lodepng.h"

// Assimp
#include <assimp\scene.h>
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>

class ResourceLoader
{
public:
	static void LoadAllMeshesFromFolder(const std::string path, Scene* scene)
	{
		WIN32_FIND_DATA fd;
		HANDLE hFind = FindFirstFile(path.c_str(), &fd);
		Assimp::Importer importer;
		if (hFind != INVALID_HANDLE_VALUE) 
		{
			do 
			{
				// read all (real) files in current folder
				// , delete '!' read other 2 default folder . and ..
				if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{					
					std::string filePath = path.substr(0, path.size() - 1) + fd.cFileName;
					const aiScene* aiscene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);
					LoadSceneFromAIScene(aiscene, scene);
				}
			} while (FindNextFile(hFind, &fd));

			FindClose(hFind);
		}
	}

	static void LoadSceneFromAIScene(const aiScene* aiscene, Scene* scene)
	{
      // Load lights
      if (aiscene->HasLights())
      {
         for (UINT i = 0; i < aiscene->mNumLights; i++)
         {
            aiLight light = aiscene->mLights[0][i];
            if (light.mType == aiLightSource_DIRECTIONAL)
               scene->dirLight.push_back(float4(light.mDirection.x, light.mDirection.y, light.mDirection.z, 0.0f));
         }
      }

      if (aiscene->HasMaterials())
      {
         // Load materials
         scene->material.resize(aiscene->mNumMaterials);
         std::vector<std::string> textureFilePaths;
         for (UINT i = 0; i < aiscene->mNumMaterials; i++)
         {
            // Get the diffuse color
            aiColor3D diffuseColor(0.0f, 0.0f, 0.0f);
            aiscene->mMaterials[i]->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
            scene->material[i].diffuse = Color((char)(diffuseColor.r * 255.0f), (char)(diffuseColor.g * 255.0f), (char)(diffuseColor.b * 255.0f));

            // Get the specular color
            aiColor3D specularColor(0.0f, 0.0f, 0.0f);
            aiscene->mMaterials[i]->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
            scene->material[i].specular = Color((char)(specularColor.r * 255.0f), (char)(specularColor.g * 255.0f), (char)(specularColor.b * 255.0f));

            // Get the ambient color and shine
            aiColor3D ambientColor(0.0f, 0.0f, 0.0f);
            aiscene->mMaterials[i]->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor);
            scene->material[i].ambient = Color((char)(ambientColor.r * 255.0f), (char)(ambientColor.g * 255.0f), (char)(ambientColor.b * 255.0f));
            aiscene->mMaterials[i]->Get(AI_MATKEY_SHININESS, scene->material[i].specularShine);

            // Get the diffuse texture map
            aiString filePath;
            UINT texCount = aiscene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE);
            // Load texture into the scene (also creates a reference in material i)
            if (texCount > 0 && aiscene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &filePath) == AI_SUCCESS)
               textureFilePaths.push_back(filePath.C_Str());
         }

         // Load textures
         scene->texture.resize(textureFilePaths.size());
         for (UINT i = 0; i < scene->texture.size(); i++)
         {
            LoadTexture(scene, textureFilePaths[i], DIFFUSEMAP, i);
            scene->material[i].diffuseMap = &scene->texture[i];
            scene->material[i].hasDiffuseMap = true;
         }
      }

      // Load Meshes
      if (aiscene->HasMeshes())
      {
			for (UINT i = 0; i < aiscene->mNumMeshes; i++)
			{   
				Mesh mesh;
				mesh.name = aiscene->mMeshes[i]->mName.C_Str();

				// Material information is already loaded in
				// The mesh just needs some direction to it
				UINT matIndex = aiscene->mMeshes[i]->mMaterialIndex;
				mesh.mat = &scene->material[matIndex];

				// Size the arrays
				mesh.vertices.resize(aiscene->mMeshes[i]->mNumVertices);
				mesh.vertexNormals.resize(aiscene->mMeshes[i]->mNumVertices);
				mesh.texCoords.resize(aiscene->mMeshes[i]->mNumVertices);

				// Get the vertices, vertex normals 
            for (UINT j = 0; j < aiscene->mMeshes[i]->mNumVertices; j++)
            {
               // Vertices
               mesh.vertices[j] = float3(aiscene->mMeshes[i]->mVertices[j].x,
                  aiscene->mMeshes[i]->mVertices[j].y,
                  aiscene->mMeshes[i]->mVertices[j].z);

               // Normals
               mesh.vertexNormals[j] = float3(aiscene->mMeshes[i]->mNormals[j].x,
                  aiscene->mMeshes[i]->mNormals[j].y,
                  aiscene->mMeshes[i]->mNormals[j].z);

               // Texture Coordinates (If there is only 1 texture we can premultiply the dimensions)
               if (mesh.mat->hasDiffuseMap && aiscene->mMeshes[i]->HasTextureCoords(0))
               {
                  // After the UV coordinates are flipped anything > 1.0f becomes negative and thus we need to shift it up. Any number will do since it's being modded but as long as it becomes positive
                  float u = /*fmod(*/aiscene->mMeshes[i]->mTextureCoords[0][j].x/* + 8.0f, 1.0f)*/;
                  float v = /*fmod(*/aiscene->mMeshes[i]->mTextureCoords[0][j].y/* + 8.0f, 1.0f)*/;

                  mesh.texCoords[j] = float2(u, v) * mesh.mat->diffuseMap->GetDim();
               }
            }

				// Indices
				mesh.indices.resize(aiscene->mMeshes[i]->mNumFaces * 3);
				for (UINT j = 0; j < aiscene->mMeshes[i]->mNumFaces; j++)
				{
					for (UINT k = 0; k < aiscene->mMeshes[i]->mFaces[j].mNumIndices; k++)
					{
						mesh.indices[j * 3 + k] = aiscene->mMeshes[i]->mFaces[j].mIndices[k];
					}
				}

				scene->mesh.push_back(mesh);
			}
		}
	}

	// Adds texture to scene
	static bool LoadTexture(Scene*& scene, std::string filePath, TextureType texType, int textureIndex)
	{
		bool results = false;
		// Remove all the preceding text in the texture filePath
		//int pos = filePath.find_last_of('\\') + 1;
        int pos = filePath.find_last_of('//') + 1;
		filePath = filePath.substr(pos, filePath.size() - pos);
		std::string name = filePath.substr(0, filePath.length() - 4);

		// If the texture doesn't already exist
		int texIndex = scene->FindTexture(name);
		if (texIndex == -1)
		{
			filePath = "Content/Textures/" + filePath;

			// Load the png using LodePng
			std::vector<unsigned char> image;
			UINT width, height;
			UINT error = lodepng::decode(image, width, height, filePath);
			//std::string errorStr = lodepng_error_text(error);

			if (error == 0)
			{
				// Parse the image so I can use it as a pointer
				unsigned char* buffer = new unsigned char[width * height * 3];
				for (UINT y = 0; y < height; y++)
				{
					for (UINT x = 0; x < width; x++)
					{
						UINT i = y * width + x;
						UINT j = i * 3;
						UINT k = i * 4;
						buffer[j] = image[k];
						buffer[j + 1] = image[k + 1];
						buffer[j + 2] = image[k + 2];
					}
				}

				// Add the texture to the scene
				scene->texture[textureIndex] = Texture(name, width, height, buffer, texType);
				results = true;
			}
		}

		return results;
	}
};