#include "dirwalk.h"

void dirInfo(const char* dir, int l_flag, int f_flag, int d_flag, char** files, int* count) {
    //Экземпляры структур для работы с директорией и файлами
    DIR* d;
    struct dirent* entry;
    struct stat file_stat;
    char path[1024]; //Массив для записи пути файла

    d = opendir(dir); //Окрытие директории
    if (d == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(d)) != NULL) { //Чтение файла за файлом
        //Пропуск текущей и родительской директорий
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(path, sizeof(path), "%s/%s", dir, entry->d_name); //Запись пути файла
        if (lstat(path, &file_stat) == -1) { //Получение информации о файле
            perror("lstat");
            continue;
        }
        
        if ((l_flag && S_ISLNK(file_stat.st_mode))      //Проверка установленных флагов
            || (f_flag && S_ISREG(file_stat.st_mode)) 
            || (d_flag && S_ISDIR(file_stat.st_mode))) {

            char* temp = malloc((strlen(path) + 2)); //Выделение памяти под путь до файла
            if (!temp) {
                perror("malloc failed");
                closedir(d);
                abort();
            }
            
            //Запись пути до файла с идентификацией для симлинка, директории и файла
            if (S_ISLNK(file_stat.st_mode)) {
                strcpy(temp, path);
                strcat(temp, "@");
            }
            else if (S_ISREG(file_stat.st_mode)) {
                strcpy(temp, path);
            }
            else {
                strcpy(temp, path);
                strcat(temp, "/");
            }

            files[(*count)++] = temp; //Запись пути в массив файлов
        }

        if (S_ISDIR(file_stat.st_mode)) { //Если файл - директория, запускается рекурсия
            dirInfo(path, l_flag, f_flag, d_flag, files, count);
        }
    }
    closedir(d); //Закрытие директории
}

int comparator(const void* a, const void* b) { //Компаратор для сравнения строк в массиве файлов
    return strcoll(*(const char**)a, *(const char**)b);
}