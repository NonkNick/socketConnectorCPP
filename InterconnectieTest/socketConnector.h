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

class socketConnector {
public:
    static int dc1;
    static int dc2;
    static int dc3;
    static int dc4;
    socketConnector();
    void printDCS();
    void sendState();
    void openPipe();


private:
//    char *myfifo_write;
//    char *myfifo_read;
//    int fd_write;
//    int fd_read;

    char *myfifo_write;
    char *myfifo_read;

    int fd_write;
    int fd_read;

    void createLengthIndicator(char *buffer, unsigned long int len);
    uint64_t getTimestamp();
    unsigned long int writeStringToPipe (int fd, const char* text);
    unsigned long int readLengthIndicatorFromPipe(int fd);
    int readStringFromPipe(char *buffer, int fd);
    unsigned long int writeFileToPipe(int fd, const char *path);

};

