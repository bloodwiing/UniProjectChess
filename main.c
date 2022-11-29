#include <stdio.h>
#include "con_lib.h"
#include "defaults.h"

int main() {
    Team * team = createDefaultTeamBlack();

    printTeam(team);

    FILE * stream = fopen("set/original.chess", "wb");
    saveTeam(team, stream);
    fclose(stream);

    stream = fopen("set/original.chess", "rb");
    Team * loaded = loadTeam(stream);
    fclose(stream);

    printTeam(loaded);
}
