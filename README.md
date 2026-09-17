# Brick
### Component library for Clay UI
Build high performance, responsive 2D UIs quickly in C/C++

## Features

- Seamless interop and all the benefits of [Clay](https://github.com/nicbarker/clay)
- Ergonomic library of UI component primitives and containers
- Automatic UI state management
- Flexible API for handling UI events
- Seperation of content, layout, and styles
- Reactive layouts adapt to screen size changes
- Sensible defaults while still providing full customization options
- Small STB style C99 header-only library, compatible with C++20
- Render agnostic and fully customizable components and styles

## Summary

Brick is render agnostic UI component library extending the UI layout library [Clay](https://github.com/nicbarker/clay). Brick provides out-of-the-box components (elements and containers) for quick use and flexibility while retaining full performance and customization. Brick adds a very thin layer of state on top of Clay to keep track of components and their state, including an event system. 

## Contents

[Examples](#)
[Quick Start](#)
[Motivation](#)
[Objectives](#)
[References](#)

OBJ:
1. add basic UI state, i.e. button hover or not, button is toggled, panel is visible or hidden
2. separate content from layout, i.e. declar button and labels in one place, use them in the layout later
3. uses 'embeded scope constructs' like Raylib to match this C style of API
4. provide sensible defaults where possible, windows grow, have some padding, etc
5. provide a global config that overrides defaults, change colors, adjust padding
6. add responsive reactive behavior to the window, i.e. resize on event window resizing and adjust layout dynamically
7. provide an easy way to localize, i.e. through Brick_TextEx or TextPro or a Brick_LocalizedText, that would stand in for normal strings and can be globally configured


