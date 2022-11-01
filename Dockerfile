FROM ubuntu:latest
RUN apt -y update && apt -y upgrade && apt-get -y install g++ libssl-dev libasio-dev libwebsocketpp-dev
RUN mkdir Blockchain
WORKDIR /Blockchain
EXPOSE 8080
ENTRYPOINT ["/bin/bash", "./entrypoint.sh"]
