set -e
mkdir -p ../build/miko_test/
gcc test.c -I./ -lsqlite `pkg-config --cflags --libs sdl3` -o test
mv test ../build/miko_test/
../build/miko_test/test
