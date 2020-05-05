# Netspeak 4 application C++

This is the core library of Netspeak 4.

It contains methods to create indexes and make queries on Netspeak indexes. The Netspeak 4 Protobuf messages and the Antlr4 grammar of the Netspeak 4 query language are also part of this project. Whatever resource is shared between Netspeak's projects is probably in here.


## Getting started

To get this to compile you'll need to also checkout a few other projects and dependencies.
To do this, the [Netspeak4 deployment](https://github.com/netspeak/netspeak4-deployment) project is recommended as it also provides a few scripts to install the dependencies and build all C++ projects.

To checkout all project follow the checkout instruction of the Netspeak4 deployment project. Using the `netspeak4-deployment/project/install-cpp-dependencies.sh` script, the dependencies of all project will be installed automatically. <br>
One of the dependencies of this project is [Antlr4](https://github.com/antlr/antlr4).
The Antlr4 compiler and runtime will both be downloaded under `./antlr4/`.
The runtime will then be built and installed. This process can take a few minutes.

### Writing code

After this is done, you can use your IDE of choice to write C++ code. We use `qmake` to generate the `Makefile` from the `.pri` and `.pro` files. Simply run the `qmake` command for this.
If this fails, see the build script in [Netspeak4 deployment](https://github.com/netspeak/netspeak4-deployment).

We use `clang-format` to format our code. If your IDE doesn't support this, you can use the `./build/format-all.sh` script to reformat of source files.

### Build

To build this project, first generate the `Makefile` using `qmake`. Then run the `make` command and that's it.

### Running tests

First (re)build this project and then run the `./build/run-tests.sh` script.
It's recommended to do it like this because otherwise the test application might have problems locating its test resources.

## Using the Netspeak4 CLI

After compiling, you can run `./build/release/netspeak4` which will start a small command line interface for Netspeak.

#### `build`

The `build` command can be used to build a new Netspeak4 index from a given data set. The basic usage works as follows:

```bash
./netspeak4 build -i "/data-set" -o "/my-index"
```

`/data-set` is the input directory that contains all phrases. Netspeak will assume all files in the input directory are simple text files containing phrase-frequency pairs.

Netspeak will parse all files in the input directory and create a index in `/my-index`, the output directory.

Depending on the number of phrase, the build process may take several hours. The whole process is usually bound by whatever storage medium is used to store the phrase and the index, so you can use your computer while the index is being created.

__NOTE:__ When building large indexes using `./netspeak4 build -i <input> -o <output>`, be sure that the limit on the maximum number of opened files by one process is high enough.
__If it's too low, the build process will fail.__
For small indexes a limit for 1024 is sufficient but for larger data sets (>10GB input), be sure it's at least 2048. You can set the limit using the `ulimit` command.

#### `shell`

After creating an index, you can use `./netspeak4 shell -i "/my-index"` to start up a small interactive shell. The shell will take any Netspeak query and display a textual representation of the result set.

#### `regex`

After creating an index, you can use `./netspeak4 regex -i "/my-index/regex-list/regex-vocab"` to start up a small interactive shell that will only return the results of the regex index. The shell will take any regex query and display the results.


## Protobuf

This project also contain the Netspeak4 protobuf message definitions: `conf/NetspeakMessages.proto`.

After changing the definition, you have to run `build/generate-protobuf-files.sh` which, as the name suggests, will (re-)generate all protobuf C++ files. The generated files are located under `src/netspeak/generated/`.

You also have to update the generated protobuf files `netspeak4-application-java` by running `src/scripts/generate-protobuf-files.sh` (this requires Java >= 1.6 to be installed).

_Note:_ In case you OS doesn't support the specific protobuf version required, you the dev console from `netspeak4-deployment`.


## Antlr4

The Antlr4 grammar of the Netspeak4 query language can be found under `conf/grammar/`. Changes to these files require you to regenerate the Antlr4 files by running `build/generate-antlr4-files.sh` (this requires Java >= 1.6 to be installed).


---

## Contributors

Michael Schmidt (2018 - 2020)

Martin Trenkmann (2008 - 2013)

Martin Potthast (2008 - 2020)

Benno Stein (2008 - 2020)
