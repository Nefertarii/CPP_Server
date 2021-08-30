#ifndef SERVERCONTROL_H_
#define SERVERCONTROL_H_

#include "Protocol.h"
#include "Headfile.h"
#include "Servprocess.h"

void Server_start_Epollcontrol();

static int epoll_has_connect = 1;
static void *Epoll_has_connect = &epoll_has_connect;

#endif