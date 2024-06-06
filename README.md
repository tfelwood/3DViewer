# Description of 3DViewer project
The **3DViewer** program is designed to **display 3D models parsed from files
in format \*.obj, in wireframe form**.
It is able to use vertexes and facets only. Qt library is used for interface
realization.

## Requirements and dependencies
To run the 3DViewer **you will require** following programs:  **Qt6 or later
version ( including qmake), bash, GCC compiler, GNU make, Mingw compiler (Qt6 contains it)**.  
**For Windows OS** it might be good to install **Cygwin** to make tests. Also you need to **add the
paths corresponding to the following to the PATH variable**:  
_C:\..(path to Qt installed)\Qt\6.4.1(Qt version)\mingw 64(mingw version)\bin_  
_C:\(path to Qt installed)\Qt\Tools\mingw1120 64(mingw tools version)\bin_  
Maybe it will be necessary to set equivalent paths in MacOS or Linux
if build goes wrong. 

## Building  
In order to build the program, use **make** in the
**root directory** of the project. Then you may execute program, **3DViewer is
located in the folder build/bin**.

## Documentation  
You can learn more about the program and its features by examining the **3DViewer.pdf** file.
