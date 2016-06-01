# spineprog

The [SPINE] is a hardware/software solution to connect sensor modules to programming environments like Max/MSP. It is based on Arduino, which is an open-source electronics platform intended for making interactive projects. It uses the [GROVE] system, which lets you physically connect sensors to your Arduino without soldering or using a breadboard (see the picture below). The tight integration of the SPINE with Max/MSP makes it ideal for developing sensor-based sound user interfaces, both for beginners and advanced users.

*spineprog* is the name of the tool that is used to configure the SPINE platform.

### Release History
- [v1.0.0] - Initial Release (2016-05-31)

### Development Hints
The `src` folder contains the source code. In order to add new modules, a new header file must be created in the `src/code` folder. The `dummy.h` can be used as a starting point.
As of now, the freshly created file must be added to the list of copied files in the `codegen.cpp` as well as to the resource files. To display the new sensor correctly in the user interface, it has to be added to the `xml/modules.xml`.

This procedure is soon to be simplified...

### License

This software is licensed under the GPL v3.

[SPINE]: <http://www.cemfi.de/research/spine/>
[GROVE]: <http://www.seeedstudio.com/wiki/Grove_System>

[v1.0.0]: <https://github.com/cemfi/spine2-spineprog/releases/tag/v1.0.0>
