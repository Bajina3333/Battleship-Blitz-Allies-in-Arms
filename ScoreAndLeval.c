#include "raylib.h"

struct star_choose{ //Use 1, 2, 3 represent how many stars are in level
    int level_1;
    int level_2;
    int level_3;
};

int kill_monster = 0; //The number of monsters in the level
int total_monster = 0; //The number of monsters killed in the level
int level_score = 0; //Monster percentage is converted into scores
int gold = 0; //Golds held by the player
int final_score = 0; //The score at the end of the game
int add_score = 0; //Convert drops and equipments to scores
bool store_enter = false; //Judge whether or not to execute the store program with true or false

