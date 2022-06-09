#pragma once
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <cstring>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
#include <chrono>
#include "socketConnector.h"

class randomValues {
    socketConnector sc;
    int dc1;
    int dc2;
    int dc3;
    int dc4;

public:
    randomValues(socketConnector socketConnector);
    void generate();
};

