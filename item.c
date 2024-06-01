#include "item.h"


int Choose_Item(Player *player) 
// 每關結束後隨機給(暫定)
{   
    // 要跳出畫面
     int item_choice = GetRandomValue(1,7); 
     switch(item_choice)
     {
      case 1:{
        player->AttackPower *= 1.6;
        return 1;
        break;
      } // Energy beam 衝能光束：傷害*1.6
      case 2:{
        player->AttackPower += 20;
        return 2;
        break;
      } // Weapon upgrade 武器升級，攻擊力+20~50
      case 3:{
        player->speed.x += 3;
        player->speed.y += 3;
        return 3;
        break;
      } // Booster 推進器(速度+3)
      case 4:{
        player->HP += 100;
        return 4;
        break;
      }
      case 5:{//補包：生命值+20
         player->HP += 21;
         return 5;
         break;
      }    
      case 6:{ 
         player->MaxHP += 100;
         return 6;
         break;
      } // 裝甲外殼(生命值上限+100)
      case 7:{
        player->AttackPower *= 2;
        return 7;
        break;
      } // 電漿砲 攻擊力*2
      default: break;
    }
}

int Event(Player *player, int *gold, Shoot *shoot ,int *ShootControl, Partner *partner ,int *cureHP)// 15種正負事件擇一
{   
    int Event_type = GetRandomValue(1,27);
    switch(Event_type)
    {   
        case 1:{ // 寵物的吸引:當你擁有寵物時增加150金幣
            if(partner->type == PARTNER_TYPE_ONE) {
               (*gold) += 150;}
            else if(partner->type == PARTNER_TYPE_TWO) {
               player->AttackPower += 44;}
            else if(partner->type == PARTNER_TYPE_THREE) {
               player->HP += 38;}
            return 1;
            break;
        }
        case 2:{ // 基地給大家加雞腿了!獲得掉寶_雞腿(生命值全滿)
            player->HP = player->MaxHP;
            return 2;
            break;
        } 
        case 3:{ // 中獎：獲得金幣300到500
            (*gold) += 300;
            return 3;
            break;
        }
        case 4:{ // 神秘星系的能量波，在接下來的移動速度*1.5
            player ->speed .x *= 1.5;
            player ->speed .y *= 1.5;
            return 4;
            break;
        }
        case 5:{ // 失去所有金幣
            (*gold) = 0;
            return 5;
            break;
        }
        case 6:{ // 黑洞引力場，接下來的移動速度*0.5
            player->speed.x *= 0.5;
            player->speed.y *= 0.5;
            return 6;
            break;}
        case 7:{ // 過路費：付錢才能進到下一關，暫定50
            (*gold) -= 50;
            return 7;
            break;
        }
        case 8:{ // 武器失靈：下一關開始只能移動不能攻擊
            (*ShootControl)= 1;
            return 8;
            break;
        }
        case 9:{ // 治癒能力提升：回復生命值速率
            (*cureHP) = 1;
            return 9;
            break;
        }
        
        default: break;
    }
}