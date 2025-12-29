#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <threads.h>
#include <miko.h>
#include <op.h>

// src/gui/MainWindow.cpp
int MainWindow(void (*)(void *));

void cleanup() {
	TTF_Quit();
	SDL_Quit();
}

void on_loop(void *);

#define entity_op_size 1028

struct map_entity biome_forest = {
	.type = 10
};

struct map_entity entity_type_fox = {
	.type = 102
};

struct map map;

int main(){ // for testing only (TODO)
	map_new(&map);

	OP_INCLUDE();

	for(int i = 0; i < 32; i++){
		map_entities_add(&map, &entity_type_fox);
	}

	map_entities_unset(&map, 12);


	for(int i = 0; i < map_entities_len(&map); i++){
		printf("%i) %i\n", i, map.entities[i].type);
	}

	printf("  === DEFRAG ===\n");
	map_entities_defrag(&map);

	for(int i = 0; i < map_entities_len(&map); i++){
		printf("%i) %i\n", i, map.entities[i].type);
	}

	if(MainWindow(on_loop) != 0) {
		cleanup();
		return 1;
	}

	return 0;
}

void (*entity_op[entity_op_size])(struct map, struct map_entity) = {
	0
};

int on_entity(struct map_entity *entity){
	return 0;
}

int on_tick(){
	size_t count = map_entities_len(&map);
	printf("%i\n", count);
	for(int i = 0; i < count; i++){
		on_entity(&map.entities[i]);
	}

	printf("tick\n");

	return 0;
}

void on_loop(void *data){
	on_tick();
}
