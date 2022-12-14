#!/bin/bash

set -e

n=${1:-5}

if [ $# -ge 1 ]; 
    then shift; 
fi

# comment to compile within the docker container
/bin/bash ./scripts/compile.sh $@

cd config

docker compose build --quiet

cd ../

if [[ $(docker network ls | grep blockchain_net) == "" ]]; then 
    docker network create --driver bridge blockchain_net
fi

cleanup() {
    echo "Cleaning up docker containers"
    docker container stop $(docker container ls -qf name="user.*_container") > /dev/null
    docker container rm $(docker container ls -aqf name="user.*_container") > /dev/null
}   

trap cleanup EXIT

echo "Starting docker containers"

for i in `seq 1 $n`; do
    # run 5 docker containers
    # each with different env var
    # and different ports

    user="user$i"
    port="$((8000 + $i))"

    if [[ $(docker container ls -aqf name="${user}_container") == "" ]]; then
        docker run -it -d \
            --name "${user}_container" \
            --env "BC_username=$user" \
            --env "BC_url=ws://${user}_container:$port/" \
            --env "BC_connection_url=ws://user1_container:8001/" \
            --network "blockchain_net" \
            --expose $port \
            -p $port:$port \
            --volume=$PWD:/blockchain \
            blockchain_img:v1 > /dev/null
        echo " - Created container for $user, on port $port"
    else
        docker container start "${user}_container"
        echo " - Started container for $user, on port $port"
    fi
done

docker attach "user1_container"
