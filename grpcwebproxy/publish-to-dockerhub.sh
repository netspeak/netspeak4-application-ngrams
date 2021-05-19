#!/bin/bash -e

docker login
docker build -t webis/grpcwebproxy:0.14.0 .
docker push webis/grpcwebproxy:0.14.0
