#pragma once
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <cstring>
#include <sys/stat.h>
#include <fcntl.h>
#include "json_struct.h"
#include <fstream>
#include <chrono>

class telemetrics {
private:
    static telemetrics* instance;
    telemetrics();
public:
    struct _telemetry {
        int dc1;
        int dc2;
        int dc3;
        int dc4;
        JS_OBJ(dc1, dc2, dc3, dc4);
    } telemetry;
    static telemetrics* getInstance();
};


