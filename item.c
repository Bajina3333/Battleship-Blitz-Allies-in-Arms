#include "item.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void Choose_Item(Player *player) 
// 每關結束後隨機給(暫定)
{   
    // 要跳出畫面
    // int item_choice = GetRandomValue(1,6); 
    // int item_choice = 5; // demo時用
    // switch(item_choice)
    // {
     // case 1:{player->AttackPower *= 1.6;} // 衝能光束：傷害*1.6
     // case 2:{player->AttackPower += 20;} // 武器升級，攻擊力+20~50
     // case 3:{治癒能力提升：回復生命值速率 +5 -> +10} 
     // 在HPsystem裡增加回血變數?
     // case 4:{player->speed.x += 5;
             // player->speed.y += 5;//} // 推進器(速度+5)
     // case 5:{
         player->HP += 100;
         // MaxHP += 100;} // 裝甲外殼(生命值上限+100)
     // case 6:{player->AttackPower *= 1.5;} // 電漿砲 攻擊力*1.5

    //}
}

/*void Event(Player *player, gold )// 15種正負事件擇一
{   
    // 要用到的：player.AttackPower, player.speed.x, player.speed.y, 等
    int Event_type = GetRandomValue(1,15);
    switch(Event_type)
    {   
        case 1:{跳轉至商店購買頁面} // 由關卡負責人寫？
        case 2:{寵物的吸引:當你擁有寵物時增加150金幣
            // if(有寵物) {
            //   gold += 150;}
        } 
        case 3:{ // 基地給大家加雞腿了!獲得掉寶_雞腿(生命值全滿)
            currentHP = MaxHP;
        } 
        case 4:{ // 中獎：獲得金幣300到500
            gold += 300;
        }
        case 6:{天使送幸福：贈送免費裝備} // call Choose_Item
        case 7:{
            for (int i = 0; i < NUM_SHOOTS; i++){
                shoot[i].speed.x *= 1.5;
            }
            player.speed.x *= 1.5;
            player.speed.y *= 1.5;
            // 神秘星系的能量波，在接下來的30秒內，攻擊速度和移動速度*1.5
            }
        //
        case 8:{無法控制battleship移動長達3秒}
        case 9:{ // 失去所有金幣
            gold = 0;
        }
        case 10:{失去所有裝備} // 改成只失去裝備(意思是回到最初始值)
        case 11:{
            for (int i = 0; i < NUM_SHOOTS; i++){
                shoot[i].speed.x *= 0.5;
            }
            player.speed.x *= 0.5;
            player.speed.y *= 0.5;
            // 黑洞引力場，20秒內的移動速度和攻擊頻率*0.5
            }
        case 12:{過路費：付錢才能進到下一關，暫定50
            gold -= 50;
        }
        case 13:{下一關怪物生命值*1.5
            for(int i=0; i < activeEnemies; i++){
                if(enemy[i].type = STRONG){
                    enemy[i].HP *= 1.5;
                }
                else if(enemy[i].type = STANDARD){
                    enemy[i].HP *= 1.5;
                }
            }
        }
        case 14:{武器失靈：下一關開始前15秒只能移動不能攻擊
            // 用一個space pulse在keyspace
        } // 要顯示計時嗎?
        // shootRate 如何關掉，射擊function多加一層控制
        case 15:{所有或一無所有: 給予玩家一分鐘的挑戰時間當受到攻擊時直接死亡，撐過則增加2000金幣} 
        // 刪除，或改成挑戰當前關卡即可       
}
}

int Drops_Time() // 決定掉寶時機
{   
    int drops_time = GetRandomValue(1,7);
    return drops_time;
}
void Drops_Type(Player *player, HPsystem hpsystem) // 決定掉寶種類
{   
    int drop_choice = GetRandomValue(1,5);
    switch(drop_choice)
    {
        case 1:{補包：生命值+20
        // currentHP += 20;
        } 
        case 2:{護盾：持續10秒內可吸收100傷害}
        case 3:{無敵星星：無視傷害5秒
        // 寫function使扣血行為停止?/扣了再加回來
        } // 需要寫一個計算血量的function
        case 4:{自動修復套件，立即回復50%的最大生命值
        // currentHP += ((0.5)*MaxHP);
        }
        case 5:{電波感擾器:使敵方攻擊停止5秒} // 目前enemy不攻擊，還是改成撞到不扣血?
    }

}*/