#include <string.h>
#include "board.h"

Board * createBoard(char * name, uint8_t size_x, uint8_t size_y, Team * teams, uint8_t team_count) {
    Board * out = malloc(sizeof(Board));
    strcpy(out->name, name);
    out->size_x = size_x;
    out->size_y = size_y;
    out->teams = teams;
    out->team_count = team_count;
    return out;
}

void saveBoard(Board * board, FILE * stream) {
    fwrite(board, sizeof(Board) - sizeof(Team *), 1, stream);
    for (int i = 0; i < board->team_count; i++)
        saveTeam(board->teams + i, stream);
}

Board * loadBoard(FILE * stream) {
    Board * out = malloc(sizeof(Board));
    fread(out, sizeof(Board) - sizeof(Team *), 1, stream);
    out->teams = malloc(sizeof(Team) * out->team_count);
    for (int i = 0; i < out->team_count; i++)
        memcpy(out->teams + i, loadTeam(stream), sizeof(Team));
    return out;
}

void printBoard(Board * board) {
    printf("Board: \n"
           "\tName: %s\n"
           "\tSize X: %d\n"
           "\tSize Y: %d\n"
           "\tTeams:\n",
           board->name, board->size_x, board->size_y);
    for (int i = 0; i < board->team_count; i++)
        printTeam(board->teams + i);
}
