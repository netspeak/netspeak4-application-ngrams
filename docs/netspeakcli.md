\page netspeakcli Netspeak CLI

After compiling, you can run the Netspeak executable which will start a small command line interface for Netspeak.

The Netspeak CLI aims to be self-documenting, so this section will be brief. To view the full CLI documentation run `netspeakcli --help`. This will list all commands and how to use them.

### Build Command

The `build` command can be used to build a new Netspeak index from a given data set. The basic usage works as follows:

```bash
./netspeakcli build -i "/data-set" -o "/my-index"
```

`/data-set` is the input directory that contains all phrases. Netspeak will assume all files in the input directory are simple text files containing phrase-frequency pairs.

Netspeak will parse all files in the input directory and create a index in `/my-index`, the output directory.

Depending on the number of phrase, the build process may take several hours. The whole process is usually bound by whatever storage medium is used to store the phrase and the index, so you can use your computer while the index is being created.

\note
    When building large indexes using `./netspeakcli build -i <input> -o <output>`, be sure that the limit on the maximum number of opened files by one process is high enough.
    __If it's too low, the build process will fail.__
    For small indexes a limit for 1024 is sufficient but for larger data sets (>10GB input), be sure it's at least 2048. You can set the limit using the `ulimit` command. <br>
    WSL users: This limit will be reset with every restart of your Linux subsystem.
