#pragma once
#include "MathHelper.h"
#include "Screen.h"
#include "Mesh.h"
#include "Camera.h"

class Rasterizer
{
public:
	RasterizationTechnique rTech;

public:
    Rasterizer();

    // Draw Lines
    void DrawBhmLineColor(Screen* screen, float2 p1, float2 p2, Color color);
    void DrawRightScanLine(Screen* screen, int x1, int x2, int y, Color color);
    void DrawSortedScanLine(Screen* screen, int x1, int x2, int y, Color color);

    // Draw Triangles
    void DrawWireTriangle(Screen* screen, Triangle2 triangle, Color color);
	
    // Surface Shading
    void DrawTriangle(Screen* screen, Triangle3 triangle, Color color);
    void DrawScanlineTriangle(Screen* screen, Triangle3 triangle, Color color);
    void DrawTriangle(Screen* screen, Triangle3 triangle, Texture* tex, float2 texCoord1, float2 texCoord2, float2 texCoord3);

    // Flat Diffuse Shading
    void DrawTriangle(Screen* screen, Triangle3 triangle, Texture* tex, float2 texCoord1, float2 texCoord2, float2 texCoord3, float diff);

    // Smooth Shading
    void DrawTriangle(Screen* screen, Triangle3 triangle, Color color, float diff1, float diff2, float diff3);
    void DrawTriangle(Screen* screen, Triangle3 triangle, Color color1, Color color2, Color color3);
    void DrawTriangle(Screen* screen, Triangle3 triangle, Texture* tex, float2 texCoord1, float2 texCoord2, float2 texCoord3, float diff1, float diff2, float diff3);		

    // Draw Circle
    void DrawBhmEllipse(Screen* screen, float2 center, float xRadius, float yRadius, Color color);

    // Draw Mesh
    void DrawMeshPoints(Screen* screen, Mesh* mesh, Camera* cam, UINT pointSize);
    void DrawMeshWireFrame(Screen* screen, Mesh* mesh, Camera* cam);

    // Surface Shaded
    void DrawScanlineSurfaceMesh(Screen* screen, Mesh* mesh, matrix4x4 view, matrix4x4 proj);
    void DrawSurfaceMesh(Screen* screen, Mesh* mesh, Camera* cam);
    void DrawTexturedSurfaceMesh(Screen* screen, Mesh* mesh, matrix4x4 view, matrix4x4 proj);

    // Smooth Shaded
    void DrawSmoothMesh(Screen* screen, Mesh* mesh, Camera* cam, float3 dirLight);
    void DrawTexturedSmoothMesh(Screen* screen, Mesh* mesh, Camera* cam, float3 dirLight);
    void DrawPhongSmoothMesh(Screen* screen, Mesh* mesh, Camera* cam, float3 dirLight);
    void DrawPhongTexturedSmoothMesh(Screen* screen, Mesh* mesh, Camera* cam, float3 dirLight); // Incomplete

    // Flat Shaded
    void DrawScanlineFlatMesh(Screen* screen, Mesh* mesh, Camera* cam, float3 dirLight);
    void DrawFlatMesh(Screen* screen, Mesh* mesh, Camera* cam, float3 dirLight);
    void DrawTexturedFlatMesh(Screen* screen, Mesh* mesh, Camera* cam, float3 dirLight);
    void DrawPhongFlatMesh(Screen* screen, Mesh* mesh, Camera* cam, float3 dirLight);
    void DrawPhongFlatTexturedMesh(Screen* screen, Mesh* mesh, Camera* cam, float3 dirLight); // Incomplete
};