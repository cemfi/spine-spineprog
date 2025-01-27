# SPINEprog

*SPINEprog* is the name of the tool that is used to configure the [SPINE] platform and to handle incoming sensor data.

[![License](https://img.shields.io/badge/license-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl.html)

[SPINE] is a hardware/software solution to connect sensor modules to programming environments like Max/MSP. It is based on Arduino, which is an open-source electronics platform intended for making interactive projects. It uses the [GROVE] system, which lets you physically connect sensors to your Arduino without soldering or using a breadboard. The tight integration of SPINE with UDP-enabled software like Max/MSP makes it ideal for developing sensor-based sound user interfaces, both for beginners and advanced users.

### Adding new modules / sensors
The `src` folder contains the source code. In order to add new modules, a new header file must be created in the `src/code` folder. You should bundle all code for the new sensor in a single header `.h` file. The `dummy.h` can be used as a starting point. The freshly created `.h` file must be added to the project resources (sub-directory `code/`). To configure the new sensor, you need to add an entry to the `xml/modules.xml`.

[SPINE]: <http://www.cemfi.de/research/spine/>
[GROVE]: <http://www.seeedstudio.com/wiki/Grove_System>

[v1.0.0]: <https://github.com/cemfi/spine2-spineprog/releases/tag/v1.0.0>
[v1.1.0]: <https://github.com/cemfi/spine2-spineprog/releases/tag/v1.1.0>
