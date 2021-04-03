# GameEngineC

Work in progress  
A high performance 2D game engine written in C++.  


## Solutions
Engine		- Core Engine and Game template  
Editor		- Editor for creating games  
CppMirror	- Reflection / Header tool  


## External libraries
OpenGL		- Graphics  
Glad		- OpenGL Loading Library  
GLFW		- Window and input  
stbh_image	- For loading images  
Freetype	- For loading ttf text font files  
cereal		- Serialization for Editor  
Dear ImGui	- UI for Editor  


## How to use

Work in progress  

### Before creating new project with Editor

Build Game.dll and Core.lib inside Engine directory  
Copy Core.lib into Editor->LevelEditor->libs  
Add default assets to Assets directory  

### Notes

After having made changes to the Core project make sure it is built and Core.lib has been copied into Editor->LevelEditor->libs  