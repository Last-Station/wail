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

static struct texture textures[] = {
	{
		.name = "slime",
		.type = "gif"
	}, {
		.name = NULL
	}
};

static SDL_FRect resize = {
	.x = (float) 0,
	.y = (float) 0,
	.w = (float) 128,
	.h = (float) 128
};
static int counter = 0;
static int y = 0;
static void on_tick(struct entity_op_data *data){
	SDL_Renderer *renderer = data->graphics->renderer;

	SDL_SetRenderDrawColor(renderer, counter, y, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderPoint(renderer, 0 + counter, y);
	SDL_RenderPoint(renderer, 0 + counter + 1, y);
	SDL_RenderPoint(renderer, 0 + counter + 2, y);
	SDL_RenderPoint(renderer, 0 + counter + 3, y);
	SDL_RenderPoint(renderer, 0 + counter, y + 1);
	SDL_RenderPoint(renderer, 0 + counter + 1, y + 1);
	SDL_RenderPoint(renderer, 0 + counter + 2, y + 1);
	SDL_RenderPoint(renderer, 0 + counter + 3, y + 1);

	counter += 3;

	struct texture slime = textures[0];
	SDL_Texture *frame = slime.animation->frames[
		(counter / 64) % slime.animation->count
	];
	SDL_SetTextureBlendMode(frame, SDL_BLENDMODE_BLEND);

	resize.x = (float) 0 + counter + 3;
	resize.y = (float) y + 1;

	struct SDL_FRect size = {
		.x = (float) 0,
		.y = (float) 0,
		.w = (float) 720,
		.h = (float) 720
	};

	printf("%i\n", (counter / 64) % slime.animation->count);
	SDL_RenderTexture(renderer, frame, NULL, &resize);
	SDL_RenderPresent(renderer);
	int width;
	int height;

	SDL_GetWindowSize(data->graphics->window, &width, &height);
	if(counter > width){
		counter = 0;
		y += 2;
	}

	//printf("%i\r", counter);
	fflush(stdout);
}

static void on_init(struct entity_op_data *data){
	SDL_Renderer *renderer = data->graphics->renderer;

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

		printf("Agh\n");

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

	printf("INITIALIZED\n");
}

static void on_new(struct entity_op_data *data){
	printf("%i CREATED\n", data->entity->type);
}

void op_biome_forest(){
	printf("EXISTS\n");

	entity_op[(unsigned int) 10] = (struct entity_op) {
		.on_tick = &on_tick,
		.on_init = &on_init,
		.on_new = &on_new
	};
}
