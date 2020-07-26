FROM alpine:3.7

RUN mkdir alexa && mkdir mosquitto && mkdir app

# clone alexa and mosquitto
RUN cd alexa     && git clone <alexa-sdk>       && cd ..
RUN cd mosquitto && git clone <mosquitto-sdk>   && cd ..

# build and install select components with switches
RUN cd alexa     && <build-alexa> <build-switches...>       && cd ..
RUN cd mosquitto && <build-mosquitto> <build-switches...>   && cd ..

# build custom app that uses alexa & mosquitto components
RUN cd app    && <build-app> <build-switches...>

# run the alexa service. optionally mosquitto broker
CMD [./app, parameters]
CMD [./mosquitto, parameters]

# after build the container can be started with "docker run <alexa-credentials-config> <with-mqtt-broker> <mqtt-broker-port>"