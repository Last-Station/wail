#include <cstdio>
#include <cstdlib>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <threads.h>
#include <miko.h>
#include <miko.c>


// src in resources/bootup.txt, symlinked into build dir
extern char bootup_txt_data[] asm("_binary_build_resources_bootup_txt_start");
extern char bootup_txt_end[] asm("_binary_build_resources_bootup_txt_end");
extern char bootup_txt_size[] asm("_binary_build_resources_bootup_txt_size");
size_t txt_size = (size_t)(bootup_txt_size);


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

int entity_op_trigger(
	void (*handle)(struct entity_op_data *),
	struct map_entity *entity,
	struct graphics *graphics
){
	struct entity_op_data data = {
		.map = &map,
		.entity = entity,
		.graphics = graphics
	};

	handle(&data);

	return 0;
}

int on_tick_entity(struct map_entity *entity, struct graphics *graphics){
	struct entity_op f = entity_op[entity->type];
	if(f.on_tick == NULL){
		printf("Warning: Unhandled entity type (%i)\n", entity->type);

		return 1;
	}

	entity_op_trigger(f.on_tick, entity, graphics);

	return 0;
}

int on_fps12_entity(struct map_entity *entity, struct graphics *graphics){
	struct entity_op f = entity_op[entity->type];
	if(f.on_fps12 == NULL){
		return 1;
	}

	entity_op_trigger(f.on_fps12, entity, graphics);

	return 0;
}

void on_fps12(void *user, void *data){
	struct graphics *graphics = (struct graphics *) data;
	size_t count = map_entities_len(&map);
	for(int i = 0; i < count; i++){
		on_fps12_entity(&map.entities[i], graphics);
	}
}

int on_fps24_entity(struct map_entity *entity, struct graphics *graphics){
	struct entity_op f = entity_op[entity->type];
	if(f.on_fps24 == NULL){
		return 1;
	}

	entity_op_trigger(f.on_fps24, entity, graphics);

	return 0;
}

void on_fps24(void *user, void *data){
	struct graphics *graphics = (struct graphics *) data;
	size_t count = map_entities_len(&map);
	for(int i = 0; i < count; i++){
		on_fps24_entity(&map.entities[i], graphics);
	}
}

int on_fps30_entity(struct map_entity *entity, struct graphics *graphics){
	struct entity_op f = entity_op[entity->type];
	if(f.on_fps30 == NULL){
		return 1;
	}

	entity_op_trigger(f.on_fps30, entity, graphics);

	return 0;
}

void on_fps30(void *user, void *data){
	struct graphics *graphics = (struct graphics *) data;
	size_t count = map_entities_len(&map);
	for(int i = 0; i < count; i++){
		on_fps30_entity(&map.entities[i], graphics);
	}
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
	fps12(NULL, NULL, NULL);
	fps24(NULL, NULL, NULL);
	fps30(NULL, NULL, NULL);
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

	fps12(&on_fps12, NULL, gdata);
	fps24(&on_fps24, NULL, gdata);
	fps30(&on_fps30, NULL, gdata);
}


int main(){
	map_new(&map);

	OP_INCLUDE();

	// Print bootup "art"
	printf("%.*s", (int)txt_size, bootup_txt_data);

	mapgen();

	if(MainWindow(on_create, on_loop) != 0) {
		cleanup();
		return 1;
	}

	return 0;
}
