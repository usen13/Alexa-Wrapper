#ifndef MQTTHANDLER_H
#define MQTTHANDLER_H

#include <mosquitto.h>
#include <json/json.h>
#include "musicplayer.hpp"

namespace {
   static const int MQTT_BROKER_PORT = 30001;
   static const std::string MUSICPLAYER_ID = "MUSICPLAYER";
   static const std::string MUSICPLAYER_TOPIC = "swarm/musicplayer";

   static mosquitto* mqttclient;
   static MusicPlayer* musicplayer;
}

class MQTTHandler
{
public:
   MQTTHandler() {
      mosquitto_lib_init();
      mqttclient = mosquitto_new(MUSICPLAYER_ID.c_str(), true, NULL);
      mosquitto_connect_callback_set   (mqttclient, &MQTTHandler::on_connect);
      mosquitto_message_callback_set   (mqttclient, &MQTTHandler::on_message);
      mosquitto_connect (mqttclient, "localhost", MQTT_BROKER_PORT, 60);
   }
   ~MQTTHandler() {
      delete musicplayer;
      mosquitto_destroy(mqttclient);
      mosquitto_lib_cleanup();
   }


   void loop_forever() {
      mosquitto_loop_forever(mqttclient, 100, 1);
   }
   static void on_connect(struct mosquitto *mosq, void *userdata, int rc) {
      if(rc == 0) {
         musicplayer = new MusicPlayer();
         mosquitto_subscribe(mqttclient, NULL, MUSICPLAYER_TOPIC.c_str(), 1);
      }
   }
   static void on_message(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message) {
      Json::Value jsonroot;
      JSONCPP_STRING err;

      std::string topic((char*)message->topic);
      std::string payload((char*)message->payload);

      Json::CharReaderBuilder builder;
      const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
      if (!reader->parse(payload.c_str(), payload.c_str() + payload.length(), &jsonroot, &err)) { return; }

      std::string action = jsonroot["action"].asString();

      if (topic == MUSICPLAYER_TOPIC) {
         if (action == "SELECT") {
            bool offline = jsonroot["offline"].asBool();
            musicplayer->select(offline ? "file://localhost/ShapeOfYou.mp3"
                                        : "https://www.soundhelix.com/examples/mp3/SoundHelix-Song-1.mp3");
         }
         if (action == "PLAY")   { musicplayer->play();  }
         if (action == "PAUSE")  { musicplayer->pause(); }
         if (action == "STOP")   { musicplayer->stop();  }
      }
}

};

#endif


