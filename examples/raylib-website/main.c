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

    Brick_ComponentId BrickTitle_LabelId = Brick_CreateLabelEx("BRICK", 0, 48);
    

    // Elements and Components

    // Text
    Brick_ComponentId ElementText_TabId = Brick_CreateButton("Text");
    Brick_ElementId ElementText_TextId = Brick_CreateText("Text");

    // Image
    Brick_ComponentId ElementImage_TabId = Brick_CreateButton("Image");

    Texture2D bricks = LoadTexture("resources/bricks.png");
    Brick_ElementId ElementImage_ImageId = Brick_CreateImage(bricks.width, bricks.height, &bricks);

    // Label
    Brick_ComponentId ComponentLabel_TabId = Brick_CreateButton("Label");
    
    // Button
    Brick_ComponentId ComponentButton_TabId = Brick_CreateButton("Button");
    
    // Label Button
    Brick_ComponentId ComponentLabelButton_TabId = Brick_CreateButton("Label Button");
    
    // Image Button
    Brick_ComponentId ComponentImageButton_TabId = Brick_CreateButton("Image Button");
    Brick_ComponentId ComponentImageButton_ButtonId = Brick_CreateImageButton(bricks.width, bricks.height, &bricks);

    // Label Group
    Brick_ComponentId ComponentLabelGroup_TabId = Brick_CreateButton("Label Group");
    
    // Button Group
    Brick_ComponentId ComponentButtonGroup_TabId = Brick_CreateButton("Button Group");

    // Scroll Box
    Brick_ComponentId ContainerScrollBox_TabId = Brick_CreateButton("ScrollBox");

    // Sidebar tabs
    Brick_ComponentId EntitiesTabIds[9] = { 
        ElementText_TabId,
        ElementImage_TabId,
        ComponentLabel_TabId,
        ComponentButton_TabId,
        ComponentLabelButton_TabId,
        ComponentImageButton_TabId,
        ComponentLabelGroup_TabId,
        ComponentButtonGroup_TabId,
        ContainerScrollBox_TabId,
    };
    Brick_ComponentId Entities_TabGroupId = Brick_CreateToggleGroup(EntitiesTabIds, 9);

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
        if (Brick_IsEventTriggered(BRICK_EVENT_TYPE_HOVER)) {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        }
        // The CLEAR event happens once when the HOVER ends
        if (Brick_IsEventTriggered(BRICK_EVENT_TYPE_CLEAR)) {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }
        
        // Raylib begin frame
        BeginDrawing();
        ClearBackground(DARKGRAY);

        // Main Layout
        Brick_BeginLayout();
            // Outer container
            Brick_BeginPanel();
                // Component tabs
                Brick_BeginVerticalBox();
                    Brick_LayoutToggleGroup(Entities_TabGroupId);
                Brick_EndVerticalBox();
                
                // Main content
                Brick_BeginVerticalLayoutEx(BRICK_ALIGN_CENTER);
                    Brick_LayoutLabel(BrickTitle_LabelId);

                    Brick_BeginPanelEx(BRICK_ALIGN_MIDDLE);
                        Brick_BeginSubPanelEx(0.6f, BRICK_ALIGN_MIDDLE);

                        if (Brick_IsButtonToggled(ElementText_TabId)) {
                            Brick_InlineText("Text");

                            Brick_LayoutText(ElementText_TextId);
                        } else if (Brick_IsButtonToggled(ElementImage_TabId)) {
                            Brick_InlineText("Image");
                            
                            Brick_LayoutImage(ElementImage_ImageId);
                        } else if (Brick_IsButtonToggled(ComponentLabel_TabId)) {
                            Brick_InlineText("Label");
                            
                        } else if (Brick_IsButtonToggled(ComponentButton_TabId)) {
                            Brick_InlineText("Button");
                            
                        } else if (Brick_IsButtonToggled(ComponentLabelButton_TabId)) {
                            Brick_InlineText("Label Button");
                            
                        } else if (Brick_IsButtonToggled(ComponentImageButton_TabId)) {
                            Brick_InlineText("Image Button");
                            
                            Brick_LayoutImageButton(ComponentImageButton_ButtonId);
                        } else if (Brick_IsButtonToggled(ComponentLabelGroup_TabId)) {
                            Brick_InlineText("Label Group");
                            
                        } else if (Brick_IsButtonToggled(ComponentButtonGroup_TabId)) {
                            Brick_InlineText("Button Group");
                            
                        } else if (Brick_IsButtonToggled(ContainerScrollBox_TabId)) {
                            Brick_InlineText("ScrollBox");
                            
                        }
                        Brick_EndSubPanel();
                    Brick_EndPanel();
                Brick_EndVerticalLayout();

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
