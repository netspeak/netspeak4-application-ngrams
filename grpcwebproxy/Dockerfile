FROM debian:buster

ENV DEBIAN_FRONTEND noninteractive

# Copy the executable
# This the gRPC-web proxy from Improbable Engineering
# https://github.com/improbable-eng/grpc-web/releases
COPY grpcwebproxy-v0.14.0-linux-x86_64 /usr/local/bin/grpcwebproxy

RUN chmod +x /usr/local/bin/grpcwebproxy

# Print the help message by default.
CMD grpcwebproxy --help
