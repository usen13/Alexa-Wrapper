#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <string>
#include <gst/gst.h>

class MusicPlayer
{
private:
   GstBus     *bus = NULL;
   GstMessage *msg = NULL;
   GstElement *pipeline = NULL;
   long time_elapsed_ns = 0;

public:
   MusicPlayer() { gst_init (NULL, NULL); }
   ~MusicPlayer() { stop(); }

   void select(const std::string& uri) {
      if (pipeline != NULL)  stop();

      std::string gst_uri = std::string("playbin uri=") + uri;
      pipeline = gst_parse_launch (gst_uri.c_str(), NULL);
      bus = gst_element_get_bus (pipeline);
   }

   void play() {
      if (pipeline) {
         gst_element_set_state (pipeline, GST_STATE_PLAYING);

         g_print("\n%ld", time_elapsed_ns);
         if (time_elapsed_ns != 0) {
            msg = gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE, GstMessageType(GST_MESSAGE_ASYNC_DONE));

            gst_element_seek (pipeline, 1.0, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH,
                              GST_SEEK_TYPE_SET, time_elapsed_ns, GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE);
         }
         else {
            msg = gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE, GstMessageType(GST_MESSAGE_STREAM_START));
         }
      }
   }

   void pause() {
      if (pipeline) {
         gst_element_set_state (pipeline, GST_STATE_PAUSED);
         msg = gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE, GstMessageType(GST_MESSAGE_ANY));

         gst_element_query_position (pipeline, GST_FORMAT_TIME, &time_elapsed_ns);
         g_print("\n%ld", time_elapsed_ns);
      }
   }

   void stop() {
      time_elapsed_ns = 0;
      if (msg)       { gst_message_unref (msg); msg = NULL; gst_object_unref (bus); bus = NULL;}
      if (pipeline)  { gst_element_set_state (pipeline, GST_STATE_NULL);  gst_object_unref (pipeline); pipeline = NULL; }
   }
};

#endif

