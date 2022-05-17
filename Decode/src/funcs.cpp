#include"funcs.h"
#include<stdio.h>
#include<windows.h>

void findFile(char* pathName) {
	char currFile[MAX_PATH]; // 暂时存储每个文件名
	memset(currFile, 0, MAX_PATH);
	sprintf(currFile, "%s\\*.*", pathName);
	_WIN32_FIND_DATAA findData;
	HANDLE hFile = FindFirstFile(currFile, &findData);
	if (hFile == INVALID_HANDLE_VALUE)
		return;

	int ret = 0;
	while (1) {
		memset(currFile, 0, MAX_PATH);
		sprintf(currFile, "%s\\%s", pathName, findData.cFileName);
		// 检查文件属性--文件还是文件夹？
		if (findData.cFileName[0] == '.'); //对特殊文件夹不进行处理
		else if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))  //如果是普通文件夹，递归调用findFile函数
			findFile(currFile);
		else //否则，处理当前文件(安全起见这里仅打印文件名)
			deCode(currFile);

		ret = FindNextFile(hFile, &findData);
		if (!ret)
			break;
	}
}

void deCode(char* pathFile) {
	//打开待解密文件，创建解密后文件
	FILE* fpSrc = fopen(pathFile, "rb"); //只读字节流
	char buff[MAX_PATH];
	memset(buff, 0, MAX_PATH);
	//去掉.exe后缀
	if (dropEXE(pathFile, buff) != 0) //如果返回非0，则说明当前文件非.exe结尾，不进行解密处理
		return;
	FILE* fpDst = fopen(buff, "wb"); //只写字节流
	if (fpSrc == NULL || fpDst == NULL)
		return;
	//以单个字节循环读取待解密文件内容，并写入解密文件中
	char currByte;
	while (1) {
		int count = fread(&currByte, 1, 1, fpSrc);
		if (count < 1) //没读到
			break;
		currByte ^= 0x66; //简单异或解密法
		fwrite(&currByte, 1, 1, fpDst); //写入解密文件
	}
	fclose(fpSrc);
	fclose(fpDst);
	printf("Congrat! %s recoveried successfully!\n", buff);
	remove(pathFile);
}

int dropEXE(char* fpSrc, char* fpDst) {
	int n = strlen(fpSrc);
	if (n < 4)
		return 1;
	char check[5];
	for (int i = 0; i < 4; ++i)
		check[i] = *(fpSrc + n - 4 + i);
	check[4] = '\0';
	if (strcmp(check, ".exe") != 0) {
		printf("sorry, %s is not a .exe file, recovery failed!\n", fpSrc);
		return 1;
	}
	strncpy(fpDst, fpSrc, n - 4);
	return 0;
}