# Minimal program that aligns realsense D400 series sensor color and depth frames

### Compiling

(change paths)

`g++ align-nogui.cpp -I/home/gekko/librealsense/include/ -L/home/gekko/librealsense/build/ -lrealsense2`

### Running

`LD_LIBRARY_PATH=/home/gekko/librealsense/build/ ./a.out`

With address sanitizer libs preloaded:

`LD_PRELOAD=/usr/lib/arm-linux-gnueabihf/libasan.so.5 LD_LIBRARY_PATH=/home/gekko/librealsense/build/ ./a.out`

