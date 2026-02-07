#include <cstdio>
#include <miko.h>
extern struct map map;
extern struct map_entity *map_center;
extern struct entity_op entity_op[entity_op_size];
static void action_move(struct map_entity_action_data *data){

}

static struct map_entity_action actions[] = {
	{
		.name = "move",
		.call = &action_move
	}
};

static struct texture textures[] = {
	{
		.name = "nil",
		.type = "gif"
	}, {
		.name = NULL
	}
};

static void on_fps12(struct entity_op_data *data){
	struct map_entity *entity = data->entity;
	entity->animation->pos++;
}

static void on_fps30(struct entity_op_data *data){

}

static void on_tick(struct entity_op_data *data){
	struct map_entity *entity = data->entity;
	SDL_Renderer *renderer = data->graphics->renderer;

	/*entity->destination->x = map_center->position->x;
	entity->destination->y = map_center->position->y;*/

	entity->animation->texture = &textures[0];
	entity->graphics->renderer = data->graphics->renderer;

	map_entity_draw(entity);
}

static void on_init(struct entity_op_data *data){
	SDL_Renderer *renderer = data->graphics->renderer;
	load_textures(textures, renderer);

	//struct map_entity *entity = entity_new(&map, 201);
}

static void on_new(struct entity_op_data *data){
	struct map_entity *entity = data->entity;
	entity->directionX = 1;
	entity->directionY = 1;
	entity->animation->pos = 0;

	entity->position->x = 0;
	entity->position->y = 0;

	entity->movspd = 10;

	entity->data = NULL;
}

void op_nil(){
	entity_op[(unsigned int) 201] = (struct entity_op) {
		.on_tick = &on_tick,
		.on_init = &on_init,
		.on_new = &on_new,

		.on_fps12 = &on_fps12,
		.on_fps24 = NULL,
		.on_fps30 = &on_fps30
	};
}
