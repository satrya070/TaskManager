# Task Manager / multi-platform application template
This is a simple C++ TaskManager multi-platform application(tested on Window, Linux) set up with CMake with the following components:
ImGui as the GUI
Sqlite as database
Google Test for unit tests

The task application in this case is an example application, but this a template that can be used as base, and can morphed into any other application that fits these components. These components can be swapped for different ones as well of course.

To set the Cmake project up run:
```
mkdir build
cd build
cmake ..
```

### Additional info Linux install
On Linux when getting errors with Mesa, make sure to have the Mesa and X11 packages installed. Or install them with:
```
sudo apt install libgl1-mesa-dev xorg-dev
```
The Mesa package included the openGL implementation for Linux that this application uses for rendering the GUI.

## Structure
A simple database abstraction layer is implemented to add other types of databases, and a repository pattern is implemented to isolate domain from the database logic. It also includes command classes that can be used to implement undo actions for example, but it can be easily deleted if not needed.
