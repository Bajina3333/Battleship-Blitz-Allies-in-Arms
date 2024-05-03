#include "partner.h"


// 初始化夥伴
// void InitPartner(Partner *partner, Vector2 playerPosition) {
    // partner->rec.width = 20;
    // partner->rec.height = 20;
    // partner->rec.x = playerPosition.x - 30;  // 初始位置在玩家旁邊
    // partner->rec.y = playerPosition.y + 20;
    // partner->health = 90;  // 可以根據具體的夥伴類型設定不同的數值
    // partner->attackPower = 25;
    // partner->attackRate = 1.0f;
    // partner->active = true;
    // partner->attackTimer = 0.0f;
// }
void InitPartner(Partner *partner, Vector2 playerPosition, PartnerType type) {
    partner->rec.width = 20;
    partner->rec.height = 20;
    partner->rec.x = playerPosition.x - 30;  // 初始位置在玩家旁边
    partner->rec.y = playerPosition.y + 20;
    partner->type = type;  // 设置类型
    partner->active = true;
    partner->attackTimer = 0.0f;

    switch (type) {
        case PARTNER_TYPE_ONE:
            partner->health = 90;
            partner->attackPower = 25;
            partner->attackRate = 1.0f;
            break;
        case PARTNER_TYPE_TWO:
            partner->health = 130;
            partner->attackPower = 15;
            partner->attackRate = 1.0f;
            break;
        case PARTNER_TYPE_THREE:
            partner->health = 50;
            partner->attackPower = 35;
            partner->attackRate = 1.0f;
            break;
    }
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


// PartnerType SelectPartnerType() {
    // int partnerType = 0;
    // const char *partnerTypes[] = { "Type 1", "Type 2", "Type 3" };

    // while (!WindowShouldClose()) {  // 确保在选择前用户不关闭窗口
        // BeginDrawing();
        // ClearBackground(RAYWHITE);
        // DrawText("Select your partner type:", 190, 200, 20, BLACK);

        // for (int i = 0; i < 3; i++) {
            // if (GuiButton((Rectangle){ 220, 250 + 50 * i, 120, 40 }, partnerTypes[i])) {
                // partnerType = i;
                // break;
            // }
        // }

        // if (partnerType != 0) break;  // 当用户做出选择后退出循环

        // EndDrawing();
    // }

    // return (PartnerType)partnerType;
// }

