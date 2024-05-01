#include "partner.h"


// 初始化夥伴
void InitPartner(Partner *partner, Vector2 playerPosition) {
    partner->rec.width = 20;
    partner->rec.height = 20;
    partner->rec.x = playerPosition.x - 30;  // 初始位置在玩家旁邊
    partner->rec.y = playerPosition.y + 20;
    partner->health = 90;  // 可以根據具體的夥伴類型設定不同的數值
    partner->attackPower = 25;
    partner->attackRate = 1.0f;
    partner->active = true;
    partner->attackTimer = 0.0f;
}

// 更新夥伴位置和攻擊
void UpdatePartner(Partner *partner, Vector2 playerPosition) {
    if (partner->active) {
        partner->rec.x = playerPosition.x - 30;  // 始終跟隨玩家
        partner->rec.y = playerPosition.y + 20;

        partner->attackTimer += GetFrameTime();
        if (partner->attackTimer >= partner->attackRate) {
            // 觸發攻擊邏輯
            partner->attackTimer = 0.0f;
        }
    }
}

bool CheckPartnerCollisionRecs(Partner *partner, Enemy *enemy) {
    if (CheckCollisionRecs(partner->rec, enemy->rec)) {
        // partner->health -= enemy.attackPower;
        return true;
    }
    return false;
}
// 繪製夥伴
void DrawPartner(Partner *partner) {
    if (partner->active) {
        DrawRectangleRec(partner->rec, BLUE);  // 使用不同顏色以便區分
    }
}
