#include <stdio.h>
#include "con_lib.h"
#include "defaults.h"

int main() {
    Scenario * scenario = createDefaultScenario();

    printScenario(scenario);

    FILE * stream = fopen("scenario/original.chess", "wb");
    saveScenario(scenario, stream);
    fclose(stream);

    stream = fopen("scenario/original.chess", "rb");
    Scenario * loaded = loadScenario(stream);
    fclose(stream);

    printScenario(loaded);
}
