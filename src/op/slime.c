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


static struct slime_data {
	size_t age;
	size_t ticks;
	size_t frame;
	double directionX;
	double directionY;
	double y;
	double x;
	double z;
	double xmov;
};

int c = 0;
static void on_fps12(struct entity_op_data *data){
	struct texture texture = textures[0];
	struct map_entity *entity = data->entity;
	struct slime_data *slime = (struct slime_data *) entity->data;
	int64_t directionX = slime->directionX == 0
		? -1
		: 1
	;

	double xmov = 4 * slime->xmov;
	struct map_entity fake;
	fake.position->x = slime->x;
	fake.position->y = slime->y;
	fake.position->z = slime->z;

	struct map_position dest = {
		.x = 100,
		.y = 200,
		.z = 0
	};

	struct map_position rest = {
		.x = 0,
		.y = 0,
		.z = 0
	};

	if(slime->frame > 49){
		/*if(slime->frame > 70){
			map_entity_move(entity, xmov * directionX, 0, 0);
		}

		map_entity_move(entity, (xmov/2) * directionX, 0, 0);*/
		map_entity_go(&fake, &dest, &rest, 10);
		//printf("[rest ] %f %f %f\n", rest.x, rest.y, rest.z);
		//printf("[slime] %f %f %f\n", slime->x, slime->y, slime->z);

		if(slime->x > rest.x)
			slime->directionX = (double) 1;
		else
			slime->directionX = (double) 0;

		/*slime->x = rest.x;
		slime->y = rest.y;
		slime->z = rest.z;*/
		slime->x += rest.x;
		slime->y += rest.y;
	} else if(slime->frame < 10){
		slime->xmov = ((double) (SDL_rand(20) + 10))/10;
	}
}

static void on_fps30(struct entity_op_data *data){
	//SDL_Renderer *renderer = data->graphics->renderer;
	struct map_entity *entity = data->entity;
	struct texture texture = textures[0];
	struct slime_data *slime = (struct slime_data *) entity->data;

	slime->age += 1;
}

static void on_tick(struct entity_op_data *data){
	struct map_entity *entity = data->entity;
	struct slime_data *slime = (struct slime_data *) entity->data;
	SDL_Renderer *renderer = data->graphics->renderer;
	struct texture texture = textures[0];
	size_t cframe = slime->frame;
	SDL_Texture *frame = texture.animation->frames[
		cframe/10
	];
	SDL_SetTextureBlendMode(frame, SDL_BLENDMODE_BLEND);

	struct SDL_FRect frect;
	map_position_to_frect(entity->position, &frect, 64, 64);

	if(slime->directionX == 0 && slime->frame > 4){
		SDL_RenderTextureRotated(renderer, frame, NULL, &frect, 0,
			NULL,
			SDL_FLIP_HORIZONTAL
		);
	} else {
		SDL_RenderTexture(renderer, frame, NULL, &frect);
	}

	int width;
	int height;

	SDL_GetWindowSize(data->graphics->window, &width, &height);

	slime->frame = slime->age % 100;
	// texture.animation->count
	/*if(slime->frame >= 5){
		if(slime->frame >= 8)
			entity->position->y += 4;

		if(slime->frame <= 7){
			entity->position->y -= 2;
		}

		entity->position->y -= 1;

		slime->frame = 5;
	}*/

	entity->position->x = slime->x;
	entity->position->y = slime->y;

	if(slime->frame >= 50){
		float ypos[2] = { 0, 0 };
		float progress = (((float)(50 - (100 - slime->frame)))/50.0f);
		if(progress > 1.0f)
			progress = 1.0f
		;
		timing(NULL, progress, ypos);
		//printf("%f %f\n", progress, ypos[0], ypos[1]);
		entity->position->y = slime->y - ypos[0]/10.0f;

		slime->frame = 50;
	}

	//printf("%i\r", counter);
	//fflush(stdout);


	fflush(stdout);
}

static void on_init(struct entity_op_data *data){
	SDL_Renderer *renderer = data->graphics->renderer;

	load_textures(textures, renderer);

	printf("INITIALIZED\n");
}

static void on_new(struct entity_op_data *data){
	//printf("%i CREATED\n", data->entity->type);
	struct map_entity *entity = data->entity;
	struct slime_data *slime =
		(struct slime_data *) malloc(sizeof(struct slime_data))
	;
	slime->x = 0;
	slime->y = 0;
	slime->age = 0;
	slime->directionX = 1;
	slime->directionY = 1;

	entity->data = (void *) slime;
}

void op_slime(){
	printf("EXISTS\n");

	entity_op[(unsigned int) 101] = (struct entity_op) {
		.on_tick = &on_tick,
		.on_init = &on_init,
		.on_new = &on_new,

		.on_fps12 = &on_fps12,
		.on_fps24 = NULL,
		.on_fps30 = &on_fps30
	};

	struct array_element *test = (struct array_element *)
		malloc(sizeof(struct array_element) * 64)
	;

	float pos[2] = { 0, 0 };
	for(float i = 0.0f; i < 1.0f; i += 0.01f){
		timing(NULL, i, pos);

		//printf("[ %f, %f ]\n", pos[0], pos[1]);
	}

	/*size_t i = 0;
	while(i < 63){
		test[i].data = (void *) "Hemlo";
		i++;
	}

	test[63].data = NULL;

	printf("Length: %zu\n", array_len(test));*/
}
