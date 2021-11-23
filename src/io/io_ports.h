#pragma once
#ifdef __cplusplus
extern "C" {
#endif
char read_port(unsigned short port);

void write_port(unsigned short port, unsigned char data);

#ifdef __cplusplus
}
#endif