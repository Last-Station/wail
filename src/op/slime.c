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


struct slime_data {
	size_t age;
	size_t ticks;
	size_t frame;
	double directionX;
	double directionY;
	double y;
	double x;
	double z;
	double xmov;
	struct map_entity target;
};

struct map_position dests[] = {
	{
		.x = 0,
		.y = 900,
		.z = 0
	}, {
		.x = 0,
		.y = 900,
		.z = 0
	}, {
		.x = 900,
		.y = 900,
		.z = 0
	}
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

	struct map_position *dest = &dests[0];

	struct map_position rest = {
		.x = 0,
		.y = 0,
		.z = 0
	};

	if(slime->frame > 49){
		map_entity_go(&fake, dest, &rest, 10);

		if(slime->x > rest.x)
			slime->directionX = (double) 1;
		else
			slime->directionX = (double) 0;

		slime->x += rest.x;
		slime->y += rest.y;
	} else if(slime->frame < 10){
		slime->xmov = ((double) (SDL_rand(20) + 10))/10;

		if(((dest->x - slime->x) < 100)
			&& ((dest->x - slime->x) > -100)
		){
			if(dest->x < 800){
				memcpy(&dests[0], &dests[2],
					sizeof(struct map_position)
				);
			} else {
				memcpy(&dests[0], &dests[1],
					sizeof(struct map_position)
				);
			}
		}
	}
}

static void on_fps30(struct entity_op_data *data){
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

	//entity->position->x = slime->x;
	//entity->position->y = slime->y;

	entity->animation->texture = &textures[0];
	entity->graphics->renderer = data->graphics->renderer;
	entity->animation->pos = cframe/10;
	if(slime->directionX == 0 && slime->frame > 4){
		entity->directionX = -1;
	} else {
		entity->directionX = 1;
	}

	map_entity_draw(entity);

	int width;
	int height;

	SDL_GetWindowSize(data->graphics->window, &width, &height);

	slime->frame = slime->age % 100;
	entity->position->x = slime->x;
	entity->position->y = slime->y;

	if(slime->frame >= 50){
		float ypos[2] = { 0, 0 };
		float progress = (((float)(50 - (100 - slime->frame)))/50.0f);
		if(progress > 1.0f)
			progress = 1.0f
		;
		timing(NULL, progress, ypos);
		entity->position->y = slime->y - ypos[0]/10.0f;

		slime->frame = 50;
	}

	fflush(stdout);
}

static void on_init(struct entity_op_data *data){
	SDL_Renderer *renderer = data->graphics->renderer;

	load_textures(textures, renderer);

	printf("INITIALIZED\n");
}

static void on_new(struct entity_op_data *data){
	struct map_entity *entity = data->entity;
	struct slime_data *slime =
		(struct slime_data *) malloc(sizeof(struct slime_data))
	;
	slime->x = 600;
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
	}
}
