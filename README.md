# Hebi Robotics setup for robot-mediated Dyadic (Human-human) Interactions
## Dependencies
Install python minimum required python 3.6 from:
https://www.python.org/downloads/

Install cmake minimum required cmake 3.12 from:
https://cmake.org/download/


### For windows
Install visual studio from:
https://visualstudio.microsoft.com/downloads/




Install _desktop development with C++_ from Visual Studio Installer


## Startup
Start the terminal from root directory
```
mkdir build
cd build
cmake -G"Visual Studio 16 2019" -Ax64 ../projects/cmake/
```
Then open the __build/hebi_human_human.sln__ from visual studio
To know the version of you Visual Studio by
```
cmake --help
```

Lets say you are using Visual Studio 2022, then you need
```
cmake -G"Visual Studio 17 2022" -Ax64 ../projects/cmake/
```

