#ifndef STRING_H
#define STRING_H
extern char getC(int a, int pos);
extern void itoa(char* buff,int num);
extern unsigned int strlen(char* str);
extern char strcmp(char* str1, char* str2);
extern void strcpy(char* dst,const char* src);
#endif