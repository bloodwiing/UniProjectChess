#include "gamemenu.h"

#include "ui/component/responsive/responsive.h"
#include "ui/render.h"


// RESPONSIVE PROCESSING
typedef struct BoardResponsiveProcessingResult {
    UserSettings * settings;
    GameState * state;
    Board * board;
    ResponsiveManager * responsive;
} BoardResponsiveProcessingResult;


// RESPONSIVE LAYOUTS
RESPONSIVE_CALLBACK(bp_boardCallback_Board) {
    BoardResponsiveProcessingResult * result = data;

    UserSettings * settings = result->settings;
    Board * board = result->board;
    GameState * state = result->state;

    Rect draw_rect = offsetRect(rect, 3, 2, -4, -3);
    Rect board_rect = getBoardCenteredRect(board, draw_rect, state->cur_x, state->cur_y);

    if (!state->piece_selected)
        renderBoard(board, draw_rect, board_rect, state->cur_x, state->cur_y, true, false);
    else
        renderBoardWithSelection(board, draw_rect, board_rect, state->cur_x, state->cur_y, state->sel_x, state->sel_y, true, false);

    drawSingleBoxContained(settings, rect, COLOR_RESET, COLOR_DARK_GRAY);

    setCursorAtTile(draw_rect, board_rect, state->cur_x, state->cur_y);
    GamePiece * game_piece = getGamePieceAtCursor(state);

    if (game_piece != NULL) {
        Piece piece = *getOriginalPiece(game_piece, board->scenario);
        Team team = board->teams[game_piece->team];
        renderPieceWithBackground(settings, team, piece, COLOR_BLACK, true);
    } else {
        renderTextColoured(settings, COLOR_DARK_GRAY, COLOR_LIGHT_GRAY, L"X");
    }
}

RESPONSIVE_CALLBACK(bp_boardCallback_StatusBig) {
    BoardResponsiveProcessingResult * result = data;

    UserSettings * settings = result->settings;
    Board * board = result->board;
    GameState * state = result->state;

    Rect inner = centerRectInRect(rect.width - 4, 3, rect);

    renderTextColouredWrappedRect(settings, COLOR_RESET, COLOR_LIGHT_GRAY, RECT_LINE(inner.x, inner.y, inner.width), L"Turn: ");
    renderTextColouredWrappedRect(settings, COLOR_RESET, getActiveTeam(board)->colour, RECT_LINE(inner.x + 6, inner.y, inner.width - 6), L"X");

    ++inner.y;
    renderTextColouredWrappedRect(settings, COLOR_RESET, COLOR_LIGHT_GRAY, RECT_LINE(inner.x, inner.y, inner.width), L"Current team: ");
    renderTextColouredWrappedRect(settings, COLOR_RESET, getActiveTeam(board)->colour, RECT_LINE(inner.x + 14, inner.y, inner.width - 14), L"%hs", getActiveTeam(board)->name);

    ++inner.y;
    renderTextColouredWrappedRect(settings, COLOR_RESET, COLOR_LIGHT_GRAY, RECT_LINE(inner.x, inner.y, inner.width), L"Selected piece: ");
    if (state->piece_selected)
        renderTextColouredWrappedRect(settings, COLOR_RESET, getActiveTeam(board)->colour, RECT_LINE(inner.x + 16, inner.y, inner.width - 16), L"%hs",
                                      getOriginalPiece(board->tiles[state->sel_x + board->width * state->sel_y]->game_piece, board->scenario)->name);
    else
        renderTextColouredWrappedRect(settings, COLOR_RESET, getActiveTeam(board)->colour, RECT_LINE(inner.x + 16, inner.y, inner.width - 16), L"None");
}

RESPONSIVE_CALLBACK(bp_boardCallback_StatusSmall) {
    BoardResponsiveProcessingResult * result = data;

    UserSettings * settings = result->settings;
    Board * board = result->board;
    GameState * state = result->state;

    Rect inner = centerRectInRect(rect.width - 4, 2, rect);

    renderTextColouredWrappedRect(settings, COLOR_RESET, COLOR_LIGHT_GRAY, RECT_LINE(inner.x, inner.y, inner.width), L"T: ");
    renderTextColouredWrappedRect(settings, COLOR_RESET, getActiveTeam(board)->colour, RECT_LINE(inner.x + 3, inner.y, inner.width - 3), L"%hs", getActiveTeam(board)->name);

    ++inner.y;
    renderTextColouredWrappedRect(settings, COLOR_RESET, COLOR_LIGHT_GRAY, RECT_LINE(inner.x, inner.y, inner.width), L"P: ");
    if (state->piece_selected)
        renderTextColouredWrappedRect(settings, COLOR_RESET, getActiveTeam(board)->colour, RECT_LINE(inner.x + 3, inner.y, inner.width - 3), L"%hs",
                                      getOriginalPiece(board->tiles[state->sel_x + board->width * state->sel_y]->game_piece, board->scenario)->name);
    else
        renderTextColouredWrappedRect(settings, COLOR_RESET, getActiveTeam(board)->colour, RECT_LINE(inner.x + 3, inner.y, inner.width - 3), L"None");
}

RESPONSIVE_CALLBACK(bp_boardCallback_MoveList) {
    BoardResponsiveProcessingResult * result = data;

    UserSettings * settings = result->settings;

    drawDoubleBoxContained(settings, rect, COLOR_RESET, COLOR_DARK_GRAY);
}


// RESPONSIVE BREAKPOINT
// Only the board list
ResponsiveBreakpoint createBoardTinyBreakpoint(BoardResponsiveProcessingResult * result) {
    ResponsiveLayout layout = createLayout(1.0f, result, bp_boardCallback_Board, NULL);
    return createResponsiveBreakpoint(10, 50, layout, ResponsiveBreakpointType_AND);
}

// RESPONSIVE BREAKPOINT
// Board + small state below
ResponsiveBreakpoint createBoardSmallBreakpoint(BoardResponsiveProcessingResult * result) {
    ResponsiveVerticalLayout * ver = createVerticalLayout(1.0f);
    addVerticalChild(ver, createLayout(0.8f, result, bp_boardCallback_Board, NULL));
    addVerticalChild(ver, createLayout(0.2f, result, bp_boardCallback_StatusSmall, NULL));

    return createResponsiveBreakpoint(20, 50, compileVerticalLayout(ver), ResponsiveBreakpointType_AND);
}

// RESPONSIVE BREAKPOINT
// Board + big state below
ResponsiveBreakpoint createBoardVerticalBreakpoint(BoardResponsiveProcessingResult * result) {
    ResponsiveVerticalLayout * ver = createVerticalLayout(1.0f);
    addVerticalChild(ver, createLayout(0.8f, result, bp_boardCallback_Board, NULL));
    addVerticalChild(ver, createLayout(0.2f, result, bp_boardCallback_StatusBig, NULL));

    return createResponsiveBreakpoint(30, 50, compileVerticalLayout(ver), ResponsiveBreakpointType_AND);
}

// RESPONSIVE BREAKPOINT
// Board vertical layout
ResponsiveBreakpoint createBoardVerticalFullBreakpoint(BoardResponsiveProcessingResult * result) {
    ResponsiveVerticalLayout * ver = createVerticalLayout(1.0f);
    addVerticalChild(ver, createLayout(0.5f, result, bp_boardCallback_Board, NULL));
    addVerticalChild(ver, createLayout(0.15f, result, bp_boardCallback_StatusBig, NULL));
    addVerticalChild(ver, createLayout(0.35f, result, bp_boardCallback_MoveList, NULL));

    return createResponsiveBreakpoint(SIZE_MAX, 50, compileVerticalLayout(ver), ResponsiveBreakpointType_AND);
}

// RESPONSIVE BREAKPOINT
// Board + short status (no move list)
ResponsiveBreakpoint createBoardLongBreakpoint(BoardResponsiveProcessingResult * result) {
    ResponsiveHorizontalLayout * hor = createHorizontalLayout(1.0f);
    addHorizontalChild(hor, createLayout(0.65f, result, bp_boardCallback_Board, NULL));
    addHorizontalChild(hor, createLayout(0.35f, result, bp_boardCallback_StatusSmall, NULL));

    return createResponsiveBreakpoint(20, 80, compileHorizontalLayout(hor), ResponsiveBreakpointType_AND);
}

// RESPONSIVE BREAKPOINT
// Board + short status + move list
ResponsiveBreakpoint createBoardLongMoveListBreakpoint(BoardResponsiveProcessingResult * result) {
    ResponsiveHorizontalLayout * hor = createHorizontalLayout(1.0f);
    addHorizontalChild(hor, createLayout(0.65f, result, bp_boardCallback_Board, NULL));

        ResponsiveVerticalLayout * ver = createVerticalLayout(0.35f);
        addVerticalChild(ver, createLayout(0.2f, result, bp_boardCallback_StatusSmall, NULL));
        addVerticalChild(ver, createLayout(0.8f, result, bp_boardCallback_MoveList, NULL));

    addHorizontalChild(hor, compileVerticalLayout(ver));


    return createResponsiveBreakpoint(SIZE_MAX, 80, compileHorizontalLayout(hor), ResponsiveBreakpointType_AND);
}

// RESPONSIVE BREAKPOINT
// Board + status (no move list)
ResponsiveBreakpoint createBoardWideBreakpoint(BoardResponsiveProcessingResult * result) {
    ResponsiveHorizontalLayout * hor = createHorizontalLayout(1.0f);
    addHorizontalChild(hor, createLayout(0.65f, result, bp_boardCallback_Board, NULL));
    addHorizontalChild(hor, createLayout(0.35f, result, bp_boardCallback_StatusBig, NULL));

    return createResponsiveBreakpoint(20, SIZE_MAX, compileHorizontalLayout(hor), ResponsiveBreakpointType_AND);
}

// RESPONSIVE BREAKPOINT
// Board full layout
ResponsiveBreakpoint createBoardFullBreakpoint(BoardResponsiveProcessingResult * result) {
    ResponsiveHorizontalLayout * hor = createHorizontalLayout(1.0f);
    addHorizontalChild(hor, createLayout(0.65f, result, bp_boardCallback_Board, NULL));

        ResponsiveVerticalLayout * ver = createVerticalLayout(0.35f);
        addVerticalChild(ver, createLayout(0.2f, result, bp_boardCallback_StatusBig, NULL));
        addVerticalChild(ver, createLayout(0.8f, result, bp_boardCallback_MoveList, NULL));

    addHorizontalChild(hor, compileVerticalLayout(ver));

    return createResponsiveBreakpoint(SIZE_MAX, SIZE_MAX, compileHorizontalLayout(hor), ResponsiveBreakpointType_AND);
}


void resumeGameLoop(UserSettings * settings, GameState * state, bool_t save_state) {
    con_clear();

    ResponsiveManager * responsive = createResponsiveManager();

    BoardResponsiveProcessingResult * result = calloc(1, sizeof(BoardResponsiveProcessingResult));
    result->responsive = responsive;
    result->settings = settings;
    result->state = state;
    result->board = state->board;

    addResponsiveBreakpoint(responsive, createBoardTinyBreakpoint(result));
    addResponsiveBreakpoint(responsive, createBoardSmallBreakpoint(result));
    addResponsiveBreakpoint(responsive, createBoardVerticalBreakpoint(result));
    addResponsiveBreakpoint(responsive, createBoardVerticalFullBreakpoint(result));
    addResponsiveBreakpoint(responsive, createBoardLongMoveListBreakpoint(result));
    addResponsiveBreakpoint(responsive, createBoardLongBreakpoint(result));
    addResponsiveBreakpoint(responsive, createBoardWideBreakpoint(result));
    addResponsiveBreakpoint(responsive, createBoardFullBreakpoint(result));

    renderResponsive(responsive);

    bool_t game_active = true;

    while (game_active) {
        if (hasConsoleSizeChanged(settings)) {
            con_clear();
            renderResponsive(responsive);
        }

        if (evaluateGameInput(state, &game_active))
            renderResponsive(responsive);
    }

    if (save_state)
        saveGameStateDefault(state);
}

void beginNewGameLoop(UserSettings * settings, Board * board, bool_t save_state) {
    GameState * state = createGameState(board);
    resumeGameLoop(settings, state, save_state);
    free(state);
}
