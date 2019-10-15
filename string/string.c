#include "string.h"
#include "../screen/screen.h"
#include "../math/math.h"
char getC(int a, int pos){
    int po = pow(10,pos);
    if(po > a){
        return -1;
    }
    return (a / po) % 10;
}
void itoa(char* buff,int num){
int i = 10;
char* tail = buff+11;
char* head = buff;
 for(i = 10; i >= 0; i--){
        char a = getC(num,i);
        if(a == -1){
            *(tail--) = 0;
            continue;
        }else{
            a += 48;
            *(head++) = a;
        }
    }
}
unsigned int strlen(char* str){
	unsigned int size = 0;
	while(str[size++] != 0);
	return size;
}
char strcmp(char* str1, char* str2){
	unsigned int len1 = strlen(str1);
	unsigned int len2 = strlen(str2);
	if(len1 != len2){
		return (len1 < len2) ? -1 : 1;
	}
	int i = 0;
	for(i = 0; i < len1; i++){
		if(str1[i] > str2[i]){
			return 1;
		}else if(str1[i] < str2[i]){
			return -1;
		}
	}
	return 0;
}
void strcpy(char* dst,const char* src)
{
	char* current = (char*)src;
	char* dest = dst;
	while(1)
	{
		*dest++ = *current;
		if(*current == '\0'){
			break;
		}
		current++;
	}
}