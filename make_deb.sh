#!/bin/bash
set -e
cd build
qmake ..
make -j4
cd ..
sudo cp build/CPUPowerGUI deb/CPUPowerGUI/opt/CPUPowerGUI

cd deb
sudo chown root.root -R ./CPUPowerGUI
dpkg -b ./CPUPowerGUI ./CPUPowerGUI_v0.1.0_amd64.deb
sudo chown jzc.jzc -R ./CPUPowerGUI