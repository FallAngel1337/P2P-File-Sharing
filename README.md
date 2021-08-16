# P2P-File-Sharing
A P2P file sharing network

# Index
- [How it works? ](#how-it-works)
- [How to install?](#how-to-install)
    - [Dependencies](#dependencies)
    - [Install from source](#install-from-source)
- [How to use?](#how-to-use)

## How it works?
<write about how it works later\>

## How to install?
Choose from where you want and follow the instructions to install the program.

### Dependencies
Sure you need: `gcc/g++`, `make` and `cmake`. You can install all of them through `apt`.

Before installing make sure you have installed the following dependencies:
- cJSON
    - [From source](https://github.com/DaveGamble/cJSON#building)
    - Using `apt` on debian-based distros:
    
        `$ sudo apt update && sudo apt install libcjson-dev`

- libconfig
    - You can check it at their [website](https://hyperrealm.github.io/libconfig/) or at [github](https://hyperrealm.github.io/libconfig/)
    - Using `apt` on debian-based distros:

        `$ sudo apt update && sudo apt install libconfig-dev`

- libssl-dev
    - Using `apt` on debian-based distros:

        `$ sudo apt update && sudo apt install libssl-dev`

### Install from source:
```
$ git clone https://githubcom/FallAngel1337/P2P-File-Sharing
$ cd P2P-File-Sharing
$ make
```

For now, you need to build both separately.
-  Building the client
    ```
    $ cd src/client/
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make
    $ ./P2P-Client --help
    ```

- Building the server (not working)
    ```
    $ cd src/server/
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make
    $ ./P2P-Server --help
    ```