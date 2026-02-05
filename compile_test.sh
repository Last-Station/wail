#!/bin/bash
# This is for dev only.

alias g++=g++ --driver-mode=g++ -Wall -Wextra -Waddress-of-packed-member -Wcast-align

# Variables
INCLUDE_LIB_PATHS="-Ibin/SDL_bin/include/ -Ibin/SDL_ttf_bin/include/ \
		-Ibin/SDL_image_bin/include/ -Ibin/tinyspline_bin/include\
		-I./src"

LINKING_FILE_PATHS="-L../bin/SDL_bin/lib/ -L../bin/SDL_ttf_bin/lib/ \
		-L../bin/SDL_image_bin/lib/"

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
mkdir build/routine
mkdir build/lib


echo -e "${YELLOW}Creating links and copying files.${NC}"

ln -s '../../../../src/res/font/Noto_Sans/NotoSans-Italic-VariableFont_wdth,wght.ttf' \
    build/resources/font/Noto_Sans/Default_Font.ttf

ln -s '../../src/res/bootup.txt' \
	build/resources/bootup.txt

# These can be ignored if they are installed on the system
set +e
cp -P bin/SDL_bin/lib/*.so* build/lib
cp -P bin/SDL_ttf_bin/lib/*.so* build/lib
cp -P bin/SDL_image_bin/lib/*.so* build/lib
set -e
# Exit on error re-enabled

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
mv routine/*.o ../build/routine
cd ..

g++ $INCLUDE_LIB_PATHS $LIB_NAMES $COMPILER_FLAGS \
	-c src/gui/window.cpp -o build/window.o

g++ $INCLUDE_LIB_PATHS $LIB_NAMES $COMPILER_FLAGS \
	-c src/gui/HSButton.cpp -o build/HSButton.o

g++ $INCLUDE_LIB_PATHS $LIB_NAMES $COMPILER_FLAGS \
	-c src/main.cpp -o build/main.o


echo -e "${YELLOW}Basically linking everything together at this point!${NC}"

cd build

USE_SYSLIB=0;
while getopts "s" opt; do
	case $opt in
	s)
		USE_SYSLIB=1
		;;
	\?)
		# Handle invalid options or missing arguments
		echo "Invalid option: -$OPTARG" >&2
		exit 1
		;;
	esac
done
echo "$USE_SYSLIB"

if [ "$USE_SYSLIB" -eq 1 ]; then
echo -e "${YELLOW}Using system lib (-s)"
g++ *.o resources/*.o op/*.o routine/*.o \
	-Wl,-rpath,'$ORIGIN/lib' -ltinyspline \
	$LINKING_FILE_PATHS $LIB_NAMES $COMPILER_FLAGS \
	-o test_build ;
else
# A rather dirty way to try to link the code in one way or try in another if the previous failed
# the difference is the presence or absence of ../bin/tinyspline_bin/lib64/libtinyspline.a
g++ *.o resources/*.o op/*.o routine/*.o \
	-Wl,-rpath,'$ORIGIN/lib' \
	../bin/tinyspline_bin/lib64/libtinyspline.a \
	$LINKING_FILE_PATHS $LIB_NAMES $COMPILER_FLAGS \
	-o test_build || \
{ echo -e "${YELLOW}Linking using provided libs failed, trying system libs!${NC}" ; \
g++ *.o resources/*.o op/*.o routine/*.o \
	-Wl,-rpath,'$ORIGIN/lib' -ltinyspline \
	$LINKING_FILE_PATHS $LIB_NAMES $COMPILER_FLAGS \
	-o test_build ; }
fi
echo -e "${GREEN}Done!${NC}"

exit
