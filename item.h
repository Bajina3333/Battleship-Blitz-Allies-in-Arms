#ifndef ITEM_H
#define ITEM_H

#include "raylib.h"
#include "type.h"

int Choose_Item(Player *player);
int Event(Player *player, int *gold, Shoot *shoot, int *ShootControl , Partner *partner ,int *cureHP, char *text);

#endif