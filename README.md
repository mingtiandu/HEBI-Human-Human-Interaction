# Hebi Robotics setup for robot-mediated Dyadic (Human-human) Interactions

## Dependencies

Install python minimum required python 3.6 from:
https://www.python.org/downloads/



Install cmake minimum required cmake 3.12 from:
https://cmake.org/download/


HEBI necessary API will be downloaded automatically, or can manually download from:
https://docs.hebi.us/tools.html#cpp-api/


### For Windows OS users

Install visual studio from:
https://visualstudio.microsoft.com/downloads/







Install *desktop development with C++* from Visual Studio Installer





## Startup

Start the terminal from root directory

```
mkdir build
cd build
cmake -G"Visual Studio 16 2019" -Ax64 ../projects/cmake/
```

Then open the **build/hebi\_human\_human.sln** from visual studio
To know the version of you Visual Studio by

```
cmake --help
```



Lets say you are using Visual Studio 2022, then you need

```
cmake -G"Visual Studio 17 2022" -Ax64 ../projects/cmake/
```



<!--stackedit\_data:
eyJoaXN0b3J5IjpbNTU4MzI2MTYwXX0=

