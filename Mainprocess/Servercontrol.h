#ifndef SERVERCONTROL_H_
#define SERVERCONTROL_H_

#include "Httprocess/Httprespone.h"
#include "Httprocess/Httprocess.h"

void Epolladd(int socketfd);
void Epolldel(int socketfd);
void Epollread(int socketfd);
void Epollwrite(int socketfd);
void Server_start_Epollcontrol();

static int epoll_has_connect = 1;
static void *Epoll_has_connect = &epoll_has_connect;

#endif