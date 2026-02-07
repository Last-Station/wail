#include <cstdio>
#include <miko.h>
#include <SDL3/SDL.h>
extern struct map map;
extern struct entity_op entity_op[entity_op_size];
extern struct map_entity *map_center;
static void action_move(struct map_entity_action_data *data){

}


static struct map_entity_action actions[] = {
	{
		.name = "move",
		.call = &action_move
	}
};

static size_t camera_nil_index = 0;
static struct map_entity *camera_nil = NULL;
static void on_fps12(struct entity_op_data *data){
	//struct map_entity *entity = data->entity;
}

static void on_fps30(struct entity_op_data *data){
	//printf("CAMERA_FPS30\n");
}

static Uint32 held[256] = { 0 };
static void on_tick(struct entity_op_data *data){
	//struct map_entity *camera = data->entity;
	struct map_entity *camera = map_center;
	/*printf("CAMERA: %.2f %.2f\n",
		entity->position->x,
		entity->position->y
	);*/

// Inside your main loop
	/*const bool* state = SDL_GetKeyboardState(NULL);
	if(state[SDL_SCANCODE_A]) {
		camera->position->x -= 20;
	}
	else if(state[SDL_SCANCODE_S]) {
		camera->position->y += 20;
	}
	else if(state[SDL_SCANCODE_W]) {
		camera->position->y -= 20;
	}
	else if(state[SDL_SCANCODE_D]) {
		camera->position->x += 20;
	}*/

	//printf("%ull\n", camera_nil);

	camera_nil = &map.entities[camera_nil_index];
	if(camera_nil != NULL){
		double tx = camera->position->x - (32/2);
		double ty = camera->position->y - (32/2);
		camera_nil->destination->x = tx;
		camera_nil->destination->y = ty;

		/*
		printf("Camnil %.2f %.2f\n",
			camera_nil->position->x,
			camera_nil->position->y
		);

		printf("Camera %.2f %.2f\n",
			camera->position->x,
			camera->position->y
		);*/

		/*camera_nil->position->x = 500;
		camera_nil->position->y = 500;
		camera_nil->destination->x = 500;
		camera_nil->destination->y = 500;*/\
		/*
		camera_nil->position->x = 500.0;
		camera_nil->position->y = 500.0;
		camera_nil->destination->x = 500.0;
		camera_nil->destination->y = 500.0;*/
	}


	for(size_t i = 0; held[i] != 0; i++){
		//printf("(%i -> %i) %i]", held[i], SDLK_A, held[i] == SDLK_A);
		if(held[i] == (Uint32) SDLK_A){
			camera->position->x -= 20;
		}
		else if(held[i] == (Uint32) SDLK_S){
			camera->position->y += 20;
		}
		else if(held[i] == (Uint32) SDLK_W){
			camera->position->y -= 20;
		}
		else if(held[i] == (Uint32)SDLK_D){
			camera->position->x += 20;
		} else {
			continue ;
		}
		//printf("%i ", held[i]);
		//fflush(stdout);
	}
}

static void on_init(struct entity_op_data *data){
	printf("CAMERA_INIT\n");
	camera_nil_index = entity_new(&map, 201);
	camera_nil = &map.entities[camera_nil_index];
	camera_nil->position->x = 0;
	camera_nil->position->y = 0;
	camera_nil->destination->x = 0;
	camera_nil->destination->y = 0;
	camera_nil->movspd = 19.5*3.0;

}

static void on_new(struct entity_op_data *data){
	struct map_entity *entity = data->entity;
	map_center = entity;
	entity->directionX = 1;
	entity->directionY = 1;
	entity->animation->pos = 0;

	entity->position->x = 0;
	entity->position->y = 0;

	entity->destination->x = 0;
	entity->destination->y = 0;
	entity->movspd = 100;

	entity->data = NULL;
}

static void on_event(struct entity_op_data *data){
	SDL_Event *event = data->event;
	struct map_entity *camera = map_center;
	size_t empty = 0;
	if((event->type != SDL_EVENT_KEY_UP)
		&& (event->type != SDL_EVENT_KEY_DOWN)
	){
		return ;
	}

	while(held[empty] != 0){
		if(held[empty] == event->key.key){
			if(event->type == SDL_EVENT_KEY_UP){
				held[empty] = 1;
			} else {
				return ;
			}
		}

		empty++;
	}

	empty = 0;

	while(held[empty] > 2){
		empty++;
	}

	if(event->type == SDL_EVENT_KEY_DOWN){
		held[empty] = event->key.key;

		//printf("DOWN [%.4s\n", (char *)(&event->key));
		//const char *keyName = SDL_GetKeyName(event->key.key);
		//printf("DOWN [%s]\n", keyName);
		/*if(event->key.key == SDLK_A){
			camera->position->x -= 20;
		}
		else if(event->key.key == SDLK_S){
			camera->position->y += 20;
		}
		else if(event->key.key == SDLK_W){
			camera->position->y -= 20;
		}
		else if(event->key.key == SDLK_D){
			camera->position->x += 20;
		}*/
	}
}

void op_camera(){
	entity_op[(unsigned int) 50] = (struct entity_op) {
		.on_tick = &on_tick,
		.on_init = &on_init,
		.on_new = &on_new,
		.on_event = &on_event,

		.on_fps12 = &on_fps12,
		.on_fps24 = NULL,
		.on_fps30 = &on_fps30
	};
}
