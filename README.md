# wail
RPG game made with C and SDL3

## progress

| Version           | Detail                        |
|:------------------|------------------------------:|
| v0.0.1            | The games in in a very unstable state with ideas being poured in and constant breaking code changes|
|                   |                               |
|                   |                               |
|                   |                               |
|                   |                               |

## Compiling

As of right now everything is meant to be ran on a linux based system.
Windows support will probably be added as soon as the project reaches a stable state.


**prerequisites**

* NodeJS v10 or newer - To run src/op_gen.js (required to compile)
* Gifsicle (optional) - To fix gifs in src/res/


**Downloading the project**

First clone this repository with its submodules,
This will pull the following additional repositories:

* SDL3
* SDL_ttf
* SDL3_image
* TinySpline

```
git clone https://github.com/Last-Station/wail --recurse-submodules
```


**Compiling the submodules**

For this you only need to run the provided script (compile_libs.sh)
WARNING: this will use a lot of CPU

In the root of the repository do the following in a terminal:
```
./compile_libs.sh
```

Note for dev: You only need to run this script once after a fresh
clone. The output will live in 'bin/'


**Compiling the project**

Once you have all the above steps done, you can simply run the:

```./compile_test.sh``` for a test build (stable)
```./compile_production.sh``` for a release build (WIP/TODO, do not use at the moment)

After running one of the above scripts, you should find an executable in build/

## Installing

TODO!
