#include "layout.h"

#include <math.h>
#include <stdlib.h>

// main instance

ResponsiveLayout createLayout(float weight, void * data, RESPONSIVE_CALLBACK(* callback)) {
    return (ResponsiveLayout){
        .weight = weight,
        .data = data,
        .callback = callback
    };
}

void runLayout(ResponsiveLayout layout, Rect rect) {
    layout.callback(rect, layout.data);
}

// horizontal "sub-class"

ResponsiveHorizontalLayout * createHorizontalLayout(float weight) {
    ResponsiveHorizontalLayout * out = calloc(1, sizeof(ResponsiveHorizontalLayout));
    out->weight = weight;
    return out;
}

void addHorizontalChild(ResponsiveHorizontalLayout * parent, ResponsiveLayout child) {
    parent->children = realloc(parent->children, sizeof(ResponsiveLayout) * ++parent->child_count);
    parent->children[parent->child_count - 1] = child;
    parent->total_child_weight += child.weight;
}

RESPONSIVE_CALLBACK(horizontalLayoutCallback) {
    ResponsiveHorizontalLayout * layout = data;
    int used = 0;
    for (int i = 0; i < layout->child_count - 1;) {
        ResponsiveLayout child = layout->children[i++];

        Rect new_rect = rect;
        new_rect.x = new_rect.x + used;
        new_rect.width = (int)round((double)new_rect.width * child.weight / layout->total_child_weight);

        runLayout(child, new_rect);

        used += new_rect.width;
    }

    // last one fill remaining without any precision errors
    rect.x += used;
    rect.width -= used;
    runLayout(layout->children[layout->child_count - 1], rect);
}

ResponsiveLayout compileHorizontalLayout(ResponsiveHorizontalLayout * parent) {
    return createLayout(parent->weight, parent, horizontalLayoutCallback);
}

// vertical "sub-class"

ResponsiveVerticalLayout * createVerticalLayout(float weight) {
    ResponsiveVerticalLayout * out = calloc(1, sizeof(ResponsiveVerticalLayout));
    out->weight = weight;
    return out;
}

void addVerticalChild(ResponsiveVerticalLayout * parent, ResponsiveLayout child) {
    parent->children = realloc(parent->children, sizeof(ResponsiveLayout) * ++parent->child_count);
    parent->children[parent->child_count - 1] = child;
    parent->total_child_weight += child.weight;
}

RESPONSIVE_CALLBACK(verticalLayoutCallback) {
    ResponsiveVerticalLayout * layout = data;
    int used = 0;
    for (int i = 0; i < layout->child_count - 1;) {
        ResponsiveLayout child = layout->children[i++];

        Rect new_rect = rect;
        new_rect.y = new_rect.y + used;
        new_rect.height = (int)round((double)new_rect.height * child.weight / layout->total_child_weight);

        runLayout(child, new_rect);

        used += new_rect.height;
    }

    // last one fill remaining without any precision errors
    rect.y += used;
    rect.height -= used;
    runLayout(layout->children[layout->child_count - 1], rect);
}

ResponsiveLayout compileVerticalLayout(ResponsiveVerticalLayout * parent) {
    return createLayout(parent->weight, parent, verticalLayoutCallback);
}
