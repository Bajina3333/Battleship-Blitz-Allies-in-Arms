#include "partner.h"


void InitPartner(Partner *partner, Vector2 playerPosition, PartnerType type) {
    partner->rec.width = 20;
    partner->rec.height = 20;
    partner->rec.x = playerPosition.x - 30;  // 初始位置在玩家旁边
    partner->rec.y = playerPosition.y + 20;
    partner->type = type;  // 设置类型
    partner->active = true;
    partner->attackTimer = 0.0f; // ?
    // partner->hitCooldown = 0.5f;
    // partner->lastHitTime = -partner->hitCooldown;  // 初始设置确保一开始可以被攻击
    partner->attackRate = 1.0;  // 每秒一次
    partner->shootTimer = 0.0;  // 初始化計時器
    partner->lastShootTime = GetTime();  // 設置初始射擊時間

    switch (type) {
        case PARTNER_TYPE_ONE:
            partner->HP = 90;
            partner->maxHealth = 90;
            partner->AttackPower = 25;
            partner->attackRate = 1.0f;
            break;
        case PARTNER_TYPE_TWO:
            partner->HP = 130;
            partner->maxHealth = 130;
            partner->AttackPower = 15;
            partner->attackRate = 1.0f;
            break;
        case PARTNER_TYPE_THREE:
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

// void PartnerShoot(Partner *partner, Shoot *partnerBullets, float deltaTime, int numBullets) {
    // 增加伙伴的攻擊計時器
    // partner->attackTimer += deltaTime;

    // 檢查是否達到射擊頻率
    // if (partner->attackTimer >= 0.2f) {
        // partner->attackTimer = 0; // 重置計時器
        // for (int i = 0; i < numBullets; i++) {
            // if (!partnerBullets[i].active) {
                // partnerBullets[i].rec.x = partner->rec.x + partner->rec.width; // 從伙伴前面發射
                // partnerBullets[i].rec.y = partner->rec.y + partner->rec.height / 2;
                // partnerBullets[i].speed.x = 200; // 根據需要設置合適的速度
                // partnerBullets[i].active = true;
                // break;
            // }
        // }
    // }

    // 更新所有活動的子彈
    // for (int i = 0; i < numBullets; i++) {
        // if (partnerBullets[i].active) {
            // partnerBullets[i].rec.x += partnerBullets[i].speed.x * deltaTime; // 更新位置
            // if (partnerBullets[i].rec.x > GetScreenWidth()) {  // 使用 GetScreenWidth() 函數
                // partnerBullets[i].active = false;
            // }
        // }
    // }
// }
void PartnerShoot(Partner *partner, Shoot *partnerBullets, float deltaTime, int numBullets, Enemy *enemies, int numEnemies, int *score, int *enemiesKill) {
    partner->attackTimer += deltaTime;

    // 檢查是否達到射擊頻率
    if (partner->attackTimer >= 0.2f) {
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
            partnerBullets[i].rec.x += partnerBullets[i].speed.x * deltaTime; // 更新位置

            // 碰撞檢測與敵人
            for (int j = 0; j < numEnemies; j++) {
                if (enemies[j].active && CheckCollisionRecs(partnerBullets[i].rec, enemies[j].rec)) {
                    partnerBullets[i].active = false; // 子彈消失
                    enemies[j].HP -= partner->AttackPower; // 扣血
                    if (enemies[j].HP <= 0) {
                        enemies[j].active = false; // 敵人死亡
                        (*enemiesKill)++;
                        *score += 100;
                        enemies[j].rec.x = GetRandomValue(-100, -1); // 可選擇將敵人移出屏幕
                        enemies[j].rec.y = GetRandomValue(-100, -1);
                    }
                }
            }

            if (partnerBullets[i].rec.x > GetScreenWidth()) {  // 使用 GetScreenWidth() 函數
                partnerBullets[i].active = false;
            }
        }
    } 
}

void UpdatePartner(Partner *partner, Vector2 playerPosition) {
    if (!partner->active) return;

    // 更新伙伴位置
    partner->rec.x = playerPosition.x - 30;
    partner->rec.y = playerPosition.y + 20;
}


void CheckPartnerCollisionRecs(Partner *partner, Enemy *enemy, int *enemiesKill) {
    if (CheckCollisionRecs(partner->rec, enemy->rec)) {
        partner->HP -= (enemy->AttackPower);
        enemy->active = false;
        enemy->rec.x = GetRandomValue(-100, -1);
        enemy->rec.y = GetRandomValue(-100, -1);
        (*enemiesKill)++;
        if (partner->HP <= 0) {
            partner->active = false;
        }
    }
}
// 繪製夥伴
void DrawPartner(Partner *partner) {
    if (partner->active) {
        DrawRectangleRec(partner->rec, BLUE);  // 使用不同顏色以便區分
    }
}


void DrawPartnerHealth(const Partner *partner) {
    if (partner->active) {
       DrawText(TextFormat("HP:%03i", partner->HP), 20, 100, 20, GRAY);
    }
}
