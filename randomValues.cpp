#include "randomValues.h"

randomValues::randomValues() {
    telemetrics* t = telemetrics::getInstance();
    dc1 = rand() % 101;



    (*t).telemetry.dc = dc1;

}

void randomValues::generate() {
    telemetrics* t = telemetrics::getInstance();
    dc1 = rand() % 101;



    (*t).telemetry.dc = dc1;

}
