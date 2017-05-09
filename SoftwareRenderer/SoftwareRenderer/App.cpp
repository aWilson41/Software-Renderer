#include "App.h"
#include "ResourceLoader.h"

void App::Initialize()
{
   
}

void App::UnLoad()
{

}

void App::Load()
{
	// Generate geometry
	ResourceLoader::LoadAllMeshesFromFolder("Content/*", &scene);

	// Set the view

	// Set the projection
	scene.cam.SetPerspective(70.0f, (float)sDevice.screen.GetScreenWidth() / (float)sDevice.screen.GetScreenHeight(), 0.01f, 2000.0f);

   for (UINT i = 0; i < scene.mesh.size(); i++)
   {
      scene.mesh[i].world = MathHelper::MatrixTranslate(0.0f, -50.0f, 700.0f) * MathHelper::MatrixRotateY(0.0f) * MathHelper::MatrixScale(15.0f);
   }
	//scene.mesh[1].world = MathHelper::MatrixTranslate(0.0f, -150.0f, 700.0f) * MathHelper::MatrixRotateY(2.0f) * MathHelper::MatrixScale(15.0f);
	//scene.mesh[2].world = MathHelper::MatrixTranslate(0.0f, -150.0f, 700.0f) * MathHelper::MatrixRotateY(2.0f) * MathHelper::MatrixScale(15.0f);

   // Add a light if the scene doesn't have one
   if(scene.dirLight.size() == 0)
	   scene.dirLight.push_back(float4(0.0f, 0.0f, -1.0f, 0.0f));
}

void App::Update()
{
	if (IsCurrentKeyDown(27) && !IsPreviousKeyDown(27))
		Quit();

	// Rotation
	if (IsCurrentKeyDown('d'))
		scene.mesh[0].world = scene.mesh[0].world * MathHelper::MatrixRotateZ(0.05f);

	if (IsCurrentKeyDown('a'))
		scene.mesh[0].world = scene.mesh[0].world * MathHelper::MatrixRotateZ(-0.05f);

	if (IsCurrentKeyDown('w'))
		scene.mesh[0].world = scene.mesh[0].world * MathHelper::MatrixRotateX(0.05f);

	if (IsCurrentKeyDown('s'))
		scene.mesh[0].world = scene.mesh[0].world * MathHelper::MatrixRotateX(-0.05f);

	if (IsCurrentKeyDown('e'))
		scene.mesh[0].world = scene.mesh[0].world * MathHelper::MatrixRotateY(0.05f);

	if (IsCurrentKeyDown('q'))
		scene.mesh[0].world = scene.mesh[0].world * MathHelper::MatrixRotateY(-0.05f);

	if(IsCurrentKeyDown('g'))
		scene.mesh[0].world = scene.mesh[0].world * MathHelper::MatrixTranslate(-0.2f, 0.0f, 0.0f);

	if (IsCurrentKeyDown('h'))
		scene.mesh[0].world = scene.mesh[0].world * MathHelper::MatrixTranslate(0.2f, 0.0f, 0.0f);

	// Scaling
	if (IsCurrentKeyDown('p'))
		scene.mesh[0].world = scene.mesh[0].world * MathHelper::MatrixScale(1.05f);

	if (IsCurrentKeyDown('o'))
		scene.mesh[0].world = scene.mesh[0].world * MathHelper::MatrixScale(0.95f);

	// Camera controls
   if (IsCurrentKeyDown('j'))
   {
      //scene.mesh[0].mat->specularShine += 0.1f;
      //scene.dirLight = MathHelper::MatrixRotateY(0.05f) * scene.dirLight;
      scene.cam.SetCameraView(MathHelper::MatrixRotateY(0.02f));
   }

   if (IsCurrentKeyDown('l'))
   {
      //scene.mesh[0].mat->specularShine -= 0.1f;
      //scene.dirLight = MathHelper::MatrixRotateY(-0.05f) * scene.dirLight;
      scene.cam.SetCameraView(MathHelper::MatrixRotateY(-0.02f));
   }

   if (IsCurrentKeyDown('i'))
   {
      //scene.dirLight = MathHelper::MatrixRotateX(0.05f) * scene.dirLight;
      scene.cam.SetCameraView(MathHelper::MatrixRotateX(-0.02f));
   }

   if (IsCurrentKeyDown('k'))
   {
      //scene.dirLight = MathHelper::MatrixRotateX(-0.05f) * scene.dirLight;
      scene.cam.SetCameraView(MathHelper::MatrixRotateX(0.02f));
   }

   if (IsCurrentKeyDown('g'))
      scene.cam.SetCameraView(MathHelper::MatrixTranslate(0.0f, 0.0f, 8.0f));

   if (IsCurrentKeyDown('t'))
      scene.cam.SetCameraView(MathHelper::MatrixTranslate(0.0f, 0.0f, -8.0f));

   if (IsCurrentKeyDown('h'))
      scene.cam.SetCameraView(MathHelper::MatrixTranslate(-8.0f, 0.0f, 0.0f));

   if (IsCurrentKeyDown('f'))
      scene.cam.SetCameraView(MathHelper::MatrixTranslate(8.0f, 0.0f, 0.0f));

	// Switch technique
   if (IsCurrentKeyDown('1'))
      sDevice.SetTech(POINTGRAPH, NOSHADER, NOSAMPLE);
   else if (IsCurrentKeyDown('2'))
      sDevice.SetTech(WIREFRAME, NOSHADER, NOSAMPLE);
   else if (IsCurrentKeyDown('3'))
      sDevice.SetTech(RASTER, NOSHADER, NOSAMPLE);
   else if (IsCurrentKeyDown('4'))
      sDevice.SetTech(RASTER, FLAT, NOSAMPLE);
   else if (IsCurrentKeyDown('5'))
      sDevice.SetTech(RASTER, SMOOTH, NOSAMPLE);
   else if (IsCurrentKeyDown('6'))
      sDevice.SetTech(RASTER, NOSHADER, POINTSAMPLE);
   else if (IsCurrentKeyDown('7'))
      sDevice.SetTech(RASTER, FLAT, POINTSAMPLE);
   else if (IsCurrentKeyDown('8'))
      sDevice.SetTech(RASTER, SMOOTH, POINTSAMPLE);
   else if (IsCurrentKeyDown('9'))
      sDevice.SetTech(RASTER, PHONGFLAT, NOSAMPLE);
   else if (IsCurrentKeyDown('0'))
      sDevice.SetTech(RASTER, PHONGSMOOTH, NOSAMPLE);
	else if(IsCurrentKeyDown('+'))
		sDevice.SetTech(RAYTRACE, SMOOTH, POINTSAMPLE);

	if (IsCurrentKeyDown('u'))
		sDevice.rasterizer.rTech = SCANLINE;
	else if (IsCurrentKeyDown('y'))
		sDevice.rasterizer.rTech = BARYCENTRIC;

	if (IsCurrentKeyDown('m') && !IsPreviousKeyDown('m'))
		Utility::SaveScreenshot(sDevice.screen.GetBuffer(), sDevice.screen.GetScreenWidth(), sDevice.screen.GetScreenHeight(), "Output/output.png");
}

void App::Draw()
{
	sDevice.screen.ClsColor(Color(135, 206, 250));
	scene.DrawSceneToBuffer(&sDevice);
}