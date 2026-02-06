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
int MainWindow(
	void (*on_create)(void *data),
	void (*on_loop)(void *data),
	void (*on_event)(void *data)
);

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

#include <routine.h>
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

struct graphics *graphics = NULL;
unsigned int entity_new_count = 0;
struct map_entity *entity_new(struct map *map,
	unsigned int type
){
	printf("entity_new(): %i\n", type);

	if(!graphics){
		printf("entity_new(): GRAPHICS UNINITIALIZED\n");
		exit(1);
	}

	// TODO: MALLOC_ALERT
	struct animation *animation = (struct animation *) malloc(
		sizeof(struct animation)
	);

	struct map_entity entity = {
		.id = entity_new_count++,
		.type = type,
		.custom = 0,

		.animation = animation,
		.graphics = graphics,
		.directionX = 0,
		.directionY = 0,
		.movspd = 0,
	};

	struct map_entity *result = map_entities_add(map, &entity);
	struct entity_op_data data = {
		.entity = result
	};
	struct entity_op *op = &entity_op[type];
	data.entity->position->x = 0.0;
	data.entity->position->y = 0.0;

	if(op->on_new != NULL){
		op->on_new(&data);
	} else {
		printf("[NEW] Warning: Unhandled entity type (%i)\n", type);
	}

	//printf("POS %.2f %.2f\n", result->position->x, result->position->y);

	return result;
}

void (*start)();

void mapgen(){
	struct map_entity *slime = entity_new(&map, 101);
	struct map_entity *entity = entity_new(&map, 50);
	//map_center = entity;

	/*entity->position->x = 200;
	entity->position->y = 200;
	/*entity->destination->x = 0;
	entity->destination->y = 0;*/
	//entity->movspd = 1;
	entity->custom = 0;
	printf("MAPGEN\n");
}

int scrw = 720;
int scrh = 540;
int test(void);

// void *data is always of struct graphics type
// ONLY CALLED ONCE AFTER WINDOW CREATION
void on_create(void *data){
	// Set a logical size of 800x600
	// SDL_LOGICAL_PRESENTATION_LETTERBOX ensures the aspect ratio stays the same
	SDL_SetRenderLogicalPresentation(renderer, 1440, 1080,
		SDL_LOGICAL_PRESENTATION_LETTERBOX
	);
	/*SDL_SetRenderLogicalPresentation(renderer, 720, 540,
		SDL_LOGICAL_PRESENTATION_LETTERBOX
	);*/

	SDL_SetWindowSize(window, 720, 540);

	struct graphics *gdata = (struct graphics *) data;
	struct entity_op_data op_data = {
		.map = &map,
		.entity = NULL,
		.graphics = gdata
	};

	//printf("WCREATE\n");
	graphics = gdata;
	map_new(&map);
	mapgen();

	for(int i = 0; i < entity_op_size; i++){
		struct entity_op *op = &entity_op[i];
		if(op->on_init == NULL)
			continue ;

		op->on_init(&op_data);
	}

	fps12(&on_fps12, NULL, gdata);
	fps24(&on_fps24, NULL, gdata);
	fps30(&on_fps30, NULL, gdata);
	test();
}

void on_event(void *data){
	SDL_Event *event = (SDL_Event *) data;
	struct entity_op_data op_data = {
		.map = &map,
		.entity = NULL,
		.graphics = graphics,
		.event = event
	};

	//printf("WEVENT\n");
	for(int i = 0; i < entity_op_size; i++){
		struct entity_op *op = &entity_op[i];
		if(op->on_event == NULL)
			continue ;

		op->on_event(&op_data);
	}
}

int main(){
	ROUTINE_INCLUDE();
	OP_INCLUDE();

	// Print bootup "art"
	printf("%.*s", (int)txt_size, bootup_txt_data);

	if(MainWindow(on_create, on_loop, on_event) != 0) {
		printf("CLEANUP\n");
		cleanup();
		return 1;
	}

	return 0;
}

int test(void){
	struct chstruct user = {
		.map = "username 32; password 64; age 4; data 16",
		.data = (char *) malloc(96)
	};

	int age = 20;
	void *mptr = malloc(256);
	size_t len;
	char *entry = chstruct_ptrget(&user, "username", &len);
	printf("struct `user` -> username is %zu byte long\n", len);
	memcpy(entry, "DevMkay", 7);
	printf(" %.32s\n", entry);

	entry = chstruct_ptrget(&user, "password", &len);
	printf("struct `user` -> password is %zu byte long\n", len);
	memcpy(entry, "RottenPoisonousPotatoForThemToEat", 57 - 24);
	printf(" %.64s\n", entry);


	printf("Final:\n");
	entry = chstruct_ptrget(&user, "username", &len);
	printf(" %.32s\n", entry);
	entry = chstruct_ptrget(&user, "password", &len);
	printf(" %.64s\n", entry);
	entry = chstruct_ptrget(&user, "password", &len);
	printf("Total:\n ");
	fwrite(user.data, 1, 96, stdout);
	printf("\n");
	printf("Cross type checking\n");
	memcpy(entry, &mptr, 16);
	mptr = NULL;
	memcpy(&mptr, entry, 16);
	entry = chstruct_ptrget(&user, "data", &len);
	printf("Data Pointer -> %ull\n", mptr);
	free(mptr);
	printf("Data Pointer FREE OK\n", mptr);

	entry = chstruct_ptrget(&user, "age", &len);
	memcpy(entry, &age, 4);
	age = 99;

	memcpy(&age, entry, 4);
	printf("Age -> %i\n", age);

	return 0;
}
