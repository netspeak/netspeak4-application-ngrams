#!/bin/bash -e

docker login
docker build -t webis/netspeak:4.1.4 .
docker push webis/netspeak:4.1.4

