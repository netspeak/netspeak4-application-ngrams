# grpcwebproxy

This is a Docker image of the [gRPC-web proxy](https://github.com/improbable-eng/grpc-web/releases) by [improbable-eng](https://github.com/improbable-eng). We do not own/maintain/develop the gRPC-web proxy but we need it to run our gRPC services. As of the time of writing, there is no official Docker image, hence this image.

## Pulling

The docker image is publicly available for everyone!

```bash
docker run webis/grpcwebproxy:0.14.0 grpcwebproxy --help
```

## Building

To build the image, you have download the correct version of [the gRPC-web proxy](https://github.com/improbable-eng/grpc-web/releases).

## Publishing

To publish the Docker image, you have to member of the [webis organization](https://hub.docker.com/u/webis). If you are a member, you can simply run the `publish-to-dockerhub.sh` script (don't forget to update the version number in the script).

_Note:_ The version of the Docker image should be the actual version of the gRPC-web proxy. To adjust the version, you have to change `Dockerfile` and `publish-to-dockerhub.sh`. Commit all changes to these files after publishing a new version. The gRPC-web proxy binary must not be committed.
