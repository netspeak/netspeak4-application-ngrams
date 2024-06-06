\page bindings Language Bindings

# Python

This project also contains Python 3 bindings using pybind11.

The Python bindings are a shared library (`.so`) and there are some limitations that come from this. The most notable limitation is that the bindings have runtime dependencies (the other Netspeak binaries all statically link their dependencies).

To run the bindings, you need to have boost-regex and boost-system installed. Assuming that you copied `netspeak4py.cpython-38-x86_64-linux-gnu.so` from the builder Docker container, then you also need to copy `/usr/lib/libantlr4-runtime.so` and `/usr/lib/libantlr4-runtime.so.4.7.1`.

To actually import the bindings in Python, make sure pass the directory `netspeak4py...so` is located via the [`PYTHONPATH`](https://docs.python.org/3/using/cmdline.html#envvar-PYTHONPATH) environment variable. If everything is setup correctly, Netspeak's Python bindings can be imported via:

```py
import netspeak4py
```

To see the API of the bindings, run `help(netspeak4py)`.