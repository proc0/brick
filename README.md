# Brick
### Render-agnostic, header-only UI Component Library for Clay

Brick is render agnostic UI component library extending the UI layout library [Clay](https://github.com/nicbarker/clay). Brick provides components for quick and flexible use while retaining full performance and customization, and adds a very thin layer of state on top of Clay to keep track of components and their state, including an event system. 

## Features

- Library of UI elements, components, and containers
- Interchangeability with [Clay](https://github.com/nicbarker/clay)
- Basic UI state management
- UI events with flexible handling
- Seperation of content, layout, and styles
- Default and customizable styles
- Small STB header-only library for C99, C++20 and above
- Render agnostic

Future potential features:
- Reactive layouts adapt to window resizing
- Default renderers for multiple graphics libs
- Unified style effects with shaders, i.e. drop shadows and gradients

For a detailed explanation see [Overview](#).

## Contents

#### [Quick Start](#)

#### [Objectives](#)

#### [Setup](#)

#### [Overview](#)

#### [Examples](#)

#### [Components](#components-1)


## Quick Start



## Objectives

Clay is a versatile UI layout library that provides an inline-style interface for creating any type of UI element. The inline configuration of every element will add up quickly with relatively simple UIs taking thousands of lines of code. This also means the content (button labels, text, etc.) is in the same area as the styles, which is in the same place as the layout structure. Brick aims to add UI components as primitive building blocks that can be combined in many ways to build any UI, as well as separating content, styles, and layout, while retaining interoperability with Clay for maximum flexibility.

Brick objectives include:
- adding UI state on top of Clay
- adding component events and a flexible way of handling them
- separating content, layout, and styles
- retaining ability to interchangeably use Clay alongside Brick
- responsive styles that adjust to window resizing
- global style configuration that can be overridden at different levels
- quick setup and quick start with reasonable defaults
- remaining render agnostic while providing default renderers and remaining compatible with Clay default renderers

## Setup

## Overview
<img src="docs/Brick_StackDiagram.drawio.png" />

## Examples

## Components

### Text

A simple text element. It can be created and used in the layout with an ID, or just using the inline function:

```C
Brick_ElementId Brick_CreateText(const char* string);
Brick_ElementId Brick_CreateTextEx(const char* string, uint16_t fontId, uint16_t fontSize);

void Brick_LayoutText(Brick_ElementId textId);

void Brick_InlineText(const char* string);
void Brick_InlineTextEx(const char* string, uint16_t fontId, uint16_t fontSize);
```

### Image

An image element depends on the whatever data the renderer expects to display pixels on the screen. It could be an image format or a texture or something else. This data is passed through in the Clay_RenderCommand.

```C
Brick_ElementId Brick_CreateImage(float width, float height, void* imageData);

void Brick_LayoutImage(Brick_ElementId imageId);
```

### Label

A text element in a box. Labels are the simplest components that enclose text elements in a box element that contains padding, alignment and other properties.

```C
Brick_ComponentId Brick_CreateLabel(const char* string);
Brick_ComponentId Brick_CreateLabelEx(const char* string, uint16_t fontId, uint16_t fontSize);

void Brick_LayoutLabel(Brick_ComponentId labelId);
```

### Button

The humble button. It is essentially a label with a background plus interactivity. It tracks its different states like hover, pressed, released that an be queried for UI functionality.

```C
Brick_ComponentId Brick_CreateButton(const char* text);
Brick_ComponentId Brick_CreateButtonEx(const char* text, uint16_t fontId, uint16_t fontSize);

void Brick_LayoutButton(Brick_ComponentId buttonId);
```

### Label Button

A label with button interactivity. Labels do not have a background like buttons do. Label buttons act like buttons while remaining texts in a box.

```C
Brick_ComponentId Brick_CreateLabelButton(const char* text);

void Brick_LayoutLabelButton(Brick_ComponentId buttonId);
```

### Image Button

An image with button interactivity. Like label-buttons, image-buttons are images that are also interactive buttons.

```C
Brick_ComponentId Brick_CreateImageButton(float width, float height, void* imageData);

void Brick_LayoutImageButton(Brick_ComponentId buttonId);
```

### Label Group

A group of labels. Create a number of labels, then use an array of label IDs to group them.

```C
Brick_ComponentId Brick_CreateLabel(const char* string);
Brick_ComponentId labelIds[3] = { labelId1, labelId2, labelId3 };
Brick_ComponentId Brick_CreateGroup(const Brick_ComponentId* labelIds, int32_t groupSize);

void Brick_LayoutGroup(Brick_ComponentId labelGroupId);
```

### Button Group

A group of buttons. Create a number of buttons, then use an array of button IDs to group them.

```C
Brick_ComponentId Brick_CreateButton(const char* text);
Brick_ComponentId buttonIds[3] = { buttonId1, buttonId2, buttonId3 };
Brick_ComponentId Brick_CreateGroup(const Brick_ComponentId* buttonIds, int32_t groupSize);

void Brick_LayoutGroup(Brick_ComponentId buttonGroupId);
```

### Scroll Box

A container that scrolls vertically. Create the container first, then use Begin and End to enclose the content that will be scrolled:

```C
Brick_ContainerId Brick_CreateScrollBox(void);

void Brick_BeginScrollBox(void);
// scroll box contents
void Brick_EndScrollBox(void);
```

