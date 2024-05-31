
#include "raylib.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Some Defines
// 定義遊戲參數
//----------------------------------------------------------------------------------
#define NUM_SHOOTS 50
#define NUM_MAX_ENEMIES 50
#define FIRST_WAVE 10
#define SECOND_WAVE 20
#define THIRD_WAVE 50

//----------------------------------------------------------------------------------
// Types and Structures Definition
// 定義一個枚舉類型 EnemyWave，用於標記遊戲中的敵人波次。
// 定義一個 Player 結構，用來存儲玩家的位置、速度和顏色。
// 定義一個 Enemy 結構，用來存儲敵人的位置、速度、是否激活和顏色。
// 定義一個 Shoot 結構，用來存儲射擊的位置、速度、是否激活和顏色。
//----------------------------------------------------------------------------------
typedef enum { FIRST = 0, SECOND, THIRD } EnemyWave;

typedef struct Player {
    Rectangle rec;
    Vector2 speed;
    Color color;
} Player;

typedef struct Enemy {
    Rectangle rec;
    Vector2 speed;
    bool active;
    Color color;
} Enemy;

typedef struct Shoot {
    Rectangle rec;
    Vector2 speed;
    bool active;
    Color color;
} Shoot;

typedef struct Button {
    Rectangle rect;
    Color color;
} Button;

//------------------------------------------------------------------------------------
// Global Variables Declaration
// 全域變數宣告
// 定義螢幕的寬度和高度。
// 定義遊戲是否結束、是否暫停、得分和是否勝利的變數。
// 初始化玩家、敵人陣列、射擊陣列和敵人波次。
// 定義射擊頻率和用於特效的 alpha 值。
// 定義當前活躍的敵人數、已殺敵人數和動畫平滑過渡的標記。
//------------------------------------------------------------------------------------
static const int screenWidth = 800;
static const int screenHeight = 450;

static bool gameOver = false;
static bool gamebegining = true;
static bool gameaction = false;
static bool gamechoose = false;
static bool pause = false;
static int score = 0;
static bool victory = false;

static Player player = { 0 };
static Enemy enemy[NUM_MAX_ENEMIES] = { 0 };
static Shoot shoot[NUM_SHOOTS] = { 0 };
static EnemyWave wave = { 0 };

static int shootRate = 0;
static float alpha = 0.0f;

static int activeEnemies = 0;
static int enemiesKill = 0;
static bool smooth = false;

static Button button_0 = { 0 };
static Button button_1 = { 0 };
static bool button_0_clicked = false;
static bool button_1_clicked = false;

//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
static void InitGame(int n);         // Initialize game
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
    // 主程式入口，設定遊戲窗口並初始化遊戲
    //---------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "classic game: space invaders");
    
    InitGame(1);

    // 設定遊戲主循環。在 Web 平台上使用 Emscripten 設定，其他平台則設定目標 FPS 為 60。
#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);

    // Main game loop
    // 當窗口未被關閉時，持續進行遊戲的更新和繪製。
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update and Draw
        UpdateDrawFrame();
    }
#endif

    // De-Initialization
    UnloadGame();         // Unload loaded data (textures, sounds, models...)

    CloseWindow();        // Close window and OpenGL context

    return 0;
}

//------------------------------------------------------------------------------------
// Module Functions Definitions (local)
//------------------------------------------------------------------------------------

// Initialize game variables

void init_button(Button *button, Rectangle rect, Color color) {
    button->rect = rect;
    button->color = color;
}

bool is_mouse_over_button(Button button) {
    return CheckCollisionPointRec(GetMousePosition(), button.rect);
}

// 初始化遊戲函式
void InitGame(int n)
{
    // Initialize game variables
    //射擊頻率、是否暫停、遊戲是否結束、是否勝利、當前的敵人波次、活躍的敵人數量、殺敵數量、分數和透明度
    shootRate = 0;
    pause = false;
    gameOver = false;
    gamebegining = true;
    gameaction = false;
    victory = false;
    smooth = false;
    button_0_clicked = false;
    button_1_clicked = false;
    
    wave = FIRST;
    activeEnemies = FIRST_WAVE;
    enemiesKill = 0;
    score = 0;
    alpha = 0;

    // Initialize player
    // 初始化玩家的位置、尺寸、速度和顏色。
    player.rec.x = 20;
    player.rec.y = 50;
    player.rec.width = 20;
    player.rec.height = 20;
    player.speed.x = 5;
    player.speed.y = 5;
    if(n == 1){
        player.color = BLACK;
    }else if(n == 2){
        player.color = RED;
    }else if(n == 3){
        player.color = BLUE;
    }
    
    // Initialize enemies
    // 通過迴圈初始化每個敵人的大小、隨機位置、速度、激活狀態和顏色。
    // 敵人的初始 x 位置在視窗的右邊外面，讓它們從右向左移動進入畫面。
    for (int i = 0; i < NUM_MAX_ENEMIES; i++)
    {
        enemy[i].rec.width = 10;
        enemy[i].rec.height = 10;
        enemy[i].rec.x = GetRandomValue(screenWidth, screenWidth + 1000);
        enemy[i].rec.y = GetRandomValue(0, screenHeight - enemy[i].rec.height);
        enemy[i].speed.x = 5;
        enemy[i].speed.y = 5;
        enemy[i].active = true;
        enemy[i].color = GRAY;
    }

    // Initialize shoots
    // 初始化射擊的位置（基於玩家的位置）、尺寸、速度和顏色。射擊預設為不活躍狀態，直到玩家發射它們。
    for (int i = 0; i < NUM_SHOOTS; i++)
    {
        shoot[i].rec.x = player.rec.x;
        shoot[i].rec.y = player.rec.y + player.rec.height / 4;
        shoot[i].rec.width = 10;
        shoot[i].rec.height = 5;
        shoot[i].speed.x = 7;
        shoot[i].speed.y = 0;
        shoot[i].active = false;
        shoot[i].color = MAROON;
    }

    // Initialize button
    init_button(&button_0, (Rectangle){screenWidth / 2 - 50, screenHeight / 2 - 100, 100, 50}, RED);
    init_button(&button_1, (Rectangle){screenWidth / 2 - 50, screenHeight / 2 + 50, 100, 50}, RED);
}

// Update game (one frame)
// 敵人的移動在遊戲的更新函式 UpdateGame() 中處理
void UpdateGame(void)
{
    if (gameaction)
    {
        // 如果遊戲未結束，檢查是否按下 'P' 鍵來切換暫停狀態
        if (IsKeyPressed('P')) pause = !pause;

        if (!pause)
        {
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

                        activeEnemies = SECOND_WAVE;
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

                        activeEnemies = THIRD_WAVE;
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

            // Player movement
            if (IsKeyDown(KEY_RIGHT)) player.rec.x += player.speed.x;
            if (IsKeyDown(KEY_LEFT)) player.rec.x -= player.speed.x;
            if (IsKeyDown(KEY_UP)) player.rec.y -= player.speed.y;
            if (IsKeyDown(KEY_DOWN)) player.rec.y += player.speed.y;

            // Player collision with enemy
            for (int i = 0; i < activeEnemies; i++)
            {
                if (CheckCollisionRecs(player.rec, enemy[i].rec)) {
                gameOver = true;
                gameaction = false;
                }
                
            }

            // Enemy behaviour
            // 敵人移動
            /* 使每個活躍的敵人朝著屏幕左側移動。如果敵人移動到屏幕左側邊界之外，它會被重新位置到屏幕右側的一個隨機位置，從而循環進入屏幕。這樣不斷重複的行為模擬了敵人波的不斷進攻 */
            for (int i = 0; i < activeEnemies; i++)
            {
                if (enemy[i].active)
                {
                    enemy[i].rec.x -= enemy[i].speed.x;

                    if (enemy[i].rec.x < 0)
                    {
                        enemy[i].rec.x = GetRandomValue(screenWidth, screenWidth + 1000);
                        enemy[i].rec.y = GetRandomValue(0, screenHeight - enemy[i].rec.height);
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
                    if (!shoot[i].active && shootRate % 20 == 0)
                    {
                        shoot[i].rec.x = player.rec.x;
                        shoot[i].rec.y = player.rec.y + player.rec.height / 4;
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
                                enemy[j].rec.x = GetRandomValue(screenWidth, screenWidth + 1000);
                                enemy[j].rec.y = GetRandomValue(0, screenHeight - enemy[j].rec.height);
                                shootRate = 0;
                                enemiesKill++;
                                score += 100;
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
    else if (gamebegining)
    {
        if (is_mouse_over_button(button_0) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            button_0_clicked = true;
        }
        if (button_0_clicked) 
        {
            InitGame(3);
            gamebegining = false;
            gameOver = false;
            gameaction = true;
        }
        if (is_mouse_over_button(button_1) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            button_1_clicked = true;
        }
        if (button_1_clicked) 
        {
            exit(1);
        }
    }
    else if (gameOver){
         if (is_mouse_over_button(button_0) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            button_0_clicked = true;
        }
        if (button_0_clicked) 
        {
            InitGame(3);
            gamebegining = false;
            gameOver = false;
            gameaction = true;
        }
        if (is_mouse_over_button(button_1) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            button_1_clicked = true;
        }
        if (button_1_clicked) 
        {
            exit(1);
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
        if (is_mouse_over_button(button_0)) {
            button_0.color = BLUE;
        } else {
            button_0.color = RED;
        }
        if (is_mouse_over_button(button_1)) {
            button_1.color = BLUE;
        } else {
            button_1.color = RED;
        }
        DrawRectangleRec(button_0.rect, button_0.color);
        DrawText("start", button_0.rect.x + button_0.rect.width / 2 - MeasureText("start", 20) / 2, button_0.rect.y + button_0.rect.height / 2 - 20 / 2, 20, WHITE);
        DrawRectangleRec(button_1.rect, button_1.color);
        DrawText("quit", button_1.rect.x + button_1.rect.width / 2 - MeasureText("quit", 20) / 2, button_1.rect.y + button_1.rect.height / 2 - 20 / 2, 20, WHITE);
    }

    else if (gameaction)
    {
        DrawRectangleRec(player.rec, player.color);

        if (wave == FIRST) DrawText("FIRST WAVE", screenWidth / 2 - MeasureText("FIRST WAVE", 40) / 2, screenHeight / 2 - 40, 40, Fade(BLACK, alpha));
        else if (wave == SECOND) DrawText("SECOND WAVE", screenWidth / 2 - MeasureText("SECOND WAVE", 40) / 2, screenHeight / 2 - 40, 40, Fade(BLACK, alpha));
        else if (wave == THIRD) DrawText("THIRD WAVE", screenWidth / 2 - MeasureText("THIRD WAVE", 40) / 2, screenHeight / 2 - 40, 40, Fade(BLACK, alpha));

        for (int i = 0; i < activeEnemies; i++)
        {
            if (enemy[i].active) DrawRectangleRec(enemy[i].rec, enemy[i].color);
        }

        for (int i = 0; i < NUM_SHOOTS; i++)
        {
            if (shoot[i].active) DrawRectangleRec(shoot[i].rec, shoot[i].color);
        }

        DrawText(TextFormat("%04i", score), 20, 20, 40, GRAY);

        if (victory) DrawText("YOU WIN", screenWidth / 2 - MeasureText("YOU WIN", 40) / 2, screenHeight / 2 - 40, 40, BLACK);

        if (pause) DrawText("GAME PAUSED", screenWidth / 2 - MeasureText("GAME PAUSED", 40) / 2, screenHeight / 2 - 40, 40, GRAY);
    }
    if(gameOver)
    {
        if (is_mouse_over_button(button_0)) {
            button_0.color = BLUE;
        } else {
            button_0.color = RED;
        }
        if (is_mouse_over_button(button_1)) {
            button_1.color = BLUE;
        } else {
            button_1.color = RED;
        }
        DrawRectangleRec(button_0.rect, button_0.color);
        DrawText("RETRY", button_0.rect.x + button_0.rect.width / 2 - MeasureText("RETRY", 20) / 2, button_0.rect.y + button_0.rect.height / 2 - 20 / 2, 20, WHITE);
        DrawRectangleRec(button_1.rect, button_1.color);
        DrawText("quit", button_1.rect.x + button_1.rect.width / 2 - MeasureText("quit", 20) / 2, button_1.rect.y + button_1.rect.height / 2 - 20 / 2, 20, WHITE);
    }

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
