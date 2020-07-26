FROM ubuntu:latest

RUN apt-get install \
            build-base pkg-config \            
	    mosquitto-dev jsoncpp-dev \
	    chrpath diffstat gawk texinfo \
	    python python3 wget \
	    git libssl-dev quilt cmake \
	    libsqlite3-dev libarchive-dev python3-dev \
	    libdb-dev libpopt-dev zlib1g-dev \
	    libssl-dev


RUN git clone git://git.openembedded.org/openembedded-core oe-core -b rocko
RUN cd oe-core 
RUN git clone git://git.openembedded.org/bitbake -b 1.36
RUN export OE_CORE_PATH=$(pwd) && cd ..

RUN mkdir alexa && cd alexa 
RUN git clone https://github.com/alexa/alexa-auto-sdk.git
RUN cd builder && ./build.sh linux -t native
RUN cd ..

COPY . .

RUN g++ -Wall main.cpp -o main `pkg-config --cflags --libs libmosquitto jsoncpp`

CMD ["./main"]
