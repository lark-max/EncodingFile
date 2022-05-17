#include<stdio.h>
#include<windows.h>
#include"funcs.h"


int main() {
	char buff[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, buff);
	findFile(buff);
	printf("Oh, ho, all file recovered£¡\n");
	system("pause");
	return 0;
}