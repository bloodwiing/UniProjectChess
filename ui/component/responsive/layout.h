#ifndef CHESS_RESPONSIVE_LAYOUT_H
#define CHESS_RESPONSIVE_LAYOUT_H

#include <stddef.h>
#include <stdint.h>

#include "ui/con_lib.h"
#include "ui/generic/rect.h"

#define RESPONSIVE_FREE_CALLBACK(name) void (name)(void * data)
#define RESPONSIVE_CALLBACK(name) void (name)(UserSettings * settings, Rect rect, void * data)

// main instance

typedef struct ResponsiveLayout {
    float weight;

    void * data;
    RESPONSIVE_FREE_CALLBACK(* free_callback);
    RESPONSIVE_CALLBACK(* callback);
} ResponsiveLayout;


/// Creates a new layout item
ResponsiveLayout createLayout(float weight, void * data, RESPONSIVE_CALLBACK(* callback), RESPONSIVE_FREE_CALLBACK(* free_callback));


/// Runs the layout item with the given data
void runLayoutWithData(UserSettings * settings, ResponsiveLayout layout, Rect rect, void * data);

/// Runs the layout item with its own data
void runLayout(UserSettings * settings, ResponsiveLayout layout, Rect rect);

/// Clears up used memory
void freeLayout(ResponsiveLayout layout);

// horizontal "sub-class" (more just an imitation of OOP)

typedef struct ResponsiveHorizontalLayout {
    float weight;

    float total_child_weight;

    size_t child_count;
    ResponsiveLayout * children;
    RESPONSIVE_CALLBACK(* callback);
} ResponsiveHorizontalLayout;

/// Creates a new Horizontal layout
ResponsiveHorizontalLayout * createHorizontalLayout(float weight);

/// Adds a layout item to the horizontal layout
void addHorizontalChild(ResponsiveHorizontalLayout * parent, ResponsiveLayout child);

/// Completes the horizontal layout and returns a layout item for use in a breakpoint or other layouts
ResponsiveLayout compileHorizontalLayout(ResponsiveHorizontalLayout * parent);

// vertical "sub-class" (more just an imitation of OOP)

typedef struct ResponsiveVerticalLayout {
    float weight;

    float total_child_weight;

    size_t child_count;
    ResponsiveLayout * children;
    RESPONSIVE_CALLBACK(* callback);
} ResponsiveVerticalLayout;

/// Creates a new Vertical layout
ResponsiveVerticalLayout * createVerticalLayout(float weight);

/// Adds a layout item to the vertical layout
void addVerticalChild(ResponsiveVerticalLayout * parent, ResponsiveLayout child);

/// Completes the vertical layout and returns a layout item for use in a breakpoint or other layouts
ResponsiveLayout compileVerticalLayout(ResponsiveVerticalLayout * parent);

#endif //CHESS_RESPONSIVE_LAYOUT_H
