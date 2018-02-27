# NEXWorld
  
NEXWorld is a NEWorld implemention, licensed under WTFPL v2!  
This was created for testing another program structure (different from NEWorld's) for Minecraft-like games...  
  
## Compilation
Dependencies include opengl, glew, sdl2, sdl2-image.  
  
### Windows
  
You can open the Visual Studio solution `./build/vs15/NEXWorld.sln` directly with Visual Studio 2017.  
  
It's recommended to use vcpkg to download the dependencies:  
```
vcpkg install glew sdl2 sdl2-image
```
Or you can put headers into directory `./include`, static libraries into `./lib` or `./lib64` for x86 and x64 configuration respectively.  
  
In order to load *mainplugin*, please set the working directory of project *client* (*Project -> Properties -> Debugging -> Working Directory*) to `$(SolutionDir)..\..\Release\x86\` or `$(SolutionDir)..\..\Release\x64\` for x86 and x64 configuration respectively.  
  
Notice: Building project *client* or *server* will not cause *mainplugin* to build. You may need to right-click it and select *build* manually.  
  
### Linux & macOS
  
First, install the dependencies:  
  
*Example using apt:*
```
apt-get install libglew-dev libsdl2-dev libsdl2-image-dev
```
*Example using homebrew:*
```
brew install glew sdl2 sdl2_image
```
  
Then clone this repository and build with CMake:  
  
```
git clone https://github.com/qiaozhanrong/NEXWorld
cd NEXWorld
cmake ./
make
```
  
The build results will be at `./release/Binaries`.  
Run `clientexec` to execute client. *(Server is not ready yet!)*  

## Feedback
  
Encountered problems? Post issues...  
