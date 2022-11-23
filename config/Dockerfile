FROM ubuntu:latest
RUN apt -y update && apt -y upgrade && apt-get -y install \
    g++ libjsoncpp-dev libssl-dev libasio-dev libwebsocketpp-dev
RUN mkdir blockchain
WORKDIR /blockchain
ENTRYPOINT ["/bin/bash", "./scripts/entrypoint.sh"]
