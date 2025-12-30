static int counter = 0;
static void on_tick(struct map *map, void *data){
	counter++;
	printf("%i\n", counter);
}

void op_biome_forest(){
	entity_op[10] = &on_tick;
}
