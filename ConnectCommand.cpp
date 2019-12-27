//
// Created by shiraz and nili on 12.12.2019.
//
#ifndef ex3__CONNECTCOMANND_CPP_
#define ex3__CONNECTCOMANND_CPP_
#include <cstring>
#include "Var.cpp"
static int client_socket = 0;
class ConnectCommand : public Command {
 public:
  virtual void execute(queue<string> &token) {
    Singleton *singleton = Singleton::getSingleton();
    //name of command
    token.pop();
    //ip and port
    string arguments = token.front();
    string ipTemp = "", portTemp = "";
    int i = 0;
    for (; arguments[i] != '"'; i++) {
    }
    while (arguments[i] != ',') {
      ipTemp += arguments[i++];
    }
    //remoove the ""
    ipTemp = ipTemp.substr(1, ipTemp.length() - 2);
    const char *ip = ipTemp.c_str();
    int size = arguments.length();
    while (i < size - 1) {
      i++;
      portTemp += arguments[i];
    }
    int port = ex1::cal(portTemp, *singleton->getVarValues());
    token.pop();
    thread thread_out_1(Connect, ip, port);
    thread_out_1.join();
    thread thread_out_2(writeMessages);
    thread_out_2.detach();
  }
  static int writeMessages() {
    Singleton* singleton = Singleton::getSingleton();
    while(true) {
      if (!singleton->isMessagesEmpty()) {
        singleton->mutex_lock.lock();
        string m = singleton->getMessage();
        singleton->mutex_lock.unlock();
        const char* message = m.c_str();
        int is_sent = send(client_socket , message , strlen(message) , 0 );
        if (is_sent == -1) {
          cout << "Error sending message" << endl;
          exit(1);
        }
      } else {
        this_thread::sleep_for(chrono::milliseconds(10));
      }
    }
    return client_socket;
  }
  static void Connect(const char *ip, int port) {
    client_socket= socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
      //error
      cerr << "Could not create a socket" << endl;
      exit(1);
    }
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip);
    address.sin_port = htons(port);
    int is_connect = connect(client_socket, (struct sockaddr *)&address, sizeof(address));
    if (is_connect == -1) {
      cerr << "Could not connect to host server" << endl;
      exit(1);
    }
  }
  virtual ~ConnectCommand() {
    close(client_socket);
  }
};
#endif