#include "partner.h"


void InitPartner(Partner *partner, Vector2 playerPosition, PartnerType type) {
    partner->rec.width = 20;
    partner->rec.height = 20;
    partner->rec.x = playerPosition.x - 30;  // 初始位置在玩家旁边
    partner->rec.y = playerPosition.y + 35;
    partner->type = type;  // 设置类型
    partner->active = true;
    partner->attackTimer = 0.0f; // ?
    // partner->hitCooldown = 0.5f;
    // partner->lastHitTime = -partner->hitCooldown;  // 初始设置确保一开始可以被攻击
    partner->attackRate = 1.0;  // 每秒一次
    partner->shootTimer = 0.0;  // 初始化計時器
    partner->lastShootTime = GetTime();  // 設置初始射擊時間
    partner->lastEffectTime = GetTime();
    partner->effectActive = false;    
    partner->effectType = '\0';
    partner->effect_duration = 10;
    
    int choosetype = GetRandomValue(1,3);

    switch (choosetype) {
        case 1:
            partner->type = PARTNER_TYPE_ONE;
            partner->HP = 90;
            partner->maxHealth = 90;
            partner->AttackPower = 25;
            partner->attackRate = 1.0f;
            break;
        case 2:
            partner->type = PARTNER_TYPE_TWO;
            partner->HP = 130;
            partner->maxHealth = 130;
            partner->AttackPower = 15;
            partner->attackRate = 1.0f;
            break;
        case 3:
            partner->type = PARTNER_TYPE_THREE;
            partner->HP = 50;
            partner->maxHealth = 50;
            partner->AttackPower = 35;
            partner->attackRate = 1.0f;
            break;
    }
}

void InitPartnerShoot(Partner *partner, Shoot *shoot, int NUM_SHOOTS) {
    for (int i = 0; i < NUM_SHOOTS; i++)
    {
        shoot[i].rec.x = partner->rec.x;
        shoot[i].rec.y = partner->rec.y + partner->rec.height/4;
        shoot[i].rec.width = 10;
        shoot[i].rec.height = 5;
        shoot[i].speed.x = 7;
        shoot[i].speed.y = 0;
        shoot[i].active = false;
        shoot[i].color = MAROON;
    }
}

void PartnerShoot(Partner *partner, Shoot *partnerBullets, float deltaTime, int numBullets, Enemy *enemies, int numEnemies, int *enemiesKill, int *total_count_Enemies, int *totalEnemies) {
    partner->attackTimer += deltaTime;

    // 檢查是否達到射擊頻率
    if (partner->attackTimer >= 0.3f) {
        partner->attackTimer = 0; // 重置計時器
        for (int i = 0; i < numBullets; i++) {
            if (!partnerBullets[i].active) {
                partnerBullets[i].rec.x = partner->rec.x + partner->rec.width; // 從伙伴前面發射
                partnerBullets[i].rec.y = partner->rec.y + partner->rec.height / 2;
                partnerBullets[i].speed.x = 200; // 根據需要設置合適的速度
                partnerBullets[i].active = true;
                break;
            }
        }
    }

    // 更新所有活動的子彈
    for (int i = 0; i < numBullets; i++) {
        if (partnerBullets[i].active) {
            partnerBullets[i].rec.x += partnerBullets[i].speed.x *deltaTime; // 更新位置

            // 碰撞檢測與敵人
            for (int j = 0; j < numEnemies; j++) {
                if (enemies[j].active && CheckCollisionRecs(partnerBullets[i].rec, enemies[j].rec)) {
                    partnerBullets[i].active = false; // 子彈消失
                    enemies[j].HP -= partner->AttackPower; // 扣血
                    if (enemies[j].HP <= 0) {
                        enemies[j].active = false; // 敵人死亡
                        (*enemiesKill)++;
                        (*total_count_Enemies)++;
                        enemies[j].rec.x = GetRandomValue(1500, 2500); // 可選擇將敵人移出屏幕
                        enemies[j].rec.y = GetRandomValue(0, 900 - enemies[j].rec.height);

                        if (total_count_Enemies < totalEnemies)
                        {
                            enemies[j].active = true;  // 重新激活敌人
                        }
                    }
                }
            }

            if (partnerBullets[i].rec.x > GetScreenWidth()) {  // 使用 GetScreenWidth() 函數
                partnerBullets[i].active = false;
            }
        }
    } 
}

void UpdatePartner(Partner *partner, Vector2 playerPosition, Player *player) {
    if (!partner->active) return;

    partner->rec.x = playerPosition.x - 30;
    partner->rec.y = playerPosition.y + 20;

    float currentTime = GetTime();

    static int origin_player_speed_x; // only init in first time
    static int origin_player_speed_y;
    if (partner->type == PARTNER_TYPE_ONE) {
        if (!(partner->effectActive) && ((currentTime - partner->lastEffectTime) >= partner->CD)) {
            origin_player_speed_x = player->speed.x; // 保存原始速度
            origin_player_speed_y = player->speed.y; // 保存原始速度
            player->speed.x += (origin_player_speed_x * 0.25);
            player->speed.y += (origin_player_speed_y * 0.25);
            partner->lastEffectTime = currentTime;
            partner->effectActive = true;
            partner->effectType = 'S';
        }

        if (partner->effectActive && (currentTime - partner->lastEffectTime) >  partner->effect_duration) {
            player->speed.x = origin_player_speed_x; // 恢復原始速度
            player->speed.y = origin_player_speed_y; // 恢復原始速度
            partner->effectActive = false;
            partner->effectType = '\0';
            partner->CD = (rand() % 5) + 10; // Random CD
        }
    }

        // PARTNER_TYPE_TWO 效果
    if (partner->type == PARTNER_TYPE_TWO) {
        if ((currentTime - partner->lastEffectTime) >= partner->CD) {
            player->HP += 10;
            if (player->HP > player->MaxHP) player->HP = player->MaxHP; // this need player max HP, for now use 100
            partner->lastEffectTime = currentTime;
            partner->CD = (rand() % 5) + 10;
            partner->effectActive = true;
            partner->effectType = 'H';
        } 
    }

    // PARTNER_TYPE_THREE 效果
    static int origin_player_attack;
    if (partner->type == PARTNER_TYPE_THREE) {
        if (!(partner->effectActive) && (currentTime - partner->lastEffectTime) >= partner->CD) {
            origin_player_attack = player->AttackPower; // 保存原始攻擊力
            player->AttackPower += (origin_player_attack * 0.25);
            partner->lastEffectTime = currentTime;
            partner->effectActive = true;
            partner->effectType = 'A';
        }

        if (partner->effectActive && (currentTime - partner->lastEffectTime) > partner->effect_duration) {
            player->AttackPower = origin_player_attack; // 恢復原始攻擊力
            partner->effectActive = false;
            partner->effectType = '\0';
            partner->CD = (rand() % 5) + 20;
        }
    }
}


void CheckPartnerCollisionRecs(Partner *partner, Enemy *enemy, int *enemiesKill, int *total_count_Enemies, Shoot *shoot) {
    if (CheckCollisionRecs(partner->rec, enemy->rec)) {
        partner->HP -= (enemy->AttackPower);
        enemy->active = false;
        enemy->rec.x = GetRandomValue(-100, -1);
        enemy->rec.y = GetRandomValue(-100, -1);
        (*enemiesKill)++;
        (*total_count_Enemies)++;
        if (partner->HP <= 0) {
            partner->active = false;
            for (int i = 0; i < 50; i++){
                shoot[i].active = false;
            }
        }
    }
}


void DrawPartnerHealth(const Partner *partner) {
    if (partner->active) {
       DrawText(TextFormat("partner_HP:%03i", partner->HP), 1250, 50, 25, RED);
    }
}