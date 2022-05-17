#pragma once
#pragma warning(disable : 4996)

// 在指定路径下递归寻找所有文件
void findFile(char* pathName);

// 解密操作
void deCode(char* pathFile);
int dropEXE(char* fpSrc, char* fpDst);
