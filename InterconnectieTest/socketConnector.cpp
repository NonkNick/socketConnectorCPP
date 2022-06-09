#include "socketConnector.h"

#define PREFIX_LENGTH 8
#define FILE_BUFFER_SIZE 2048
int socketConnector::dc1;
int socketConnector::dc2;
int socketConnector::dc3;
int socketConnector::dc4;

socketConnector::socketConnector() {

    std::cout << "in sc" << std::endl;

    char *myfifo_write = "/tmp/greppel_out";
    char *myfifo_read  = "/tmp/greppel_in";

    mkfifo(myfifo_read, 0666);
    mkfifo(myfifo_write, 0666);

    int fd_write = open(myfifo_write, O_WRONLY);
    int fd_read  = open(myfifo_read, O_RDONLY);
    std::cout << "na fifo en fd" << std::endl;


//    for (;;) {
        char hexlen[8 + 1];
        size_t chars_read;

        uint64_t start = getTimestamp();

        std::cout << " | dc1 " << dc1 << " | ";
        std::cout << "dc2 " << dc2 << " | ";
        std::cout << "dc3 " << dc3 << " | ";
        std::cout << "dc4 " << dc4 << " | " << std::endl;

//        std::string s = std::to_string(dc1);
//        char const *pchar1 = s.c_str();  //use char const* as target type
//        s = std::to_string(dc2);
//        char const *pchar2 = s.c_str();  //use char const* as target type
//        s = std::to_string(dc3);
//        char const *pchar3 = s.c_str();  //use char const* as target type
//        s = std::to_string(dc4);
//        char const *pchar4 = s.c_str();  //use char const* as target type
    writeIntToPipe(fd_write, dc1);
    writeIntToPipe(fd_write, dc2);
    writeIntToPipe(fd_write, dc3);
    writeIntToPipe(fd_write, dc4);


//        writeStringToPipe(fd_write, pchar1);
//        writeStringToPipe(fd_write, pchar2);
//        writeStringToPipe(fd_write, pchar3);
//        writeStringToPipe(fd_write, pchar4);

//        writeStringToPipe(fd_write, MyCommand.filename);
//        writeStringToPipe(fd_write, MyCommand.randomString);
//        std::string str = std::to_string(i);
//        char const *pchar = str.c_str();
//        writeStringToPipe(fd_write, pchar);



        uint64_t ended = getTimestamp();
//        std::cout << "Sending finished in " << (ended - start) << "ms" << std::endl;

//        char answer[512];
//        readStringFromPipe(answer, fd_read);
//
//        std::cout << answer << std::endl;
//    }
}

void socketConnector::printDCS() {
    std::cout << " | dc1 " << dc1 << " | ";
    std::cout << "dc2 " << dc2 << " | ";
    std::cout << "dc3 " << dc3 << " | ";
    std::cout << "dc4 " << dc4 << " | " << std::endl;
}

void socketConnector::setDc1(int value) {
    dc1 = value;
    std::cout << dc1 << std::endl;

}

void socketConnector::setDc2(int value) {
    dc2 = value;
}

void socketConnector::setDc3(int value) {
    dc3 = value;
}

void socketConnector::setDc4(int value) {
    dc4 = value;
}



/***
 * Create a string indicating the hexadecimal representation of the given parameter LEN
 * @param buffer a pointer to a buffer where the result is stored. make sure to declare enough space to hold the result
 * It is advised to the the constant PREFIX_LENGTH+1
 * @param len the integer number to be converted.
 */
void socketConnector::createLengthIndicator(char *buffer, unsigned long int len){
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
uint64_t socketConnector::getTimestamp() {
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
unsigned long int socketConnector::writeStringToPipe (int fd, const char* text){
    char hexlen[PREFIX_LENGTH + 1];
    unsigned long int len = strlen(text);

    // first create the prefix in HEX-format: how many bytes will follow?
    createLengthIndicator(hexlen, len);
    write (fd, hexlen,PREFIX_LENGTH  );
    write (fd, text, len);
    return len;
}//writeStringToPipe

unsigned long int socketConnector::writeIntToPipe (int fd, const int number){
    std::string tmp = std::to_string(number);
    char const *num_char = tmp.c_str();

    char hexlen[PREFIX_LENGTH + 1];
    unsigned long int len = strlen(num_char);

    // first create the prefix in HEX-format: how many bytes will follow?
    createLengthIndicator(hexlen, len);
    write (fd, hexlen,PREFIX_LENGTH  );
    write (fd, num_char, len);
    return len;
}//writeStringToPipe


/**
 * Reads a length indicator (string in hexadecimal format)
 * @param fd the file descriptor of the pipe (see also mkfifo)
 * @return the number converted to an unsigned long int
 */
unsigned long int socketConnector::readLengthIndicatorFromPipe(int fd){
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
int socketConnector::readStringFromPipe(char *buffer, int fd) {
    int nrOfBytesToRead = readLengthIndicatorFromPipe(fd);

    read(fd, buffer, nrOfBytesToRead);

    buffer[nrOfBytesToRead] = '\0';

    return nrOfBytesToRead;
}//readStringFromPipe

unsigned long int socketConnector::writeFileToPipe(int fd, const char *path) {
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
