#!/bin/bash

./scripts/generate_keys.sh

cd config

docker compose build

for i in `seq 1 10`; do
    # run 10 docker containers
    # each with different env var
    # and different ports
done