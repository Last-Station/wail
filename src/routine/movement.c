#include <cstdio>
#include <cstdlib>
#include <miko.h>
extern struct map map;
extern struct entity_op entity_op[entity_op_size];
static void on_tick(struct entity_op_data *data){

}

static void on_init(struct entity_op_data *data){
	size_t index = entity_new(&map, 3001);
	struct map_entity *entity = &map.entities[index];
	entity->position->x = 0;
	entity->position->y = 0;
	entity->custom = 1;
}

static void on_new(struct entity_op_data *data){

}

static void move(struct map_entity *entity){
	if(entity->custom > 0)
		return ;

	struct map_position *destination = entity->destination;
	struct map_position rest = {
		.x = 0,
		.y = 0,
		.z = 0
	};

	if(map_position_within(entity->position, destination, entity->movspd)){
		memcpy(entity->position,
			entity->destination,
			sizeof(struct map_position)
		);

		return ;
	}

	map_entity_go(entity, destination, &rest, entity->movspd);
	//printf("%i NIL %.2f %.2f\n", entity->type, rest.x, rest.y);
	//memcpy(entity->position, &rest, sizeof(struct map_position));
	entity->position->x += rest.x;
	entity->position->y += rest.y;
}

static int i = 0;
static void on_fps30(struct entity_op_data *data){
	if(i < 30){
		i++;

		return ;
	}

	if(!map.entities)
		return ;

	int i = 0;
	for(; map.entities[i].type != 0; i++){
		//printf(":%i\n", map.entities[i].type);
		move(&map.entities[i]);
	}
	//printf("%i\n", i);
}

void routine_movement(){
	//printf("ROUTINE_MOVE\n");
	entity_op[(unsigned int) 3001] = (struct entity_op) {
		.on_tick = &on_tick,
		.on_init = &on_init,
		.on_new = &on_new,

		.on_fps12 = NULL,
		.on_fps24 = NULL,
		.on_fps30 = &on_fps30
	};
}
