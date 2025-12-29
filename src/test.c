#include <stdlib.h>
#include <threads.h>
#include <miko.h>
#include <op.h>

#define entity_op_size 1028

struct map_entity biome_forest = {
	.type = 10
};

struct map_entity entity_type_fox = {
	.type = 102
};

struct map map;

void (*entity_op[entity_op_size])(struct map, struct map_entity) = {
	0
};

int on_entity(struct map_entity *entity){

}

int on_tick(){
	size_t count = map_entities_len(&map);
	for(int i = 0; i < count; i++){
		on_entity(&map.entities[i]);
	}

	printf("tick\n");
}

int test2(){
	OP_INCLUDE();

	while(1){
		//on_tick();
	}
}

int main(int argc, char **argv){
	map_new(&map);

	for(int i = 0; i < 32; i++){
		map_entities_add(&map, &entity_type_fox);
	}

	map_entities_unset(&map, 12);

	for(int i = 0; i < map_entities_len(&map); i++){
		printf("%i) %i\n", i, map.entities[i].type);
	}

	printf("  === DEFRAG ===\n");
	map_entities_defrag(&map);

	for(int i = 0; i < map_entities_len(&map); i++){
		printf("%i) %i\n", i, map.entities[i].type);
	}

	test2();

	return 0;
}
