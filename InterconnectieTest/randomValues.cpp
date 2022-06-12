#include "randomValues.h"

randomValues::randomValues(socketConnector socketConnector) {
    sc = socketConnector;
    dc1 = rand() % 101;
    dc2 = rand() % 101;
    dc3 = rand() % 101;
    dc4 = rand() % 101;
    sc.dc1 = dc1;
    sc.dc2 = dc2;
    sc.dc3 = dc3;
    sc.dc4 = dc4;
}

void randomValues::generate() {
    dc1 = rand() % 101;
    dc2 = rand() % 101;
    dc3 = rand() % 101;
    dc4 = rand() % 101;
//    std::cout << " | dc1 " << dc1 << "ye | ";
//    std::cout << "dc2 " << dc2 << "ye | ";
//    std::cout << "dc3 " << dc3 << "ye | ";
//    std::cout << "dc4 " << dc4 << "ye | " << std::endl;
    sc.dc1 = dc1;
    sc.dc2 = dc2;
    sc.dc3 = dc3;
    sc.dc4 = dc4;

}
