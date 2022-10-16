FROM ubuntu:latest
RUN apt update && apt upgrade && apt-get install -y g++ libssl-dev
RUN mkdir Blockchain
WORKDIR /Blockchain
EXPOSE 8080
ENTRYPOINT ["sh", "./entrypoint.sh"]