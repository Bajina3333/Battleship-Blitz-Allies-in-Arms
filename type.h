#ifndef TYPES_H
#define TYPES_H

#include "raylib.h"

// 定義敵人波次的枚舉類型
typedef enum { FIRST = 0, SECOND, THIRD } EnemyWave;

// 玩家結構
typedef struct Player {
    Rectangle rec;
    Vector2 speed;
    Color color;
} Player;

// 敵人結構
typedef struct Enemy {
    Rectangle rec;
    Vector2 speed;
    bool active;
    Color color;
} Enemy;

// 射擊結構
typedef struct Shoot {
    Rectangle rec;
    Vector2 speed;
    bool active;
    Color color;
} Shoot;

// 夥伴結構
typedef struct Partner {
    Rectangle rec;
    int health;
    int attackPower;
    float attackRate;
    bool active;
    float attackTimer;
} Partner;

// 其他共享結構和宣告 ...

#endif // TYPES_H