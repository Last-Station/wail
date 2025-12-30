#!/bin/bash
# For now this custom build script should be good enough

# People like colors
YELLOW='\033[1;33m'
GREEN='\033[1;32m'
NC='\033[0m'

cd src
node op_gen.js
cd ..

# Yes, this is all kinda hardcoded, live with it.
echo -e "${YELLOW}Removing old build.${NC}"
rm -r build

echo -e "${YELLOW}Creating build directory.${NC}"
mkdir -p build/resources/font/Noto_Sans

# Exit on error
set -e

echo -e "${YELLOW}Creating file links.${NC}"

ln -s '../../../../resources/font/Noto_Sans/NotoSans-Italic-VariableFont_wdth,wght.ttf' \
    build/resources/font/Noto_Sans/Default_Font.ttf

echo -e "${YELLOW}Compiling resources.${NC}"

ld -r -b binary build/resources/font/Noto_Sans/Default_Font.ttf \
 -o build/resources/Default_Font.ttf.o

echo -e "${YELLOW}Compiling executables.${NC}"

g++ \
 -ISDL3/include/ -ISDL3_ttf/include/ -ISDL_image/include/ -lSDL3_image -lSDL3_ttf -lSDL3 \
 -I./src  -Wall -Wextra -pedantic \
 -c src/gui/window.cpp \
 -o build/window.o

g++ \
 -ISDL3/include/ -ISDL3_ttf/include/ -ISDL_image/include/ -lSDL3_image -lSDL3_ttf -lSDL3 \
  -I./src -c src/gui/HSButton.cpp -o build/HSButton.o \
 -Wall -Wextra -pedantic \

g++ -c src/main.cpp -I./src -o build/main.o \
 -Wall -Wextra -pedantic \
 -ISDL3/include/ -ISDL3_ttf/include/ -ISDL_image/include/ -lSDL3_image -lSDL3_ttf -lSDL3

echo -e "${YELLOW}Basically linking everything together at this point!${NC}"

cd build
ln -s ../src/res res

g++ *.o resources/*.o ../src/op/*.o -I ../src -o test_build \
 -ISDL3/include/ -ISDL3_ttf/include/ -ISDL_image/include/ -L../../sdl_img_build/ -lSDL3_image -lSDL3_ttf -lSDL3

echo -e "${GREEN}Done!${NC}"

exit
