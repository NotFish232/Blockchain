#!/bin/bash

set -e

/bin/bash ./scripts/compile.sh

/bin/bash ./scripts/generate_keys.sh

cd config

docker compose build

cd ../

for i in `seq 1 10`; do
    # run 10 docker containers
    # each with different env var
    # and different ports
    user="user$i"
    port="$((8000 + $i))"
    echo "Starting container for $user, on port $port"
    docker run -d \
        --name "${user}_container" \
        --env "BLOCKCHAIN_USER=$user" \
        --env "BLOCKCHAIN_PORT=$port" \
        --expose $port \
        -p $port:$port \
        --volume=$PWD:/blockchain \
        blockchain_img:v1
done
