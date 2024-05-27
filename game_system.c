#include "game_system.h"


int LevelStar(int enemiesKill, int total_enemies){
    if(3*enemiesKill >= 2*total_enemies){
        return 3;        
    }
    else if((3*enemiesKill >= total_enemies) && (3*enemiesKill < 2*total_enemies) ){
        return 2;        
    }
    else{
        return 1;
    }
}
