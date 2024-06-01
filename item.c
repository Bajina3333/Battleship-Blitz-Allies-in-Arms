#include "item.h"
#include<string.h>


int Choose_Item(Player *player) 
// 每關結束後隨機給(暫定)
{   
    // 要跳出畫面
     int item_choice = GetRandomValue(1,7); 
     switch(item_choice)
     {
      case 1:{// Energy beam 衝能光束：傷害*1.6
        player->AttackPower *= 1.6;
        return 1;
        break;
      } 
      case 2:{// machine_gun 武器升級，攻擊力+20~50
        player->AttackPower += 20;
        return 2;
        break;
      } 
      case 3:{// Booster 推進器(速度+3)
        player->speed.x += 3;
        player->speed.y += 3;
        return 3;
        break;
      } 
      case 4:{ //cure 補包：生命值+100
        player->HP += 100;
        return 4;
        break;
      }
      case 5:{ //healing potion
     
         player->HP += 21;
         return 5;
         break;
      }    
      case 6:{ // armor 裝甲外殼(生命值上限+100)
         player->MaxHP += 100;
         return 6;
         break;
      } 
      case 7:{// plasma_cannon 電漿砲 攻擊力*2
        player->AttackPower *= 2;
        return 7;
        break;
      } 
      default: break;
    }
}

int Event(Player *player, int *gold, Shoot *shoot ,int *ShootControl, Partner *partner ,int *cureHP, char *text)
{   
    const char *words = "";
    int Event_type = GetRandomValue(1,27);
    switch(Event_type)
    {   
        case 1:{ // Partner Attraction: Gain benefits when you have a partner
            words = "Partner Attraction: Gain benefits when you have a partner";
            strcpy(text, words);
            if(partner->type == PARTNER_TYPE_ONE) {
               (*gold) += 150;}
            else if(partner->type == PARTNER_TYPE_TWO) {
               player->AttackPower += 44;}
            else if(partner->type == PARTNER_TYPE_THREE) {
               player->HP += 38;}
            return 1;
            break;
        }
        case 2:{ // Obtain First Aid Kit, Health Fully Restored
            words = "Obtain First Aid Kit, Health Fully Restored";
            strcpy(text, words);
            player->HP = player->MaxHP;
            return 2;
            break;
        } 
        case 3:{ // Jackpot: Gain 300 Bonus
            words = "Jackpot: Gain 300 Bonus";
            strcpy(text, words);
            (*gold) += 300;
            return 3;
            break;
        }
        case 4:{ // Energy Wave from the Mysterious Galaxy, Movement Speed *1.5 for the rest time
            words = "Energy Wave from the Mysterious Galaxy, Movement Speed *1.5 for the rest time";
            strcpy(text, words);
            player ->speed .x *= 1.5;
            player ->speed .y *= 1.5;
            return 4;
            break;
        }
        case 5:{ // Lose All golds
            words = "Lose All golds";
            strcpy(text, words);
            (*gold) = 0;
            return 5;
            break;
        }
        case 6:{ // Black Hole Gravitational Field, Movement Speed *0.5 for the rest time
            words = "Black Hole Gravitational Field, Movement Speed *0.5 for the rest time";
            strcpy(text, words);
            player->speed.x *= 0.5;
            player->speed.y *= 0.5;
            return 6;
            break;}
        case 7:{ // Toll: Pay 50 to Enter the Next Level
            words = "Toll: Pay 50 to Enter the Next Level";
            strcpy(text, words);
            (*gold) -= 50;
            return 7;
            break;
        }
        case 8:{ // Weapon Malfunction: Can Only Move, Cannot Attack in the Next Level
            words = "Weapon Malfunction: Can Only Move, Cannot Attack in the Next Level";
            strcpy(text, words);
            (*ShootControl)= 1;
            return 8;
            break;
        }
        case 9:{ // Enhanced Healing Ability: Recover +1 HP per Second
            words = "Partner Attraction: Gain benefits when you have a partner";
            strcpy(text, words);
            (*cureHP) = 1;
            return 9;
            break;
        } 
        
        default: break;
    }
}