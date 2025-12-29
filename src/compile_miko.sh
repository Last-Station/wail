gcc test.c -I./ -lsqlite `pkg-config --cflags --libs sdl3` -o test && ./test
