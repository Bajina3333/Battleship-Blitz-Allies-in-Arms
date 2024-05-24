    #include "raylib.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

static void UpdateDrawFrame(void);

typedef struct Button {
    Rectangle rect;
    Color color;
} Button;

Button button_0 = {0};
bool button_0_clicked = false;

void init_button(Button *button, Rectangle rect, Color color) {
    button->rect = rect;
    button->color = color;
}

bool is_mouse_over_button(Button button) {
    return CheckCollisionPointRec(GetMousePosition(), button.rect);
}

int main() {
    static const int screenWidth = 800;
    static const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib");

    init_button(&button_0, (Rectangle){screenWidth / 2 - 100, screenHeight / 2 - 50, 200, 100}, RED);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        UpdateDrawFrame();
    }

    CloseWindow(); // Don't forget to close the window

    return 0;
}

static void UpdateDrawFrame(void) {
    if (is_mouse_over_button(button_0)) {
        button_0.color = BLUE;
    } else {
        button_0.color = RED;
    }

    if (is_mouse_over_button(button_0) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        button_0_clicked = true;
    }

    BeginDrawing();

    if (button_0_clicked) {
        ClearBackground(BLACK);
    } else {
        ClearBackground(RAYWHITE);
    }

    DrawRectangleRec(button_0.rect, button_0.color);

    DrawText("Click me", button_0.rect.x + button_0.rect.width / 2 - MeasureText("Click me", 20) / 2, button_0.rect.y + button_0.rect.height / 2 - 20 / 2, 20, WHITE);

    DrawText("test", 10, 40, 20, DARKGRAY);

    DrawFPS(10, 10);

    EndDrawing();
}

        
        