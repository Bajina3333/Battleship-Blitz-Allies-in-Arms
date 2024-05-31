#ifndef TYPES_H
#define TYPES_H

#include "raylib.h"
#include "raygui.h" 

// 定義敵人波次的枚舉類型
typedef enum { FIRST = 0, SECOND, THIRD, BOSSWAVE} EnemyWave;
typedef enum {STANDARD, STRONG, BOSS}EnemyType;
typedef enum {EASY, MEDIUM, HARD, BOSSLEVEL} Difficulty;

extern int standard_num;
extern int Strong_num;

typedef struct Player{
    int HP;
    int AttackPower;
    Rectangle rec;
    Vector2 speed;
    Color color;
} Player;

// 敵人結構(Boss)
/*typedef struct Boss{
    Rectangle rec;
    bool active;
    Color color;
    int HP;
    int Atk;
    float frequency;
}Boss;*/

//敵人結構
typedef struct Enemy{
    Rectangle rec;
    Vector2 speed;
    bool active;
    int AttackPower;
    Color color;
    int HP;
    float frequency;
    EnemyType type;
} Enemy;

// 射擊結構
typedef struct Shoot {
    Rectangle rec;
    Vector2 speed;
    bool active;
    Color color;
} Shoot;

typedef enum {
    PARTNER_TYPE_ONE,
    PARTNER_TYPE_TWO,
    PARTNER_TYPE_THREE
} PartnerType;

typedef struct Partner {
    Rectangle rec;
    int HP;
    int maxHealth;
    int AttackPower;
    float attackRate;
    bool active;
    float attackTimer;
    float shootTimer;  // 跟踪下一次射擊的計時器
    float lastShootTime;  // 上一次射擊的時間
    PartnerType type;  // 添加一个类型字段
} Partner;

typedef struct Star_choose{ //Use 1, 2, 3 represent how many stars are in level
    int level_1;
    int level_2;
    int level_3;
}Star_choose;

///////////////////////////////////////
// 其他共享結構和宣告 ...


#endif // TYPES_H
