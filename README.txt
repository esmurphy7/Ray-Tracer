CSC 305 - Assignment 1
Evan Murphy

====================================
Features Implemented
 - Compiles and Runs (see compilation section)
 - Two spheres
 - Single plane
 - Diffuse shading
 - Single light source
 - Fixed camera
 - Hard Shadows

====================================
Known Issues
 - The light source seems to cast a shadow itself onto other objects
 - When the spheres are moved to some particular positions, the lighting effect is warped
 - The light source doesn't seem to light a realistic amount of the spheres' surface
    - to fix this, a bias value could be added to bias rays towards the light source

====================================
Compilation
- This assignment was developed, compiled, and run on Windows using the CLion IDE and CMake.
- To compile from command line however, run "g++ -std=c++11 -o cscA1.exe main.cpp" to produce "cscA1.exe" executable
