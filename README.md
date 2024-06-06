<p align="center">
    <img width=300pt src="docs/media/netspeak-banner.svg"/>
    <h3 align="center">One word leads to another</h3>
</p>

---

<p align="center">
  <a href="https://github.com/netspeak/netspeak4-application-ngrams/blob/master/LICENSE">
  <img alt="MIT License" src="https://img.shields.io/github/license/netspeak/netspeak4-application-ngrams.svg"/>
  </a>
  <a href="https://github.com/netspeak/netspeak4-application-ngrams/releases">
  <img alt="Current Release" src="https://img.shields.io/github/release/netspeak/netspeak4-application-ngrams.svg"/>
  </a>
  <img alt="Test Status" src="https://github.com/netspeak/netspeak4-application-ngrams/actions/workflows/tests.yml/badge.svg"/>
  <br>
  <a href="https://netspeak.github.io/netspeak4-application-ngrams/index.html">Documentation</a> &nbsp;|&nbsp;
  <a href="https://webis.de/publications.html?q=netspeak">Publications</a>
</p>

This is the core library of Netspeak.

It contains methods to create indexes and make queries on Netspeak indexes. The Netspeak Protobuf messages and the Antlr4 grammar of the Netspeak query language are also part of this project. Whatever resource is shared between Netspeak's projects is probably in here.


# Getting started
The easiest way to get started is using the NetspeakCLI through the docker image:
```bash
docker run webis/netspeak:4.1.4 netspeak4 --help
```

#### For Windows users

Netspeak is a Linux project. To compile and run it on your system, install WSL with any Linux distribution and run all commands there.

# Contributing code

To write code, open the project in your IDE of choice (e.g. VSCode with the C++ plugin) and start typing!

Keep in mind that new files have to registered in `CMakeLists.txt`.

We use `clang-format` to format our code. If your IDE doesn't support this, you can use the `./build/format-all.sh` script to reformat all source files.


# Publishing a new release
\todo describe


---

## Contributors

- Michael Schmidt (2018 - 2020)
- Martin Trenkmann (2008 - 2013)
- Martin Potthast (2008 - 2020)
- Benno Stein (2008 - 2020)

<!--a href = "https://github.com/netspeak/netspeak4-application-ngrams/graphs/contributors">
  <img src = "https://contrib.rocks/image?repo=netspeak/netspeak4-application-ngrams"/>
</a-->