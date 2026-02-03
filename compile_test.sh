#!/bin/bash
# This is for dev only.

# Variables
INCLUDE_LIB_PATHS="-Ibin/SDL_bin/include/ -Ibin/SDL_ttf_bin/include/ \
		-Ibin/SDL_image_bin/include/ -Ibin/tinyspline_bin/include\
		-I./src"

LINKING_FILE_PATHS="-L../bin/SDL_bin/lib/ -L../bin/SDL_ttf_bin/lib/ \
		-L../bin/SDL_image_bin/lib/ -L../bin/tinyspline_bin/lib64/"

LIB_NAMES="-lSDL3_image -lSDL3_ttf -lSDL3"

COMPILER_FLAGS="-std=c++20 -Wall -Wextra -pedantic"

# People like colors
YELLOW='\033[1;33m'
GREEN='\033[1;32m'
NC='\033[0m'

# Exit on error
set -e


# Yes, this is all kinda hardcoded, live with it.
echo -e "${YELLOW}Removing old build.${NC}"
rm -rf build


echo -e "${YELLOW}Creating build directory.${NC}"
mkdir -p build/resources/font/Noto_Sans
mkdir build/op
mkdir build/lib


echo -e "${YELLOW}Creating links and copying files.${NC}"

ln -s '../../../../resources/font/Noto_Sans/NotoSans-Italic-VariableFont_wdth,wght.ttf' \
    build/resources/font/Noto_Sans/Default_Font.ttf

ln -s '../../resources/bootup.txt' \
	build/resources/bootup.txt

# TODO: Check on other systems if they accept the copied files, if not,
# create appropriate symlinks.
cp bin/SDL_bin/lib/*.so.0.2* build/lib
cp bin/SDL_ttf_bin/lib/*.so.0.2* build/lib
cp bin/SDL_image_bin/lib/*.so.0.2* build/lib


echo -e "${YELLOW}Compiling resources.${NC}"

ld -r -b binary build/resources/font/Noto_Sans/Default_Font.ttf \
 -o build/resources/Default_Font.ttf.o

ld -r -b binary build/resources/bootup.txt \
	-o build/resources/bootup.o


echo -e "${YELLOW}Compiling executables.${NC}"

# Needs nodejs
cd src
MOVUP=1 node op_gen.js
mv op/*.o ../build/op
cd ..

g++ $INCLUDE_LIB_PATHS $LIB_NAMES $COMPILER_FLAGS \
	-c src/gui/window.cpp -o build/window.o

g++ $INCLUDE_LIB_PATHS $LIB_NAMES $COMPILER_FLAGS \
	-c src/gui/HSButton.cpp -o build/HSButton.o

g++ $INCLUDE_LIB_PATHS $LIB_NAMES $COMPILER_FLAGS \
	-c src/main.cpp -o build/main.o


echo -e "${YELLOW}Basically linking everything together at this point!${NC}"

cd build

g++ *.o resources/*.o op/*.o \
	-Wl,-rpath '$ORIGIN/lib' \
	../bin/tinyspline_bin/lib64/libtinyspline.a \
	$LINKING_FILE_PATHS $LIB_NAMES $COMPILER_FLAGS \
	-o test_build

echo -e "${GREEN}Done!${NC}"

exit
