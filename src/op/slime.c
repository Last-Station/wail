#include <cstdio>
#include <miko.h>
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
	int cframe = (counter / 32) % slime.animation->count;
	SDL_Texture *frame = slime.animation->frames[
		cframe
	];
	SDL_SetTextureBlendMode(frame, SDL_BLENDMODE_BLEND);

	if(cframe > 4)
		map_entity_move(entity, 2, 0, 0);

	struct SDL_FRect frect;
	map_position_to_frect(entity->position, &frect, 64, 64);

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

static void op_fps12(void *data){
	struct map_entity *entity = (struct map_entity *) data;

	printf("%ull -> %ull                  \r", entity->id, SDL_GetTicks());
	fflush(stdout);
}

static void on_new(struct entity_op_data *data){
	printf("%i CREATED\n", data->entity->type);
	struct map_entity *entity = data->entity;

	entity->position->x = 0;
	entity->position->y = 0;
	entity->position->z = 0;

	fps12(&op_fps12, data->entity);
}

void op_slime(){
	printf("EXISTS\n");

	entity_op[(unsigned int) 101] = (struct entity_op) {
		.on_tick = &on_tick,
		.on_init = &on_init,
		.on_new = &on_new
	};
}
