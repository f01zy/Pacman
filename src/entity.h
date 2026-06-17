#ifndef ENTITY_INCLUDED
#define ENTITY_INCLUDED

#include "types.h"

void initialize_entities(struct GameContext *game);
void change_entity_desired_direction(struct Entity *entity, enum Direction dir);
void iterate_entity(struct State *state, struct Entity *entity);

#endif
