#include "raylib.h"
#include "type.h"
#include "partner.h"
#include "game_system.h"
#include <stdbool.h>

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Some Defines
//----------------------------------------------------------------------------------
#define NUM_SHOOTS 50
#define NUM_MAX_ENEMIES 50
#define EASY_WAVE 30
#define MEDIUM_WAVE 50
#define HARD_WAVE 70
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
//------------------------------------------------------------------------------------
static const int screenWidth = 1500;
static const int screenHeight = 900;

static bool gameOver = false;
static bool pause =  false;
static int score = 0; //enemies percentage is converted into scores
static bool victory = false;

static Player player = { 0 };
static Partner partner = { 0 };
static Enemy enemy[NUM_MAX_ENEMIES] = { 0 };
static Shoot shoot[NUM_SHOOTS] = { 0 };
static Shoot partner_shoot[NUM_SHOOTS] = { 0 };
static EnemyWave wave = { 0 };
static Star_choose star_choose = {0};
static Difficulty difficulty = EASY;

static int shootRate = 0;
static float alpha = 0.0f; //alpha的值範圍是 0.0 到 1.0，其中 0.0 表示完全透明，1.0 表示完全不透明，用來逐步增加或減少文字的透明度，實現文字淡入淡出的效果


static int totalEnemies = 0;//The number of enemies should be killed in the level
static int activeEnemies = 0; //The number of enemies at that level
static int total_count_Enemies = 0; //The number of enemies actuall appeared in the level
static int enemiesKill = 0; //The number of enemies in the level
static bool smooth = false;

int gold = 0; //Golds held by the player
int final_score = 0; //The score at the end of the game
int add_score = 0; //Convert drops and equipments to scores
bool store_enter = false; //Judge whether or not to execute the program of store with true or false

static float stopTimer = 0.0f;
static bool stopForScore = false;
static float scoreAlpha = 0.0f;
static bool displayScoreTime = false;

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
    InitWindow(screenWidth, screenHeight, "Battleship Blitz: Allies in Arms"); //change

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
    totalEnemies = EASY_WAVE;
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

    // Initialize score_system
    gold = 0;
    final_score = 0;
    add_score = 0;
    store_enter = false;
    star_choose.level_1 = 0;
    star_choose.level_2 = 0;
    star_choose.level_3 = 0;
    
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
            enemy[i].type = STANDARD; //enemy似乎沒有type成員
            enemy[i].speed.x = 5;
            enemy[i].speed.y = 5;
            enemy[i].HP = 100;
            enemy[i].AttackPower = 10;
            enemy[i].color = GRAY;
        } 
        else if (randType == 1) {
            enemy[i].type = STRONG; //enemy似乎沒有type成員
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
// Update game (one frame)
void UpdateGame(void)
{
    float deltaTime = GetFrameTime(); // Get time for the last frame to smooth animations and calculations

    if (!gameOver)
    {
        if (IsKeyPressed('P')) pause = !pause;

        if (!pause)
        {
            if (stopForScore) 
            {
                stopTimer += deltaTime;
                scoreAlpha += 0.02f;

                if (stopTimer >= 2.0f) 
                {
                    stopForScore = false;
                    stopTimer = 0.0f;
                    scoreAlpha = 0.0f;
                    
                    if(difficulty == EASY){
                        if (wave == FIRST) 
                        {
                            activeEnemies = SECOND_WAVE_1;
                            wave = SECOND;
                            
                        } 
                        else if (wave == SECOND) 
                        {
                            activeEnemies = THIRD_WAVE_1;
                            wave = THIRD;
                            
                        } 
                        else if (wave == THIRD) 
                        {
                            activeEnemies = FIRST_WAVE_2;
                            difficulty = MEDIUM;
                            wave = FIRST;
                            
                        }
                    }
                    if(difficulty == MEDIUM){
                        if (wave == FIRST) 
                        {
                            wave = SECOND;
                            activeEnemies = SECOND_WAVE_2;
                        } 
                        else if (wave == SECOND) 
                        {
                            wave = THIRD;
                            activeEnemies = THIRD_WAVE_2;
                        } 
                        else if (wave == THIRD) 
                        {
                            wave = FIRST;
                            activeEnemies = FIRST_WAVE_3;
                            difficulty = MEDIUM;
                        }
                    }
                    if(difficulty == HARD){
                        if (wave == FIRST) 
                        {
                            wave = SECOND;
                            activeEnemies = SECOND_WAVE_3;
                        } 
                        else if (wave == SECOND) 
                        {
                            wave = THIRD;
                            activeEnemies = THIRD_WAVE_3;
                        } 
                        else if (wave == THIRD) {};
                    }

                    // Reset enemy active state
                    for (int i = 0; i < activeEnemies; i++) 
                    {
                        if (!enemy[i].active) enemy[i].active = true;
                    }

                    // Indicate score time display is complete
                    displayScoreTime = false;
                }
            }
            else if (displayScoreTime) 
            {
                scoreAlpha += 0.02f;

                if (scoreAlpha >= 1.0f) 
                {
                    scoreAlpha = 1.0f;
                    stopForScore = true; // Initiate stopForScore to start fade-out
                }
            }
            else 
            {   
                if(difficulty == EASY){
                    switch (wave)
                    {
                        case FIRST:
                        {
                            totalEnemies = EASY_WAVE; 
                            if (!smooth)
                            {
                                alpha += 0.02f;

                                if (alpha >= 1.0f) smooth = true;
                            }

                            if (smooth) alpha -= 0.02f;

                        } break;
                        case SECOND:
                        {
                            totalEnemies = EASY_WAVE;
                            if (!smooth)
                            {
                                alpha += 0.02f;

                                if (alpha >= 1.0f) smooth = true;
                            }

                            if (smooth) alpha -= 0.02f;

                        } break;
                        case THIRD:
                        {
                            totalEnemies = EASY_WAVE;
                            if (!smooth)
                            {
                                alpha += 0.02f;

                                if (alpha >= 1.0f) smooth = true;
                            }

                            if (smooth) alpha -= 0.02f;


                        } break;
                        default: break;
                    }
                }
                if(difficulty == MEDIUM){
                    switch (wave)
                    {
                        case FIRST:
                        {
                            totalEnemies = MEDIUM_WAVE;
                            if (!smooth)
                            {
                                alpha += 0.02f;

                                if (alpha >= 1.0f) smooth = true;
                            }

                            if (smooth) alpha -= 0.02f;

                        } break;
                        case SECOND:
                        {
                            totalEnemies = MEDIUM_WAVE;
                            if (!smooth)
                            {
                                alpha += 0.02f;

                                if (alpha >= 1.0f) smooth = true;
                            }

                            if (smooth) alpha -= 0.02f;

                        } break;
                        case THIRD:
                        {
                            totalEnemies = MEDIUM_WAVE;
                            if (!smooth)
                            {
                                alpha += 0.02f;

                                if (alpha >= 1.0f) smooth = true;
                            }
                            if (smooth) alpha -= 0.02f;

                        } break;
                        default: break;
                    }
                }
                if(difficulty == HARD){
                    switch (wave)
                    {
                        case FIRST:
                        {
                            totalEnemies = HARD_WAVE;
                            if (!smooth)
                            {
                                alpha += 0.02f;

                                if (alpha >= 1.0f) smooth = true;
                            }

                            if (smooth) alpha -= 0.02f;

                        } break;
                        case SECOND:
                        {
                            totalEnemies = HARD_WAVE;
                            if (!smooth)
                            {
                                alpha += 0.02f;

                                if (alpha >= 1.0f) smooth = true;
                            }

                            if (smooth) alpha -= 0.02f;

                        } break;
                        case THIRD:
                        {
                            totalEnemies = HARD_WAVE;
                            if (!smooth)
                            {
                                alpha += 0.02f;

                                if (alpha >= 1.0f) smooth = true;
                            }

                            if (smooth) alpha -= 0.02f;

                        } break;
                        default: break;
                    }
                }
                // Regular game update logic here...

                // Player movement
                if (IsKeyDown(KEY_RIGHT)) player.rec.x += player.speed.x;
                if (IsKeyDown(KEY_LEFT)) player.rec.x -= player.speed.x;
                if (IsKeyDown(KEY_UP)) player.rec.y -= player.speed.y;
                if (IsKeyDown(KEY_DOWN)) player.rec.y += player.speed.y;

                // Partner movement
                UpdatePartner(&partner, (Vector2){ player.rec.x, player.rec.y });
                PartnerShoot(&partner, &partner_shoot, deltaTime, NUM_SHOOTS, &enemy, NUM_MAX_ENEMIES, &score, &enemiesKill);

                // Player collision with enemy
                for (int i = 0; i < activeEnemies; i++)
                {
                    if (CheckCollisionRecs(player.rec, enemy[i].rec)) 
                    {
                        player.HP -= enemy[i].AttackPower;
                        enemy[i].active = false;
                        enemy[i].rec.x = GetRandomValue(-100, -1);
                        enemy[i].rec.y = GetRandomValue(-100, -1);
                        enemiesKill++;
                        total_count_Enemies++;
                    }
                    CheckPartnerCollisionRecs(&partner, &enemy[i], &enemiesKill);
                }

                if(player.HP <= 0) gameOver = true;

                // Enemy behavior
                for (int i = 0; i < activeEnemies; i++)
                {
                    if (enemy[i].active)
                    {
                        enemy[i].rec.x -= enemy[i].speed.x;

                        if (enemy[i].rec.x < 0)
                        {
                            total_count_Enemies++;
                            enemy[i].rec.x = GetRandomValue(screenWidth, screenWidth + 1000);
                            enemy[i].rec.y = GetRandomValue(0, screenHeight - enemy[i].rec.height);
                        }
                    }
                }

                // Wall behavior
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
                                    if (enemy[j].HP <= 0)
                                    {
                                        enemy[j].active = false;
                                        enemy[j].rec.x = GetRandomValue(screenWidth, screenWidth + 1000);  // 假设屏幕宽度为800，敌人从右边出现
                                        enemy[j].rec.y = GetRandomValue(0, screenHeight - enemy[j].rec.height);  // 保持在屏幕高度范围内
                                        enemiesKill++;
                                        score += 100;
                                        total_count_Enemies++;
                                        
                                        // 重新激活敌人，直到总敌人数达到 totalEnemies
                                        if (total_count_Enemies < totalEnemies)
                                        {
                                            enemy[j].active = true;  // 重新激活敌人
                                        }
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

                // Wave transition logic
                if (total_count_Enemies >= totalEnemies  && !displayScoreTime)
                {
                    displayScoreTime = true;
                    scoreAlpha = 0.0f;

                    // Award gold based on performance
                    if(difficulty == EASY){
                        if (wave == FIRST) 
                        {
                            smooth = false;
                            alpha = 0.0f;
                            star_choose.level_1 = LevelStar(enemiesKill, totalEnemies);
                            if(star_choose.level_1 == 3) gold += 300; 
                            else if(star_choose.level_1 == 2) gold += 200;
                            else if(star_choose.level_1 == 1) gold += 100;
                            gold += 200;
                        } 
                        else if (wave == SECOND) 
                        {
                            smooth = false;
                            alpha = 0.0f;
                            star_choose.level_2 = LevelStar(enemiesKill, totalEnemies);
                            if(star_choose.level_2 == 3) gold += 300; 
                            else if(star_choose.level_2 == 2) gold += 200;
                            else if(star_choose.level_2 == 1) gold += 100;
                            gold += 200;
                        } 
                        else if (wave == THIRD) 
                        {
                            smooth = false;
                            alpha = 0.0f;
                            star_choose.level_3 = LevelStar(enemiesKill, totalEnemies);
                            if(star_choose.level_3 == 3) gold += 300; 
                            else if(star_choose.level_3 == 2) gold += 200;
                            else if(star_choose.level_3 == 1) gold += 100;
                            gold += 200;
                        }
                    }
                    if(difficulty == MEDIUM){
                        if (wave == FIRST) 
                        {
                            smooth = false;
                            alpha = 0.0f;
                            star_choose.level_1 = LevelStar(enemiesKill, totalEnemies);
                            if(star_choose.level_1 == 3) gold += 300; 
                            else if(star_choose.level_1 == 2) gold += 200;
                            else if(star_choose.level_1 == 1) gold += 100;
                            gold += 200;
                        } 
                        else if (wave == SECOND) 
                        {
                            smooth = false;
                            alpha = 0.0f;
                            star_choose.level_2 = LevelStar(enemiesKill, totalEnemies);
                            if(star_choose.level_2 == 3) gold += 300; 
                            else if(star_choose.level_2 == 2) gold += 200;
                            else if(star_choose.level_2 == 1) gold += 100;
                            gold += 200;
                        } 
                        else if (wave == THIRD) 
                        {
                            smooth = false;
                            alpha = 0.0f;
                            star_choose.level_3 = LevelStar(enemiesKill, totalEnemies);
                            if(star_choose.level_3 == 3) gold += 300; 
                            else if(star_choose.level_3 == 2) gold += 200;
                            else if(star_choose.level_3 == 1) gold += 100;
                            gold += 200;
                        }
                    }
                    if(difficulty == HARD){
                        if (wave == FIRST) 
                        {
                            smooth = false;
                            alpha = 0.0f;
                            star_choose.level_1 = LevelStar(enemiesKill, totalEnemies);
                            if(star_choose.level_1 == 3) gold += 300; 
                            else if(star_choose.level_1 == 2) gold += 200;
                            else if(star_choose.level_1 == 1) gold += 100;
                            gold += 200;
                        } 
                        else if (wave == SECOND) 
                        {
                            smooth = false;
                            alpha = 0.0f;
                            star_choose.level_2 = LevelStar(enemiesKill, totalEnemies);
                            if(star_choose.level_2 == 3) gold += 300; 
                            else if(star_choose.level_2 == 2) gold += 200;
                            else if(star_choose.level_2 == 1) gold += 100;
                            gold += 200;
                        } 
                        else if (wave == THIRD) 
                        {
                            smooth = false;
                            alpha = 0.0f;
                            star_choose.level_3 = LevelStar(enemiesKill, totalEnemies);
                            if(star_choose.level_3 == 3) gold += 300; 
                            else if(star_choose.level_3 == 2) gold += 200;
                            else if(star_choose.level_3 == 1) gold += 100;
                            gold += 200;
                        }
                    }

                    enemiesKill = 0;
                    total_count_Enemies = 0;

                    // Reset enemies for the next wave
                    for (int i = 0; i < activeEnemies; i++)
                    {
                        enemy[i].active = true;
                        enemy[i].rec.x = GetRandomValue(screenWidth, screenWidth + 1000);
                        enemy[i].rec.y = GetRandomValue(0, screenHeight - enemy[i].rec.height);
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
// Draw game (one frame)
void DrawGame(void)
{
    BeginDrawing();

    ClearBackground(RAYWHITE);
    DrawPartnerHealth(&partner);

    if (!gameOver)
    {
        DrawRectangleRec(player.rec, player.color);
        
        // Draw partner
        DrawPartner(&partner);

        // Draw wave text
        if (difficulty == EASY) DrawText("EASY", screenWidth/2 - MeasureText("EASY", 40)/2, screenHeight/2, 40, Fade(BLACK, alpha));
        else if (difficulty == MEDIUM) DrawText("MEDIUM", screenWidth/2 - MeasureText("MEDIUM", 40)/2, screenHeight/2, 40, Fade(BLACK, alpha));
        else if (difficulty == HARD) DrawText("HARD", screenWidth/2 - MeasureText("HARD", 40)/2, screenHeight/2, 40, Fade(BLACK, alpha));
        if (wave == FIRST) DrawText("FIRST WAVE", screenWidth/2 - MeasureText("FIRST WAVE", 40)/2, screenHeight/2 - 40, 40, Fade(BLACK, alpha));
        else if (wave == SECOND) DrawText("SECOND WAVE", screenWidth/2 - MeasureText("SECOND WAVE", 40)/2, screenHeight/2 - 40, 40, Fade(BLACK, alpha));
        else if (wave == THIRD) DrawText("THIRD WAVE", screenWidth/2 - MeasureText("THIRD WAVE", 40)/2, screenHeight/2 - 40, 40, Fade(BLACK, alpha));
        
        
        // Draw score time message
        if (displayScoreTime) 
        {
            DrawText("SCORE TIME", screenWidth/2 - MeasureText("SCORE TIME", 40)/2, screenHeight/2 - 40, 40, Fade(BLACK, scoreAlpha));
        }

        for (int i = 0; i < activeEnemies; i++)
        {
            if (enemy[i].active) DrawRectangleRec(enemy[i].rec, enemy[i].color);
        }

        for (int i = 0; i < NUM_SHOOTS; i++)
        {
            if (shoot[i].active) DrawRectangleRec(shoot[i].rec, shoot[i].color);
            
            if (partner_shoot[i].active) 
            {
                DrawRectangleRec(partner_shoot[i].rec, partner_shoot[i].color);
            }
        }

        DrawText(TextFormat("enemiesKill:%04i", enemiesKill), 20, 20, 20, GRAY);
        DrawText(TextFormat("total_count_Enemies:%04i", total_count_Enemies), 20, 40, 20, GRAY);
        DrawText(TextFormat("HP:%03i", player.HP), 20, 80, 20, GRAY);
        DrawText(TextFormat("gold:%04i", gold), 500, 20, 20, GRAY);

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