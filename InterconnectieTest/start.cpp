#include "start.h"

int main() {
    std::cout << "voor sc en rv" << std::endl;

    socketConnector sc;
    std::cout << "na sc en voor rv" << std::endl;

    randomValues rv(sc);
    std::cout << "na sc en rv" << std::endl;


    while(true) {
        rv.generate();
        sc.printDCS();
    }

    return 0;
}
