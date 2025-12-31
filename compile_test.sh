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

LIB_NAMES="-lSDL3_image -lSDL3_ttf -lSDL3"

COMPILER_FLAGS="-Wall -Wextra -pedantic"

# People like colors
YELLOW='\033[1;33m'
GREEN='\033[1;32m'
NC='\033[0m'


# Yes, this is all kinda hardcoded, live with it.
echo -e "${YELLOW}Removing old build.${NC}"
rm -rf build


echo -e "${YELLOW}Creating build directory.${NC}"
mkdir -p build/resources/font/Noto_Sans
mkdir build/op


# Exit on error
set -e


echo -e "${YELLOW}Creating links.${NC}"

ln -s '../../../../resources/font/Noto_Sans/NotoSans-Italic-VariableFont_wdth,wght.ttf' \
    build/resources/font/Noto_Sans/Default_Font.ttf

ln -s ../src/res build/res


echo -e "${YELLOW}Compiling resources.${NC}"

ld -r -b binary build/resources/font/Noto_Sans/Default_Font.ttf \
 -o build/resources/Default_Font.ttf.o


echo -e "${YELLOW}Compiling executables.${NC}"

# THIS .JS IS FAILING ON PC (FIX/DELETE?)
# cd src
# node op_gen.js
# cd ..

g++ $INCLUDE_LIB_PATHS $LIB_NAMES $COMPILER_FLAGS \
	-c src/op/biome_forest.c -o build/op/biome_forest.o

g++ $INCLUDE_LIB_PATHS $LIB_NAMES $COMPILER_FLAGS \
	-c src/op/slime.c -o build/op/slime.o

g++ $INCLUDE_LIB_PATHS $LIB_NAMES $COMPILER_FLAGS \
	-c src/gui/window.cpp -o build/window.o

g++ $INCLUDE_LIB_PATHS $LIB_NAMES $COMPILER_FLAGS \
	-c src/gui/HSButton.cpp -o build/HSButton.o

g++ $INCLUDE_LIB_PATHS $LIB_NAMES $COMPILER_FLAGS \
	-c src/main.cpp -o build/main.o


echo -e "${YELLOW}Basically linking everything together at this point!${NC}"

cd build

g++ $LINKING_FILE_PATHS $LIB_NAMES $COMPILER_FLAGS \
	*.o resources/*.o op/*.o -o test_build


echo -e "${GREEN}Done!${NC}"

exit
