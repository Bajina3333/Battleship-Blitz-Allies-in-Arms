#include "partner.h"


void InitPartner(Partner *partner, Vector2 playerPosition, PartnerType type) {
    partner->rec.width = 20;
    partner->rec.height = 20;
    partner->rec.x = playerPosition.x - 30;  // 初始位置在玩家旁边
    partner->rec.y = playerPosition.y + 20;
    partner->type = type;  // 设置类型
    partner->active = true;
    // partner->attackTimer = 0.0f; // ?
    partner->hitCooldown = 0.5f;
    partner->lastHitTime = -partner->hitCooldown;  // 初始设置确保一开始可以被攻击

    switch (type) {
        case PARTNER_TYPE_ONE:
            partner->health = 90;
            partner->maxHealth = 90;
            partner->attackPower = 25;
            partner->attackRate = 1.0f;
            break;
        case PARTNER_TYPE_TWO:
            partner->health = 130;
            partner->maxHealth = 130;
            partner->attackPower = 15;
            partner->attackRate = 1.0f;
            break;
        case PARTNER_TYPE_THREE:
            partner->health = 50;
            partner->maxHealth = 50;
            partner->attackPower = 35;
            partner->attackRate = 1.0f;
            break;
    }
}

void PartnerShoot() {
    
}

void UpdatePartner(Partner *partner, Vector2 playerPosition) {
    if (!partner->active) return;

    // 更新伙伴位置
    partner->rec.x = playerPosition.x - 30;
    partner->rec.y = playerPosition.y + 20;
}

bool CheckPartnerCollisionRecs(Partner *partner, Enemy *enemy, float currentTime) {
    if (CheckCollisionRecs(partner->rec, enemy->rec)) {
        if (currentTime - partner->lastHitTime > partner->hitCooldown) {
            partner->health -= (enemy->attackPower);
            partner->lastHitTime = currentTime;
            if (partner->health <= 0) {
                partner->active = false;
                return false;
            }
            return true;
        }
    }
    return false;
}
// 繪製夥伴
void DrawPartner(Partner *partner) {
    if (partner->active) {
        DrawRectangleRec(partner->rec, BLUE);  // 使用不同顏色以便區分
    }
}

void DrawPartnerHealth(const Partner *partner) {
    if (partner->active) {
        int healthBarWidth = (int)(200.0f * ((float)(partner->health) / (partner->maxHealth)));
        DrawRectangle(10, 10, healthBarWidth, 20, RED);
        DrawText(TextFormat("%d", partner->health), 220, 10, 20, BLACK);
    }
}
