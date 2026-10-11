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

#define ELEMENT_TEXT_SUMMARY "A simple text element. It can be created and used in the layout with an ID, or just using the inline function:\n\nBrick_ElementId Brick_CreateText(const char* string)\nBrick_CreateTextEx(const char* string, uint16_t fontId, uint16_t fontSize)\n\nvoid Brick_LayoutText(textId)\n\nvoid Brick_InlineText(const char* string)\nvoid Brick_InlineTextEx(const char* string, uint16_t fontId, uint16_t fontSize)\n\n"
#define ELEMENT_IMAGE_SUMMARY "An image element depends on the whatever data the renderer expects to display pixels on the screen. It could be an image format or a texture or something else. This data is passed through in the Clay_RenderCommand.\n\nBrick_ElementId Brick_CreateImage(float width, float height, void* imageData)\n\nvoid Brick_LayoutImage(Brick_ElementId imageId)\n"

#define COMPONENT_LABEL_SUMMARY "A text element in a box. Labels are the simplest components that enclose text elements in a box element that contains padding, alignment and other properties.\n\nBrick_ComponentId Brick_CreateLabel(const char* string)\nBrick_ComponentId Brick_CreateLabelEx(const char* string, uint16_t fontId, uint16_t fontSize)\n\nvoid Brick_LayoutLabel(ComponentId labelId)\n"
#define COMPONENT_BUTTON_SUMMARY "The humble button. It is essentially a label with a background plus interactivity. It tracks its different states like hover, pressed, released that an be queried for UI functionality.\n\nBrick_ComponentId Brick_CreateButton(const char* text)\nBrick_ComponentId Brick_CreateButtonEx(const char* text, uint16_t fontId, uint16_t fontSize)\n\nvoid Brick_LayoutButton(Brick_ComponentId buttonId)\n"
#define COMPONENT_LABELBUTTON_SUMMARY "A label with button interactivity. Labels do not have a background like buttons do. Label buttons act like buttons while remaining texts in a box.\n\nBrick_ComponentId Brick_CreateLabelButton(const char* text)\n\nvoid Brick_LayoutLabelButton(Brick_ComponentId buttonId)\n"
#define COMPONENT_IMAGEBUTTON_SUMMARY "An image with button interactivity. Like label-buttons, image-buttons are images that are also interactive buttons.\n\nBrick_ComponentId Brick_CreateImageButton(float width, float height, void* imageData)\nvoid Brick_LayoutImageButton(Brick_ComponentId buttonId)\n"
#define COMPONENT_LABELGROUP_SUMMARY "A group of labels. Create a number of labels, then use an array of label IDs to group them.\n\nBrick_ComponentId Brick_CreateLabel(const char* string)\nBrick_ComponentId labelIds[3] = { labelId1, labelId2, labelId3 }\nBrick_ComponentId Brick_CreateGroup(const Brick_ComponentId* labelIds, int32_t groupSize)\n\nvoid Brick_LayoutGroup(Brick_ComponentId labelGroupId)"
#define COMPONENT_BUTTONGROUP_SUMMARY "A group of buttons. Create a number of buttons, then use an array of button IDs to group them.\n\nBrick_ComponentId Brick_CreateButton(const char* text)\nBrick_ComponentId buttonIds[3] = { buttonId1, buttonId2, buttonId3 }\nBrick_ComponentId Brick_CreateGroup(const Brick_ComponentId* buttonIds, int32_t groupSize)\n\nvoid Brick_LayoutGroup(Brick_ComponentId buttonGroupId)"

#define CONTAINER_SCROLLBOX_SUMMARY "A container that scrolls vertically. Create the container first, then use Begin and End to enclose the content that will be scrolled:\n\nBrick_ContainerId Brick_CreateScrollBox(void)\n\nvoid Brick_BeginScrollBox(void)\n    [contents]\nvoid Brick_EndScrollBox(void)\n"
#define CONTAINER_SCROLLBOX_TEXT "Faucibus purus in massa tempor nec. Nec ullamcorper sit amet risus nullam eget felis eget nunc. Diam vulputate ut pharetra sit amet aliquam id diam. Lacus suspendisse faucibus interdum posuere lorem. A diam sollicitudin tempor id. Amet massa vitae tortor condimentum lacinia. Aliquet nibh praesent tristique magna."

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

    #define COMP_TITLE_FONT_SIZE 36

    // Text
    Brick_ComponentId ElementText_LabelId = Brick_CreateLabelEx("Text", 0, COMP_TITLE_FONT_SIZE);
    Brick_ComponentId ElementText_TabId = Brick_CreateButton("Text");
    Brick_ElementId ElementText_TextId = Brick_CreateText("Text");

    // Image
    Brick_ComponentId ElementImage_LabelId = Brick_CreateLabelEx("Image", 0, COMP_TITLE_FONT_SIZE);
    Brick_ComponentId ElementImage_TabId = Brick_CreateButton("Image");

    Texture2D bricks = LoadTexture("resources/bricks.png");
    Brick_ElementId ElementImage_ImageId = Brick_CreateImage(bricks.width, bricks.height, &bricks);

    // Label
    Brick_ComponentId ComponentLabel_LabelId = Brick_CreateLabelEx("Label", 0, COMP_TITLE_FONT_SIZE);
    Brick_ComponentId ComponentLabel_TabId = Brick_CreateButton("Label");

    Brick_ComponentId ComponentLabel2_LabelId = Brick_CreateLabel("Label");
    
    // Button
    Brick_ComponentId ComponentButton_LabelId = Brick_CreateLabelEx("Button", 0, COMP_TITLE_FONT_SIZE);
    Brick_ComponentId ComponentButton_TabId = Brick_CreateButton("Button");
    
    Brick_ComponentId ComponentButton_ButtonId = Brick_CreateButton("Button");
    
    // Label Button
    Brick_ComponentId ComponentLabelButton_LabelId = Brick_CreateLabelEx("Label Button", 0, COMP_TITLE_FONT_SIZE);
    Brick_ComponentId ComponentLabelButton_TabId = Brick_CreateButton("Label Button");

    Brick_ComponentId ComponentLabelButton_LabelButtonId = Brick_CreateLabelButton("Label Button");
    
    // Image Button
    Brick_ComponentId ComponentImageButton_LabelId = Brick_CreateLabelEx("Image Button", 0, COMP_TITLE_FONT_SIZE);
    Brick_ComponentId ComponentImageButton_TabId = Brick_CreateButton("Image Button");

    Brick_ComponentId ComponentImageButton_ImageButtonId = Brick_CreateImageButton(bricks.width, bricks.height, &bricks);

    // Label Group
    Brick_ComponentId ComponentLabelGroup_LabelId = Brick_CreateLabelEx("Label Group", 0, COMP_TITLE_FONT_SIZE);
    Brick_ComponentId ComponentLabelGroup_TabId = Brick_CreateButton("Label Group");
    
    Brick_ComponentId ComponentLabel3_LabelId = Brick_CreateLabel("Label 1");
    Brick_ComponentId ComponentLabel4_LabelId = Brick_CreateLabel("Label 2");
    Brick_ComponentId ComponentLabel5_LabelId = Brick_CreateLabel("Label 3");

    Brick_ComponentId labelGroup[3] = { ComponentLabel3_LabelId, ComponentLabel4_LabelId, ComponentLabel5_LabelId };
    Brick_ComponentId ComponentLabelGroup_LabelGroupId = Brick_CreateGroup(labelGroup, 3);

    // Button Group
    Brick_ComponentId ComponentButtonGroup_LabelId = Brick_CreateLabelEx("Button Group", 0, COMP_TITLE_FONT_SIZE);
    Brick_ComponentId ComponentButtonGroup_TabId = Brick_CreateButton("Button Group");
    
    Brick_ComponentId ComponentButton3_ButtonId = Brick_CreateButton("Button 1");
    Brick_ComponentId ComponentButton4_ButtonId = Brick_CreateButton("Button 2");
    Brick_ComponentId ComponentButton5_ButtonId = Brick_CreateButton("Button 3");

    Brick_ComponentId buttonGroup[3] = { ComponentButton3_ButtonId, ComponentButton4_ButtonId, ComponentButton5_ButtonId };
    Brick_ComponentId ComponentButtonGroup_ButtonGroupId = Brick_CreateGroup(buttonGroup, 3);

    // Scroll Box
    Brick_ComponentId ContainerScrollBox_LabelId = Brick_CreateLabelEx("ScrollBox", 0, COMP_TITLE_FONT_SIZE);
    Brick_ComponentId ContainerScrollBox_TabId = Brick_CreateButton("ScrollBox");

    Brick_ContainerId ContainerScrollBox_ScrollBoxId = Brick_CreateScrollBox();

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

    Brick_ContainerSettings Comp_ContainerSettings = PLEX(Brick_ContainerSettings){ 0, BRICK_ALIGN_MIDDLE, CLAY_TOP_TO_BOTTOM };

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
                Brick_BeginVerticalBoxEx(BRICK_ALIGN_MIDDLE);
                    Brick_LayoutToggleGroup(Entities_TabGroupId);
                Brick_EndVerticalBox();
                
                // Main content
                Brick_BeginVerticalLayoutEx(BRICK_ALIGN_CENTER);
                    Brick_LayoutLabel(BrickTitle_LabelId);

                    Brick_BeginPanelEx(PLEX(Brick_ContainerSettings){ 0, BRICK_ALIGN_LEFT, CLAY_TOP_TO_BOTTOM });

                        if (Brick_IsButtonToggled(ElementText_TabId)) {
                            Brick_LayoutLabel(ElementText_LabelId);
                            Brick_InlineText(ELEMENT_TEXT_SUMMARY);

                            Brick_BeginPanelEx(Comp_ContainerSettings);
                                Brick_LayoutText(ElementText_TextId);
                            Brick_EndPanel();
                        } else if (Brick_IsButtonToggled(ElementImage_TabId)) {
                            Brick_LayoutLabel(ElementImage_LabelId);
                            Brick_InlineText(ELEMENT_IMAGE_SUMMARY);
                            
                            Brick_BeginPanelEx(Comp_ContainerSettings);
                                Brick_LayoutImage(ElementImage_ImageId);
                            Brick_EndPanel();
                        } else if (Brick_IsButtonToggled(ComponentLabel_TabId)) {
                            Brick_LayoutLabel(ComponentLabel_LabelId);
                            Brick_InlineText(COMPONENT_LABEL_SUMMARY);

                            Brick_BeginPanelEx(Comp_ContainerSettings);
                                Brick_LayoutLabel(ComponentLabel2_LabelId);
                            Brick_EndPanel();
                        } else if (Brick_IsButtonToggled(ComponentButton_TabId)) {
                            Brick_LayoutLabel(ComponentButton_LabelId);
                            Brick_InlineText(COMPONENT_BUTTON_SUMMARY);
                            
                            Brick_BeginPanelEx(Comp_ContainerSettings);
                                Brick_LayoutButton(ComponentButton_ButtonId);
                            Brick_EndPanel();
                        } else if (Brick_IsButtonToggled(ComponentLabelButton_TabId)) {
                            Brick_LayoutLabel(ComponentLabelButton_LabelId);
                            Brick_InlineText(COMPONENT_LABELBUTTON_SUMMARY);
                            
                            Brick_BeginPanelEx(Comp_ContainerSettings);
                                Brick_LayoutLabelButton(ComponentLabelButton_LabelButtonId);
                            Brick_EndPanel();
                        } else if (Brick_IsButtonToggled(ComponentImageButton_TabId)) {
                            Brick_LayoutLabel(ComponentImageButton_LabelId);
                            Brick_InlineText(COMPONENT_IMAGEBUTTON_SUMMARY);
                            
                            Brick_BeginPanelEx(Comp_ContainerSettings);
                                Brick_LayoutImageButton(ComponentImageButton_ImageButtonId);
                            Brick_EndPanel();
                        } else if (Brick_IsButtonToggled(ComponentLabelGroup_TabId)) {
                            Brick_LayoutLabel(ComponentLabelGroup_LabelId);
                            Brick_InlineText(COMPONENT_LABELGROUP_SUMMARY);

                            Brick_BeginPanelEx(Comp_ContainerSettings);
                                Brick_LayoutGroup(ComponentLabelGroup_LabelGroupId);
                            Brick_EndPanel();
                        } else if (Brick_IsButtonToggled(ComponentButtonGroup_TabId)) {
                            Brick_LayoutLabel(ComponentButtonGroup_LabelId);
                            Brick_InlineText(COMPONENT_BUTTONGROUP_SUMMARY);

                            Brick_BeginPanelEx(Comp_ContainerSettings);
                                Brick_LayoutGroup(ComponentButtonGroup_ButtonGroupId);
                            Brick_EndPanel();
                        } else if (Brick_IsButtonToggled(ContainerScrollBox_TabId)) {
                            Brick_LayoutLabel(ContainerScrollBox_LabelId);
                            Brick_InlineText(CONTAINER_SCROLLBOX_SUMMARY);

                            Brick_BeginScrollBox(ContainerScrollBox_ScrollBoxId);
                                Brick_InlineText(CONTAINER_SCROLLBOX_TEXT);
                                Brick_LayoutImage(ElementImage_ImageId);
                                Brick_InlineText(CONTAINER_SCROLLBOX_TEXT);
                            Brick_EndScrollBox();                            
                        }
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
