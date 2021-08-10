# P2P-File-Sharing
A P2P file sharing network

# Index
- [How it works? ](#how-it-works)
- [How to install?](#how-to-install)
    - [Dependencies](#dependencies)
    - [Install from Source](#install-from-source)
- [How to use](#how-to-use)

## How it works?
<write about how it works later\>

## How to install?
<Need to make the instructions work properly\>

Choose from where you want and follow the instructions to install the program.

### Dependencies
Before installing make sure you have installed the following dependencie(s):
- cJSON
    - [From source](https://github.com/DaveGamble/cJSON#building)
    - Using `apt` on debian-based distros 
    
        `$ sudo apt update && sudo apt install libcjson-dev`

### Install from source:
```
$ git clone https://github.com/FallAngel1337/P2P-File-Sharing.git
$ mkdir build
$ cd build/
$ cmake .. && make
$ cd ../bin
$ ./P2P-Server
```

