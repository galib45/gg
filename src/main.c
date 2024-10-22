#include "gg.h"

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 200

int main(void) {
    // Initialize Raylib
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Timer");
    SetTargetFPS(60);
    // Set the window to always be on top
    // SetWindowState(FLAG_WINDOW_TOPMOST);

    if(!ggInit()) return -1;
    char *text; Vector2 mouse = {0};
    float totalsecs = 30*60;
    int hr, min, sec, mili;
    float rem = totalsecs;

    bool timerRunning = false;

    // Main game loop
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        // mouse = GetMousePosition();
        // text = TextFormat("%.2f, %.2f", mouse.x, mouse.y);
        if (timerRunning && rem > 0) rem -= GetFrameTime();
        hr = rem / 3600; min = (rem - hr*3600) / 60;
        sec = (rem - hr*3600 - min*60); mili = (rem - hr*3600 - min*60 - sec) * 1000;

        text = TextFormat("%02d:%02d:%02d.%d", hr, min, sec, (int)mili/100);

        bool clicked = ggDrawTextPro(
            &ggfont, text, (Vector2) {200, 110}, 60, 
            DARKGRAY, GGTEXT_ALIGN_TOP_CENTER
        );

        if (clicked) {
            timerRunning = !timerRunning;
        }
        
        ggClear();
        EndDrawing();
    }

    if(!ggClean()) return -1;
    CloseWindow();

    return 0;
}
