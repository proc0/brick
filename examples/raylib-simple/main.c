#define BRICK_IMPLEMENTATION
#include "brick.h"

#include "raylib.h"

int main(void) {
    InitWindow(1080, 720, "Brick Raylib Simple");

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(GRAY);
    	DrawFPS(20, 20);
        EndDrawing();
    }

    CloseWindow();

	return 0;
}
