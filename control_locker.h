#ifndef CONTROL_LOCKER_H   // guardas de cabeçalho, impedem inclusões cíclicas
#define CONTROL_LOCKER_H

#include <stdio.h>
#include <string.h>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

struct termios tty;

int connectControl(void);

#endif
