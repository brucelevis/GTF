# GTF
**Graphics Tools Framework**
A Framework to create C/C++ desktop tools oriented to 2D/3D graphics manipulation. 
This project started as an extraction of reaprofitable code from the <a href="https://www.youtube.com/watch?v=f0P1ZqO0zOk" target="_blank">TexPainter project</a>. 
The idea is to implement a feature-rich library that powers tools like TexPainter and others with similar needs.

Highlighted Features
--------------------
- Multiple windows with shared OpenGL 3 context on Windows and MacOSX
- Non destructive ImGui integration with additional widgets
- Abstract render hardware interface 
- Template class collection for custom node graphs

Coming soon
-----------
- Texture loader (JPEG, PNG, TGA, HDR)
- Texture writer (PNG, TGA, HDR) 
- 3D Mesh loader (OBJ, FBX)
- Save/Load dialogs helper
- A docking system based on the LumixEngine implementation
- 3D Mesh viewer (as example)

Help needed for EVERYTHING! but mostly...
-----------------------------------------
- API Design, some decisions I made are not the best ones
- Build system implementation/configuration, maybe CMake?
- Replacing GLFW with something that let us use DirectX in the future
- Replacing tinyfielddialogs, while it is a very useful library I prefer a more native-os-api approach
- Experience with *bgfx* to discuss about it's integration here instead of the current render hardware interface


Examples
--------
**CalcGraph Example**
<br />
<a href="http://www.youtube.com/watch?v=m6eteEPQ0Lg" target="_blank">
<img src="http://img.youtube.com/vi/m6eteEPQ0Lg/0.jpg"/>
</a>

**NoiseGenerator Example**
<br />
<a href="http://www.youtube.com/watch?v=XCk2LmzZI3A" target="_blank">
<img src="http://img.youtube.com/vi/XCk2LmzZI3A/0.jpg"/>
</a>

Credits
-------
Developed by [David Gallardo](https://twitter.com/galloscript) portalg@gmail.com

Libraries
---------
- GLFW - OpenGL windows
- ImGui - Grahic user interface
- glm - maths
- Autodesk FBX - FBX mesh loading
- tiny_obj_loader - OBJ mesh loading
- stb headers - texture loading/write, text utilites, etc...
- glad - OpenGL functions loader
- khrplatform - khronos types
- mikktspace - Synched normals
- tinyfielddialogs - load/save dialogs

