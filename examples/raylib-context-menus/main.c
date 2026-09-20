// define BRICK_IMPLEMENTATION only ONCE in one file across a project (compile target)
// then include "brick.h", which can also be included in other files (without BRICK_IMPLEMENTATION)
#define BRICK_IMPLEMENTATION
#include "brick.h"

// include default Clay renderers or a custom one
#include "renderers/raylib/clay_renderer_raylib.c"

#include "raylib.h"

// Example settings
#define EXAMPLE_TITLE "Brick Raylib Context Menus"
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

    // file menu button
    Brick_ComponentId file_ButtonId = Brick_CreateLabelButton("File");
    // file menu option buttons
    Brick_ComponentId fileOpen_ButtonId = Brick_CreateLabelButton("Open");
    Brick_ComponentId fileSave_ButtonId = Brick_CreateLabelButton("Save");
    Brick_ComponentId fileExport_ButtonId = Brick_CreateLabelButton("Export");
    Brick_ComponentId fileQuit_ButtonId = Brick_CreateLabelButton("Quit");
    // file menu button group (avoids calling layout on every single one)
    Brick_ComponentId fileMenuGroup[4] = { fileOpen_ButtonId, fileSave_ButtonId, fileExport_ButtonId, fileQuit_ButtonId };
    Brick_ComponentId fileMenu_ButtonGroupId = Brick_CreateToggleGroup(fileMenuGroup, 4);

    // continue defining all the buttons:
    // edit menu
    Brick_ComponentId edit_ButtonId = Brick_CreateLabelButton("Edit");
    Brick_ComponentId editCopy_ButtonId = Brick_CreateLabelButton("Copy");
    Brick_ComponentId editPaste_ButtonId = Brick_CreateLabelButton("Paste");
    Brick_ComponentId editProject_ButtonId = Brick_CreateLabelButton("Project");
    Brick_ComponentId editSettings_ButtonId = Brick_CreateLabelButton("Settings");
    // edit menu button group
    Brick_ComponentId editMenuGroup[4] = { editCopy_ButtonId, editPaste_ButtonId, editProject_ButtonId, editSettings_ButtonId };
    Brick_ComponentId editMenu_ButtonGroupId = Brick_CreateToggleGroup(editMenuGroup, 4);

    // page menu
    Brick_ComponentId page_ButtonId = Brick_CreateLabelButton("Page");
    Brick_ComponentId pageResize_ButtonId = Brick_CreateLabelButton("Resize");
    Brick_ComponentId pageCrop_ButtonId = Brick_CreateLabelButton("Crop");
    Brick_ComponentId pageLayer_ButtonId = Brick_CreateLabelButton("Layer");
    // etc
    Brick_ComponentId pageMenuGroup[3] = { pageResize_ButtonId, pageCrop_ButtonId, pageLayer_ButtonId };
    Brick_ComponentId pageMenu_ButtonGroupId = Brick_CreateToggleGroup(pageMenuGroup, 3);

    // view menu
    Brick_ComponentId view_ButtonId = Brick_CreateLabelButton("View");
    Brick_ComponentId viewShowSidebar_ButtonId = Brick_CreateLabelButton("Show Sidebar");
    Brick_ComponentId viewHideSidebar_ButtonId = Brick_CreateLabelButton("Hide Sidebar");
    Brick_ComponentId viewColorTheme_ButtonId = Brick_CreateLabelButton("Color Theme");
    Brick_ComponentId viewMenuGroup[3] = { viewShowSidebar_ButtonId, viewHideSidebar_ButtonId, viewColorTheme_ButtonId };
    Brick_ComponentId viewMenu_ButtonGroupId = Brick_CreateToggleGroup(viewMenuGroup, 3);

    // tool menu
    Brick_ComponentId tool_ButtonId = Brick_CreateLabelButton("Tool");
    Brick_ComponentId toolExtensions_ButtonId = Brick_CreateLabelButton("Extensions");
    Brick_ComponentId toolTransform_ButtonId = Brick_CreateLabelButton("Transform");
    Brick_ComponentId toolFilters_ButtonId = Brick_CreateLabelButton("Filters");
    Brick_ComponentId toolMenuGroup[3] = { toolExtensions_ButtonId, toolTransform_ButtonId, toolFilters_ButtonId };
    Brick_ComponentId toolMenu_ButtonGroupId = Brick_CreateToggleGroup(toolMenuGroup, 3);

    // help menu
    Brick_ComponentId help_ButtonId = Brick_CreateLabelButton("Help");
    Brick_ComponentId helpAbout_ButtonId = Brick_CreateLabelButton("About");
    Brick_ComponentId helpMenuGroup[1] = { helpAbout_ButtonId };
    Brick_ComponentId helpMenu_ButtonGroupId = Brick_CreateToggleGroup(helpMenuGroup, 1);

    // finally all of the menu buttons themselves in one button group
    Brick_ComponentId topMenuGroup[6] = { file_ButtonId, edit_ButtonId, page_ButtonId, view_ButtonId, tool_ButtonId, help_ButtonId };
    Brick_ComponentId topMenu_ButtonGroupId = Brick_CreateToggleGroup(topMenuGroup, 6);

    // and for the UI state, a variable for the menu button, and the menu button group
    // to track which menu button was pressed and then set the menu group accordingly.
    Brick_ComponentId currentMenuId = (Brick_ComponentId){0, 0, 0};
    Brick_ComponentId currentMenuGroupId = (Brick_ComponentId){0, 0, 0};

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

        // Update events with pointer data and delta time and save event array
        Brick_EventArray eventArray = Brick_UpdateEvents(pointerData, GetFrameTime());

        // iterate over the events to check which menu was pressed
        // and save the button element id as well as the button group element id
        // so we can render the menu in the layout section. As described in the layout
        // comment section, there are multiple ways of achieving this. This way is less
        // repetitive but requires handling events.
        for (int i=0; i<eventArray.length; i++) {
            Brick_Event* event = Brick_EventArray_Get(&eventArray, i);

            // for this example just handle press events
            if (event->type == BRICK_EVENT_TYPE_PRESS) {
                // set the current menu to the pressed event element id
                currentMenuId = event->componentId;

                // check to see which of the menu buttons was pressed
                // and set the current menu group to that group defined at that top
                if (BRICK_ID_EQUALS(currentMenuId, file_ButtonId)) {
                    currentMenuGroupId = fileMenu_ButtonGroupId;
                } else if (BRICK_ID_EQUALS(currentMenuId, edit_ButtonId)) {
                    currentMenuGroupId = editMenu_ButtonGroupId;
                } else if (BRICK_ID_EQUALS(currentMenuId, page_ButtonId)) {
                    currentMenuGroupId = pageMenu_ButtonGroupId;
                } else if (BRICK_ID_EQUALS(currentMenuId, view_ButtonId)) {
                    currentMenuGroupId = viewMenu_ButtonGroupId;
                } else if (BRICK_ID_EQUALS(currentMenuId, tool_ButtonId)) {
                    currentMenuGroupId = toolMenu_ButtonGroupId;
                } else if (BRICK_ID_EQUALS(currentMenuId, help_ButtonId)) {
                    currentMenuGroupId = helpMenu_ButtonGroupId;
                } else {
                    currentMenuId = (Brick_ComponentId){0, 0, 0};
                    currentMenuGroupId = (Brick_ComponentId){0, 0, 0};
                }
            }
        }

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
            // A floating panel at the center of the screen
            Brick_BeginFloatingPanel();
                // the button that was created during init
                Brick_BeginWrapper();
                Brick_BeginHorizontalStack();
                    Brick_LayoutGroup(topMenu_ButtonGroupId);
                Brick_EndHorizontalStack();
                Brick_EndWrapper();

                // render the context menu if it is toggled
                // check that the ID is not null (simple convention to close the dropdown)
                if (BRICK_ID_NOT_NULL(currentMenuId) && Brick_IsButtonToggled(currentMenuId)) {
                    // the dropdown container for the menu
                    Brick_BeginDropdown(currentMenuId);
                    // the vertical stack of menu options in the dropdown
                    Brick_BeginVerticalStack();
                        // all the buttons in the menu group
                        Brick_LayoutGroup(currentMenuGroupId);
                    Brick_EndVerticalStack();
                    Brick_EndDropdown();
                }

                // Alternatively, this can be rendered without handling events
                // by using the button element id and the button group element ids
                // directly. This is more verbose but no event handling is required.

                // if (Brick_IsButtonToggled(file_ButtonId)) {
                //     Brick_BeginDropdown(file_ButtonId);
                //     Brick_BeginVerticalStack();
                //         Brick_LayoutButtonGroup(fileMenu_ButtonGroupId);
                //     Brick_EndVerticalStack();
                //     Brick_EndDropdown();
                // } else if (Brick_IsButtonToggled(edit_ButtonId)) {
                //     Brick_BeginDropdown(edit_ButtonId);
                //     Brick_BeginVerticalStack();
                //         Brick_LayoutButtonGroup(editMenu_ButtonGroupId);
                //     Brick_EndVerticalStack();
                //     Brick_EndDropdown();
                // } else if (Brick_IsButtonToggled(page_ButtonId)) {
                //     Brick_BeginDropdown(page_ButtonId);
                //     Brick_BeginVerticalStack();
                //         Brick_LayoutButtonGroup(pageMenu_ButtonGroupId);
                //     Brick_EndVerticalStack();
                //     Brick_EndDropdown();
                // } else if (Brick_IsButtonToggled(view_ButtonId)) {
                //     Brick_BeginDropdown(view_ButtonId);
                //     Brick_BeginVerticalStack();
                //         Brick_LayoutButtonGroup(viewMenu_ButtonGroupId);
                //     Brick_EndVerticalStack();
                //     Brick_EndDropdown();
                // } else if (Brick_IsButtonToggled(tool_ButtonId)) {
                //     Brick_BeginDropdown(tool_ButtonId);
                //     Brick_BeginVerticalStack();
                //         Brick_LayoutButtonGroup(toolMenu_ButtonGroupId);
                //     Brick_EndVerticalStack();
                //     Brick_EndDropdown();
                // } else if (Brick_IsButtonToggled(help_ButtonId)) {
                //     Brick_BeginDropdown(help_ButtonId);
                //     Brick_BeginVerticalStack();
                //         Brick_LayoutButtonGroup(helpMenu_ButtonGroupId);
                //     Brick_EndVerticalStack();
                //     Brick_EndDropdown();
                // }
            // Always close containers
            Brick_EndFloatingPanel();
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
