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

static void on_fps12(struct entity_op_data *data){
	//struct map_entity *entity = data->entity;
}

static void on_fps30(struct entity_op_data *data){
	//printf("CAMERA_FPS30\n");
}

static void on_tick(struct entity_op_data *data){
	//printf("CAMERA_TICK\n");
	struct map_entity *entity = data->entity;
	/*printf("CAMERA: %.2f %.2f\n",
		entity->position->x,
		entity->position->y
	);*/
}

static void on_init(struct entity_op_data *data){
	//printf("CAMERA_INIT\n");
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
	if(event->type == SDL_EVENT_KEY_DOWN){
		//printf("DOWN [%.4s\n", (char *)(&event->key));
		//const char *keyName = SDL_GetKeyName(event->key.key);
		//printf("DOWN [%s]\n", keyName);
		if(event->key.key == SDLK_A){
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
		}
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
