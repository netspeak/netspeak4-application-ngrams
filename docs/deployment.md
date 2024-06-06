\page deployment Deployment
\tableofcontents


## Protobuf

This project also contain the Netspeak protobuf message definitions: `conf/NetspeakMessages.proto`.

After changing the definition, you have to run `build/generate-protobuf-files.sh` which, as the name suggests, will (re-)generate all protobuf C++ files. The generated files are located under `src/netspeak/generated/`.

You also have to update the generated protobuf files `netspeak4-application-java` by running `src/scripts/generate-protobuf-files.sh` (this requires Java >= 1.6 to be installed).

\note In case you OS doesn't support the specific protobuf version required, use the dev console from `netspeak4-deployment`.


## Antlr4

The Antlr4 grammar of the Netspeak query language can be found under `conf/grammar/`. Changes to these files require you to regenerate the Antlr4 files by running `build/generate-antlr4-files.sh` (this requires Java >= 1.6 to be installed).


## gRPC-web proxy

A [gRPC-web proxy](https://github.com/netspeak/grpcwebproxy) is necessary to use Netspeak in browsers. This is how you use the proxy.

### Linux

You need to have Docker installed.

Serve the index(es):

```cmd
docker run -p 9000:9000 -v /path/to/index:/index:ro webis/netspeak:4.1.3 netspeak4 serve -c /index/index.properties -p 9000
```

Run the proxy:

```cmd
docker run --network="host" webis/grpcwebproxy:0.14.0 grpcwebproxy --allow_all_origins --backend_addr=localhost:9000 --backend_tls=false --run_tls_server=false
```

### Windows

To run the gRPC-web proxy on Windows, you need to have WSL2 and Docker Desktop (with WSL2 backend) installed.

Serve the index(es):

```cmd
docker run -p 9000:9000 -v C:\path\to\index:/index:ro webis/netspeak:4.1.3 netspeak4 serve -c /index/index.properties -p 9000
```

Run the proxy:

```cmd
docker run -p 8080:8080 webis/grpcwebproxy:0.14.0 grpcwebproxy --allow_all_origins --backend_addr=host.docker.internal:9000 --backend_tls=false --run_tls_server=false
```
