#!/bin/bash
# For now this custom build script should be good enough

# Variables

INCLUDE_LIB_PATHS="-Ibin/SDL_bin/include/ -Ibin/SDL_ttf_bin/include/ \
		-Ibin/SDL_image_bin/include/ \
			-Lbin/SDL_bin/lib/ -Lbin/SDL_ttf_bin/lib/ \
		-Lbin/SDL_image_bin/lib/ \
		-I./src"

LINKING_FILE_PATHS="-I../bin/SDL_bin/include/ -I../bin/SDL_ttf_bin/include/ \
		-I../bin/SDL_image_bin/include/ \
			-L../bin/SDL_bin/lib/ -L../bin/SDL_ttf_bin/lib/ \
		-L../bin/SDL_image_bin/lib/ \
		-I../src"

LIB_NAMES="-lSDL3_image -lSDL3_ttf -lSDL3 -ltinyspline"

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
echo -ne "\r${BLUE}${DONE} Creating build directory.${NC}\n"

# Exit on error
set -e


echo -ne "${YELLOW}${WAIT} Creating links.${NC}"
ln -s '../../../../resources/font/Noto_Sans/NotoSans-Italic-VariableFont_wdth,wght.ttf' \
    build/resources/font/Noto_Sans/Default_Font.ttf

ln -s ../src/res build/res

osleep 1
echo -ne "\r${BLUE}${DONE} Creating links.${NC}\n"
echo -ne "${YELLOW}${WAIT} Compiling resources.${NC}"

ld -r -b binary build/resources/font/Noto_Sans/Default_Font.ttf \
 -o build/resources/Default_Font.ttf.o

osleep 1
echo -ne "\r${BLUE}${DONE} Compiling resources.${NC}\n"

# THIS .JS IS FAILING ON PC (FIX/DELETE?)
# Run this after you installed nodejs -Miko
node print_todos.js
cd src
MOVUP=1 node op_gen.js
mv op/*.o ../build/op
#echo -e "$(cat bootup.txt)" > bootup
cd ..

# These 2 are handled by op_gen.js -Miko
# g++ $INCLUDE_LIB_PATHS $LIB_NAMES $COMPILER_FLAGS \
# 	-c src/op/biome_forest.c -o build/op/biome_forest.o

# g++ $INCLUDE_LIB_PATHS $LIB_NAMES $COMPILER_FLAGS \
#	 -c src/op/slime.c -o build/op/slime.o

g++ $INCLUDE_LIB_PATHS $LIB_NAMES $COMPILER_FLAGS \
	-c src/gui/window.cpp -o build/window.o  >> build/compile_log 2>&1

g++ $INCLUDE_LIB_PATHS $LIB_NAMES $COMPILER_FLAGS \
	-c src/gui/HSButton.cpp -o build/HSButton.o  >> build/compile_log 2>&1

g++ $INCLUDE_LIB_PATHS $LIB_NAMES $COMPILER_FLAGS \
	-c src/main.cpp -o build/main.o  >> build/compile_log 2>&1

echo -ne "${YELLOW}${WAIT} Compiling executables.${NC}"

osleep 1
echo -ne "\r${BLUE}${DONE} Compiling executables.${NC}\n"
echo -ne "${YELLOW}${WAIT} Generating final executable${NC}"

cd build

g++ $LINKING_FILE_PATHS $LIB_NAMES $COMPILER_FLAGS \
	*.o resources/*.o op/*.o -o test_build >> compile_log 2>&1

osleep 1
echo -ne "\r${BLUE}${DONE} Generating final executable${NC}\n"

exit
