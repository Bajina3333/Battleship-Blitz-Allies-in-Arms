#include "game_system.h"


int LevelStar(int enemiesKill, int total_enemies){
    if((enemiesKill % total_enemies) >= 6){
        return 3;        
    }
    else if((enemiesKill % total_enemies) >= 3 ){
        return 2;        
    }
    else{
        return 1;
    }
}
