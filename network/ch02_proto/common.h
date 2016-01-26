/* 
 * File:   common.h
 * Author: ycherkasov
 *
 * Created on 25 Январь 2011 г., 15:36
 */

#ifndef COMMON_H
#define	COMMON_H

#define isvalidsock(s) ( ( s ) >= 0 )

typedef int SOCKET;

#define NLISTEN 5

#include <string>
#include <stdexcept>
#include <iostream>
#include <netdb.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <errno.h>

#endif	/* COMMON_H */

