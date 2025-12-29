#!/bin/bash
# For now this custom build script should be good enough

# People like colors
YELLOW='\033[1;33m'
GREEN='\033[1;32m'
NC='\033[0m'

# Yes, this is all kinda hardcoded, live with it.
echo -e "${YELLOW}Removing old build.${NC}"
rm -r build

echo -e "${YELLOW}Creating build directory.${NC}"
mkdir build
mkdir build/resources

# Exit on error
set -e

echo -e "${YELLOW}Compiling resources.${NC}"

# Put resources here TODO

echo -e "${YELLOW}Compiling executables.${NC}"

# Put src files here TODO

echo -e "${YELLOW}Basically linking everything together at this point!${NC}"

cd build

# TODO

echo -e "${GREEN}Done!${NC}"

exit
