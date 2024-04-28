#ifndef PARTNER_H
#define PARTNER_H

#include "raylib.h"
#include "type.h"

// 夥伴結構定義
// typedef struct Partner {
    // Rectangle rec;
    // int health;
    // int attackPower;
    // float attackRate;
    // bool active;
    // float attackTimer;
// } Partner;

// 夥伴相關函數宣告
void InitPartner(Partner *partner, Vector2 playerPosition);
void UpdatePartner(Partner *partner, Vector2 playerPosition);
void DrawPartner(Partner *partner);
bool CheckPartnerCollisionRecs(Partner *partner, Enemy *enemy);

#endif
