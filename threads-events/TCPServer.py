from socket import socket, AF_INET, SOCK_STREAM

serverPort = 12000
serverSocket = socket(AF_INET, SOCK_STREAM)
serverSocket.bind(("", serverPort))
serverSocket.listen(1)
print("The server is ready to receive")
connectionSocket, addr = serverSocket.accept()
print("addr bound to the socket on the other end of the connection ", addr)
while True:
    # socket.recv(bufsize)
    sentence = connectionSocket.recv(1024).decode()
    print("received sentence ", sentence)
    capitalizedSentence = sentence.upper()
    print("changed sentence ", capitalizedSentence)
    print("encoded message to send", capitalizedSentence.encode())
    connectionSocket.send(capitalizedSentence.encode())

    if "exit" in sentence.lower():
        connectionSocket.close()
