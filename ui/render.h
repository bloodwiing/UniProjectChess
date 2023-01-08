#ifndef CHESS_RENDER_H
#define CHESS_RENDER_H

#include <stdarg.h>
#include <wchar.h>

#include "./generic/rect.h"
#include "./generic/shape.h"
#include "./generic/text.h"

#include "engine/board.h"
#include "engine/gamepiece.h"

#include "settings/settings.h"

#include "model/piece.h"
#include "model/scenario.h"
#include "model/team.h"

/// Renders a Chess Piece with the given colours
/// \param settings Display settings
/// \param bg Background colour
/// \param fg Foreground colour
/// \param piece The piece to render to screen
void renderPieceColoured(UserSettings * settings, int bg, int fg, Piece piece);

/// Renders a Chess Piece with Team colours and a background
/// \param settings Display settings
/// \param team The team that the Piece belongs to
/// \param piece The piece to render to screen
/// \param bg Background colour
/// \param invert A flag to invert the background and foreground colours (colouring the background with the team colour)
void renderPieceWithBackground(UserSettings * settings, Team team, Piece piece, int bg, bool_t invert);

/// Renders a Chess Board Game Piece with Team colours and a background
/// \param settings Display settings
/// \param scenario The current Scenario of the Board
/// \param game_piece The Game Piece that exists on the Board
/// \param bg Background colour
/// \param invert A flag to invert the background and foreground colours (colouring the background with the team colour)
void renderGamePieceWithBackground(UserSettings * settings, Scenario * scenario, GamePiece * game_piece, int bg, bool_t invert);

/// Renders a Chess Piece with Team colours
/// \param settings Display settings
/// \param team The team that the Piece belongs to
/// \param piece The piece to render to screen
void renderPiece(UserSettings * settings, Team team, Piece piece);


/// Renders the whole Game Board to the screen
/// \param board The Game board to render
/// \param draw_rect The rect where the board can be drawn to
/// \param board_rect The rect of the board's size and offset
/// \param cur_x Currently hovered tile's X
/// \param cur_y Currently hovered tile's Y
/// \param with_coords A flag to draw the board with Ranks and Files
/// \param margin A flag to clear the whole draw_rect around the board
void renderBoard(Board * board, Rect draw_rect, Rect board_rect, int cur_x, int cur_y, bool_t with_coords, bool_t margin);

/// Renders the whole Game Board to the screen with an active selection, displaying possible moves of the selected piece
/// \param board The Game board to render
/// \param draw_rect The rect where the board can be drawn to
/// \param board_rect The rect of the board's size and offset
/// \param cur_x Currently hovered tile's X
/// \param cur_y Currently hovered tile's Y
/// \param cur_x Currently selected tile's X
/// \param cur_y Currently selected tile's Y
/// \param with_coords A flag to draw the board with Ranks and Files
/// \param margin A flag to clear the whole draw_rect around the board
void renderBoardWithSelection(Board * board, Rect draw_rect, Rect board_rect, int cur_x, int cur_y, int sel_x, int sel_y, bool_t with_coords, bool_t margin);

/// Renders the Scenario Board preview to the screen
/// \param board The Game board to render
/// \param settings Display settings
/// \param draw_rect The rect where the board can be drawn to
/// \param board_rect The rect of the board's size and offset
/// \param with_coords A flag to draw the board with Ranks and Files
/// \param margin A flag to clear the whole draw_rect around the board
void renderScenario(Scenario * scenario, UserSettings * settings, Rect draw_rect, Rect board_rect, bool_t with_coords, bool_t margin);


/// A helper function to automatically move the cursor to the character where a board tile is
/// \param draw_rect The rect where the board can be drawn to
/// \param board_rect The rect of the board's size and offset
/// \param x The tile's X
/// \param y The tile's Y
void setCursorAtTile(Rect draw_rect, Rect board_rect, int x, int y);

/// A helper function to check if a tile is visible on screen or if it's out of the draw boundaries
/// \param board_rect The rect of the board's size and offset
/// \param x The tile's X
/// \param y The tile's Y
/// \return true if the tile is visible, false otherwise
bool_t isTileVisible(Rect board_rect, int x, int y);


/// An effect that clears the screen in a chequerboard pattern
void ditherEffect();


/// A helper function to clear a portion of a screen
void clearRect(Rect rect);

#endif //CHESS_RENDER_H
