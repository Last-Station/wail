#include <stdint.h>
#include <math.h>

struct map_entity *map_center = NULL;
size_t map_width = 4096;
size_t map_height = 4096;

static SDL_FRect resize = {
	.x = (float) 0,
	.y = (float) 0,
	.w = (float) 64,
	.h = (float) 64
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
		//printf("%s\n", path);

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

		size_t j = 0;
		for(; j < animation->count; j++){
			//printf("[%i] adat -> %ull\n", i, animation->frames);
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

		animation->frames[j] = NULL;
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

/*struct map_entity **/
size_t map_entities_add(struct map *map,
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

	//return &map->entities[count];
	return count;
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

void degrad_to_xyz(
	double radius,
	double theta,
	double phi,
	double *output
){
	double theta_rad = theta * M_PI / 180.0;
	double phi_rad = phi * M_PI / 180.0;

	output[0] = radius * sin(phi_rad) * cos(theta_rad);
	output[1] = radius * sin(phi_rad) * sin(theta_rad);
	output[2] = radius * cos(phi_rad);
}

void xyz_to_degrad(
	double x,
	double y,
	double z,

	double *output
){
	double radius = sqrt((x * x) + (y * y) + (z * z));
	if(radius == 0){
		output[0] = output[1] = output[2] = 0.0;

		return ;
	}

	double theta = atan2(y, x);
	double phi = acos(z / radius);

	output[0] = radius;
	output[1] = theta * (180.0 / M_PI);
	output[2] = phi * (180.0 / M_PI);
}

/*
	Go `dist` distance to dx, dy, dz
*/

void map_entity_go_legacy(struct map_entity *entity,
	struct map_position *position,
	struct map_position *result,
	double dist
){
	double px = (double) position->x;
	double py = (double) position->y;
	double pz = (double) position->z;

	px = px - entity->position->x;
	py = py - entity->position->y;
	pz = pz - entity->position->z;

	double out[3];
	xyz_to_degrad(px, py, pz, out);
	printf("RAD %f THETA %f PHI %f\n", out[0], out[1], out[2]);
	degrad_to_xyz(dist, out[1], out[2], out);

	result->x = entity->position->x + out[0];
	result->y = entity->position->y + out[1];
	result->z = entity->position->z + out[2];

	printf("[p3    ] %f %f %f\n", px, py, pz);
	printf("[entity] %f %f %f\n", entity->position->x, entity->position->y, entity->position->z);
	printf("[output] %f %f %f\n", result->x, result->y, result->z);
}

// Thanks Gemini for this new function
void map_entity_go(struct map_entity *entity,
	struct map_position *position,
	struct map_position *result,
	double dist
){
	double dx = position->x - entity->position->x;
	double dy = position->y - entity->position->y;
	double dz = position->z - entity->position->z;
	double distance = sqrt(dx*dx + dy*dy + dz*dz);

	if (distance > 0.1) {
		double speed = dist;

		result->x = (dx / distance) * speed;
		result->y = (dy / distance) * speed;
		result->z = (dz / distance) * speed;
	}
}

double get_entity_distance(struct map_entity *e1, struct map_entity *e2) {
	double dx = e1->position->x - e2->position->x;
	double dy = e1->position->y - e2->position->y;
	double dz = e1->position->z - e2->position->z;

	return sqrt(dx*dx + dy*dy + dz*dz);
}

/**
* Checks if the distance between two entities is less than or equal to a given limit.
* * @param e1    Pointer to the first entity
* @param e2    Pointer to the second entity
* @param limit The maximum distance to check for
* @return      bool (true if within limit, false otherwise)
*/
char map_entity_within(
	struct map_entity *e1,
	struct map_entity *e2,
	double limit
){
	// Basic null checks to prevent crashes
	if (!e1 || !e1->position || !e2 || !e2->position) {
		return 0;
	}

	double dx = e1->position->x - e2->position->x;
	double dy = e1->position->y - e2->position->y;
	double dz = e1->position->z - e2->position->z;

	// Calculate distance squared: x^2 + y^2 + z^2
	double dist_sq = (dx * dx) + (dy * dy) + (dz * dz);

	// Compare against limit squared
	return dist_sq <= (limit * limit);
}


double get_position_distance(struct map_position *e1, struct map_position *e2) {
	double dx = e1->x - e2->x;
	double dy = e1->y - e2->y;
	double dz = e1->z - e2->z;

	return sqrt(dx*dx + dy*dy + dz*dz);
}

/**
* Checks if the distance between two entities is less than or equal to a given limit.
* * @param e1    Pointer to the first entity
* @param e2    Pointer to the second entity
* @param limit The maximum distance to check for
* @return      bool (true if within limit, false otherwise)
*/
char map_position_within(
	struct map_position *e1,
	struct map_position *e2,
	double limit
){
	// Basic null checks to prevent crashes
	if (!e1 || !e2) {
		return 0;
	}

	double dx = e1->x - e2->x;
	double dy = e1->y - e2->y;
	double dz = e1->z - e2->z;

	// Calculate distance squared: x^2 + y^2 + z^2
	double dist_sq = (dx * dx) + (dy * dy) + (dz * dz);

	// Compare against limit squared
	return dist_sq <= (limit * limit);
}

void map_entity_translate(
	struct map_entity *center,
	struct map_entity *entity,
	struct map_position *position
){
	position->x = (entity->position->x - center->position->x)
		+ scrwc
	;
	position->y = (entity->position->y - center->position->y)
		+ scrhc
	;
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

void ffpsnull(void *user, void *data){ return ; }
void (*fpsnull)(void *, void *) = &ffpsnull;

/*void (**fps12flist)(void *) = { fpsnull };
void (**fps24flist)(void *) = { fpsnull };
void (**fps30flist)(void *) = { fpsnull };*/

struct fps_callback *fps12list = NULL;
struct fps_callback *fps24list = NULL;
struct fps_callback *fps30list = NULL;

uint64_t fps12last = 0;
uint64_t fps24last = 0;
uint64_t fps30last = 0;

void fps_push(struct fps_callback **list, void (*f)(void *, void *),
	void *user,
	void *data
){
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
	list[0][size].user = user;
	list[0][size].data = data;
	list[0][size + 1].f = fpsnull;
}

void fps_call(struct fps_callback *list){
	size_t size = 0;
	while(list != NULL && list[size].f != fpsnull){
		list[size].f(list[size].user, list[size].data);

		size++;
	}
}

uint64_t fps_get_time(){
	return (uint64_t) SDL_GetTicks();
}

void fps12(void (*f)(void *, void *), void *user, void *data){
	fps_push(&fps12list, f, user, data);

	if(f != NULL)
		return ;

	uint64_t now = fps_get_time();
	uint64_t ticks = (now - fps12last)/83;

	if(ticks > 0){
		for(uint64_t i = 0; i < ticks; i++){
			fps_call(fps12list);
		}

		fps12last = now;
	}
}

// TODO: Implement for fps 24 and fps 30
void fps24(void (*f)(void *, void *), void *user, void *data){
	fps_push(&fps24list, f, user, data);

	if(f != NULL)
		return ;

	uint64_t now = fps_get_time();
	uint64_t ticks = (now - fps24last)/41;

	if(ticks > 0){
		for(uint64_t i = 0; i < ticks; i++){
			fps_call(fps24list);
		}

		fps24last = now;
	}

	return ;
}

void fps30(void (*f)(void *, void *), void *user, void *data){
	fps_push(&fps30list, f, user, data);

	if(f != NULL)
		return ;

	uint64_t now = fps_get_time();
	uint64_t ticks = (now - fps30last)/33;

	if(ticks > 0){
		for(uint64_t i = 0; i < ticks; i++){
			fps_call(fps30list);
		}

		fps30last = now;
	}

	return ;
}

/*size_t array_len(void *varray, int usize){
	size_t count = 0;
	char *array = (char *) varray;

	while((array + (usize * count)) != 0)
		count++;

	return count;
}

void array_push(void *varray, void *vadata, int usize){
	size_t count = array_len(varray, usize);
	char **carray = (char **) varray;
	char *array = carray[0];

	char *na = (char *) realloc(array, usize * (count + 2));
	if(na == NULL){
		exit(1); // TODO: Out of Memory
	}

	memcpy((na + (usize * (count))), vadata, usize);
	memset((na + (usize * (count + 1))), NULL, usize);

	carray[0] = na;
}*/

size_t array_len(struct array_element *list){
	size_t size = 0;
	while(list[size].data != NULL)
		size++;

	return size;
}

void array_push(struct array_element **list, struct array_element *element){
	size_t count = array_len(list[0]);
	struct array_element *ns = (struct array_element *) realloc(list[0],
		sizeof(struct array_element) * (count + 2)
	);

	if(ns == NULL){
		exit(1); // TODO: Out of Memory
	}

	memcpy(&ns[count], element, sizeof(struct array_element));
	ns[count + 1].data = NULL;

	list[0] = ns;
}

void timing(tsBSpline *spline, float pos, float *result){
	tsStatus status;
	tsBSpline dspline;
	tsError error;

	if(spline == NULL){
		spline = &dspline;
		//error = ts_bspline_new(3, 2, 4, TS_CLAMPED, spline, &status);
		error = ts_bspline_new(4, 2, 3, TS_CLAMPED, spline, &status);
		//printf("%i\n", error);

		tsReal ctrlp[] = {
			0.0,
			500.0,
			1000.0,
			1000.0,
			1000.0,
			1000.0,
			0.0
		};

		tsError err = ts_bspline_set_control_points(
			spline,
			ctrlp,
			&status
		);
		if (err != TS_SUCCESS) {
			// TODO:
			// If this fails, your points remain zero
		}
	}

	tsReal min;
	tsReal max;
	ts_bspline_domain(spline, &min, &max);

	float u = min + ((max - min) * pos);

	tsDeBoorNet net;
	ts_bspline_eval(spline, u, &net, &status);

	//printf("%f %f | %f\n", min, max, u);

	result[0] = ts_deboornet_result_ptr(&net)[0];
	result[1] = ts_deboornet_result_ptr(&net)[1];

	ts_deboornet_free(&net);
}

struct texture_animation *map_entity_get_animation(
	const struct map_entity *entity
){
	return entity->animation->texture->animation;
}

/** map_entity_draw usage guide
	entity->directionX
		determines whether the texture is rendered flipped or not

	entity->position
		determines where the entity is relative to the map

*/
void map_entity_draw_cropped(struct map_entity *entity,
	double sx, double sy,
	double sw, double sh,
	double w, double h
){
	struct texture_animation *anim = map_entity_get_animation(entity);
	struct map_entity_base *data = (struct map_entity_base *) entity->data;

	//printf("DRAW\n");
	SDL_Renderer *renderer = entity->graphics->renderer;
	//printf("REN %ull\n", renderer);
	SDL_Texture *frame = anim->frames[
		entity->animation->pos % anim->count
	];
	SDL_SetTextureBlendMode(frame, SDL_BLENDMODE_BLEND);
	//printf("SET_TEXTURE_BLEND_MODE\n");
	//printf("DEBUG %i\n", entity->type);

	if(!map_center){
		// TODO: map_entity_draw(): OPTIM Check map_center elsewhere
		printf("MAP_CENTER_UNINITIALIZED\n");
		exit(1);
	}

	struct map_position sposition = {
		.x = sx,
		.y = sy
	};

	struct SDL_FRect sfrect;
	map_position_to_frect(&sposition, &sfrect, sw, sh);

	struct map_position position = { 0 };
	map_entity_translate(
		map_center,
		entity,
		&position
	);
	//printf("MAP_ENTITY_TRANSLATE\n");

	/*
	printf("\033[2A\r                                     \n");
	printf("                                     \n");
	printf("\033[2A");
	printf("POSITION %.2f %.2f\n", position.x, position.y);*/
	/*printf("POSITION/DEBUG %.2f %.2f\n",
		map_center->position->x,
		map_center->position->y);*/
	//printf("POS %.2f %.2f\n", result->position->x, result->position->y);

	struct SDL_FRect frect;
	map_position_to_frect(&position, &frect, w, h);

	if(entity->directionX == -1){
		SDL_RenderTextureRotated(renderer, frame, &sfrect, &frect,
			entity->rotation,

			NULL,
			SDL_FLIP_HORIZONTAL
		);
	} else {
		if(entity->rotation != 0){
			SDL_RenderTextureRotated(renderer, frame, &sfrect,
				&frect,
				entity->rotation,

				NULL,
				SDL_FLIP_NONE
			);

			return ;
		}

		SDL_RenderTexture(renderer, frame, &sfrect, &frect);
	}
}

void map_entity_draw_resized(struct map_entity *entity, double w, double h){
	struct texture_animation *anim = map_entity_get_animation(entity);
	struct map_entity_base *data = (struct map_entity_base *) entity->data;

	//printf("DRAW\n");
	SDL_Renderer *renderer = entity->graphics->renderer;
	//printf("REN %ull\n", renderer);
	SDL_Texture *frame = anim->frames[
		entity->animation->pos % anim->count
	];
	SDL_SetTextureBlendMode(frame, SDL_BLENDMODE_BLEND);
	//printf("SET_TEXTURE_BLEND_MODE\n");
	//printf("DEBUG %i\n", entity->type);

	if(!map_center){
		// TODO: map_entity_draw(): OPTIM Check map_center elsewhere
		printf("MAP_CENTER_UNINITIALIZED\n");
		exit(1);
	}

	struct map_position position = { 0 };
	map_entity_translate(
		map_center,
		entity,
		&position
	);
	//printf("MAP_ENTITY_TRANSLATE\n");

	/*
	printf("\033[2A\r                                     \n");
	printf("                                     \n");
	printf("\033[2A");
	printf("POSITION %.2f %.2f\n", position.x, position.y);*/
	/*printf("POSITION/DEBUG %.2f %.2f\n",
		map_center->position->x,
		map_center->position->y);*/
	//printf("POS %.2f %.2f\n", result->position->x, result->position->y);
	struct SDL_FRect frect;
	map_position_to_frect(&position, &frect, w, h);

	if(entity->directionX == -1){
		SDL_RenderTextureRotated(renderer, frame, NULL, &frect,
			entity->rotation,

			NULL,
			SDL_FLIP_HORIZONTAL
		);
	} else {
		if(entity->rotation != 0){
			SDL_RenderTextureRotated(renderer, frame, NULL,
				&frect,
				entity->rotation,

				NULL,
				SDL_FLIP_NONE
			);

			return ;
		}

		SDL_RenderTexture(renderer, frame, NULL, &frect);
	}
}

void map_entity_draw(struct map_entity *entity){
	return map_entity_draw_resized(entity, 64, 64);
}

SDL_Texture *map_entity_frame_at(struct map_entity *entity, double pos){
	struct animation *meta = entity->animation;
	struct texture_animation *animation = meta
		->texture
		->animation
	;

	// if pos is fraction of zero, treat it as percentage
	if(pos < 1.0){
		pos = ((double) animation->count * pos);
	}

	return animation->frames[(size_t) fmod(pos, (double)animation->count)];
}

SDL_Texture *map_entity_frame_get(struct map_entity *entity){
	return map_entity_frame_at(entity, (double) entity->animation->pos);
}

SDL_Texture *map_entity_frame_next(struct map_entity *entity){
	return map_entity_frame_at(
		entity, (double) entity->animation->pos++
	);
}

SDL_Texture *map_entity_frame_minmax(struct map_entity *entity,
	size_t min,
	size_t max
){
	if(entity->animation->pos < min){
		entity->animation->pos = min - 1;
	} else if(entity->animation->pos > max){
		entity->animation->pos = max - 1;
	}

	return map_entity_frame_next(entity);
}

unsigned char chstruct_is_text(const char c){
	if(c < 65){
		if(c < 48 || c > 57){
			return 0;
		}

		return 1;
	}

	if(c > 90){
		if(c < 97 || c > 122){
			return 0;
		}

		return 1;
	}

	return 0;
}

char *chstruct_ptrget(
	struct chstruct *cstrt,
	const char *key,
	size_t *len
){
	char *map = cstrt->map;
	char *data = cstrt->data;

	size_t key_size = strlen(key);
	size_t map_size = strlen(map);
	size_t blindex = 0;
	if(key_size > map_size)
		return NULL;

	char memsize[256] = { 0 };
	char *kstart = map;
	char *kend = map;
	while(1){
		while(!chstruct_is_text(kstart[0])){
			if((kstart - map) > map_size) goto end;

			kstart++;
		}

		kend = strchr(kstart, ' ');
		if(kend == NULL) goto end;
		if(key_size == (kend - kstart)
			&& strncmp(kstart, key, kend - kstart) == 0
		){
			kstart = kend + 1;
			while(!chstruct_is_text(kstart[0])){
				if((kstart - map) > map_size) goto end;

				kstart++;
			}

			//printf("debug: found match %s\n", key);
			kend = strchr(kstart, ';');
			if(kend == NULL) kend = strchr(kstart, 0);
			if(kend == NULL) goto end;
			memcpy(memsize, kstart, kend - kstart);
			memsize[(kend - kstart)] = 0;

			len[0] = (size_t) strtoull(memsize, NULL, 10);
			//memcpy(dest, data, len[0]);
			return data;
		} else {
			kstart = kend + 1;
			while(!chstruct_is_text(kstart[0])){
				if((kstart - map) > map_size) goto end;

				kstart++;
			}

			kend = strchr(kstart, ';');
			if(kend == NULL) goto end;
			memcpy(memsize, kstart, kend - kstart);
			memsize[(kend - kstart)] = 0;
			data = data + (size_t) strtoull(memsize, NULL, 10);
			kstart = kend + 1;
		}

		if((kstart - map) > map_size) goto end;
	}

	end:
		len[0] = 0;

		return NULL;
}
