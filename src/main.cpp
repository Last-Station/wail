#include <cstdio>
#include <cstdlib>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <threads.h>
#include <miko.h>
#include <miko.c>

// src/gui/MainWindow.cpp
int MainWindow(void (*on_create)(void *data), void (*on_loop)(void *data));

void cleanup() {
	TTF_Quit();
	SDL_Quit();
}

void on_loop(void *);

#define biome_forest 10

/*struct map_entity biome_forest = {
	.type = 10
};*/

struct map_entity entity_type_fox = {
	.type = 102
};

struct map map;
struct entity_op entity_op[entity_op_size] = {
	0
};

#include <op.h>

int on_tick_entity(struct map_entity *entity, struct graphics *graphics){
	struct entity_op f = entity_op[entity->type];
	if(f.on_tick == NULL){
		printf("[TICK] Warning: Unhandled entity type (%i)\n", entity->type);

		return 1;
	}

	struct entity_op_data data = {
		.map = &map,
		.entity = entity,
		.graphics = graphics
	};

	f.on_tick(&data);

	return 0;
}

int on_tick(struct graphics *graphics){
	size_t count = map_entities_len(&map);
	for(int i = 0; i < count; i++){
		on_tick_entity(&map.entities[i], graphics);
	}

	return 0;
}

void on_loop(void *data){
	struct graphics *gdata = (struct graphics *) data;
	on_tick(gdata);
}

unsigned int entity_new_count = 0;
struct map_entity *entity_new(struct map *map,
	unsigned int type
){
	struct map_entity entity = {
		.id = entity_new_count++,
		.type = type
	};

	struct entity_op_data data = {
		.entity = &entity
	};
	struct entity_op *op = &entity_op[type];
	if(op->on_new != NULL){
		op->on_new(&data);
	} else {
		printf("[NEW] Warning: Unhandled entity type (%i)\n", type);
	}

	return map_entities_add(map, &entity);
}

void (*start)();

void mapgen(){
	struct map_entity *entity = entity_new(&map, 101);
}

void on_create(void *data){
	struct graphics *gdata = (struct graphics *) data;
	struct entity_op_data op_data = {
		.map = &map,
		.entity = NULL,
		.graphics = gdata
	};

	for(int i = 0; i < entity_op_size; i++){
		struct entity_op *op = &entity_op[i];
		if(op->on_init == NULL)
			continue ;

		op->on_init(&op_data);
	}
}

int main(){/*
	IMG_Init(IMG_INIT_JPG
		| IMG_INIT_PNG
		| IMG_INIT_TIF
		| IMG_INIT_WEBP
		| IMG_INIT_JXL
		| IMG_INIT_AVIF
	);*/
	map_new(&map);

	OP_INCLUDE();

	int i = 0;/*
	for(i = 0; i < 32; i++){
		map_new(&map, biome_forest);
	}

	map_entities_unset(&map, 12);


	for(i = 0; i < map_entities_len(&map); i++){
		//printf("%i) %i\n", i, map.entities[i].type);
	}

	printf("  === DEFRAG ===\n");
	map_entities_defrag(&map);

	for(i = 0; i < map_entities_len(&map); i++){
		//printf("%i) %i\n", i, map.entities[i].type);
	}*/

	mapgen();

	if(MainWindow(on_create, on_loop) != 0) {
		cleanup();
		return 1;
	}

	return 0;
}
