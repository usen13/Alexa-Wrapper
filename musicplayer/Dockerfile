FROM alpine:3.12

RUN apk add --no-cache --update \
            build-base pkgconfig \
            gstreamer-dev gst-plugins-good \
            mosquitto-dev jsoncpp-dev

COPY . .

RUN g++ -Wall main.cpp -o main `pkg-config --cflags --libs gstreamer-1.0 libmosquitto jsoncpp`

CMD ["./main"]
