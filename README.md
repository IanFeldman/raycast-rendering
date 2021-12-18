# raycast-rendering

![Video demonstrating movement](https://i.imgur.com/eCPEj69.gif)

A simple SDL2 demo of a raycast rendering engine similar to the 1992 Wolfenstein 3D engine. 

## Dependencies
- Git
- C++ Compiler (gcc, ...)
- SDL2 Library

On MacOS:
```
$brew install git sdl2
```
*g++ can be installed through the mac command line developer tools*

## Build Instructions
```sh
# Clone this repo
git clone https://github.com/IanFeldman/raycast-rendering.git
cd raycast-rendering

# Create a build folder
mkdir build
cd build

# Build (g++ on MacOS)
g++ *.cpp -o raycast-rendering -I /path/to/include -L /path/to/lib -l SDL2-2.0.0

# Run
./raycast-rendering
```

## Controls
| Key | Description |
| --- | --- |
| `w` | Move forward |
| `s` | Move backward |
| `a` | Look left |
| `d` | Look right |
| `m` | Toggle map |
| `esc` | Quit |

## Gallery
![Screenshot](https://i.imgur.com/p9GFEJM.png)
![Screenshot](https://i.imgur.com/3tPRK2l.png)
![Screenshot](https://i.imgur.com/FPNUblO.png)
