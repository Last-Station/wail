#ifndef MIKO_H
#define MIKO_H
#define POINTER_UNITIALIZED ((void *)0x144)
#define MAP_ENTITY_NULL 0
#define MAP_ENTITY_UNSET 1
#define OUT_OF_MEMORY 2

#include <cstdlib>
#include <tinyspline.h>
#include <sqlite3.h>
#include <threads.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

extern SDL_Window *	window;
extern SDL_Renderer *	renderer;
extern SDL_Texture *	texture;
extern SDL_Surface *	surface;
extern SDL_IOStream *	StarupImgIO;
extern TTF_Font *	font;
extern SDL_Texture *	TextTexture;
extern SDL_IOStream *	fontIO;

#define entity_op_size 1028

struct graphics {
	SDL_Window *window;
	SDL_Renderer *renderer;
};

struct entity_op_data {
	struct map *map;
	struct map_entity *entity;
	struct graphics *graphics;
};

struct entity_op {
	void (*on_tick)(struct entity_op_data *);
	void (*on_init)(struct entity_op_data *);
	void (*on_new)(struct entity_op_data *);

	void (*on_fps12)(struct entity_op_data *);
	void (*on_fps24)(struct entity_op_data *);
	void (*on_fps30)(struct entity_op_data *);
};

struct texture_animation {
	size_t count;

	SDL_Texture **frames;
};

struct texture {
	char *name;
	char *type;
	SDL_Texture *texture;
	struct texture_animation *animation;
	IMG_Animation *animation_data;
};

void load_textures(
	struct texture *,
	SDL_Renderer *
);
struct animation {
	struct texture *texture;
	size_t pos;
};

struct map_position {
	double x;
	double y;
	double z;
};

struct map_entity_action_data {
	char *data;
};

struct map_entity_action {
	char *name;
	void (*call)(struct map_entity_action_data *);
};

struct map_entity {
	unsigned int id;
	unsigned int type;

	struct map_entity *next;
	struct map_entity_action *actions;
	struct map_position position[1];
	struct map_position destination[1];
	struct animation *animation;
	struct graphics *graphics;
	int64_t directionX;
	int64_t directionY;

	void *data;
};

/*struct map_entity_base {
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
};*/

struct map {
	struct map_entity *entities;
};

struct fps_callback {
	void (*f)(void *, void *);
	void *user;
	void *data;
};

int map_new(struct map *);
size_t map_entities_len(struct map *);
struct map_entity *map_entities_add(struct map *, struct map_entity *);
int map_entities_unset(struct map *, size_t);
int map_entities_defrag(struct map *);
struct map_entity *map_entities_get(struct map *, size_t);
int map_entity_position(struct map_entity *entity);
void map_entity_move(
	struct map_entity *, float, float, float
);
void map_entity_draw(
	struct map_entity *
);

void map_entity_translate(
	struct map_entity *, struct map_entity *, struct map_position *
);

void map_position_to_frect(struct map_position *, SDL_FRect *, float, float);
unsigned long long get_time();

uint64_t fps_get_time();
void fps_call(struct fps_callback *);
void fps_push(struct fps_callback **, void (*)(void *), void *);

void fps12(void (*)(void *, void *), void *, void *);
void fps24(void (*)(void *, void *), void *, void *);
void fps30(void (*)(void *, void *), void *, void *);

struct array_element {
	void *data;
};

size_t array_len(struct array_element *);
void array_push(struct array_element **, struct array_element *);

void timing(tsBSpline *spline, float pos, float *result);
void xyz_to_degrad(double, double, double, double *);
void degrad_to_xyz(double, double, double, double *);
void map_entity_go(struct map_entity *,
	struct map_position *,
	struct map_position *,
	double
);

#endif
