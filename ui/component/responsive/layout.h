#ifndef CHESS_RESPONSIVE_LAYOUT_H
#define CHESS_RESPONSIVE_LAYOUT_H

#include <stdint.h>

#include "model/rect.h"

#include "ui/con_lib.h"

#define RESPONSIVE_CALLBACK(name) void (name)(Rect rect, void * data)

// main instance

typedef struct ResponsiveLayout {
    float weight;

    void * data;
    RESPONSIVE_CALLBACK(* callback);
} ResponsiveLayout;

ResponsiveLayout createLayout(float weight, void * data, RESPONSIVE_CALLBACK(* callback));

void runLayout(ResponsiveLayout layout, Rect rect);

// horizontal "sub-class" (more just an imitation of OOP)

typedef struct ResponsiveHorizontalLayout {
    float weight;

    float total_child_weight;

    size_t child_count;
    ResponsiveLayout * children;
    RESPONSIVE_CALLBACK(* callback);
} ResponsiveHorizontalLayout;

ResponsiveHorizontalLayout * createHorizontalLayout(float weight);
void addHorizontalChild(ResponsiveHorizontalLayout * parent, ResponsiveLayout child);
ResponsiveLayout compileHorizontalLayout(ResponsiveHorizontalLayout * parent);

// vertical "sub-class" (more just an imitation of OOP)

typedef struct ResponsiveVerticalLayout {
    float weight;

    float total_child_weight;

    size_t child_count;
    ResponsiveLayout * children;
    RESPONSIVE_CALLBACK(* callback);
} ResponsiveVerticalLayout;

ResponsiveVerticalLayout * createVerticalLayout(float weight);
void addVerticalChild(ResponsiveVerticalLayout * parent, ResponsiveLayout child);
ResponsiveLayout compileVerticalLayout(ResponsiveVerticalLayout * parent);

#endif //CHESS_RESPONSIVE_LAYOUT_H
