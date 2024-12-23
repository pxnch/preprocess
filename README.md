# Preprocess
a simple tool to run prebuild scripts in lua to generate header files

# How to use - CMake
## Get the Repo
```cmd
git submodule add https://github.com/pxnch/preprocess.git
git submodule update --init --recursive
```
## CMakeList
```cmake
add_subdirectory(preprocess)
...
target_link_libraries(<NAME> preprocess)
```

# How it works
This will now run preprocess everytime as the first build step and the target can include all headers generated by it. 
Every .lua File inside of the preprocess/scripts/ Folder will be run

# Lua API
## Lua Version
This is running [CMake Lua Version 5.4.7](https://github.com/walterschell/Lua/tree/52674cd192943015b99f51225f8cf8a116aa7293) - [Manual](https://www.lua.org/manual/5.4/)
## Custom API
```lua
local header = preprocess.create_header(name: string) -- Create new Header Object
header:add_content(content: string) -- Add Raw Text to the Header
header:add_macro(name: string, content: any) -- Create a Macro
header:write() -- Write to the File
```