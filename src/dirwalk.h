#ifndef DIRWALK_H
#define DIRWALK_H

#define _POSIX_C_SOURCE 200112L //Использование POSIX-совместимых функций и возможностей

#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <locale.h>
#include <getopt.h>

//Объявление прототипов функций
void dirInfo(const char* dir, int l_flag, int f_flag, int d_flag, int s_flag, char** files, int* count);

int counter(const char* dir);

int comparator(const void* a, const void* b);

#endif //DIRWALK_H