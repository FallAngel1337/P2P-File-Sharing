# P2P-File-Sharing
A P2P file sharing network

# Index
- [How it works? ](#how-it-works)
- [How to install?](#how-to-install)
    - [Dependencies](#dependencies)
    - [Install from source](#install-from-source)
- [How to use?](#how-to-use)

## How it works?
It's basically a Bittorrent-like network implementation.
First of all, you need to create the torret file.
For that you just need to pass the file to the client arguments. This will generate a JSON file containing the
file fields and metadata. After that the server will know that you have one copy of the original file and will add you the owners list. Also other nodes can download it from you (this feature is under development).

To share the file to others, you give them your torrent file. Then they'll send that to their clients and the server will reply with the file owner IP:PORT, so that the client can download the file from you.

The server does not store any single byte of the file you are sharing, just some metadata info.

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
    $ ./P2P-Client <file>
    ```

- Building the server (not working)
    ```
    $ cd src/server/
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make
    $ ./P2P-Server
    ```

## How to use?
