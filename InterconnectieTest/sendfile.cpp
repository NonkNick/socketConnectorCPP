#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <cstring>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
#include <chrono>

#define PREFIX_LENGTH 8
#define FILE_BUFFER_SIZE 2048

typedef struct _command {
    const char* filename;
    const char* randomString;
} command;

typedef struct _greppelState {
    char* dc1;
    char* dc2;
    char* dc3;
    char* dc4;
} greppelState;

/***
 * Create a string indicating the hexadecimal representation of the given parameter LEN
 * @param buffer a pointer to a buffer where the result is stored. make sure to declare enough space to hold the result
 * It is advised to the the constant PREFIX_LENGTH+1
 * @param len the integer number to be converted.
 */
void createLengthIndicator(char *buffer, unsigned long int len){
    char format[5];
    // setup the format using the constant
    sprintf(format, "%%0%dX", PREFIX_LENGTH);

    // print in HEX to the given buffer
    sprintf(buffer, format , len);
}//createLengthIndicator

/***
 * gets the current timestamp in milliseconds (UNIX style, since 1/1/1970 00:00:00)
 * @return the number of milleseconds
 */
uint64_t getTimestamp() {
    return std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::system_clock::now().time_since_epoch()
    ).count();
}//getTimestamp

/**
 * writes a string to the pipe. the string is prefixed with a hexadecimal indicator, indicating the length.
 * @param fd File Descriptor of the pipe (see also mkfifo)
 * @param text the text to be written to the pipe
 * @return
 */
unsigned long int writeStringToPipe (int fd, const char* text){
    char hexlen[PREFIX_LENGTH + 1];
    unsigned long int len = strlen(text);

    // first create the prefix in HEX-format: how many bytes will follow?
    createLengthIndicator(hexlen, len);
    write (fd, hexlen,PREFIX_LENGTH  );
    write (fd, text, len);
    return len;
}//writeStringToPipe

unsigned long int writeIntToPipe (int fd, const int* number){
    char hexlen[PREFIX_LENGTH + 1];
    unsigned long int len = sizeof(number);

    // first create the prefix in HEX-format: how many bytes will follow?
    createLengthIndicator(hexlen, len);
    write (fd, hexlen,PREFIX_LENGTH  );
    write (fd, number, len);
    return len;
}//writeStringToPipe


/**
 * Reads a length indicator (string in hexadecimal format)
 * @param fd the file descriptor of the pipe (see also mkfifo)
 * @return the number converted to an unsigned long int
 */
unsigned long int readLengthIndicatorFromPipe(int fd){
    char hexlen[PREFIX_LENGTH + 1];

    read(fd, hexlen, 8);
    hexlen[8]='\0';

    return std::stoi(hexlen, 0,  16);

}//readLengthIndicatorFromPipe

/**
 * reads a string from the given pipe, storing the string in the given buffer
 * @param buffer the buffer to hold the string read from pipe; make sure the buffer is large enough!
 * @param fd the file descriptor of the pipe (see also mkfifo)
 * @return
 */
int readStringFromPipe(char *buffer, int fd) {
    int nrOfBytesToRead = readLengthIndicatorFromPipe(fd);

    read(fd, buffer, nrOfBytesToRead);

    buffer[nrOfBytesToRead] = '\0';

    return nrOfBytesToRead;
}//readStringFromPipe

unsigned long int writeFileToPipe(int fd, const char *path) {
    char hexlen[PREFIX_LENGTH+1];
    char buffer[FILE_BUFFER_SIZE];

    // create the streamreader, indicate binary (byte-for-byte reading)
    std::ifstream infile(path, std::ifstream::binary);

    //get length of file (seek to end, then back to begin
    infile.seekg(0, std::ios::end);
    size_t length = infile.tellg();
    infile.seekg(0, std::ios::beg);

    // write the length of the file in HEX to the pipe
    createLengthIndicator(hexlen, ( unsigned int) length);
    write (fd, hexlen, PREFIX_LENGTH  );

    // read the file
    while (!infile.eof()){
        // read from the file, maximum size of the buffer
        infile.read(buffer, sizeof(buffer));

        // determine how many bytes were read
        int bytesRead = infile.gcount();

        // write the file to the pipe.
        write(fd, buffer, bytesRead);
    }

    return (unsigned long int ) length;
}//writeFileToPipe

/**
 * Repeatedly sends a file through a PIPE (see mkfifo), followed by an instruction and waits for an answer
 * measured transfer times: 1.9GB in 2945mshtop (1853882368 / 2945 = 629501 bytes/ms (@buffer size = 2048 bytes)
 * @return
 */
int main() {
    char *myfifo_write = "/tmp/greppel_out";
    char *myfifo_read  = "/tmp/greppel_in";

    mkfifo(myfifo_read, 0666);
    mkfifo(myfifo_write, 0666);

    int fd_write = open(myfifo_write, O_WRONLY);
    int fd_read  = open(myfifo_read, O_RDONLY);

    command MyCommand;
    MyCommand.filename = "tester";  // FIXME: Change filename according to current situation
    MyCommand.randomString = "fhsfvbafbsdifeiufhbsdhbcisufbeiuwfiuhdsaiudhaisu";
    greppelState MyGreppelState;

    long int i=0;
    for (;;) {
        char hexlen[8 + 1];
        size_t chars_read;

        uint64_t start = getTimestamp();

        std::string str1 = std::to_string(rand() % 101);
        char *dc1 = const_cast<char *>(str1.c_str());
        std::string str2 = std::to_string(rand() % 101);
        char *dc2 = const_cast<char *>(str2.c_str());
        std::string str3 = std::to_string(rand() % 101);
        char *dc3 = const_cast<char *>(str3.c_str());
        std::string str4 = std::to_string(rand() % 101);
        char *dc4 = const_cast<char *>(str4.c_str());

        MyGreppelState.dc1 = dc1;
        MyGreppelState.dc2 = dc2;
        MyGreppelState.dc3 = dc3;
        MyGreppelState.dc4 = dc4;
//        std::cout << "Sending finished in " << MyGreppelState.dc1 << "ms" << std::endl;
//        std::cout << "Sending finished in " << MyGreppelState.dc2 << "ms" << std::endl;
//        std::cout << "Sending finished in " << MyGreppelState.dc3 << "ms" << std::endl;
//        std::cout << "Sending finished in " << MyGreppelState.dc4 << "ms" << std::endl;

        writeStringToPipe(fd_write, MyGreppelState.dc1);
        writeStringToPipe(fd_write, MyGreppelState.dc2);
        writeStringToPipe(fd_write, MyGreppelState.dc3);
        writeStringToPipe(fd_write, MyGreppelState.dc4);

//        writeStringToPipe(fd_write, MyCommand.filename);
//        writeStringToPipe(fd_write, MyCommand.randomString);
//        std::string str = std::to_string(i);
//        char const *pchar = str.c_str();
//        writeStringToPipe(fd_write, pchar);


        i++;
        uint64_t ended = getTimestamp();
        std::cout << "Sending finished in " << (ended - start) << "ms" << std::endl;

//        char answer[512];
//        readStringFromPipe(answer, fd_read);
//
//        std::cout << answer << std::endl;
    }

}