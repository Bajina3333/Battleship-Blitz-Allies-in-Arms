#include "raylib.h"
#include "type.h"
#include "partner.h"
#include "game_system.h"
#include "item.h"
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
#define BOSS_WAVE 1
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
static bool gamebegining = false;
static bool gameaction = false;
static bool gamechoose = false;
static bool pause =  false;
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
static int ShootControl = 0;
static int cureHP = 0;
static float timeCounter = 0.0f;
static int itemchoose = 0;
static int eventchoose = 0;
static char text[100] = "";
static int playerchoose = 0;

static int shootRate = 0;
static float alpha = 0.0f; //alpha的值範圍是 0.0 到 1.0，其中 0.0 表示完全透明，1.0 表示完全不透明，用來逐步增加或減少文字的透明度，實現文字淡入淡出的效果

static int totalEnemies = 0;//The number of enemies should be killed in the level
static int activeEnemies = 0; //The number of enemies at that level
static int total_count_Enemies = 0; //The number of enemies actually appeared in the level
static int enemiesKill = 0; //The number of enemies in the level
static bool smooth = false;

int gold = 0; //Golds held by the player
bool store_enter = false; //Judge whether or not to execute the program of store with true or false
int displayenemiesKill = 0;
int displaytotalEnemies = 0;
int displaygold = 0;

static Button button_start = { 0 };
static Button button_quit = { 0 };
static Button button_1 = { 0 };
static Button button_2 = { 0 };
static Button button_3 = { 0 };
static bool button_start_clicked = false;
static bool button_quit_clicked = false;
static bool button_1_clicked = false;
static bool button_2_clicked = false;
static bool button_3_clicked = false;

static float stopTimer = 0.0f;
static bool stopForScore = false;
static float scoreAlpha = 0.0f;
static bool displayScoreTime = false;
static bool displayitem = false;

static Texture2D background_ocean = { 0 };
static Texture2D background_city = { 0 };
static Texture2D background_forest = { 0 };
static Texture2D background_hell = { 0 };
static Texture2D background_heaven = { 0 };

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
static Texture2D booster = { 0 };
static Texture2D healing_potion = { 0 };

static Texture2D partner_1 = { 0 };
static Texture2D partner_2 = { 0 };
static Texture2D partner_3 = { 0 };

static Texture2D player_1 = { 0 };
static Texture2D player_2 = { 0 };
static Texture2D player_3 = { 0 };

static Texture2D FullStar = { 0 };
static Texture2D TwoStar = { 0 };
static Texture2D OneStar = { 0 };
static Texture2D items[3];

static float effectStartTime = 0;
static bool showEffect = false;
static char effectType = '\0';

//------------------------------------------------------------------------------------
// Module Functions Declaration (local) 
//------------------------------------------------------------------------------------
static void InitGame(int n);         // Initialize game
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
    background_heaven = LoadTexture("img/background/heaven.png");

    normal = LoadTexture("img/enemy/normal.png");
    strong = LoadTexture("img/enemy/strong.png");
    boss = LoadTexture("img/enemy/Boss.png");

    armor = LoadTexture("img/equipment/armor.png");
    artillery = LoadTexture("img/equipment/artillery.png");
    cure = LoadTexture("img/equipment/cure.png");
    energy_beam = LoadTexture("img/equipment/energy_beam.png");
    machine_gun = LoadTexture("img/equipment/machine_gun.png");
    missile = LoadTexture("img/equipment/missile.png");
    plasma_cannon = LoadTexture("img/equipment/plasma_cannon.png");
    thruster = LoadTexture("img/equipment/thruster.png");
    upgrade = LoadTexture("img/equipment/upgrade.png");

    drumstick = LoadTexture("img/item/drumstick.png");
    first_aid_kit = LoadTexture("img/item/first_aid_kit.png");
    fix_tool = LoadTexture("img/item/fix_tool.png");
    invincible_stars = LoadTexture("img/item/invincible_stars.png");
    iron_shield = LoadTexture("img/item/iron_shield.png");
    radio_jammer = LoadTexture("img/item/radio_jammer.png");
    booster = LoadTexture("img/item/shock_wave.png");
    healing_potion = LoadTexture("img/item/healing_potion.png");
 
    partner_1 = LoadTexture("img/partner/attack.png");
    partner_2 = LoadTexture("img/partner/recover.png");
    partner_3 = LoadTexture("img/partner/sheid.png");

    player_1 = LoadTexture("img/player/1.png");
    player_2 = LoadTexture("img/player/2.png");
    player_3 = LoadTexture("img/player/3.png");
    
    FullStar = LoadTexture("img/system/FullStar.png"); //從main開始算位置
    TwoStar = LoadTexture("img/system/TwoStar.png");
    OneStar = LoadTexture("img/system/OneStar.png");

    InitGame(1);

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
void init_button(Button *button, Rectangle rect, Color color) {
    button->rect = rect;
    button->color = color;
}

bool is_mouse_over_button(Button button) {
    return CheckCollisionPointRec(GetMousePosition(), button.rect);
}
// Initialize game variables
void InitGame(int n)
{
    // Initialize player
    if(n == 1){
        player.HP = 90;
        player.MaxHP = 90;
        player.AttackPower = 25;
        player.speed.x = 8;
        player.speed.y = 8;
        player.color = RED;
    }else if(n == 2){
        player.HP = 130;
        player.MaxHP = 130;
        player.AttackPower = 15;
        player.speed.x = 5;
        player.speed.y = 5;
        player.color = BLUE;
    }else if(n == 3){
        player.HP = 75;
        player.MaxHP = 75;
        player.AttackPower = 10;
        player.speed.x = 10;
        player.speed.y = 10;
        player.color = GREEN;
    }
    player.rec.x =  10;
    player.rec.y = screenHeight / 2;
    player.rec.width = 40;
    player.rec.height = 40;

    // Initialize game variables
    shootRate = 0;
    pause = false;
    gameOver = false;
    gamebegining = true;
    gameaction = false;
    gamechoose = false;
    victory = false;
    smooth = false;
    wave = FIRST;
    totalEnemies = EASY_WAVE;
    total_count_Enemies = 0;
    activeEnemies = FIRST_WAVE_1;
    difficulty = EASY;
    enemiesKill = 0;
    alpha = 0;
    displayenemiesKill = 0;
    displaytotalEnemies = 0;
    displayitem = false;
    displaygold = 0;
    ShootControl = -1;
    cureHP =0;
    timeCounter = 0.0f;
    itemchoose = 0;
    eventchoose = 0;
    playerchoose = 0;
    *text = "";
    items[0];
    items[1];
    items[2];

    // Initialize score_system
    gold = 0;
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

    energy_beam.width = 150;
    energy_beam.height = 60;
    machine_gun.width = 145;
    machine_gun.height = 60;
    machine_gun.width = 145;
    booster.width = 77;
    booster.height = 60;
    cure.width = 61;
    cure.height = 60;
    healing_potion.width = 50;
    healing_potion.height = 60;
    armor.width = 48;
    armor.height = 60;
    plasma_cannon.width = 101;
    plasma_cannon.height = 60;
    
    player_1.width = 40;
    player_1.height = 40;
    player_2.width = 40;
    player_2.height = 40;
    player_3.width = 40;
    player_3.height = 40;
    
    partner_1.width = 30;
    partner_1.height = 30;
    partner_2.width = 30;
    partner_2.height = 30;
    partner_3.width = 30;
    partner_3.height = 30;


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

    init_button(&button_start, (Rectangle){screenWidth / 2 - 100, screenHeight / 2 + 50, 200, 50}, RED);
    init_button(&button_quit, (Rectangle){screenWidth / 2 - 100, screenHeight / 2 + 200, 200, 50}, RED);
    init_button(&button_1, (Rectangle){screenWidth / 2 - 600, screenHeight / 2 - 150 , 300, 300}, RED);
    init_button(&button_2, (Rectangle){screenWidth / 2 - 150, screenHeight / 2 - 150 , 300, 300}, RED);
    init_button(&button_3, (Rectangle){screenWidth / 2 + 300, screenHeight / 2 - 150 , 300, 300}, RED);
    // Initialize partner shoots
    InitPartnerShoot(&partner, &partner_shoot, NUM_SHOOTS);
}

// Update game (one frame)
void UpdateGame(void)
{
    float deltaTime = GetFrameTime(); // Get time for the last frame to smooth animations and calculations

    if (gamebegining)
    {
        if (is_mouse_over_button(button_start) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            gamebegining = false;
            gamechoose = true;
        }
        if (is_mouse_over_button(button_quit) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            exit(1);
        }
    }
    else if (gamechoose){
         if (is_mouse_over_button(button_1) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            InitGame(1);
            playerchoose = 1;
            gameaction = true;
            gamebegining = false;
            
        }
        if (is_mouse_over_button(button_2) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            InitGame(2);
            playerchoose = 2;
            gameaction = true;
            gamebegining = false;
        }
        if (is_mouse_over_button(button_3) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            InitGame(3);
            playerchoose = 3;
            gameaction = true;
            gamebegining = false;
        }
        
    }
    else if(gameOver){
         if (is_mouse_over_button(button_start) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            gameOver = false;
            gamechoose = true;
        }        
        if (is_mouse_over_button(button_quit) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
             exit(1);
        }
    }
    else if (gameaction && victory != true)
    {
        if (IsKeyPressed('P')) pause = !pause;

        if (!pause)
        {
            if (stopForScore) 
            {
                stopTimer += deltaTime;
                scoreAlpha += 0.02f;

                if (stopTimer >= 4.0f) 
                {
                    stopForScore = false;
                    stopTimer = 0.0f;
                    scoreAlpha = 0.0f;
                    
                    if(difficulty == EASY){
                        if (wave == FIRST) 
                        {   
                            ShootControl--;
                            if(ShootControl<0)ShootControl = -1;
                            activeEnemies = SECOND_WAVE_1;
                            wave = SECOND;

                        } 
                        else if (wave == SECOND) 
                        {
                            ShootControl--;
                            activeEnemies = THIRD_WAVE_1;
                            wave = THIRD;
                            
                        } 
                        else if (wave == THIRD) 
                        {
                            displayitem = false;
                            
                            ShootControl--;
                            activeEnemies = FIRST_WAVE_2;
                            difficulty = MEDIUM;
                            wave = FIRST;
         
                        }
                        
                    }
                    if(difficulty == MEDIUM){
                        if (wave == FIRST) 
                        {
                            ShootControl--;
                            wave = SECOND;
                            activeEnemies = SECOND_WAVE_2;
                        } 
                        else if (wave == SECOND) 
                        {
                            ShootControl--;
                            wave = THIRD;
                            activeEnemies = THIRD_WAVE_2;
                        } 
                        else if (wave == THIRD) 
                        {
                            
                            displayitem = false;
                            ShootControl--;
                            wave = FIRST;
                            activeEnemies = FIRST_WAVE_3;
                            difficulty = HARD;
                        }
                        
                    }
                    if(difficulty == HARD){
                        if (wave == FIRST) 
                        {
                            ShootControl--;
                            wave = SECOND;
                            activeEnemies = SECOND_WAVE_3;
                        } 
                        else if (wave == SECOND) 
                        {
                            ShootControl--;
                            wave = THIRD;
                            activeEnemies = THIRD_WAVE_3;
                        } 
                        else if (wave == THIRD) {
                            ShootControl--;
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
                    totalEnemies = BOSS_WAVE;
                    if (!smooth)
                        {
                            activeEnemies = BOSS_1;
                            enemiesKill = 0;
                            for(int i = 0; i < activeEnemies;i++){
                                enemy[i].active = true;
                                enemy[i].rec.width = 360; 
                                enemy[i].rec.height = 450;
                                enemy[i].rec.x = 800;
                                enemy[i].rec.y = 0;
                                enemy[i].speed.x = 0;
                                enemy[i].speed.y = 3;
                                enemy[i].HP = 10000;
                                enemy[i].AttackPower = 100;
                                enemy[i].type = BOSS;
                                enemy[i].frequency = 5;
                                
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
                
                timeCounter += deltaTime;
                if(cureHP>0){
                    if(player.HP<player.MaxHP){
                        if (timeCounter >= 1.0f)
                            {
                                player.HP += cureHP;
                                timeCounter = 0.0f; // 重置计时器
                            }
                    }
                }
                
                // Partner movement
                UpdatePartner(&partner, (Vector2){ player.rec.x, player.rec.y },&player);
                float currentTime = GetTime();
                if (partner.effectActive) {
                    if ((currentTime - partner.lastEffectTime) < 1) {
                        showEffect = true;
                        effectType = partner.effectType;
                        effectStartTime = currentTime;
                    } else {
                        if ((effectType == 'H' && (currentTime - effectStartTime) >= 3) || 
                            (effectType == 'A' && (currentTime - effectStartTime) >= 10) || 
                            (effectType == 'S' && (currentTime - effectStartTime) >= 10)) {
                            showEffect = false;
                        }
                    }
                }
                PartnerShoot(&partner, &partner_shoot, deltaTime, NUM_SHOOTS, &enemy, NUM_MAX_ENEMIES, &enemiesKill, &total_count_Enemies, &totalEnemies);

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
                    CheckPartnerCollisionRecs(&partner, &enemy[i], &enemiesKill, &total_count_Enemies, &partner_shoot[i]);
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
                        if (enemy[i].speed.y == 0) {
                            enemy[i].speed.y = 3;
                        }
                        enemy[i].rec.y += enemy[i].speed.y;
                        if (enemy[i].rec.y  <= 0 || enemy[i].rec.y  + 450 >= screenHeight)
                        {
                            enemy[i].speed.y *= -1;
                        }
                    
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
                if (IsKeyDown(KEY_SPACE) && ShootControl<0)
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
                            eventchoose = Event(&player,&gold,&shoot, &ShootControl , &partner, &cureHP, text);
                            
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
                            eventchoose = Event(&player,&gold,&shoot, &ShootControl , &partner, &cureHP, text);
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
                            displayitem = true;
                            itemchoose = Choose_Item(&player); 
                            eventchoose = Event(&player,&gold,&shoot, &ShootControl , &partner, &cureHP,  text);
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
                            eventchoose = Event(&player,&gold,&shoot, &ShootControl , &partner, &cureHP,  text);
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
                            eventchoose = Event(&player,&gold,&shoot, &ShootControl , &partner, &cureHP,  text);
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
                            eventchoose = Event(&player,&gold,&shoot, &ShootControl , &partner, &cureHP,  text);
                            displayitem = true;
                            itemchoose = Choose_Item(&player);
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
                            eventchoose = Event(&player,&gold,&shoot, &ShootControl , &partner, &cureHP,  text);
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
                            eventchoose = Event(&player,&gold,&shoot, &ShootControl , &partner, &cureHP,  text);
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
                            displayitem = true;
                            itemchoose = Choose_Item(&player); 
                            eventchoose = Event(&player,&gold,&shoot, &ShootControl , &partner, &cureHP,  text);
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
}

// Draw game (one frame)
void DrawGame(void)
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    if(gamebegining)
    {
        if (is_mouse_over_button(button_start)) {
            button_start.color = BLUE;
        }
        else {
            button_start.color = RED;
        }
        if (is_mouse_over_button(button_quit)) {
            button_quit.color = BLUE;
        }
        else {
            button_quit.color = RED;
        }
        DrawText("WELCOME TO", screenWidth/2 - MeasureText("WELCOME TO", 40)/2, 200, 40, RED);
        DrawText("Battleship Blitz: Allies in Arms", screenWidth/2 - MeasureText("Battleship Blitz: Allies in Arms", 60)/2, 350, 60, RED);
        DrawRectangleRec(button_start.rect, button_start.color);
        DrawText("start", button_start.rect.x + button_start.rect.width / 2 - MeasureText("start", 20) / 2, button_start.rect.y + button_start.rect.height / 2 - 20 / 2, 20, WHITE);
        DrawRectangleRec(button_quit.rect, button_quit.color);
        DrawText("quit", button_quit.rect.x + button_quit.rect.width / 2 - MeasureText("quit", 20) / 2, button_quit.rect.y + button_quit.rect.height / 2 - 20 / 2, 20, WHITE);
    }
    else if(gameOver)
    {
        if (is_mouse_over_button(button_start)) {
            button_start.color = BLUE;
        } else {
            button_start.color = RED;
        }
        if (is_mouse_over_button(button_quit)) {
            button_quit.color = BLUE;
        } else {
            button_quit.color = RED;
        }
        
        DrawText("GAMEOVER", screenWidth/2 - MeasureText("GAMEOVER", 40)/2, 250, 40, RED);
        DrawRectangleRec(button_start.rect, button_start.color);
        DrawText("RETRY", button_start.rect.x + button_start.rect.width / 2 - MeasureText("RETRY", 20) / 2, button_start.rect.y + button_start.rect.height / 2 - 20 / 2, 20, WHITE);
        DrawRectangleRec(button_quit.rect, button_quit.color);
        DrawText("quit", button_quit.rect.x + button_quit.rect.width / 2 - MeasureText("quit", 20) / 2, button_quit.rect.y + button_quit.rect.height / 2 - 20 / 2, 20, WHITE);
    }else if(gamechoose)
    {
        if (is_mouse_over_button(button_1)) {
            button_1.color = BLUE;
        } else {
            button_1.color = RED;
        }
        if (is_mouse_over_button(button_2)) {
            button_2.color = BLUE;
        } else {
            button_2.color = RED;
        }
        if (is_mouse_over_button(button_3)) {
            button_3.color = BLUE;
        } else {
            button_3.color = RED;
        }
        DrawRectangleRec(button_1.rect, button_1.color);
        DrawText("player1", button_1.rect.x + button_1.rect.width / 2 - MeasureText("player1", 20) / 2, button_1.rect.y + button_1.rect.height / 2 - 20 / 2, 20, WHITE);
        DrawRectangleRec(button_2.rect, button_2.color);
        DrawText("player2", button_2.rect.x + button_2.rect.width / 2 - MeasureText("player2", 20) / 2, button_2.rect.y + button_2.rect.height / 2 - 20 / 2, 20, WHITE);
        DrawRectangleRec(button_3.rect, button_3.color);
        DrawText("player3", button_3.rect.x + button_3.rect.width / 2 - MeasureText("player3", 20) / 2, button_3.rect.y + button_3.rect.height / 2 - 20 / 2, 20, WHITE);
    
    }else if(gameaction)
    {
        
        if (difficulty == EASY) DrawTexture(background_ocean, 0, 0, WHITE);
        else if (difficulty == MEDIUM) DrawTexture(background_city, 0, 0, WHITE);
        else if (difficulty == HARD) DrawTexture(background_forest, 0, 0, WHITE);
        else if (difficulty == BOSSLEVEL){
            DrawTexture(background_hell, 0, 0, WHITE);
            DrawText(TextFormat("BossHP:%4i", enemy[0].HP), 250, 40, 45, WHITE);
        }

        DrawPartnerHealth(&partner);
        
        if(playerchoose == 1){
            DrawTextureEx(player_1, (Vector2){player.rec.x, player.rec.y}, 0.0f, 1.0f, RAYWHITE);
        }
        else if(playerchoose == 2){
            DrawTextureEx(player_2, (Vector2){player.rec.x, player.rec.y}, 0.0f, 1.0f, RAYWHITE);
        }
        else if(playerchoose == 3){
            DrawTextureEx(player_3, (Vector2){player.rec.x, player.rec.y}, 0.0f, 1.0f, RAYWHITE);
        }

        // Draw partner
        if (partner.active) {
            if(partner.AttackPower == 25){
                DrawTextureEx(partner_1, (Vector2){partner.rec.x, partner.rec.y}, 0.0f, 1.0f, RAYWHITE);
            }
            else if(partner.AttackPower == 15){
                DrawTextureEx(partner_2, (Vector2){partner.rec.x, partner.rec.y}, 0.0f, 1.0f, RAYWHITE);
            }
            else if(partner.AttackPower == 35){
                DrawTextureEx(partner_3, (Vector2){partner.rec.x, partner.rec.y}, 0.0f, 1.0f, RAYWHITE);
            }
        }

        float currentTime = GetTime();
        if (showEffect) {
            int fontSize = player.rec.width; // 字體大小設置為玩家寬度
            if (effectType == 'H' && (currentTime - effectStartTime) < 3) {
                DrawText("+", player.rec.x + player.rec.width, player.rec.y - player.rec.height / 2, 20, GRAY);
            } else if (effectType == 'A' && (currentTime - effectStartTime) < 7) {
                DrawText("^", player.rec.x + player.rec.width, player.rec.y - player.rec.height / 2, 40, ORANGE);
            } else if (effectType == 'S' && (currentTime - effectStartTime) < 10) {
                DrawText("^", player.rec.x + player.rec.width, player.rec.y - player.rec.height / 2, 40, BLUE); // 使用藍色箭頭表示速度增加
            } else {
                showEffect = false;
            }
        }

        // Draw wave text
        if (difficulty == EASY) DrawText("EASY", screenWidth/2 - MeasureText("EASY", 40)/2, screenHeight/2, 40, Fade(BLACK, alpha));
        else if (difficulty == MEDIUM) DrawText("MEDIUM", screenWidth/2 - MeasureText("MEDIUM", 40)/2, screenHeight/2, 40, Fade(BLACK, alpha));
        else if (difficulty == HARD) DrawText("HARD", screenWidth/2 - MeasureText("HARD", 40)/2, screenHeight/2, 40, Fade(BLACK, alpha));
        else if (difficulty == BOSSLEVEL) DrawText("BOSSLEVEL", screenWidth/2 - MeasureText("BOSSLEVEL", 40)/2, screenHeight/2, 40, Fade(BLACK, alpha));
        if (wave == FIRST) DrawText("FIRST WAVE", screenWidth/2 - MeasureText("FIRST WAVE", 40)/2, screenHeight/2 - 40, 40, Fade(BLACK, alpha));
        else if (wave == SECOND) DrawText("SECOND WAVE", screenWidth/2 - MeasureText("SECOND WAVE", 40)/2, screenHeight/2 - 40, 40, Fade(BLACK, alpha));
        else if (wave == THIRD) DrawText("THIRD WAVE", screenWidth/2 - MeasureText("THIRD WAVE", 40)/2, screenHeight/2 - 40, 40, Fade(BLACK, alpha));
        
        
        
        // Draw score time message
        if (displayScoreTime  && difficulty != BOSSLEVEL) 
        {
            DrawText("SCORE TIME", screenWidth/2 - MeasureText("SCORE TIME", 40)/2, screenHeight/3 -60 , 40, Fade(BLUE, scoreAlpha));  
            
            // 格式化 "Enemies Killed" 字符串
            const char *enemiesKilledText = TextFormat("Enemies Killed: %04i / %04i", displayenemiesKill, displaytotalEnemies);
            // 计算格式化字符串的宽度
            int enemiesKilledTextWidth = MeasureText(enemiesKilledText, 40);
            // 绘制 "Enemies Killed" 文本
            DrawText(enemiesKilledText, screenWidth/2 - enemiesKilledTextWidth/2,  2*screenHeight/3 -190, 40, Fade(BLACK, scoreAlpha));

            if(wave == FIRST){
                if(star_choose.level_1 == 3){
                    DrawTexture(FullStar, screenWidth/2 - FullStar.width/2, screenHeight/2 - FullStar.height/2 - 130, Fade(WHITE, scoreAlpha));
                    DrawText("Gold+500", screenWidth/2 - MeasureText("Gold+500", 40)/2, screenHeight/2 -90, 40, Fade(BLACK, scoreAlpha));
                }
                else if(star_choose.level_1 == 2){
                    DrawTexture(TwoStar, screenWidth/2 - TwoStar.width/2, screenHeight/2 - TwoStar.height/2 - 130, Fade(WHITE, scoreAlpha));
                    DrawText("Gold+300", screenWidth/2 - MeasureText("Gold+300", 40)/2, screenHeight/2 -90, 40, Fade(BLACK, scoreAlpha));
                }
                else if(star_choose.level_1 == 1){
                    DrawTexture(OneStar, screenWidth/2 - OneStar.width/2, screenHeight/2 - OneStar.height/2 - 130, Fade(WHITE, scoreAlpha));
                    DrawText("Gold+100", screenWidth/2 - MeasureText("Gold+100", 40)/2, screenHeight/2 -90, 40, Fade(BLACK, scoreAlpha));
                }
            }
            else if(wave == SECOND){
                if(star_choose.level_2 == 3){
                    DrawTexture(FullStar, screenWidth/2 - FullStar.width/2, screenHeight/2 - FullStar.height/2 - 130, Fade(WHITE, scoreAlpha));
                    DrawText("Gold+500", screenWidth/2 - MeasureText("Gold+500", 40)/2, screenHeight/2 -90, 40, Fade(BLACK, scoreAlpha));
                }
                else if(star_choose.level_2 == 2){
                    DrawTexture(TwoStar, screenWidth/2 - TwoStar.width/2, screenHeight/2 - TwoStar.height/2 - 130, Fade(WHITE, scoreAlpha));
                    DrawText("Gold+300", screenWidth/2 - MeasureText("Gold+300", 40)/2, screenHeight/2 -90, 40, Fade(BLACK, scoreAlpha));
                }
                else if(star_choose.level_2 == 1){
                    DrawTexture(OneStar, screenWidth/2 - OneStar.width/2, screenHeight/2 - OneStar.height/2 - 130, Fade(WHITE, scoreAlpha));
                    DrawText("Gold+100", screenWidth/2 - MeasureText("Gold+100", 40)/2, screenHeight/2 -90, 40, Fade(BLACK, scoreAlpha));
                }
            }
            else if(wave == THIRD){
                if(star_choose.level_3 == 3){
                    DrawTexture(FullStar, screenWidth/2 - FullStar.width/2, screenHeight/2 - FullStar.height/2 - 130, Fade(WHITE, scoreAlpha));
                    DrawText("Gold+500", screenWidth/2 - MeasureText("Gold+500", 40)/2, screenHeight/2 -90, 40, Fade(BLACK, scoreAlpha));
                }
                else if(star_choose.level_3 == 2){
                    DrawTexture(TwoStar, screenWidth/2 - TwoStar.width/2, screenHeight/2 - TwoStar.height/2 - 130, Fade(WHITE, scoreAlpha));
                    DrawText("Gold+300", screenWidth/2 - MeasureText("Gold+300", 40)/2, screenHeight/2 -90, 40, Fade(BLACK, scoreAlpha));
                }
                else if(star_choose.level_3 == 1){
                    DrawTexture(OneStar, screenWidth/2 - OneStar.width/2, screenHeight/2 - OneStar.height/2 - 130, Fade(WHITE, scoreAlpha));
                    DrawText("Gold+100", screenWidth/2 - MeasureText("Gold+100", 40)/2, screenHeight/2 -90, 40, Fade(BLACK, scoreAlpha));
                }
            }
            if(displayitem == true){
                switch(itemchoose){
                    case 1:{
                        DrawTexture(energy_beam, screenWidth/2 -  energy_beam.width/2, screenHeight/2 - energy_beam.height/2 +150, Fade(WHITE, scoreAlpha));
                        if(difficulty == EASY){
                            items[0] = energy_beam;
                            items[0].height = 100;
                            items[0].width = 260;
                        }
                        else if(difficulty == MEDIUM){
                            items[1] = energy_beam;
                            items[1].height = 100;
                            items[1].width = 260;
                        }
                        else{
                            items[2] = energy_beam;
                            items[2].height = 100;
                            items[2].width = 260;
                        }
                        break;
                    }
                    case 2:{
                        DrawTexture(machine_gun, screenWidth/2 -  machine_gun.width/2, screenHeight/2 - machine_gun.height/2 +150, Fade(WHITE, scoreAlpha));
                        if(difficulty == EASY){
                            items[0] = machine_gun;
                            items[0].height = 100;
                            items[0].width = 250;
                        }
                        else if(difficulty == MEDIUM){
                            items[1] = machine_gun;
                            items[1].height = 100;
                            items[1].width = 250;
                        }
                        else{
                            items[2] = machine_gun;
                            items[2].height = 100;
                            items[2].width = 250;
                        }
                        break;
                    }
                    case 3:{
                        DrawTexture(booster, screenWidth/2 -  booster.width/2, screenHeight/2 - booster.height/2 +150, Fade(WHITE, scoreAlpha));
                        if(difficulty == EASY){
                            items[0] = booster;
                            items[0].height = 130;
                            items[0].width = 166;
                        }
                        else if(difficulty == MEDIUM){
                            items[1] = booster;
                            items[1].height = 130;
                            items[1].width = 166;
                        }
                        else{
                            items[2] = booster;
                            items[2].height = 130;
                            items[2].width = 166;
                        }
                        break;
                    }
                    case 4:{
                        DrawTexture(cure, screenWidth/2 -  cure.width/2, screenHeight/2 - cure.height/2 +150, Fade(WHITE, scoreAlpha));
                        if(difficulty == EASY){
                            items[0] = cure;
                            items[0].height = 130;
                            items[0].width = 131;
                        }
                        else if(difficulty == MEDIUM){
                            items[1] = cure;
                            items[1].height = 130;
                            items[1].width = 131;
                        }
                        else{
                            items[2] = cure;
                            items[2].height = 130;
                            items[2].width = 131;
                        }
                        break;
                    }
                    case 5:{
                        DrawTexture(healing_potion, screenWidth/2 -  healing_potion.width/2, screenHeight/2 - healing_potion.height/2 +150, Fade(WHITE, scoreAlpha));
                        if(difficulty == EASY){
                            items[0] = healing_potion;
                            items[0].height = 130;
                            items[0].width = 108;
                        }
                        else if(difficulty == MEDIUM){
                            items[1] = healing_potion;
                            items[1].height = 130;
                            items[1].width = 108;
                        }
                        else{
                            items[2] = healing_potion;
                            items[2].height = 130;
                            items[2].width = 108;
                        }
                        break;
                    }
                    case 6:{
                        DrawTexture(armor, screenWidth/2 - armor.width/2, screenHeight/2 - armor.height/2 +150, Fade(WHITE, scoreAlpha));
                        if(difficulty == EASY){
                            items[0] = armor;
                            items[0].height = 130;
                            items[0].width = 104;
                        }
                        else if(difficulty == MEDIUM){
                            items[1] = armor;
                            items[1].height = 130;
                            items[1].width = 104;
                        }
                        else{
                            items[2] = armor;
                            items[2].height = 130;
                            items[2].width = 104;
                        }
                        break;
                    }
                    case 7:{
                        DrawTexture(plasma_cannon, screenWidth/2 - plasma_cannon.width/2, screenHeight/2 - plasma_cannon.height/2 +150, Fade(WHITE, scoreAlpha));
                        if(difficulty == EASY){
                            items[0] = plasma_cannon;
                            items[0].height = 130;
                            items[0].width = 218;
                        }
                        else if(difficulty == MEDIUM){
                            items[1] = plasma_cannon;
                            items[1].height = 130;
                            items[1].width = 218;
                        }
                        else{
                            items[2] = plasma_cannon;
                            items[2].height = 130;
                            items[2].width = 218;
                        }
                        break;
                    }
                    default: break;
                }
                DrawText("You have got an item", screenWidth/2 - MeasureText("You have got an item", 40)/2, 2*screenHeight/3 -80 , 40, Fade(RED, scoreAlpha));    
            }
            if(eventchoose <= 9){
                DrawText("Community Chest and Chance", screenWidth/2 - MeasureText("Community Chest and Chance", 40)/2, 2*screenHeight/3 +50 , 40, Fade(RED, scoreAlpha));
                DrawText(text, screenWidth/2- MeasureText(text, 30)/2, 2*screenHeight/3 +100, 30,  Fade(BLACK, scoreAlpha));
            }
        }
        
        

        int temp = 0;
        int temp2 = 0;

        for (int i = 0; i < activeEnemies; i++)
        {
            if (enemy[i].active){

                if (enemy[i].type == BOSS) {
                    DrawTextureEx(boss, (Vector2){enemy[i].rec.x, enemy[i].rec.y}, 0.0f, 1.0f, RAYWHITE);
                } else if(enemy[i].type == STANDARD){
                    DrawTextureEx(normal, (Vector2){enemy[i].rec.x, enemy[i].rec.y}, 0.0f, 0.5f, RAYWHITE);
                } else{
                    DrawTextureEx(strong, (Vector2){enemy[i].rec.x, enemy[i].rec.y}, 0.0f, 1.0f, RAYWHITE);
                }

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
        DrawText(TextFormat("Total_Enemies:%04i", total_count_Enemies), 20, 80, 20, RED);
        DrawText(TextFormat("player_HP:%03i", player.HP), 1300, 20, 20, RED);
        DrawText(TextFormat("gold:%04i", gold), 20,40, 20, RED);
        DrawText(TextFormat("AttackPower:%04i", player.AttackPower), 1300, 60, 20, RED);
        DrawText(TextFormat("speed:%2.1f", player.speed.x), 1300, 80, 20, RED);

        if (victory){
            items[2].height = 130;
            DrawTexture(background_heaven, 0, 0, WHITE);
            DrawText("~YOU WIN~", screenWidth/2 - MeasureText("~YOU WIN~", 100)/2, screenHeight/2 - 220, 100, GOLD);
            const char *GoldText = TextFormat("Gold:%04i", gold);
            int GoldTextWidth = MeasureText(GoldText, 60);
            DrawText(GoldText, screenWidth/2 - GoldTextWidth/2 , screenHeight/2 - 105, 60, GOLD);
            DrawRectangle(400, 405, 700, 200, WHITE);
            DrawTexture(items[0], 500 - items[0].width/2 , 525 - items[0].height/2, WHITE);
            DrawTexture(items[1], screenWidth/2 - items[1].width/2, 525 - items[1].height/2, WHITE);
            DrawTexture(items[2], 1000 - items[2].width/2 , 525 - items[2].height/2, WHITE);
        }

        if (pause) DrawText("GAME PAUSED", screenWidth/2 - MeasureText("GAME PAUSED", 40)/2,  40, 40, RED);
    }

    EndDrawing();
}

// Unload game variables
void UnloadGame(void)
{
    for (int i = 0; i < 3; i++) {
        UnloadTexture(items[i]);
    }
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
    UnloadTexture(booster);
    UnloadTexture(healing_potion);
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