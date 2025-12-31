#include <stdint.h>

struct map_entity *map_center;

size_t map_width = 4096;
size_t map_height = 4096;

static SDL_FRect resize = {
	.x = (float) 0,
	.y = (float) 0,
	.w = (float) 32,
	.h = (float) 32
};

void load_textures(
	struct texture *textures,
	SDL_Renderer *renderer
){
	for(int i = 0; textures[i].name != NULL; i++){
		const char *path_format = "src/res/%s.%s";
		char path[2048] = { 0 };
		snprintf(path, 2048, path_format,
			textures[i].name,
			textures[i].type
		);

		textures[i].animation_data = IMG_LoadAnimation(
			path
		);

		printf("%lld\n", textures[i].animation_data);

		struct texture_animation *animation =
			(struct texture_animation *) malloc(
				sizeof(struct texture_animation)
			)
		;

		textures[i].animation = animation;
		animation->count = textures[i].animation_data->count;
		animation->frames = (SDL_Texture **) malloc(
			sizeof(SDL_Texture *) * (animation->count + 2)
		);

		SDL_Rect int_size = {
			.x = (int) 0,
			.y = (int) 0,
			.w = (int) 720,
			.h = (int) 720
		};

		SDL_Rect int_resize = {
			.x = (int) 0,
			.y = (int) 0,
			.w = (int) resize.w,
			.h = (int) resize.h
		};

		for(size_t j = 0; j < animation->count; j++){
			SDL_Surface *surface = textures[i]
				.animation_data
				->frames[j]
			;

			SDL_Surface *ns = SDL_CreateSurface(
				resize.w,
				resize.h,
				surface->format
			);

			SDL_BlitSurfaceScaled(surface, &int_size, ns,
				&int_resize,
				SDL_SCALEMODE_LINEAR
			);

			const SDL_PixelFormatDetails *details =
				SDL_GetPixelFormatDetails(ns->format);

			Uint32 white_pixel = SDL_MapRGB(
				details, NULL, 255, 255, 255
			);
			SDL_SetSurfaceColorKey(ns, true, white_pixel);

			animation->frames[j] = SDL_CreateTextureFromSurface(
				renderer, ns
			);

			SDL_DestroySurface(ns);
		}

		animation->frames[animation->count] = NULL;
	}
}
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

struct map_entity *map_entities_add(struct map *map,
	struct map_entity *entity
){
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

	return &map->entities[count];
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

int map_entity_position(struct map_entity *entity){

}

void map_entity_move(
	struct map_entity *entity,

	float dx,
	float dy,
	float dz
){
	entity->position->x += dx;
	entity->position->y += dy;
	entity->position->z += dz;
}

void map_entity_translate(
	struct map_entity *center,
	struct map_entity *entity,
	struct map_position *position
){
	position->x = entity->position->x - center->position->x;
	position->y = entity->position->y - center->position->y;
	position->z = entity->position->z - center->position->z;
}

void map_position_to_frect(
	struct map_position *position,
	SDL_FRect *frect,

	float width,
	float height
){
	frect->x = position->x;
	frect->y = position->y;
	frect->w = width;
	frect->h = height;
}

void ffpsnull(void *data){ return ; }
void (*fpsnull)(void *) = &ffpsnull;

/*void (**fps12flist)(void *) = { fpsnull };
void (**fps24flist)(void *) = { fpsnull };
void (**fps30flist)(void *) = { fpsnull };*/

struct fps_callback *fps12list = NULL;
struct fps_callback *fps24list = NULL;
struct fps_callback *fps30list = NULL;

uint64_t fps12last = 0;
uint64_t fps24last = 0;
uint64_t fps30last = 0;

void fps_push(struct fps_callback **list, void (*f)(void *), void *data){
	if(f == NULL)
		return ;

	size_t size = 0;
	while(list[0] != NULL && list[0][size].f != fpsnull){
		size++;
	}

	struct fps_callback *np = (struct fps_callback *) realloc(
		list[0],
		sizeof(struct fps_callback)*(size + 2)
	);

	if(np == NULL){
		exit(1); // TODO: Out of Memory
	}

	list[0] = np;
	list[0][size].f = f;
	list[0][size].data = data;
	list[0][size + 1].f = fpsnull;
}

void fps_call(struct fps_callback *list){
	size_t size = 0;
	while(list != NULL && list[size].f != fpsnull){
		list[size].f(list[size].data);

		size++;
	}
}

uint64_t fps_get_time(){
	return (uint64_t) SDL_GetTicks();
}

void fps12(void (*f)(void *), void *data){
	fps_push(&fps12list, f, data);

	if(f != NULL)
		return ;

	uint64_t now = fps_get_time();
	uint64_t ticks = (now - fps12last)/83;
	if(ticks > 0){
		for(uint64_t i = 0; i < ticks; i++)
			fps_call(fps12list);

		fps12last = now;
	}
}

void fps24(void (*f)(void *), void *data){
	return ;
}

void fps30(void (*f)(void *), void *data){
	return ;
}
