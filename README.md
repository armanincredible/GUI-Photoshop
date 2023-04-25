# GUI & Photoshop

## About

This project is aimed at creating a gui and based on it to build a program like photoshop. At the moment, it implements drawing and filling primitives, as well as interaction with external plugins. The project uses the graphic lib sfml.

## Build and usage

- Install SFML
- Clone project 
``` 
  git clone https://github.com/armanincredible/Compiler.git
```
- Build it and run
```
  make
  ./do
```

Also you can use -DDEBUG (check makefile) to get log file with name Log.log

## Plugins

The GUI has support for adding a plugin. The plugin.h file contains the API that is used to interact with the plugin. An example plugin can be found in plugins/brush.h and its source file.
