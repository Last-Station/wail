#include <cstdio>
#include <miko.h>
extern struct map map;
extern struct map_entity *map_center;
extern struct entity_op entity_op[entity_op_size];
static void action_move(struct map_entity_action_data *data){

}

static struct map_entity_action actions[] = {
	{
		.name = "move",
		.call = &action_move
	}
};

static struct texture textures[] = {
	{
		.name = "box",
		.type = "gif"
	},{
		.name = "boxw",
		.type = "gif"
	},{
		.name = "box",
		.type = "gif"
	},{
		.name = "boxh",
		.type = "gif"
	},{
		.name = "box",
		.type = "gif"
	},{
		.name = "boxw",
		.type = "gif"
	},{
		.name = "box",
		.type = "gif"
	},{
		.name = "boxh",
		.type = "gif"
	},{
		.name = "box",
		.type = "gif"
	},{
		.name = "boxbg",
		.type = "gif"
	},
	{
		.name = NULL
	}
};

struct button {
	struct map_entity borders[9];
	struct animation animations[9];
	struct texture textures[9];
	double width;
	double height;
};

static void on_fps12(struct entity_op_data *data){
	struct map_entity *entity = data->entity;
	entity->animation->pos++;
}

static void on_fps30(struct entity_op_data *data){

}

static void draw(struct entity_op_data *data){
	struct map_entity *entity = (struct map_entity *) data->entity;
	struct button *button = (struct button *) entity->data;
	button->width++;
	button->height++;

	if(button->width > 600){
		button->width = 100;
	}

	if(button->height > 200){
		button->height = 30;
	}


	struct map_entity *cmp = button->borders;
	struct texture *texture = textures;

	double x = entity->position->x;
	double y = entity->position->y;
	double width = button->width;
	double height = button->height;
	double tsize = 32;
	double min = width;
	if(height < width)
		min = height;

	cmp += 8;
	texture += 9;
	cmp->animation->texture = texture;
	cmp->graphics = data->graphics;
	cmp->position->x = x + tsize;
	cmp->position->y = y + tsize;
	while(cmp->position->x < x + width - tsize - tsize){
		while(cmp->position->y < y + height + tsize - tsize - tsize){
			map_entity_draw(cmp);
			cmp->position->y += tsize;
		}
		/*map_entity_draw_cropped(cmp, 0, 0,
			64, (y + height - cmp->position->y) * 2,
			64, (y + height - cmp->position->y) * 2
		);*/

		/*map_entity_draw_cropped(cmp, 0, 0,
			x + width - cmp->position->x, 64,
			x + width - cmp->position->x, 64
		);*/

		cmp->position->x += tsize;
		cmp->position->y = y + tsize;
	}

	while(cmp->position->y < y + height + tsize - tsize - tsize){
		map_entity_draw_cropped(cmp, 0, 0,
			x + width - cmp->position->x, 64,
			x + width - cmp->position->x, 64
		);
		cmp->position->y += tsize;
	}

	/*map_entity_draw_cropped(cmp, 0, 0,
		x + width - cmp->position->x, 64,
		x + width - cmp->position->x, 64
	);*/

	// print borders

	cmp = button->borders;
	texture = textures;

	cmp++;
	texture++;
	cmp->animation->texture = texture;
	cmp->graphics = data->graphics;
	cmp->position->x = x + tsize;
	cmp->position->y = y + tsize * 0;
	while(cmp->position->x + tsize < x + width - tsize){
		map_entity_draw(cmp);
		cmp->position->x += tsize;
	}

	map_entity_draw_cropped(cmp, 0, 0,
		x + width - cmp->position->x, 64,
		x + width - cmp->position->x, 64
	);

	cmp++;
	texture++;
	cmp++;
	texture++;
	cmp->animation->texture = texture;
	cmp->graphics = data->graphics;
	cmp->position->x = x + width - tsize;
	cmp->position->y = y + tsize;
	cmp->directionX = -1;
	while(cmp->position->y + tsize < y + height){
		map_entity_draw(cmp);
		cmp->position->y += tsize;
	}

	/* TODO: I HAVE NO IDEA HOW THIS WORKS */
	/*map_entity_draw_cropped(cmp, 0, 0,
		64, (y + height - cmp->position->y),
		64, (y + height - cmp->position->y)
	);*/
	map_entity_draw_cropped(cmp, 0, 0,
		64, (y + height - cmp->position->y) * 2,
		64, (y + height - cmp->position->y) * 2
	);

	cmp++;
	texture++;
	cmp++;
	texture++;
	cmp->animation->texture = texture;
	cmp->graphics = data->graphics;
	cmp->position->x = x + tsize;
	cmp->rotation = 180;
	cmp->position->y = y + height;
	while(cmp->position->x + tsize < x + width - tsize){
		map_entity_draw(cmp);
		cmp->position->x += tsize;
	}

	map_entity_draw_cropped(cmp, 0, 0,
		x + width - cmp->position->x, 64,
		x + width - cmp->position->x, 64
	);

	cmp++;
	texture++;
	cmp++;
	texture++;
	cmp->animation->texture = texture;
	cmp->graphics = data->graphics;
	cmp->position->x = x;
	cmp->position->y = y + tsize;
	while(cmp->position->y + tsize < y + height){
		map_entity_draw(cmp);
		cmp->position->y += tsize;
	}

	//printf("%.2f\n", y + height - cmp->position->y);

	map_entity_draw_cropped(cmp, 0, 0,
		64, (y + height - cmp->position->y) * 2,
		64, (y + height - cmp->position->y) * 2
	);

	cmp++;
	texture++;
	cmp++;
	texture++;

	cmp = button->borders;
	texture = textures;

	//print corners later

	cmp->animation->texture = texture;
	cmp->graphics = data->graphics;
	cmp->position->x = x;
	cmp->position->y = y + tsize * 0;
	map_entity_draw_resized(cmp, 64, 64);
	cmp++;
	texture++;
	cmp++;
	texture++;
	cmp->animation->texture = texture;
	cmp->graphics = data->graphics;
	cmp->position->x = x + width - tsize;
	cmp->position->y = y + tsize * 0;
	cmp->directionX = -1;
	map_entity_draw_resized(cmp, 64, 64);
	cmp++;
	texture++;
	cmp++;
	texture++;
	cmp->animation->texture = texture;
	cmp->graphics = data->graphics;
	cmp->position->x = x + width - tsize;
	cmp->position->y = y + height;
	cmp->rotation = 180;
	map_entity_draw_resized(cmp, 64, 64);
	cmp++;
	texture++;
	cmp++;
	texture++;
	cmp->animation->texture = texture;
	cmp->graphics = data->graphics;
	cmp->position->x = x;
	cmp->position->y = y + height;
	cmp->rotation = 180;
	cmp->directionX = -1;
	map_entity_draw_resized(cmp, 64, 64);
	cmp++;
	texture++;
	cmp++;
	texture++;
}

static void on_tick(struct entity_op_data *data){
	struct map_entity *entity = data->entity;
	SDL_Renderer *renderer = data->graphics->renderer;

	entity->position->x = map_center->position->x;
	entity->position->y = map_center->position->y;

	entity->animation->texture = &textures[0];
	entity->graphics->renderer = data->graphics->renderer;

	draw(data);
}

static void on_init(struct entity_op_data *data){
	SDL_Renderer *renderer = data->graphics->renderer;
	load_textures(textures, renderer);

	entity_new(&map, 3002);
}

static void on_new(struct entity_op_data *data){
	struct map_entity *entity = data->entity;
	entity->directionX = 1;
	entity->directionY = 1;
	entity->animation->pos = 0;

	entity->position->x = 0;
	entity->position->y = 0;

	struct button *user = (struct button *) malloc(sizeof(struct button));

	entity->custom = 1;
	entity->data = (void *) user;
	user->width = 100;
	user->height = 70;

	for(int i = 0; i < 9; i++){
		user->borders[i].animation = &user->animations[i];
		user->borders[i].rotation = 0;
		user->animations[i].pos = 0;
		user->borders[i].custom = 1;
	}
}

static void on_event(struct entity_op_data *data){
	struct map_entity *entity = data->entity;
	if(!entity)
		return ;

	struct button *button = (struct button *) entity->data;
	SDL_Event *event = data->event;

	if(event->type != SDL_EVENT_MOUSE_BUTTON_DOWN)
		return ;

	struct map_position position;
	SDL_ConvertEventToRenderCoordinates(entity->graphics->renderer, event);

	if(event->button.button == SDL_BUTTON_LEFT){
		map_entity_translate(
			map_center,
			entity,
			&position
		);


		printf("CLEV   x%f y%f\n", position.x, position.y);
		printf("CLEVXY x%f y%f\n", event->button.x, event->button.y);

		if(event->button.x < position.x)
			return ;

		if(event->button.y < position.y)
			return ;

		if(event->button.x > position.x + button->width)
			return ;

		if(event->button.y > position.y + button->height)
			return ;

		printf("BUTTON_CLICK\n");
		button->width = 100;
		button->height = 100;
	}
}

void op_button(){
	entity_op[(unsigned int) 3002] = (struct entity_op) {
		.on_tick = &on_tick,
		.on_init = &on_init,
		.on_new = &on_new,
		.on_event = &on_event,

		.on_fps12 = &on_fps12,
		.on_fps24 = NULL,
		.on_fps30 = &on_fps30
	};

	/*
	entity_op[(unsigned int) 3003] = (struct entity_op) {
		.on_tick = NULL,
		.on_init = NULL,
		.on_new = NULL,

		.on_fps12 = NULL,
		.on_fps24 = NULL,
		.on_fps30 = NULL
	};*/
}
