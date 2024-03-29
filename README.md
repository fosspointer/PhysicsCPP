# Physics school project

![First Screenshot](./docs/screenshot_1.png)
![Second Screenshot](./docs/screenshot_2.png)

## Dependencies

- SFML 2.5.1 for rendering graphics (packaged as submodule)

## Requirements

- System that supports legacy OpenGL (graphics only)
- CMake 3.16 or greater
- Ninja/GNU Make
- A c++ compiler such as clang or g++ with c++ 17 support (standard c++ support, no need for extension support)
- Git for cloning recursively (optional, *.zip downloads are not encouraged)
- (All of the mentioned tools must be in the PATH in order for the instructions to work properly)

## Build instructions for bash/powershell

```bash
git clone --recursive https://github.com/undefinedpp/physics.git
cd physics && mkdir build && cd build && cmake -G [name of generator] ..
```

For CMake generator examples, see [Link to the CMake manual](https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html)

## Personal coding environment

See [my user repository](https://github.com/undefinedpp)

## Pros for developers

- Modular and extensible
- Simple, easy to understand UI library
- Easy to code demonstrations in
- Well organized source control

## UI library elements
- Labels
- Buttons
- Layouts (HLayout, VLayout, HGrid)
- Sliders
- Dropdowns

## Future plans

- Allow usage of different rendering/audio engines and implementing such cases
- Expand demonstration capabilites of the software
- Optimize for ease-of-use of client-implementation, simplifying the proccess further 
