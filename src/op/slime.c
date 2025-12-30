#include <cstdio>
#include <miko.h>
extern struct entity_op entity_op[entity_op_size];
struct biome_forest_data {

};

static void action_move(void *data){

}

static struct map_entity_action actions[] = {
	{
		.name = "move",
		.call = &action_move
	}
};

static struct texture textures[] = {
	{
		.name = "slime",
		.type = "gif"
	}, {
		.name = NULL
	}
};

int counter = 0;
static void on_tick(struct entity_op_data *data){
	struct map_entity *entity = data->entity;
	SDL_Renderer *renderer = data->graphics->renderer;
	struct texture slime = textures[0];
	SDL_Texture *frame = slime.animation->frames[
		(counter / 64) % slime.animation->count
	];
	SDL_SetTextureBlendMode(frame, SDL_BLENDMODE_BLEND);

	map_entity_move(entity, 5, 0, 0);

	struct SDL_FRect frect;
	map_position_to_frect(entity->position, &frect, 32, 32);

	//printf("%i\n", (counter / 64) % slime.animation->count);
	SDL_RenderTexture(renderer, frame, NULL, &frect);
	int width;
	int height;

	SDL_GetWindowSize(data->graphics->window, &width, &height);

	counter++;

	//printf("%i\r", counter);
	//fflush(stdout);
}

static void on_init(struct entity_op_data *data){
	SDL_Renderer *renderer = data->graphics->renderer;

	load_textures(textures, renderer);

	printf("INITIALIZED\n");
}

static void on_new(struct entity_op_data *data){
	printf("%i CREATED\n", data->entity->type);
	struct map_entity *entity = data->entity;

	entity->position->x = 0;
	entity->position->y = 0;
	entity->position->z = 0;
}

void op_slime(){
	printf("EXISTS\n");

	entity_op[(unsigned int) 101] = (struct entity_op) {
		.on_tick = &on_tick,
		.on_init = &on_init,
		.on_new = &on_new
	};
}
