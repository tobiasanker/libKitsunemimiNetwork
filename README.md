# libKitsuneNetwork

![Gitlab pipeline status](https://img.shields.io/gitlab/pipeline/tobiasanker/libKitsuneNetwork?label=build%20and%20test&style=flat-square)
![GitHub tag (latest SemVer)](https://img.shields.io/github/v/tag/tobiasanker/libKitsuneNetwork?label=version&style=flat-square)
![GitHub](https://img.shields.io/github/license/tobiasanker/libKitsuneNetwork?style=flat-square)
![C++Version](https://img.shields.io/badge/c%2B%2B-14-blue?style=flat-square)
![Platform](https://img.shields.io/badge/platform-Linux--x64-lightgrey?style=flat-square)

## Description

This is a small library for network connections. It provides servers and clients for unix-domain-sockets, tcp-sockets and ssl encrypted tcp-sockets. 

## Common Information aboud my projects

Here some common information about my projects and my code-styling. It's not complete and a bit short. I will write a styling-guide for my projects, where I will write this a bit longer with more explanation, why I make it like this.

### About my kitsune libraries

1. All my libraries beginning with `libKitsune`, because I needed a naming to identify my own libraries and I decided to use `Kitsune` as name, because Kitsunemimi are moe. ;)

2. The namespace of the code within the libraries is identically to the name of the library. So for example all content of `libKitsuneCommon` has the namespace `Kitsune::Common`.

3. If you only want to use the library, beside the binary you only ne the public methods and variables in the header-files, which are located in the `include`-directory of each `libKitsune`-repo. I try my best to make these as small and self-explaining, as possible. 

### About my repositories in general

1. I programming only on Linux. Sorry, but I have no time for supporting Windows and Mac.

2. All my projects are mirrored to gitlab, where I have still my gitlab-ci runner.

3. In each project-repository is an `build.sh`-script to make it easier for you to build the project by yourself. It download and link all required git-repositories in the correct version and link it all. This script is also used by the gitlab-ci-runner and so it is everytime up-to-date.

4. All my projects have unit-tests, which are executed by the ci-runner in each commit. Most of the functions are tested. Primary some negative-tests are missing. I try to avoid mocks in the tests as much as possible. Even tcp-socket-tests using the localhost for real data-transfer-tests.

5. For the naming of variables, methods and classes, I use camel case and for the names of files and directories, I use snake case.

6. About my comments:

    - Many comments in my code seems to be a bit useless, but I like to use them for optical separation. I also write them, when I think I finished the programming task and while writing the comments I recheck the code under the comment again and often I find improvements or bugs while this.

    - At first I planned to use doxygen comment-style for methods and files, but I think I will change this, because while writing the current state of the code, I don't like it to write big comments for each simple self-explaining method.

    - I don't like it to write much comments into header-files. More exactly I absolutly hate it, when I have to search the provided functions of a header between a massive amount of comments. Header-files are for me primary to have an overview of all provided functions, which I want to see as fast as possible. Comments of functions, parameters and return-values in my code are only written into the source-files. So when something is unclear for a specific method, then look into the source-file. If the comment there are not helpful for you, then please write me a mail or an issue to fix this. 

7. I try to avoid templates and macros as much as possible. I don't dislike them, but I only use them, when I don't have another solution. 

## Build

I write my projects with the Qt-creator, but without Qt itself. 

### Requirements

#### Official repositories

paket | version
--- | ---
g++ | 6.3.0
qt5-qmake | 5.7.1
libssl1.1 | 1.1.0k
libssl-dev | 1.1.0k
libboost-filesystem-dev | 1.62
libsqlite3-dev | 3.16.2

This are the version I have installed under Debian Stable via apt. Some older or newer version should work as well. I write my projects with the Qt-creator, but without Qt itself. Thats why qmake is required to build my projects.

IMPORTANT: All my projects are only tested on Linux. 

#### Kitsune-repositories

Repository-Name | Version-Tag | Download-Path
--- | --- | ---
libKitsuneCommon | 0.5.1 |  https://github.com/tobiasanker/libKitsuneCommon.git
libKitsunePersistence | 0.4.0 |  https://github.com/tobiasanker/libKitsunePersistence.git

### build library

In all of my repositories you will find a `build.sh`. You only have to run this script. It doesn't required sudo, because you have to install required tool via apt, for example, by yourself. But if other projects from me are required, it download them from github and build them in the correct version too. This script is also use by the ci-pipeline, so its tested with every commit.

Before running the build-script:

```bash
.
└── libKitsuneNetwork
    ├── build.sh
    └── ...
```

After running the build-script:

```bash
.
├── build
│   ├── libKitsuneCommon
│   │   └── ...
│   ├── libKitsunePersistence
│   │   └── ...
│   └── libKitsuneNetwork
│       └── ...
│
├── libKitsuneCommon
│   └── ...
├── libKitsunePersistence
│   └── ...
├── libKitsuneNetwork
│   ├── build.sh
│   └── ...
│
└── result
    ├── include
    │   ├── libKitsuneCommon
    │   │   └── ...
    │   ├── libKitsunePersistence
    │   │	└── ...
    │   └── libKitsuneNetwork
    │       └── ...
    │
    ├── libKitsuneCommon.so -> libKitsuneCommon.so.0.5.1
    ├── libKitsuneCommon.so.0 -> libKitsuneCommon.so.0.5.1
    ├── libKitsuneCommon.so.0.5 -> libKitsuneCommon.so.0.5.1
    ├── libKitsuneCommon.so.0.5.1
    │
    ├── libKitsunePersistence.so -> libKitsunePersistence.so.0.3.0
    ├── libKitsunePersistence.so.0 -> libKitsunePersistence.so.0.3.0
    ├── libKitsunePersistence.so.0.3 -> libKitsunePersistence.so.0.3.0
    ├── libKitsunePersistence.so.0.3.0
    │
    ├── libKitsuneNetwork.so -> libKitsuneNetwork.so.0.2.0
    ├── libKitsuneNetwork.so.0 -> libKitsuneNetwork.so.0.2.0
    ├── libKitsuneNetwork.so.0.2 -> libKitsuneNetwork.so.0.2.0
    └── libKitsuneNetwork.so.0.2.0
```

It create automatic a `build` and `result` directory in the directory, where you have cloned the project. At first it build all into the `build`-directory and after all build-steps are finished, it copy the include directory from the cloned repository and the build library into the `result`-directory. So you have all in one single place.

Tested on Debian and Ubuntu. If you use Centos, Arch, etc and the build-script fails on your machine, then please write me a mail and I will try to fix the script.


## Usage

(sorry, that the usage-chapter here is very short, but while writing I found some points, which must become better.)

This libray contains servers and clients for unix-domain-sockets, tcp-sockets and ssl encrypted tcp-sockets. Each socket and server runs in its own thread.

The inheritance tree of the classes has the following look. For servers its the same with only servers instead of sockets.

```
        Thread (libKitsuneCommon)
          ^
          |
    AbstractSocket
    ^            ^
    |            |
TcpSocket    UnixDomainSocket
    ^
    |
TlsTcpSocket
```

Because each socket run in its own thread, this thread have to handle incoming messages. So the class to handle these messages have to inherite the class `MessageTrigger` (`message_trigger.h`). The `runTask` of this class is called whenever the socket get some data. This has to append to the server and sockets. See following examples:

```cpp
#include <message_trigger.h>
#include <message_ring_buffer.h>
#include <abstract_socket.h>

class DemoBuffer : public MessageTrigger
{
public:
    DemoBuffer() {};
    ~DemoBuffer() {};

	uint64_t
	runTask(MessageRingBuffer &recvBuffer,
	        AbstractSocket* socket)
	{
	    // example
	    const uint8_t* dataPointer = getDataPointer(recvBuffer, NUMBER_OF_BYTES);
	    // do something with the dataPointer
	    // recvBuffer contains the data and socket is the socket, which had the message received
	}

};
```

Similar to the message-trigger, to handle messages by the thread of the socket, there is another class `ConnectionTrigger` (`connection_trigger.h`) to handle incoming connections by the thread of the server. This class has an abstract method called `handleConnection`, which is called for every socket, which is created by the server. See example:

```cpp
#include <connection_trigger.h>
#include <abstract_socket.h>

class IncomeTrigger : public ConnectionTrigger
{
public:
    IncomeTrigger() {};
    ~IncomeTrigger() {};

    void
    handleConnection(AbstractSocket* socket)
    {
        // this example starts the thread of every socket, which is created by the server. 
        // this MUST be done, if the socket should be able the receive data.
        socket->start()
    }

};
```

Example to create server and socket:
(the `TlsTcpSocket` of the example can also replaced by the `UnixDomainSocket` or `TcpSocket`. Only the values of the conectructor are different)

```cpp
#include <tls_tcp/tls_tcp_server.h>
#include <tls_tcp/tls_tcp_socket.h>

TlsTcpServer* server = nullptr;
TlsTcpSocket* socketClientSide = nullptr;
TlsTcpSocket* socketServerSide = nullptr;

// init the demo-buffer from above
DemoBuffer* buffer = new DemoBuffer();
IncomeTrigger* incomeTrigger = new IncomeTrigger();

// create server on port 12345 and start his thread
server = new TlsTcpServer("/tmp/cert.pem",
                          "/tmp/key.pem",
                          buffer,           // <- message-trigger, which is given to any, socket which is spawned by the server
                          incomeTrigger);   // <- connection-trigger, which is called for every incoming connection
                                    
// let the server listen on port
server->initServer(12345);
// start the thread, so it can create a socket for every incoming 
//    connection in the background
server->start();

// create a client which works as client and connect to the server
socketClientSide = new TlsTcpSocket("127.0.0.1",
                                    12345,
                                    "/tmp/cert.pem",
                                    "/tmp/key.pem");
// if the client should only send and never receive messages,
//    it doesn't need the following two lines. These init the buffer
//    for incoming messages and starting the thread of the client-socket
socketClientSide->addNetworkTrigger(buffer);
socketClientSide->start();
//..

// get socket on server-side, which was spawned by the incoming connection
if(server->getNumberOfSockets() > 0)
{
    socketServerSide = static_cast<TlsTcpSocket*>(m_server->getSocket(0));
}

// send data
socketClientSide->sendMessage(std::string("any message"));
// instead of socketClientSide you can use socketServerSide the same way

```


## Contributing

Please give me as many inputs as possible: Bugs, bad code style, bad documentation and so on.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
