#ifndef MIKO_H
#define MIKO_H
#define POINTER_UNITIALIZED ((void *)0x144)
#define MAP_ENTITY_NULL 0
#define MAP_ENTITY_UNSET 1

#include <sqlite3.h>
#include <threads.h>
#include <SDL3/SDL.h>

struct map_entity {
	unsigned int id;
	unsigned int type;

	struct map_entity *next;
	void *data;
};

struct map {
	struct map_entity *entities;
};

int map_new(struct map *map){
	memset(map, 0, sizeof(struct map));

	return 0;
}

size_t map_entities_len(struct map *map){
	if(map->entities == NULL){
		return 0;
	}

	size_t i = 0;
	while(map->entities[i].type != 0){
		i++;
	}

	return i;
}

int map_entities_add(struct map *map, struct map_entity *entity){
	void *nalloc = POINTER_UNITIALIZED;
	size_t count = 0;
	for(;
		(map->entities != NULL)
			&& (map->entities[count].type != MAP_ENTITY_NULL);
	count++){
		if(map->entities[count].type == 1){
			break ;
		}
	}

	nalloc = realloc(map->entities,
		(sizeof(struct map_entity) * (count + 2))
	);
	if(nalloc == NULL){
		exit(1); //TODO: OUT OF MEMORY
	}
	map->entities = (struct map_entity *) nalloc;
	memcpy(&map->entities[count], entity, sizeof(struct map_entity));
	map->entities[count + 1].type = 0;

	return 0;
}

int map_entities_unset(struct map *map, size_t index){
	if(map->entities == NULL){
		return -1;
	}

	size_t count = map_entities_len(map);
	if(index > count)
		return -1;

	map->entities[index].type = MAP_ENTITY_UNSET;

	return 0;
}

int map_entities_defrag(struct map *map){
	if(map->entities == NULL)
		return -1;

	size_t count = map_entities_len(map);
	struct map_entity *copy = (struct map_entity *)
		malloc(sizeof(struct map_entity)*count)
	;
	size_t new_size = 0;
	size_t position = 0;
	while(map->entities[position].type != 0){
		if(map->entities[position].type == 1){
			position++;

			continue ;
		}

		memcpy(&copy[new_size],
			&map->entities[position],
			sizeof(struct map_entity)
		);

		position++;
		new_size++;
	}

	copy[new_size].type = 0;

	new_size++;
	copy = (struct map_entity *)
		realloc(copy, sizeof(struct map_entity)*new_size);
	map->entities = copy;

	return 0;
}

struct map_entity *map_entities_get(struct map *map, size_t index){
	if(map->entities == NULL){
		return NULL;
	}

	size_t count = map_entities_len(map);
	if(index > count)
		return NULL;

	return &map->entities[index];
}

struct animation {
	SDL_Surface **frames;
};

#endif
