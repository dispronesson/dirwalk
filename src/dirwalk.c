#include "dirwalk.h"

void dirInfo(const char* dir, int l_flag, int f_flag, int d_flag, int s_flag, char** files, int* count) {
    //Экземпляры структур для работы с директорией и файлами
    DIR* d;
    struct dirent* entry;
    struct stat file_stat;
    char* path; //Массив для записи пути файла

    d = opendir(dir); //Окрытие директории
    if (d == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(d)) != NULL) { //Чтение файла за файлом
        //Пропуск текущей и родительской директорий
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        //Выделение памяти под путь файла с запасом
        size_t length = strlen(dir) + strlen(entry->d_name) + 3;
        path = malloc(length);
        if (!path) {
            perror("malloc failed");
            closedir(d);
            abort();
        }

        snprintf(path, length, "%s/%s", dir, entry->d_name); //Запись пути файла
        if (lstat(path, &file_stat) == -1) { //Получение информации о файле
            perror("lstat");
            continue;
        }
        
        if ((l_flag && S_ISLNK(file_stat.st_mode))      //Проверка установленных флагов
            || (f_flag && S_ISREG(file_stat.st_mode)) 
            || (d_flag && S_ISDIR(file_stat.st_mode))) {
            
            //Запись пути до файла с идентификацией для симлинка и директории
            if (S_ISLNK(file_stat.st_mode)) {
                strcat(path, "@");
            }
            else if (S_ISDIR(file_stat.st_mode)) {
                strcat(path, "/");
            }

            if (!s_flag) {
                printf("%s\n", path); //Вывод на экран, если сортировка не нужна
            }
            else {
                files[(*count)++] = path;
            }
        }

        if (path[strlen(path) - 1] == '/') { //Обрезка лишнего '/' в пути
            path[strlen(path) - 1] = '\0';
        }

        if (S_ISDIR(file_stat.st_mode)) { //Если файл - директория, запускается рекурсия
            dirInfo(path, l_flag, f_flag, d_flag, s_flag, files, count);
        }

        if (!s_flag) { //Очистка памяти, если сортировка не нужна
            free(path);
        }
    }
    closedir(d); //Закрытие директории
}

int counter(const char* dir) { //Данная функция аналогично dirwalk, одна высчитывает только кол-во строк путей к файлам
    DIR* d;
    struct dirent* entry;
    struct stat file_stat;
    char* path;
    int size = 0;

    d = opendir(dir);
    if (d == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(d)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        size_t length = strlen(dir) + strlen(entry->d_name) + 2;
        path = malloc(length);
        if (!path) {
            perror("malloc failed");
            closedir(d);
            abort();
        }

        snprintf(path, length, "%s/%s", dir, entry->d_name);
        if (lstat(path, &file_stat) == -1) {
            perror("lstat");
            continue;
        }

        if (S_ISDIR(file_stat.st_mode)) {
            size += counter(path);
        }

        size++;
        free(path);
    }
    closedir(d);
    return size;
}

int comparator(const void* a, const void* b) { //Компаратор для сравнения строк в массиве файлов
    return strcoll(*(const char**)a, *(const char**)b);
}