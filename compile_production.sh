#!/bin/bash
# This is WIP, and will be the main compile script when release

# Variables

INCLUDE_LIB_PATHS="-Ibin/SDL_bin/include/ -Ibin/SDL_ttf_bin/include/ \
		-Ibin/SDL_image_bin/include/ -Ibin/tinyspline_bin/include\
		-I./src"

LINKING_FILE_PATHS="-L../bin/SDL_bin/lib/ -L../bin/SDL_ttf_bin/lib/ \
		-L../bin/SDL_image_bin/lib/ -L../bin/tinyspline_bin/lib64/"

LIB_NAMES="-lSDL3_image -lSDL3_ttf -lSDL3"

COMPILER_FLAGS="-Wall -Wextra -pedantic"

# People like colors
YELLOW='\033[1;33m'
GREEN='\033[1;32m'
BLUE='\033[1;34m'
NC='\033[0m'

WAIT="";
DONE="";

shopt -s expand_aliases
alias osleep=':'

# Yes, this is all kinda hardcoded, live with it.
echo -ne "${YELLOW}${WAIT} Removing old build.${NC}"
rm -rf build
osleep 1
echo -ne "\r${BLUE}${DONE} Removing old build.${NC}\n"


echo -ne "${YELLOW}${WAIT} Creating build directory.${NC}"
osleep 1
mkdir -p build/resources/font/Noto_Sans
mkdir build/op
mkdir build/lib
echo -ne "\r${BLUE}${DONE} Creating build directory.${NC}\n"


# Exit on error
set -e

echo -ne "${YELLOW}${WAIT} Copying necessary files.${NC}"
osleep 1

# TODO: Check on other systems if they accept the copied files, if not,
# create appropriate symlinks.
cp bin/SDL_bin/lib/*.so.0.2* build/lib
cp bin/SDL_ttf_bin/lib/*.so.0.2* build/lib
cp bin/SDL_image_bin/lib/*.so.0.2* build/lib

echo -ne "\r${BLUE}${DONE} Copying necessary files.${NC}\n"


echo -ne "${YELLOW}${WAIT} Creating links.${NC}"

ln -s '../../../../resources/font/Noto_Sans/NotoSans-Italic-VariableFont_wdth,wght.ttf' \
	build/resources/font/Noto_Sans/Default_Font.ttf

ln -s '../../resources/bootup.txt' \
	build/resources/bootup.txt

osleep 1
echo -ne "\r${BLUE}${DONE} Creating links.${NC}\n"


echo -ne "${YELLOW}${WAIT} Compiling resources.${NC}"

ld -r -b binary build/resources/font/Noto_Sans/Default_Font.ttf \
	-o build/resources/Default_Font.ttf.o

ld -r -b binary build/resources/bootup.txt \
	-o build/resources/bootup.o

osleep 1
echo -ne "\r${BLUE}${DONE} Compiling resources.${NC}\n"


# Needs nodejs
node print_todos.js
cd src
MOVUP=1 node op_gen.js
mv op/*.o ../build/op
cd ..

g++ -c src/gui/window.cpp \
	$INCLUDE_LIB_PATHS $COMPILER_FLAGS \
	-o build/window.o  >> build/compile_log 2>&1

g++ -c src/gui/HSButton.cpp \
	$INCLUDE_LIB_PATHS $COMPILER_FLAGS \
	-o build/HSButton.o  >> build/compile_log 2>&1

g++ -c src/main.cpp \
	$INCLUDE_LIB_PATHS $COMPILER_FLAGS \
	-o build/main.o  >> build/compile_log 2>&1

echo -ne "${YELLOW}${WAIT} Compiling executables.${NC}"

osleep 1
echo -ne "\r${BLUE}${DONE} Compiling executables.${NC}\n"


echo -ne "${YELLOW}${WAIT} Generating final executable${NC}"

cd build

g++ *.o resources/*.o op/*.o \
	-Wl,-rpath '$ORIGIN/lib' \
	../bin/tinyspline_bin/lib64/libtinyspline.a \
	$LINKING_FILE_PATHS $LIB_NAMES $COMPILER_FLAGS \
	-o test_build >> compile_log 2>&1

osleep 1
echo -ne "\r${BLUE}${DONE} Generating final executable${NC}\n"

exit
