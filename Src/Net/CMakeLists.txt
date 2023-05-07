set(NetSRC 
    acceptor.cpp
    connector.cpp
    inetaddress.cpp
    socket.cpp
    socketpack.cpp
    tcpclient.cpp
    tcpconnection.cpp
    tcpserver.cpp
)

message("Process:" ${CMAKE_CURRENT_SOURCE_DIR})

add_library(wasi_net STATIC ${NetSRC})