#include "raylib.h"
#include "type.h"
#include "partner.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Some Defines
//----------------------------------------------------------------------------------
#define NUM_SHOOTS 50
#define NUM_MAX_ENEMIES 50
#define FIRST_WAVE_1 12
#define SECOND_WAVE_1 18
#define THIRD_WAVE_1 36
#define FIRST_WAVE_2 18
#define SECOND_WAVE_2 30
#define THIRD_WAVE_2 42
#define FIRST_WAVE_3 24
#define SECOND_WAVE_3 36
#define THIRD_WAVE_3 48
//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
// typedef enum { FIRST , SECOND, THIRD } EnemyWave;

// typedef struct Player{
    // int HP;
    // int AttackPower;
    // Rectangle rec;
    // Vector2 speed;
    // Color color;
// } Player;

// typedef struct Enemy{
    // int HP;
    // int AttackPower;
    // Rectangle rec;
    // Vector2 speed;
    // bool active;
    // Color color;
// } Enemy;

// typedef struct Shoot{
    // Rectangle rec;
    // Vector2 speed;
    // bool active;
    // Color color;
// } Shoot;

//------------------------------------------------------------------------------------
// Global Variables Declaration

typedef enum {EASY, MEDIUM, HARD} Difficulty;
static const int screenWidth = 1500;
static const int screenHeight = 900;

static bool gameOver = false;
static bool pause =  false;
static int score = 0;
static bool victory = false;

static Player player = { 0 };
static Partner partner = { 0 };
static Enemy enemy[NUM_MAX_ENEMIES] = { 0 };
static Shoot shoot[NUM_SHOOTS] = { 0 };
static Shoot partner_shoot[NUM_SHOOTS] = { 0 };
static EnemyWave wave = { 0 };

static int shootRate = 0;
static float alpha = 0.0f;

static int activeEnemies = 0;
static int enemiesKill = 0;
static bool smooth = false;
Difficulty difficulty = EASY;

//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
static void InitGame(void);         // Initialize game
static void UpdateGame(void);       // Update game (one frame)
static void DrawGame(void);         // Draw game (one frame)
static void UnloadGame(void);       // Unload game
static void UpdateDrawFrame(void);  // Update and Draw (one frame)

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization (Note windowTitle is unused on Android)
    //---------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "classic game: space invaders");

    InitGame();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update and Draw
        //----------------------------------------------------------------------------------
        UpdateDrawFrame();
        //----------------------------------------------------------------------------------
    }
#endif
    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadGame();         // Unload loaded data (textures, sounds, models...)

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//------------------------------------------------------------------------------------
// Module Functions Definitions (local)
//------------------------------------------------------------------------------------
// Initialize game variables
void InitGame(void)
{
    // Initialize game variables
    shootRate = 0;
    pause = false;
    gameOver = false;
    victory = false;
    smooth = false;
    wave = FIRST;
    activeEnemies = FIRST_WAVE_1;
    enemiesKill = 0;
    score = 0;
    alpha = 0;

    // Initialize player
    player.HP = 100;
    player.AttackPower = 20;
    player.rec.x =  10;
    player.rec.y = screenHeight / 2;
    player.rec.width = 20;
    player.rec.height = 20;
    player.speed.x = 8;
    player.speed.y = 8;
    player.color = RED;

    // Initialize partner
    int chosenType = 0;
    InitPartner(&partner, (Vector2){player.rec.x, player.rec.y}, chosenType);


    // Initialize enemies
       for (int i = 0; i < NUM_MAX_ENEMIES; i++)
    {
        enemy[i].rec.width = 20;
        enemy[i].rec.height = 20;
        enemy[i].rec.x = GetRandomValue(screenWidth, screenWidth + 1000);
        enemy[i].rec.y = GetRandomValue(0, screenHeight - enemy[i].rec.height);
        enemy[i].active = true;

        // 隨機選擇敵人類型
        int randType = GetRandomValue(0, 1);
        if (randType == 0) {
            enemy[i].type = STANDARD;
            enemy[i].speed.x = 5;
            enemy[i].speed.y = 5;
            enemy[i].HP = 100;
            enemy[i].AttackPower = 10;
            enemy[i].color = GRAY;
        } 
         else if (randType == 1) {
            enemy[i].type = STRONG;
            enemy[i].speed.x = 3;
            enemy[i].speed.y = 3;
            enemy[i].HP = 200;
            enemy[i].AttackPower = 20;
            enemy[i].color = GREEN;
            }
    }

    // Initialize shoots
    for (int i = 0; i < NUM_SHOOTS; i++)
    {
        shoot[i].rec.x = player.rec.x;
        shoot[i].rec.y = player.rec.y + player.rec.height/4;
        shoot[i].rec.width = 10;
        shoot[i].rec.height = 5;
        shoot[i].speed.x = 7;
        shoot[i].speed.y = 0;
        shoot[i].active = false;
        shoot[i].color = MAROON;
    }
    // Initialize partner shoots
    InitPartnerShoot(&partner, &partner_shoot, NUM_SHOOTS);
}

// Update game (one frame)
void UpdateGame(void)
{
    float deltaTime = GetFrameTime(); // 獲取上一幀的時間，用於平滑動畫和計算
    if (!gameOver)
    {
        if (IsKeyPressed('P')) pause = !pause;
        if (!pause)
        {   
            if(difficulty == EASY){
            switch (wave)
            {
                case FIRST:
                {
                    if (!smooth)
                    {
                        alpha += 0.02f;

                        if (alpha >= 1.0f) smooth = true;
                    }

                    if (smooth) alpha -= 0.02f;

                    if (enemiesKill == activeEnemies)
                    {
                        enemiesKill = 0;

                        for (int i = 0; i < activeEnemies; i++)
                        {
                            if (!enemy[i].active) enemy[i].active = true;
                        }

                        activeEnemies = SECOND_WAVE_1;
                        wave = SECOND;
                        smooth = false;
                        alpha = 0.0f;
                    }
                } break;
                case SECOND:
                {
                    if (!smooth)
                    {
                        alpha += 0.02f;

                        if (alpha >= 1.0f) smooth = true;
                    }

                    if (smooth) alpha -= 0.02f;

                    if (enemiesKill == activeEnemies)
                    {
                        enemiesKill = 0;

                        for (int i = 0; i < activeEnemies; i++)
                        {
                            if (!enemy[i].active) enemy[i].active = true;
                        }

                        activeEnemies = THIRD_WAVE_1;
                        wave = THIRD;
                        smooth = false;
                        alpha = 0.0f;
                    }
                } break;
                case THIRD:
                {
                    if (!smooth)
                    {
                        alpha += 0.02f;

                        if (alpha >= 1.0f) smooth = true;
                    }

                    if (smooth) alpha -= 0.02f;

                    if (enemiesKill == activeEnemies)
                    {
                        enemiesKill = 0;

                        for (int i = 0; i < activeEnemies; i++)
                        {
                            if (!enemy[i].active) enemy[i].active = true;
                        }

                        activeEnemies = FIRST_WAVE_2;
                        wave = FIRST;
                        smooth = false;
                        alpha = 0.0f;
                        difficulty = MEDIUM;
                    }

                } break;
                default: break;
            }
        }
         if(difficulty == MEDIUM){
            switch (wave)
            {
                case FIRST:
                {
                    if (!smooth)
                    {
                        alpha += 0.02f;

                        if (alpha >= 1.0f) smooth = true;
                    }

                    if (smooth) alpha -= 0.02f;

                    if (enemiesKill == activeEnemies)
                    {
                        enemiesKill = 0;

                        for (int i = 0; i < activeEnemies; i++)
                        {
                            if (!enemy[i].active) enemy[i].active = true;
                        }

                        activeEnemies = SECOND_WAVE_2;
                        wave = SECOND;
                        smooth = false;
                        alpha = 0.0f;
                    }
                } break;
                case SECOND:
                {
                    if (!smooth)
                    {
                        alpha += 0.02f;

                        if (alpha >= 1.0f) smooth = true;
                    }

                    if (smooth) alpha -= 0.02f;

                    if (enemiesKill == activeEnemies)
                    {
                        enemiesKill = 0;

                        for (int i = 0; i < activeEnemies; i++)
                        {
                            if (!enemy[i].active) enemy[i].active = true;
                        }

                        activeEnemies = THIRD_WAVE_2;
                        wave = THIRD;
                        smooth = false;
                        alpha = 0.0f;
                    }
                } break;
                case THIRD:
                {
                    if (!smooth)
                    {
                        alpha += 0.02f;

                        if (alpha >= 1.0f) smooth = true;
                    }
                    if (smooth) alpha -= 0.02f;

                    if (enemiesKill == activeEnemies)
                    {
                        enemiesKill = 0;

                        for (int i = 0; i < activeEnemies; i++)
                        {
                            if (!enemy[i].active) enemy[i].active = true;
                        }

                        activeEnemies = FIRST_WAVE_3;
                        wave = FIRST;
                        smooth = false;
                        alpha = 0.0f;
                        difficulty = HARD;
                    }

                } break;
                default: break;
            }
        }
         if(difficulty == HARD){
            switch (wave)
            {
                case FIRST:
                {
                    if (!smooth)
                    {
                        alpha += 0.02f;

                        if (alpha >= 1.0f) smooth = true;
                    }

                    if (smooth) alpha -= 0.02f;

                    if (enemiesKill == activeEnemies)
                    {
                        enemiesKill = 0;

                        for (int i = 0; i < activeEnemies; i++)
                        {
                            if (!enemy[i].active) enemy[i].active = true;
                        }

                        activeEnemies = SECOND_WAVE_3;
                        wave = SECOND;
                        smooth = false;
                        alpha = 0.0f;
                    }
                } break;
                case SECOND:
                {
                    if (!smooth)
                    {
                        alpha += 0.02f;

                        if (alpha >= 1.0f) smooth = true;
                    }

                    if (smooth) alpha -= 0.02f;

                    if (enemiesKill == activeEnemies)
                    {
                        enemiesKill = 0;

                        for (int i = 0; i < activeEnemies; i++)
                        {
                            if (!enemy[i].active) enemy[i].active = true;
                        }

                        activeEnemies = THIRD_WAVE_3;
                        wave = THIRD;
                        smooth = false;
                        alpha = 0.0f;
                    }
                } break;
                case THIRD:
                {
                    if (!smooth)
                    {
                        alpha += 0.02f;

                        if (alpha >= 1.0f) smooth = true;
                    }

                    if (smooth) alpha -= 0.02f;

                    if (enemiesKill == activeEnemies) victory = true;

                } break;
                default: break;
            }
        }

            // Player movement
            if (IsKeyDown(KEY_RIGHT)) player.rec.x += player.speed.x;
            if (IsKeyDown(KEY_LEFT)) player.rec.x -= player.speed.x;
            if (IsKeyDown(KEY_UP)) player.rec.y -= player.speed.y;
            if (IsKeyDown(KEY_DOWN)) player.rec.y += player.speed.y;

            // Partner movement
            UpdatePartner(&partner, (Vector2){ player.rec.x, player.rec.y });
             // 更新射擊
            // PartnerShoot(&partner, &partner_shoot, deltaTime, NUM_SHOOTS); 
            PartnerShoot(&partner, &partner_shoot, deltaTime, NUM_SHOOTS, &enemy, NUM_MAX_ENEMIES, &score, &enemiesKill);

            // Get Time to avoid 
            // float currentTime = GetTime();

            // Player collision with enemy
            for (int i = 0; i < activeEnemies; i++)
            {
                if (CheckCollisionRecs(player.rec, enemy[i].rec)) {
                    player.HP -= enemy[i].AttackPower;
                    enemy[i].active = false;
                    enemy[i].rec.x = GetRandomValue(-100, -1);
                    enemy[i].rec.y = GetRandomValue(-100, -1);
                    enemiesKill++;
                }
                CheckPartnerCollisionRecs(&partner, &enemy[i], &enemiesKill);
            }
            if(player.HP <= 0)gameOver = true;
            
             // Enemy behaviour
            for (int i = 0; i < activeEnemies; i++)
            {
                if (enemy[i].active)
                {
                    enemy[i].rec.x -= enemy[i].speed.x;

                    if (enemy[i].rec.x < 0)
                    {
                        /*enemy[i].rec.x = GetRandomValue(screenWidth, screenWidth + 1000);
                        enemy[i].rec.y = GetRandomValue(0, screenHeight - enemy[i].rec.height);*/
                        enemy[i].active = false;  // Deactivate the enemy
                        enemiesKill++;  // Increment the enemiesKill counter
                    
                    }
                }
            }

            // Wall behaviour
            if (player.rec.x <= 0) player.rec.x = 0;
            if (player.rec.x + player.rec.width >= screenWidth) player.rec.x = screenWidth - player.rec.width;
            if (player.rec.y <= 0) player.rec.y = 0;
            if (player.rec.y + player.rec.height >= screenHeight) player.rec.y = screenHeight - player.rec.height;

            
            // Shoot initialization
            if (IsKeyDown(KEY_SPACE))
            {
                shootRate += 5;

                for (int i = 0; i < NUM_SHOOTS; i++)
                {
                    if (!shoot[i].active && shootRate%20 == 0)
                    {
                        shoot[i].rec.x = player.rec.x;
                        shoot[i].rec.y = player.rec.y + player.rec.height/4;
                        shoot[i].active = true;
                        break;
                    }
                }
            }

            // Shoot logic
            for (int i = 0; i < NUM_SHOOTS; i++)
            {
                if (shoot[i].active)
                {
                    // Movement
                    shoot[i].rec.x += shoot[i].speed.x;

                    // Collision with enemy
                    for (int j = 0; j < activeEnemies; j++)
                    {
                        if (enemy[j].active)
                        {
                            if (CheckCollisionRecs(shoot[i].rec, enemy[j].rec))
                            {
                                shoot[i].active = false;
                                shootRate = 0;
                                enemy[j].HP -= player.AttackPower;
                                    if(enemy[j].HP <= 0)
                                    {   
                                        enemy[j].active = false;
                                        enemy[j].rec.x = GetRandomValue(-100, -1);
                                        enemy[j].rec.y = GetRandomValue(-100, -1);
                                        enemiesKill++;
                                        score += 100;
                                    }                            
                            }
                            if (shoot[i].rec.x + shoot[i].rec.width >= screenWidth)
                            {
                                shoot[i].active = false;
                                shootRate = 0;
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            InitGame();
            gameOver = false;
        }
    }
}

// Draw game (one frame)
void DrawGame(void)
{
    BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawPartnerHealth(&partner);

        if (!gameOver)
        {
            DrawRectangleRec(player.rec, player.color);
            
            // draw partner
            DrawPartner(&partner);

            if (wave == FIRST) DrawText("FIRST WAVE", screenWidth/2 - MeasureText("FIRST WAVE", 40)/2, screenHeight/2 - 40, 40, Fade(BLACK, alpha));
            else if (wave == SECOND) DrawText("SECOND WAVE", screenWidth/2 - MeasureText("SECOND WAVE", 40)/2, screenHeight/2 - 40, 40, Fade(BLACK, alpha));
            else if (wave == THIRD) DrawText("THIRD WAVE", screenWidth/2 - MeasureText("THIRD WAVE", 40)/2, screenHeight/2 - 40, 40, Fade(BLACK, alpha));
            if (difficulty == EASY) DrawText("EASY", screenWidth/2 - MeasureText("EASY", 40)/2, screenHeight/2, 40, Fade(BLACK, alpha));
            else if (difficulty == MEDIUM) DrawText("MEDIUM", screenWidth/2 - MeasureText("MEDIUM", 40)/2, screenHeight/2, 40, Fade(BLACK, alpha));
            else if (difficulty == HARD) DrawText("HARD", screenWidth/2 - MeasureText("HARD", 40)/2, screenHeight/2, 40, Fade(BLACK, alpha));

            for (int i = 0; i < activeEnemies; i++)
            {
                if (enemy[i].active) DrawRectangleRec(enemy[i].rec, enemy[i].color);
            }

            for (int i = 0; i < NUM_SHOOTS; i++)
            {
                if (shoot[i].active) DrawRectangleRec(shoot[i].rec, shoot[i].color);
                
                if (partner_shoot[i].active) {
                    DrawRectangleRec(partner_shoot[i].rec, partner_shoot[i].color);
                }
            }

            DrawText(TextFormat("score:%04i", score), 20, 20, 20, GRAY);
            DrawText(TextFormat("HP:%03i", player.HP), 20, 80, 20, GRAY);

            if (victory) DrawText("YOU WIN", screenWidth/2 - MeasureText("YOU WIN", 40)/2, screenHeight/2 - 40, 40, BLACK);

            if (pause) DrawText("GAME PAUSED", screenWidth/2 - MeasureText("GAME PAUSED", 40)/2, screenHeight/2 - 40, 40, GRAY);
        }
        else DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth()/2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20)/2, GetScreenHeight()/2 - 50, 20, GRAY);

    EndDrawing();
}

// Unload game variables
void UnloadGame(void)
{
    // TODO: Unload all dynamic loaded data (textures, sounds, models...)
}

// Update and Draw (one frame)
void UpdateDrawFrame(void)
{
    UpdateGame();
    DrawGame();
}
