# P2P-File-Sharing
A P2P file sharing network.
The project stills under development, so if something is missing or its bad, don't worry, I'll take care of it.

# Index
- [How it works? ](#how-it-works)
- [How to install?](#how-to-install)
    - [Dependencies](#dependencies)
    - [Install from source](#install-from-source)
- [How to use?](#how-to-use)

## Todo list
- [ ] Create the listen client daemon to send the requested file.
- [ ] Do soem code refactoring (readability)
- [ ] Use protobuf instead of JSON?

## How it works?
It's basically a BitTorrent-like network implementation. So you have those who have the file and provide it tho the other nodes (seeders) and the other who donwload the file and become seeders too.

 
 It uses a torrent file, that is basically a `JSON` format file that contains some file metadata.


Below we have a simple diagram showing how the client handles the creation of the torrent file.

 ![Client-Side](images/client-side-workflow.png)

 Also how the server reply you with the file owner `IP` and `PORT` and add you the file owner list.

 ![Server-Side](images/server-uploading.png)

 And finally how the download workflow is.

 ![Downloading](images/downloading.png)

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
```

After that you can configure the client and server through the config files located at `conf` folder.
Then continue with 
```
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