#ifndef KERNELIO_H
#define KERNELIO_H

char getc(void);
void putc(char c);
void std_putc(char c);

void puts(const char * s);
void std_puts(const char * s);

void gets(char * buf, int buflen);

void printf(const char * fmt, ...);

#endif
