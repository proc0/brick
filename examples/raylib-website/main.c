// define BRICK_IMPLEMENTATION only ONCE in one file across a project (compile target)
// then include "brick.h", which can also be included in other files (without BRICK_IMPLEMENTATION)
#define BRICK_IMPLEMENTATION
#include "brick.h"

// include default Clay renderers or a custom one
#include "renderers/raylib/clay_renderer_raylib.c"

#include "raylib.h"

// Example settings
#define EXAMPLE_TITLE "Brick Raylib Simple"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define FONT_PATH "resources/Roboto-Regular.ttf"

int main(void) {
    // Initialize the Clay renderer. This function calls InitOverlay which loads shaders for transparent windows.
    // Additionally it also initializes Raylib with InitWindow. If Raylib needs to initialize separately, simply
    // call InitOverlay from the Clay renderers folder (renderers/raylib/clay_renderer_raylib.c) separately, or
    // use a custom Clay renderer that handles it differently.
    Clay_Raylib_Initialize(SCREEN_WIDTH, SCREEN_HEIGHT, EXAMPLE_TITLE, FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    
    // Initialize fonts into an array that can be passed to the Clay MeasureText function
    // that is specific to the graphics library, i.e. Raylib. Here the font is initialized
    // with Raylib LoadFontEx, and the filter is configured so it renders smoothly.
    Font fonts[1];
    fonts[0] = LoadFontEx(FONT_PATH, 48, 0, 400);
    SetTextureFilter(fonts[0].texture, TEXTURE_FILTER_BILINEAR);

    // Initialize Brick by passing screen dimensions, the Clay MeasureText function specific to the
    // graphics library being used, i.e. Raylib, and the array of fonts that were initialized above.
    Brick_Initialize(SCREEN_WIDTH, SCREEN_HEIGHT, Raylib_MeasureText, fonts);

    // Create Brick elements and save their Ids to be used with the same element's
    // Layout<element> function. Here we create a button and save its ID.
    // Brick_ElementId helloWorld_ButtonId = Brick_CreateButton("BRICK");

    while(!WindowShouldClose()) {
        // Use Brick_Resize with window dimensions for esponsive element and container sizes
        if (IsWindowResized()) {
            Brick_Resize((float)GetScreenWidth(), (float)GetScreenHeight());
        }
        // Update Brick Events. Brick_UpdateEvents take pointer information to pass on to Clay:
        // the mouse position (x and y), the mouseWheel scroll position (scrollX, scrollY),
        // whether the mouse is pressed or released (the exact frame when this happens)
        // and finally the DeltaTime of each frame, here Raylibe provides GetFrameTime for this.
        Vector2 mouseWheel = GetMouseWheelMoveV();
        bool isPressed = IsMouseButtonDown(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
        bool isReleased = IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
        Brick_PointerData pointerData = (Brick_PointerData){ 
            .x = GetMouseX(), 
            .y = GetMouseY(), 
            .scrollX = mouseWheel.x, 
            .scrollY = mouseWheel.y, 
            .pressed = isPressed, 
            .released = isReleased
        };
        // Update events with pointer data and delta time
        Brick_UpdateEvents(pointerData, GetFrameTime());

        // Handle element events, either by saving the EventArray returned by UpdateEvents (not used now)
        // or using any of the event queries i.e. IsEventTriggeredById
        // Here we set the mouse cursor to the hand on button hover, for this specific button
        // if (Brick_IsEventTriggeredById(BRICK_EVENT_TYPE_HOVER, helloWorld_ButtonId)) {
        //     SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        // }
        // // The CLEAR event happens once when the HOVER ends
        // if (Brick_IsEventTriggeredById(BRICK_EVENT_TYPE_CLEAR, helloWorld_ButtonId)) {
        //     SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        // }
        
        // Raylib begin frame
        BeginDrawing();
        ClearBackground(DARKGRAY);

        // Main Layout
        Brick_BeginLayout();
            // A floating panel at the center of the screen
            Brick_BeginPanel();
                // inline text for the title
                Brick_InlineTextEx("Brick", 0, 48);
                // the button that was created during init

            // Always close containers
            Brick_EndPanel();
        // Close the main layout and save Clay RenderCommands to pass unto the Clay renderer.
        Clay_RenderCommandArray renderCommands = Brick_EndLayout(GetFrameTime());
        // Call the Clay renderer with the Clay_RenderCommandArray. This is included from
        // Clay's renderers folder (renderers/raylib/clay_renderer_raylib.c), but a custom renderer
        // can also be used, that could handle fonts differently (i.e. as a class member)
        Clay_Raylib_Render(renderCommands, fonts);

    	DrawFPS(20, 20);
        EndDrawing();
    }

    // This cleans up the Clay arena
    Brick_Destroy();
    // If using the default Raylib renderer from Clay's renderer,
    // this function should be called to cleanup the temp string buffer.
    Clay_Raylib_Close();

	return 0;
}
