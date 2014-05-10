# Ray Tracer

This is a tiny ray tracer, rendering scene purely using CPU.

It uses the `SDL` to present the image to users. (See http://www.libsdl.org/)

# Compile

## Under Windows

Microsoft Visual C++ 2010 & 2012 are currently supported. To compile, please open the respective project file in the root folder. We already provide `SDL.dll` in the root folder and the SDL library in the folder `SDL-1.2.15`.

Please, switch to the `Release` mode to improve the efficiency of the program.

## Under Linux

To compile, please type `make` in the folder `/ray_tracer`, then the file `rt` will be generated in this folder. You should have the SDL library installed first. (You can download SDL library from http://www.libsdl.org/download-1.2.php)

# Demos

We currently provide 4 demos for use. 

The first demo presents two Utah teapots ported from GLUT, with the `material_transparent` to show the reflection & refraction.

The second demo presents the Earth with texture. This is a real-time demo, you can press left & right to rotate the camera.

The third demo presents a quadratic surface.

The forth demo presents the famous Stanford Bunny (See http://en.wikipedia.org/wiki/Stanford_bunny) stored with the ply file format in `/ray_tracer/demos/stanford_bunny.ply`.

# Reference

Ray Tracing from the Ground Up (2007)

Fundamentals of Computer Graphics (2009)