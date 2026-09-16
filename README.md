# cpp-utility

The `kotone` library is a collection of useful C++23 data structures and algorithms for [AtCoder](https://atcoder.jp/home) and AtCoder-like platforms. All implementations are compatible with AtCoder's latest environment (C++23 GCC 15.2.0 since November 2025).

The library features self-contained implementations that programmers can use without modifying the internal code. The exception is `kotone/misc`, which includes snippets, notes, and many other files that do not belong to the aforementioned category.

> [!WARNING]
> **This repository is under construction.**
>
> * Some header files lack sufficient documentation.
>
> * Deprecated or unused headers have been moved to the `legacy` directory.

## Installation

1. Create a directory for your C++ source files.

2. Download the `kotone` directory and place it in the same directory as your C++ files.

3. When compiling with `g++`, use the `-I .` option to include the library. For example:

    ```bash
    g++ main.cpp -std=c++20 -I .
    ```

### Using the AtCoder Library

Some headers in this library depend on the [AtCoder Library (ACL)](https://github.com/atcoder/ac-library). To use these components:

1. Download the `atcoder` directory from the ACL repository.

2. Place it in the same directory as your C++ files.

Refer to the [official ACL repository](https://github.com/atcoder/ac-library) for more details.
