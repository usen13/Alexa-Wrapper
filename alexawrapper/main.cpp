#include <mosquitto.h>
#include "mqtthandler.hpp"

int main (int argc, char *argv[])
{
   MQTTHandler().loop_forever();
   return 0;
}

