#ifndef SERVER_H
#define SERVER_H

#include <WebServer.h>

extern WebServer server;

void startServer();
void handleClient();

#endif
