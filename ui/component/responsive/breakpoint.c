#include "breakpoint.h"

ResponsiveBreakpoint createResponsiveBreakpoint(size_t height, size_t width, ResponsiveLayout layout) {
    return (ResponsiveBreakpoint){
        .height = height,
        .width = width,
        .layout = layout
    };
}

bool_t checkResponsiveBreakpoint(ResponsiveBreakpoint breakpoint, ConSize screen) {
    return screen.height <= breakpoint.height && screen.width <= breakpoint.width;
}
