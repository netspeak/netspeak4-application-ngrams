# Netspeak 4 application C++

[![Actions Status](https://github.com/netspeak/netspeak4-application-cpp/workflows/Build/badge.svg)](https://github.com/netspeak/netspeak4-application-cpp/actions)

This is the core library of Netspeak 4.

It contains methods to create indexes and make queries on Netspeak indexes. The Netspeak 4 Protobuf messages and the Antlr4 grammar of the Netspeak 4 query language are also part of this project. Whatever resource is shared between Netspeak's projects is probably in here.


## Getting started

### Installing dependencies

Before you can compile and run anything, you have to install the dependencies. To do this, simply run:

```bash
sudo bash build/install-dependencies.sh
```

This will install, a C++ compiler, build tools, gRPC, Protobuf tools, and other general dependencies.

Some of the dependencies will by compiled from source and might be compiled/installed in parallel causing noticeable stuttering or even freezes for a few seconds. The heaviest dependencies will be installed globally, so they only have to be installed once.

#### For Windows users

Netspeak 4 is a Linux project. To compile and run it on your system, install WSL with any Linux distribution and run all commands there.

#### For Webis members

Among the dependencies are some Webis-only projects.
These will be checked out via git in readonly mode.
If you want to make changes to these projects and see how the affect Netspeak, replace the project folders in `./build/dependencies/` with symbolic links to wherever your pushable version is.

### Building

To build this project, first generate the `Makefile` using CMake. This project gets its own local CMake version, so you have to run the following command once:

```bash
bash build/cmake.sh
```

Use the `make` command to compile the `netspeak4` and `netspeak4-test` executables.

```bash
make
```

### Running tests

First build this project and then run the `./build/run-tests.sh` script:

```bash
bash ./build/run-tests.sh
```

(It's recommended to do it like this because otherwise the test application might have problems locating its test resources.)

### Writing code

To write code, open the project in your IDE of choice (e.g. VSCode with the C++ plugin) and start typing!

Keep in mind that new files have to registered in `CMakeLists.txt`.

We use `clang-format` to format our code. If your IDE doesn't support this, you can use the `./build/format-all.sh` script to reformat all source files.


## Using the Netspeak4 CLI

After compiling, you can run `./netspeak4` which will start a small command line interface for Netspeak.

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
For small indexes a limit for 1024 is sufficient but for larger data sets (>10GB input), be sure it's at least 2048. You can set the limit using the `ulimit` command. <br>
WSL users: This limit will be reset with every restart of your Linux subsystem.

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


## Netspeak query language

The Netspeak query syntax as described here should be used as reference. There
might be other syntax information out there, e.g. at netspeak.org, which
provides some syntactical simplifications in form of easier to use wildcards or
operators. However, these modified syntaxes are just front-ends and do not work
with the original Netspeak interface. Here is the truth:

```
?   is a placeholder for exactly one word and can be sequenced to search for
    exaclty two, three, four ... words.

    Example:    how to ? this
                -> how to use this
                -> how to do this
                -> how to cite this

*   is a placeholder for zero or many words.

    Example:    see * works
                -> see how it works
                -> see if it works
                -> see what works

[]  compares options, i.e. it checks each word or phrase between these
    brackets plus the so called empty word at that position in the query.

    Example:    it's [ great well "so good" ]
                -> it's
                -> it's great
                -> it's well
                -> it's so good

{}  checks the order, i.e. it tries to find each permutation of the given
    sequence of words or phrases at that position in the query.

    Example:    for { "very important people" only }
                -> for very important people only
                -> for only very important people

#   searches for alternatives of the word following. This operator requests
    the optional Netspeak hash-dictionary component and uses [] to compare
    each retrieved alternative (except that the empty word is not checked).
    The mapping from word to alternatives is completely up to the user when
    building Netspeak, for netspeak.org we use this operator for a synonym
    search providing the Wordnet dictionary.

    Example:    waiting for #response
                -> waiting for response
                -> waiting for answer
                -> waiting for reply
```

You can combine the introduced wildcards and operators as you want, but with the
exception that you may not place any wildcard within bracket operators. Also
nested brackets are not allowed. As you can see in the examples above you can
quote phrases to be handled as one entity is `[]` and `{}`.


---

## Contributors

Michael Schmidt (2018 - 2020)

Martin Trenkmann (2008 - 2013)

Martin Potthast (2008 - 2020)

Benno Stein (2008 - 2020)
