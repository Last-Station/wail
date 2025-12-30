#include <cstdio>
#include <miko.h>
extern struct entity_op entity_op[entity_op_size];

struct biome_forest_data {

};

static struct map_entity_action actions[] = {
	{
		.name = "move"
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

static int counter = 0;
static int y = 0;
static void on_tick(struct entity_op_data *data){
	SDL_Renderer *renderer = data->graphics->renderer;

	/*SDL_SetRenderDrawColor(renderer, counter, y, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderPoint(renderer, 0 + counter, y);
	SDL_RenderPoint(renderer, 0 + counter + 1, y);
	SDL_RenderPoint(renderer, 0 + counter + 2, y);
	SDL_RenderPoint(renderer, 0 + counter + 3, y);
	SDL_RenderPoint(renderer, 0 + counter, y + 1);
	SDL_RenderPoint(renderer, 0 + counter + 1, y + 1);
	SDL_RenderPoint(renderer, 0 + counter + 2, y + 1);
	SDL_RenderPoint(renderer, 0 + counter + 3, y + 1);*/

	counter += 3;

	struct texture slime = textures[0];
	SDL_Texture *frame = slime.animation->frames[
		(counter / 64) % slime.animation->count
	];
	SDL_SetTextureBlendMode(frame, SDL_BLENDMODE_BLEND);

	struct SDL_FRect resize = {
		.x = (float) 0 + counter + 3,
		.y = (float) y + 1,
		.w = (float) 32,
		.h = (float) 32
	};

	//printf("%i\n", (counter / 64) % slime.animation->count);
	SDL_RenderTexture(renderer, frame, NULL, &resize);
	int width;
	int height;

	SDL_GetWindowSize(data->graphics->window, &width, &height);
	if(counter > width){
		counter = 0;
		y += 2;
	}

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

	//struct map_entity *entity = data->entity;
}

void op_biome_forest(){
	printf("EXISTS\n");

	entity_op[(unsigned int) 10] = (struct entity_op) {
		.on_tick = &on_tick,
		.on_init = &on_init,
		.on_new = &on_new
	};
}
