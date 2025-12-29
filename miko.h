#ifndef MIKO_H
#define MIKO_H

#include <sqlite3.h>

struct map_entity {
	int id;
	int type;

	struct map_entity *next;
	void *data;
};

struct map {
	struct map_entity *entities;
};

int map_new(struct map *map){
	memset(map, 0, sizeof(struct map));
}

int map_entities_add(struct map_entity *entity){

}

int map_entities_remove(struct map_entity *entity){

}

int map_entities_get(int index){

}

#endif
