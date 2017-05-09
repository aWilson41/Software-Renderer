# Software-Renderer
This is a software renderer I made using C++ and SDL for 2D display of rendered results. Essentially this does the job of OpenGL and D3D on the
processor without the using them. Everything is written from scratch except my use of SDL to display and manage a window and LodePNG to load textures.

Can currently rasterize point graphs, wireframes, flat shaded objects, smooth shaded objects, it can sample textures, and can do flat shaded ray casting.
To switch between results you can use the keys 0-9. 
To rotate the object viewed you can use wasd. 
To move the camera you can use ijkl.
To switch between scanline and barycentric triangle filling press U/Y.
To export a png image of the current results press m.