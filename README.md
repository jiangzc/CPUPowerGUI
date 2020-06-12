## CPUPowerGUI



CPUPowerGUI is a CPU frequency scaling tool for Linux with graphical user interface. It create a mapping between the kernel information in /sys/devices/system/cpu/* and UI elements.  This program uses C++ and Qt5 Framework.  



Feature:  

+ Change all cores' frequency to pre-defined value. 
+ Turn on or off a single cpu core.
+ View details of CPU cores.
+ Modify values of available CPU options.



## ScreenShot

![screenshot1](/home/jzc/CPUPowerGUI/pic/screenshot1.png)

![screenshot2](/home/jzc/CPUPowerGUI/pic/screenshot2.png)

![screenshot3](/home/jzc/CPUPowerGUI/pic/screenshot3.png)



## Install

For Debian based distros, download released CPUPowerGUI deb package and install it.   

For other distros, compile the program and run the executable file as root user.  

```bash
mkdir build && cd build
qmake ..
make -j4
sudo ./CPUPowerGUI
```

