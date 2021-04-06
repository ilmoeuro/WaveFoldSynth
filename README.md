## Prerequisites (Windows)

You need the following programs:

 - Microsoft Visual C++ Build Tools 2019
 - CMake 3.14 or newer

Additionally, you need to have VST3 SDK installed into the following location:

> C:\Users\Public\Steinberg\VST_SDK\VST3_SDK

## Build instructions (Windows)

Configure using the following commands:

    cd \Path\To\Project
    mkdir build
    cd build
    cmake .. -DSMTG_CREATE_PLUGIN_LINK=NO

Build using the following commands:

    cd \Path\To\Project\build
    cmake --build .