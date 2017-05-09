# Software-Renderer
This is a software renderer I made using C++ and SDL for 2D display of rendered results. I wrote this to understand how d3d and openGL work under the 
hood. Everything is written from scratch except my use of SDL to display and manage a window and LodePNG to load textures.

Can currently rasterize point graphs, wireframes, flat shaded objects, smooth shaded objects, it can sample textures for a quite a few of the shading
techniques and can do flat shaded ray casting. To switch between reseults you can use the keys 0-9. To rotate the object viewed you can use wasd. You
can also use ijkl to move the camera around. Press U and Y to switch between Scanline and Barycentric triangle filling techniques. Finally you can also
press m to export a screenshot to a png.