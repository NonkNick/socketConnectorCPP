#include "telemetrics.h"
//int telemetrics::dc1;
//int telemetrics::dc2;
//int telemetrics::dc3;
//int telemetrics::dc4;
telemetrics* telemetrics::getInstance()
{
    if (instance == 0)
    {
        instance = new telemetrics();
    }

    return instance;
}
telemetrics::telemetrics(){

}