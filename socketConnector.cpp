#include "socketConnector.h"

#define PREFIX_LENGTH 8
#define FILE_BUFFER_SIZE 2048

void socketConnector::setup() {

}


int main() {
    socketConnector sc;
    return 0;
}

unsigned long int writeStringToPipe (int fd, const char* text){
    char hexlen[PREFIX_LENGTH + 1];
    unsigned long int len = strlen(text);

    // first create the prefix in HEX-format: how many bytes will follow?
    createLengthIndicator(hexlen, len);
    write (fd, hexlen,PREFIX_LENGTH  );
    write (fd, text, len);
    return len;
}//writeStringToPipe
