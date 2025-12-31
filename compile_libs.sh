#!/bin/bash
# For now this custom build script should be good enough

# People like colors
YELLOW='\033[1;33m'
GREEN='\033[1;32m'
NC='\033[0m'

# Exit on error
set -e

# Yes, this is all kinda hardcoded, live with it.
echo -e "${YELLOW}Removing old bin.${NC}"
rm -rf bin

echo -e "${YELLOW}Creating bin directory.${NC}"
mkdir bin
mkdir bin/SDL_build
mkdir bin/SDL_ttf_build
mkdir bin/SDL_image_build

mkdir bin/SDL_bin && cd bin/SDL_bin
SDL_INSTALL_DIR="$(pwd)"
cd ../../
mkdir bin/SDL_ttf_bin && cd bin/SDL_ttf_bin
SDL_ttf_INSTALL_DIR="$(pwd)"
cd ../../
mkdir bin/SDL_image_bin && cd bin/SDL_image_bin
SDL_image_INSTALL_DIR="$(pwd)"
cd ../../

echo -e "${YELLOW}Compiling SDL.${NC}"
cd bin/SDL_build
cmake --install-prefix $SDL_INSTALL_DIR ../../SDL
make -j`nproc`
make install
cd ../../
rm -rf bin/SDL_build

echo -e "${YELLOW}Compiling SDL_ttf.${NC}"
cd bin/SDL_ttf_build
cmake --install-prefix $SDL_ttf_INSTALL_DIR ../../SDL_ttf
make -j`nproc`
make install
cd ../../
rm -rf bin/SDL_ttf_build

echo -e "${YELLOW}Compiling SDL_image.${NC}"
cd bin/SDL_image_build
cmake --install-prefix $SDL_image_INSTALL_DIR ../../SDL_image
make -j`nproc`
make install
cd ../../
rm -rf bin/SDL_image_build

echo -e "${GREEN}Done!${NC}"

exit
