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
#define EASY_WAVE 40
#define MEDIUM_WAVE 70
#define HARD_WAVE 100
#define FIRST_WAVE_1 12
#define SECOND_WAVE_1 24
#define THIRD_WAVE_1 36
#define FIRST_WAVE_2 18
#define SECOND_WAVE_2 30
#define THIRD_WAVE_2 42
#define FIRST_WAVE_3 24
#define SECOND_WAVE_3 36
#define THIRD_WAVE_3 48
#define BOSS_1 1

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
static Shoot enemy_shoot[NUM_MAX_ENEMIES][NUM_SHOOTS] = { {{0}} };
static float enemy_cooltime[NUM_MAX_ENEMIES] = { 0 };
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
int displayenemiesKill = 0;
int displaytotalEnemies = 0;
int displaygold = 0;

static float stopTimer = 0.0f;
static bool stopForScore = false;
static float scoreAlpha = 0.0f;
static bool displayScoreTime = false;

static Texture2D background_ocean = { 0 };
static Texture2D background_city = { 0 };
static Texture2D background_forest = { 0 };
static Texture2D background_hell = { 0 };

static Texture2D normal = { 0 };
static Texture2D strong = { 0 };
static Texture2D boss = { 0 };

static Texture2D armor = { 0 };
static Texture2D artillery = { 0 };
static Texture2D cure = { 0 };
static Texture2D energy_beam = { 0 };
static Texture2D machine_gun = { 0 };
static Texture2D missile = { 0 };
static Texture2D plasma_cannon = { 0 };
static Texture2D thruster = { 0 };
static Texture2D upgrade = { 0 };

static Texture2D drumstick = { 0 };
static Texture2D first_aid_kit = { 0 };
static Texture2D fix_tool = { 0 };
static Texture2D invincible_stars = { 0 };
static Texture2D iron_shield = { 0 };
static Texture2D radio_jammer = { 0 };
static Texture2D shock_wave = { 0 };

static Texture2D partner_1 = { 0 };
static Texture2D partner_2 = { 0 };
static Texture2D partner_3 = { 0 };

static Texture2D player_1 = { 0 };
static Texture2D player_2 = { 0 };
static Texture2D player_3 = { 0 };

static Texture2D FullStar = { 0 };
static Texture2D TwoStar = { 0 };
static Texture2D OneStar = { 0 };


//------------------------------------------------------------------------------------
// Module Functions Declaration (local) 
//------------------------------------------------------------------------------------
static void InitGame(void);         // Initialize game
static void UpdateGame(void);       // Update game (one frame)
static void DrawGame(void);         // Draw game (one frame)
static void UnloadGame(void);       // Unload game
static void UpdateDrawFrame(void);  // Update and Draw (one frame)
//unactive all shoots
void ResetEnemyBullets(void) { 
    for (int i = 0; i < NUM_MAX_ENEMIES; i++) {
        for (int j = 0; j < NUM_SHOOTS; j++) {
            enemy_shoot[i][j].active = false;
        }
    }
}
//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization (Note windowTitle is unused on Android)
    //---------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "Battleship Blitz: Allies in Arms"); //change

    background_ocean = LoadTexture("img/background/ocean.png");
    background_city = LoadTexture("img/background/city.png");
    background_forest = LoadTexture("img/background/forest.png");
    background_hell = LoadTexture("img/background/hell.png");

    normal = LoadTexture("img/enemy/normal.png");
    strong = LoadTexture("img/enemy/strong.png");
    boss = LoadTexture("img/enemy/Boss.png");

    armor = LoadTexture("img/equipment/armor.png");
    artillery = LoadTexture("img/equipment/artillery.png");
    cure = LoadTexture("img/equipment/cure.png");
    energy_beam = LoadTexture("img/equipment/energy beam.png");
    machine_gun = LoadTexture("img/equipment/machine gun.png");
    missile = LoadTexture("img/equipment/missile.png");
    plasma_cannon = LoadTexture("img/equipment/plasma cannon.png");
    thruster = LoadTexture("img/equipment/thruster.png");
    upgrade = LoadTexture("img/equipment/upgrade.png");

    drumstick = LoadTexture("img/item/drumstick.png");
    first_aid_kit = LoadTexture("img/item/first aid kit.png");
    fix_tool = LoadTexture("img/item/fix tool.png");
    invincible_stars = LoadTexture("img/item/invincible stars.png");
    iron_shield = LoadTexture("img/item/iron shield.png");
    radio_jammer = LoadTexture("img/item/radio jammer.png");
    shock_wave = LoadTexture("img/item/shock wave.png");
 
    partner_1 = LoadTexture("img/partner/attack.png");
    partner_2 = LoadTexture("img/partner/recover.png");
    partner_3 = LoadTexture("img/partner/sheid.png");

    player_1 = LoadTexture("img/player/1.png");
    player_2 = LoadTexture("img/player/2.png");
    player_3 = LoadTexture("img/player/3.png");
    
    FullStar = LoadTexture("img/system/FullStar.png"); //從main開始算位置
    TwoStar = LoadTexture("img/system/TwoStar.png");
    OneStar = LoadTexture("img/system/OneStar.png");

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
    UnloadGame();        // Unload loaded data (textures, sounds, models...)

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
    wave = THIRD;
    totalEnemies = EASY_WAVE;
    total_count_Enemies = 0;
    activeEnemies = FIRST_WAVE_1;
    difficulty = HARD;
    enemiesKill = 0;
    score = 0;
    alpha = 0;
    displayenemiesKill = 0;
    displaytotalEnemies = 0;
    displaygold = 0;

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

    //use (圖片變數).width/height to adjest the size
    background_ocean.width = 1500;
    background_ocean.height = 900;
    background_city.width = 1500;
    background_city.height = 900;
    background_forest.width = 1500;
    background_forest.height = 900;
    background_hell.width = 1500;
    background_hell.height = 900;


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
    }

    for(int i=0; i < activeEnemies; i++){
        if(i < 2){
            enemy[i].type = STRONG;
            enemy[i].speed.x = 2.5;
            enemy[i].speed.y = 2.5;
            enemy[i].HP = 45;
            enemy[i].AttackPower = 5;
            enemy[i].color = GREEN;
            enemy[i].frequency = 0.3;
        }
        else{
            enemy[i].type = STANDARD;
            enemy[i].speed.x = 2;
            enemy[i].speed.y = 2;
            enemy[i].HP = 12;
            enemy[i].AttackPower = 6;
            enemy[i].color = GRAY;
            enemy[i].frequency = 0.3;
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

    for(int i = 0; i < NUM_MAX_ENEMIES; i++){
        for (int j = 0; j < NUM_SHOOTS; j++) {
            enemy_shoot[i][j].rec.x = enemy[i].rec.x;
            enemy_shoot[i][j].rec.y = enemy[i].rec.y + enemy[i].rec.height / 4;
            enemy_shoot[i][j].rec.width = 10;
            enemy_shoot[i][j].rec.height = 5;
            enemy_shoot[i][j].speed.x = -7; // 子彈向左移動
            enemy_shoot[i][j].speed.y = 0;
            enemy_shoot[i][j].active = false;
            enemy_shoot[i][j].color = BLUE;
        }
        enemy_cooltime[i] = 0;
    }

    // Initialize partner shoots
    InitPartnerShoot(&partner, &partner_shoot, NUM_SHOOTS);
}

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
                            difficulty = HARD;
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
                        else if (wave == THIRD) {
                            wave = BOSSWAVE;
                            activeEnemies = BOSS_1;
                            difficulty = BOSSLEVEL;

                        }
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
                            
                            if(alpha == 0.00f){
                                for(int i = 0; i < activeEnemies;i++){
                                    enemy[i].active = true;
                                    if (i < 20) {
                                        enemy[i].type = STANDARD;
                                        enemy[i].speed.x = 2;
                                        enemy[i].speed.y = 2;
                                        enemy[i].HP = 24;
                                        enemy[i].AttackPower = 12;
                                        enemy[i].color = GRAY;
                                        enemy[i].frequency = 0.1;
                                    }        
                                    else {
                                        enemy[i].type = STRONG;
                                        enemy[i].speed.x = 2.5;
                                        enemy[i].speed.y = 2.5;
                                        enemy[i].HP = 20;
                                        enemy[i].AttackPower = 20;
                                        enemy[i].color = GREEN;
                                        enemy[i].frequency = 0.1;
                                    }
                                }
                            }

                        } break;
                        case THIRD:
                        {
                            totalEnemies = EASY_WAVE;
                            if (!smooth)
                            {
                                for(int i = 0; i < activeEnemies;i++){
                                    enemy[i].active = true;
                                    if (i < 20) {
                                        enemy[i].type = STANDARD;
                                        enemy[i].speed.x = 2;
                                        enemy[i].speed.y = 2;
                                        enemy[i].HP = 36;
                                        enemy[i].AttackPower = 18;
                                        enemy[i].color = GRAY;
                                        enemy[i].frequency = 0.1;
                                    }        
                                    else {
                                        enemy[i].type = STRONG;
                                        enemy[i].speed.x = 2.5;
                                        enemy[i].speed.y = 2.5;
                                        enemy[i].HP = 45;
                                        enemy[i].AttackPower = 20;
                                        enemy[i].color = GREEN;
                                        enemy[i].frequency = 0.1;
                                        }
                                }

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
                                for(int i = 0; i < activeEnemies;i++){
                                    enemy[i].active = true;
                                    if (i < 15) {
                                        enemy[i].type = STANDARD;
                                        enemy[i].speed.x = 2.5;
                                        enemy[i].speed.y = 2.5;
                                        enemy[i].HP = 24;
                                        enemy[i].AttackPower = 15;
                                        enemy[i].color = GRAY;
                                        enemy[i].frequency = 0.15;
                                    } 
                                    else{
                                        enemy[i].type = STRONG;
                                        enemy[i].speed.x = 2.7;
                                        enemy[i].speed.y = 2.7;
                                        enemy[i].HP = 60;
                                        enemy[i].AttackPower = 10;
                                        enemy[i].color = GREEN;
                                        enemy[i].frequency = 0.15;
                                    }
                                    
                                }

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
                                for(int i = 0; i < activeEnemies;i++){
                                    enemy[i].active = true;
                                    if (i < 25) {
                                        enemy[i].type = STANDARD;
                                        enemy[i].speed.x = 2.5;
                                        enemy[i].speed.y = 2.5;
                                        enemy[i].HP = 48;
                                        enemy[i].AttackPower = 30;
                                        enemy[i].color = GRAY;
                                        enemy[i].frequency = 0.15;
                                    } 
                                    else{
                                        enemy[i].type = STRONG;
                                        enemy[i].speed.x = 2.5;
                                        enemy[i].speed.y = 2.5;
                                        enemy[i].HP = 30;
                                        enemy[i].AttackPower = 30;
                                        enemy[i].color = GREEN;
                                        enemy[i].frequency = 0.15;
                                    }
                                    
                                }

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
                                for(int i = 0; i < activeEnemies;i++){
                                    enemy[i].active = true;                      
                                    if(i < 35){
                                        enemy[i].type = STANDARD;
                                        enemy[i].speed.x = 2.5;
                                        enemy[i].speed.y = 2.5;
                                        enemy[i].HP = 72;
                                        enemy[i].AttackPower = 45;
                                        enemy[i].color = GRAY;
                                        enemy[i].frequency = 0.15;
                                    } 
                                    else{
                                        enemy[i].type = STRONG;
                                        enemy[i].speed.x = 2.7;
                                        enemy[i].speed.y = 2.7;
                                        enemy[i].HP = 60;
                                        enemy[i].AttackPower = 30;
                                        enemy[i].color = GREEN;
                                        enemy[i].frequency = 0.15;
                                    }
                                    
                                }

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
                                for(int i = 0; i < activeEnemies;i++){
                                    enemy[i].active = true;
                                    if (i < 20) {
                                        enemy[i].type = STANDARD;
                                        enemy[i].speed.x = 2.7;
                                        enemy[i].speed.y = 2.7;
                                        enemy[i].HP = 36;
                                        enemy[i].AttackPower = 18; //取大概整數
                                        enemy[i].color = GRAY;
                                        enemy[i].frequency = 0.2;
                                    } 
                                    else{
                                        enemy[i].type = STRONG;
                                        enemy[i].speed.x = 3;
                                        enemy[i].speed.y = 3;
                                        enemy[i].HP = 75;
                                        enemy[i].AttackPower = 5;
                                        enemy[i].color = GREEN;
                                        enemy[i].frequency = 0.2;
                                    }
                                    
                                }

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
                                for(int i = 0; i < activeEnemies;i++){
                                    enemy[i].active = true;
                                    if (i < 30) {
                                        enemy[i].type = STANDARD;
                                        enemy[i].speed.x = 2.7;
                                        enemy[i].speed.y = 2.7;
                                        enemy[i].HP = 20;//調換了standard and strong atk and hp value
                                        enemy[i].AttackPower = 40;
                                        enemy[i].color = GRAY;
                                        enemy[i].frequency = 0.2;
                                    } 
                                    else {
                                        enemy[i].type = STRONG;
                                        enemy[i].speed.x = 3;
                                        enemy[i].speed.y = 3;
                                        enemy[i].HP = 72;
                                        enemy[i].AttackPower = 45;
                                        enemy[i].color = GREEN;
                                        enemy[i].frequency = 0.2;
                                    }
                                    
                                }

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
                                for(int i = 0; i < activeEnemies;i++){
                                    enemy[i].active = true;
                                    if (i < 40) {
                                        enemy[i].type = STANDARD;
                                        enemy[i].speed.x = 2.7;
                                        enemy[i].speed.y = 2.7;
                                        enemy[i].HP = 75;
                                        enemy[i].AttackPower = 40;//switch HP and atk of strong and standard
                                        enemy[i].color = GRAY;
                                        enemy[i].frequency = 0.2;
                                    } 
                                    else {
                                        enemy[i].type = STRONG;
                                        enemy[i].speed.x = 3;
                                        enemy[i].speed.y = 3;
                                        enemy[i].HP = 108;
                                        enemy[i].AttackPower = 68;
                                        enemy[i].color = GREEN;
                                        enemy[i].frequency = 0.2;
                                    }
                                }

                                alpha += 0.02f;

                                if (alpha >= 1.0f) smooth = true;
                            }

                            if (smooth) alpha -= 0.02f;

                        } break;
                        default: break;
                    }
                }
                if(difficulty == BOSSLEVEL){
                    if (!smooth)
                        {
                            for(int i = 0; i < activeEnemies;i++){
                                for(int j = 0; j < NUM_SHOOTS; j++){
                                    enemy_shoot[i][j].rec.width = 50;
                                    enemy_shoot[i][j].rec.height = 25;
                                }
                                enemy[i].active = true;
                                enemy[i].rec.width = 300; 
                                enemy[i].rec.height = 300;
                                enemy[i].speed.x = 0;
                                enemy[i].speed.y = 0;
                                enemy[i].HP = 1000;
                                enemy[i].AttackPower = 100;
                                enemy[i].type = BOSS;
                                
                            }
                            alpha += 3.0f;

                            if (alpha >= 1.0f) smooth = true;
                        }

                        if (smooth) alpha -= 0.02f;

                }
            

                // Player movement
                if (IsKeyDown(KEY_RIGHT)) player.rec.x += player.speed.x;
                if (IsKeyDown(KEY_LEFT)) player.rec.x -= player.speed.x;
                if (IsKeyDown(KEY_UP)) player.rec.y -= player.speed.y;
                if (IsKeyDown(KEY_DOWN)) player.rec.y += player.speed.y;

                // Partner movement
                UpdatePartner(&partner, (Vector2){ player.rec.x, player.rec.y });
                PartnerShoot(&partner, &partner_shoot, deltaTime, NUM_SHOOTS, &enemy, NUM_MAX_ENEMIES, &score, &enemiesKill, &total_count_Enemies, &totalEnemies);

                // Player collision with enemy
                for (int i = 0; i < activeEnemies; i++)
                {
                    if (CheckCollisionRecs(player.rec, enemy[i].rec))
                    {
                        player.HP -= enemy[i].AttackPower;
                        enemy[i].active = false;
                        enemy[i].rec.x = GetRandomValue(screenWidth, screenWidth + 1000);
                        enemy[i].rec.y = GetRandomValue(0, screenHeight - enemy[i].rec.height);
                        enemiesKill++;
                        total_count_Enemies++;

                        if (total_count_Enemies < totalEnemies)
                        {
                            enemy[i].active = true;  // 重新激活敌人
                        }
                    }
                    CheckPartnerCollisionRecs(&partner, &enemy[i], &enemiesKill, &total_count_Enemies);
                }

                if(player.HP <= 0) gameOver = true;

                // Enemy behavior
                for (int i = 0; i < activeEnemies; i++)
                {
                    if (enemy[i].active && enemy[i].type!=BOSS)
                    {
                        enemy[i].rec.x -= enemy[i].speed.x;

                        if (enemy[i].rec.x < 0)
                        {
                            total_count_Enemies++;
                            enemy[i].rec.x = GetRandomValue(screenWidth, screenWidth + 1000);
                            enemy[i].rec.y = GetRandomValue(0, screenHeight - enemy[i].rec.height);
                        }
                    }
                    else if(enemy[i].type == BOSS) {
                        enemy[i].rec.x = screenWidth/2 - 150;
                        enemy[i].rec.y = screenHeight/2 - 150;

                    }
                }

                for(int i = 0; i < activeEnemies; i++){
                    if(enemy_cooltime[i] > 1/enemy[i].frequency)
                    {
                        for (int j = 0; j < NUM_SHOOTS; j++) {
                            if (!enemy_shoot[i][j].active) {
                                enemy_shoot[i][j].rec.x = enemy[i].rec.x;
                                enemy_shoot[i][j].rec.y = enemy[i].rec.y + enemy[i].rec.height / 4;
                                enemy_shoot[i][j].active = true;
                                break;
                            }
                        }
                        enemy_cooltime[i] = 0;
                    }
                    else{
                        enemy_cooltime[i] += deltaTime;
                    }
                }

                for (int i = 0; i < NUM_SHOOTS; i++)
                {
                    if (shoot[i].active)
                    {
                        for (int j = 0; j < NUM_MAX_ENEMIES; j++)
                        {
                            for (int k = 0; k < NUM_SHOOTS; k++)
                            {
                                if (enemy_shoot[j][k].active && CheckCollisionRecs(shoot[i].rec, enemy_shoot[j][k].rec))
                                {
                                    shoot[i].active = false;
                                    enemy_shoot[j][k].active = false;
                                    break;
                                }
                            }
                        }
                    }
                }

                for(int i = 0; i < NUM_MAX_ENEMIES; i++){
                    for (int j = 0; j < NUM_SHOOTS; j++) {
                        if (enemy_shoot[i][j].active) {
                            enemy_shoot[i][j].rec.x += enemy_shoot[i][j].speed.x;

                            if (CheckCollisionRecs(enemy_shoot[i][j].rec, player.rec))
                            {
                                player.HP -= enemy[i].AttackPower; 
                                enemy_shoot[i][j].active = false; 
                                enemy_shoot[i][j].rec.x = -1000;
                            }

                            if (enemy_shoot[i][j].rec.x < 0) {
                                enemy_shoot[i][j].active = false; 
                            }
                        }
                    }

                }

                // Wall behavior
                if (player.rec.x <= 0) player.rec.x = 0;
                if (player.rec.x + player.rec.width >= screenWidth) player.rec.x = screenWidth - player.rec.width;
                if (player.rec.y <= 0) player.rec.y = 0;
                if (player.rec.y + player.rec.height >= screenHeight) player.rec.y = screenHeight - player.rec.height;

                // Shoot initialization
                if (IsKeyDown(KEY_SPACE) )
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
                                        enemy[j].rec.x = GetRandomValue(screenWidth, screenWidth + 1000);
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
                                    shoot[i].rec.x = -1000; // avoid bug
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

                    displaytotalEnemies = totalEnemies;
                    displayenemiesKill = enemiesKill;

                    // Award gold based on performance
                    if(difficulty == EASY){
                        if (wave == FIRST) 
                        {
                            smooth = false;
                            alpha = 0.0f;
                            ResetEnemyBullets();
                            star_choose.level_1 = LevelStar(enemiesKill, totalEnemies);
                            if(star_choose.level_1 == 3) gold += 500; 
                            else if(star_choose.level_1 == 2) gold += 300;
                            else if(star_choose.level_1 == 1) gold += 100;
                           
                        } 
                        else if (wave == SECOND) 
                        {
                            smooth = false;
                            alpha = 0.0f;
                            ResetEnemyBullets();
                            star_choose.level_2 = LevelStar(enemiesKill, totalEnemies);
                            if(star_choose.level_2 == 3) gold += 500; 
                            else if(star_choose.level_2 == 2) gold += 300;
                            else if(star_choose.level_2 == 1) gold += 100;
                            
                        } 
                        else if (wave == THIRD) 
                        {
                            smooth = false;
                            alpha = 0.0f;
                            ResetEnemyBullets();
                            star_choose.level_3 = LevelStar(enemiesKill, totalEnemies);
                            if(star_choose.level_3 == 3) gold += 500; 
                            else if(star_choose.level_3 == 2) gold += 300;
                            else if(star_choose.level_3 == 1) gold += 100;
                            
                        }
                    }
                    if(difficulty == MEDIUM){
                        if (wave == FIRST) 
                        {
                            smooth = false;
                            alpha = 0.0f;
                            ResetEnemyBullets();
                            star_choose.level_1 = LevelStar(enemiesKill, totalEnemies);
                            if(star_choose.level_1 == 3) gold += 500; 
                            else if(star_choose.level_1 == 2) gold += 300;
                            else if(star_choose.level_1 == 1) gold += 100;
                            
                        } 
                        else if (wave == SECOND) 
                        {
                            smooth = false;
                            alpha = 0.0f;
                            ResetEnemyBullets();
                            star_choose.level_2 = LevelStar(enemiesKill, totalEnemies);
                            if(star_choose.level_2 == 3) gold += 500; 
                            else if(star_choose.level_2 == 2) gold += 300;
                            else if(star_choose.level_2 == 1) gold += 100;
                            
                        } 
                        else if (wave == THIRD) 
                        {
                            smooth = false;
                            alpha = 0.0f;
                            ResetEnemyBullets();
                            star_choose.level_3 = LevelStar(enemiesKill, totalEnemies);
                            if(star_choose.level_3 == 3) gold += 500; 
                            else if(star_choose.level_3 == 2) gold += 300;
                            else if(star_choose.level_3 == 1) gold += 100;
                            
                        }
                    }
                    if(difficulty == HARD){
                        if (wave == FIRST) 
                        {
                            smooth = false;
                            alpha = 0.0f;
                            ResetEnemyBullets();
                            star_choose.level_1 = LevelStar(enemiesKill, totalEnemies);
                            if(star_choose.level_1 == 3) gold += 500; 
                            else if(star_choose.level_1 == 2) gold += 300;
                            else if(star_choose.level_1 == 1) gold += 100;
                            
                        } 
                        else if (wave == SECOND) 
                        {
                            smooth = false;
                            alpha = 0.0f;
                            ResetEnemyBullets();
                            star_choose.level_2 = LevelStar(enemiesKill, totalEnemies);
                            if(star_choose.level_2 == 3) gold += 500; 
                            else if(star_choose.level_2 == 2) gold += 300;
                            else if(star_choose.level_2 == 1) gold += 100;
                            
                        } 
                        else if (wave == THIRD) 
                        {
                            smooth = false;
                            alpha = 0.0f;
                            ResetEnemyBullets();
                            star_choose.level_3 = LevelStar(enemiesKill, totalEnemies);
                            if(star_choose.level_3 == 3) gold += 500; 
                            else if(star_choose.level_3 == 2) gold += 300;
                            else if(star_choose.level_3 == 1) gold += 100;
                            
                        }
                    }
                    else if(difficulty == BOSSLEVEL){
                        
                        smooth = false;
                        alpha = 0.0f;
                        enemy[0].active = false;
                        victory = true;
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
void DrawGame(void)
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    if (!gameOver)
    {
        
        if (difficulty == EASY) DrawTexture(background_ocean, 0, 0, WHITE);
        else if (difficulty == MEDIUM) DrawTexture(background_city, 0, 0, WHITE);
        else if (difficulty == HARD) DrawTexture(background_forest, 0, 0, WHITE);
        else if (difficulty == BOSSLEVEL) DrawTexture(background_hell, 0, 0, WHITE);


        DrawPartnerHealth(&partner);

        DrawRectangleRec(player.rec, player.color);
        // Draw partner
        DrawPartner(&partner);

        // Draw wave text
        if (difficulty == EASY) DrawText("EASY", screenWidth/2 - MeasureText("EASY", 40)/2, screenHeight/2, 40, Fade(BLACK, alpha));
        else if (difficulty == MEDIUM) DrawText("MEDIUM", screenWidth/2 - MeasureText("MEDIUM", 40)/2, screenHeight/2, 40, Fade(BLACK, alpha));
        else if (difficulty == HARD) DrawText("HARD", screenWidth/2 - MeasureText("HARD", 40)/2, screenHeight/2, 40, Fade(BLACK, alpha));
        else if (difficulty == BOSSLEVEL) DrawText("BOSSLEVEL", screenWidth/2 - MeasureText("BOSSLEVEL", 40)/2, screenHeight/2, 40, Fade(BLACK, alpha));
        if (wave == FIRST) DrawText("FIRST WAVE", screenWidth/2 - MeasureText("FIRST WAVE", 40)/2, screenHeight/2 - 40, 40, Fade(BLACK, alpha));
        else if (wave == SECOND) DrawText("SECOND WAVE", screenWidth/2 - MeasureText("SECOND WAVE", 40)/2, screenHeight/2 - 40, 40, Fade(BLACK, alpha));
        else if (wave == THIRD) DrawText("THIRD WAVE", screenWidth/2 - MeasureText("THIRD WAVE", 40)/2, screenHeight/2 - 40, 40, Fade(BLACK, alpha));
        
        
        
        // Draw score time message
        if (displayScoreTime) 
        {
            DrawText("SCORE TIME", screenWidth/2 - MeasureText("SCORE TIME", 40)/2, screenHeight/3 +50 , 40, Fade(BLUE, scoreAlpha));

            // 格式化 "Enemies Killed" 字符串
            const char *enemiesKilledText = TextFormat("Enemies Killed: %04i / %04i", displayenemiesKill, displaytotalEnemies);
            // 计算格式化字符串的宽度
            int enemiesKilledTextWidth = MeasureText(enemiesKilledText, 40);
            // 绘制 "Enemies Killed" 文本
            DrawText(enemiesKilledText, screenWidth/2 - enemiesKilledTextWidth/2,  2*screenHeight/3 -90, 40, Fade(BLACK, scoreAlpha));

            if(wave == FIRST){
                if(star_choose.level_1 == 3){
                    DrawTexture(FullStar, screenWidth/2 - FullStar.width/2, screenHeight/2 - FullStar.height/2 - 30, Fade(WHITE, scoreAlpha));
                    DrawText("Gold+500", screenWidth/2 - MeasureText("Gold+500", 40)/2, screenHeight/2 +10, 40, Fade(BLACK, scoreAlpha));
                }
                else if(star_choose.level_1 == 2){
                    DrawTexture(TwoStar, screenWidth/2 - TwoStar.width/2, screenHeight/2 - TwoStar.height/2 - 30, Fade(WHITE, scoreAlpha));
                    DrawText("Gold+300", screenWidth/2 - MeasureText("Gold+300", 40)/2, screenHeight/2 +10, 40, Fade(BLACK, scoreAlpha));
                }
                else if(star_choose.level_1 == 1){
                    DrawTexture(OneStar, screenWidth/2 - OneStar.width/2, screenHeight/2 - OneStar.height/2 - 30, Fade(WHITE, scoreAlpha));
                    DrawText("Gold+100", screenWidth/2 - MeasureText("Gold+100", 40)/2, screenHeight/2 +10, 40, Fade(BLACK, scoreAlpha));
                }
            }
            else if(wave == SECOND){
                if(star_choose.level_2 == 3){
                    DrawTexture(FullStar, screenWidth/2 - FullStar.width/2, screenHeight/2 - FullStar.height/2 - 30, Fade(WHITE, scoreAlpha));
                    DrawText("Gold+500", screenWidth/2 - MeasureText("Gold+500", 40)/2, screenHeight/2 +10, 40, Fade(BLACK, scoreAlpha));
                }
                else if(star_choose.level_2 == 2){
                    DrawTexture(TwoStar, screenWidth/2 - TwoStar.width/2, screenHeight/2 - TwoStar.height/2 - 30, Fade(WHITE, scoreAlpha));
                    DrawText("Gold+300", screenWidth/2 - MeasureText("Gold+300", 40)/2, screenHeight/2 +10, 40, Fade(BLACK, scoreAlpha));
                }
                else if(star_choose.level_2 == 1){
                    DrawTexture(OneStar, screenWidth/2 - OneStar.width/2, screenHeight/2 - OneStar.height/2 - 30, Fade(WHITE, scoreAlpha));
                    DrawText("Gold+100", screenWidth/2 - MeasureText("Gold+100", 40)/2, screenHeight/2 +10, 40, Fade(BLACK, scoreAlpha));
                }
            }
            if(wave == THIRD){
                if(star_choose.level_3 == 3){
                    DrawTexture(FullStar, screenWidth/2 - FullStar.width/2, screenHeight/2 - FullStar.height/2 - 30, Fade(WHITE, scoreAlpha));
                    DrawText("Gold+500", screenWidth/2 - MeasureText("Gold+500", 40)/2, screenHeight/2 +10, 40, Fade(BLACK, scoreAlpha));
                }
                else if(star_choose.level_3 == 2){
                    DrawTexture(TwoStar, screenWidth/2 - TwoStar.width/2, screenHeight/2 - TwoStar.height/2 - 30, Fade(WHITE, scoreAlpha));
                    DrawText("Gold+300", screenWidth/2 - MeasureText("Gold+300", 40)/2, screenHeight/2 +10, 40, Fade(BLACK, scoreAlpha));
                }
                else if(star_choose.level_3 == 1){
                    DrawTexture(OneStar, screenWidth/2 - OneStar.width/2, screenHeight/2 - OneStar.height/2 - 30, Fade(WHITE, scoreAlpha));
                    DrawText("Gold+100", screenWidth/2 - MeasureText("Gold+100", 40)/2, screenHeight/2 +10, 40, Fade(BLACK, scoreAlpha));
                }
            }
        }

        int temp = 0;
        int temp2 = 0;

        for (int i = 0; i < activeEnemies; i++)
        {
            if (enemy[i].active){
                DrawRectangleRec(enemy[i].rec, enemy[i].color);
                temp++;

                for (int j = 0; j < NUM_SHOOTS; j++) {

                    if (enemy_shoot[i][j].active) {
                        DrawRectangleRec(enemy_shoot[i][j].rec, enemy_shoot[i][j].color);
                        // if(i == 0){
                        //     temp2++;
                        // }
                    }
                }
            }

        }

        for (int i = 0; i < NUM_SHOOTS; i++)
        {
            if (shoot[i].active) DrawRectangleRec(shoot[i].rec, shoot[i].color);
            
            if (partner_shoot[i].active) 
            {
                DrawRectangleRec(partner_shoot[i].rec, partner_shoot[i].color);
            }
        }
        DrawText(TextFormat("EnemiesKill:%04i", enemiesKill), 20, 20, 20, RED);
        DrawText(TextFormat("Total_Enemies:%04i", total_count_Enemies), 20, 40, 20, RED);
        DrawText(TextFormat("HP:%03i", player.HP), 20, 80, 20, RED);
        DrawText(TextFormat("gold:%04i", gold), 500, 20, 20, RED);

        if (victory) DrawText("YOU WIN", screenWidth/2 - MeasureText("YOU WIN", 40)/2, screenHeight/2 - 40, 40, RED);

        if (pause) DrawText("GAME PAUSED", screenWidth/2 - MeasureText("GAME PAUSED", 40)/2, screenHeight/2 - 40, 40, RED);
    }
    else DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth()/2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20)/2, GetScreenHeight()/2 - 50, 20, RED);

    EndDrawing();
}

// Unload game variables
void UnloadGame(void)
{
    UnloadTexture(background_ocean);
    UnloadTexture(background_city);
    UnloadTexture(background_forest);
    UnloadTexture(background_hell);
    UnloadTexture(normal);
    UnloadTexture(strong);
    UnloadTexture(boss);
    UnloadTexture(armor);
    UnloadTexture(artillery);
    UnloadTexture(cure);
    UnloadTexture(energy_beam);
    UnloadTexture(machine_gun);
    UnloadTexture(missile);
    UnloadTexture(plasma_cannon);
    UnloadTexture(thruster);
    UnloadTexture(upgrade);
    UnloadTexture(drumstick);
    UnloadTexture(first_aid_kit);
    UnloadTexture(fix_tool);
    UnloadTexture(invincible_stars);
    UnloadTexture(iron_shield);
    UnloadTexture(radio_jammer);
    UnloadTexture(shock_wave);
    UnloadTexture(partner_1);
    UnloadTexture(partner_2);
    UnloadTexture(partner_3);
    UnloadTexture(FullStar);
    UnloadTexture(TwoStar);
    UnloadTexture(OneStar);
    
    // TODO: Unload all dynamic loaded data (textures, sounds, models...)
}

// Update and Draw (one frame)
void UpdateDrawFrame(void)
{
    UpdateGame();
    DrawGame();
}