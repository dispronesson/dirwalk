#include "dirwalk.h"

int main(int argc, char* argv[]) {
    int l_flag = 0, f_flag = 0, d_flag = 0, s_flag = 0; //Флаги для сортировки
    char* dr = "."; //Просмотр текущей директории по умолчанию
    int firstDir = (argc > 1 && argv[1][0] != '-'); //Проверка первого символа второго аргумента из командной строки
    char** files = NULL; //Массив для файлов
    int count = 0; //Счетчик элементов в массиве файлов

    int opt;
    while ((opt = getopt(argc - firstDir, argv + firstDir, "lfds")) != -1) { //Чтение флагов
        switch (opt) {
            case 'l': l_flag = 1; break;
            case 'f': f_flag = 1; break;
            case 'd': d_flag = 1; break;
            case 's': s_flag = 1; break;
            default: exit(EXIT_FAILURE);
        }
    }

    if (!l_flag && !f_flag && !d_flag) { //Проверка флагов
        l_flag = f_flag = d_flag = 1;
    }

    if (firstDir) { //Если второй аргумент НЕ является флагом
        dr = argv[1]; //Установить путь к директории
    }
    else if (optind < argc) { //Если индекс первого необработанного аргумента меньше количества всех аргументов
        dr = argv[optind]; //Установить путь к директории
    }

    if (dr[strlen(dr) - 1] == '/') { //Обрезка лишнего '/' в пути
        dr[strlen(dr) - 1] = '\0';
    }

    if (s_flag) {
        count = counter(dr);
        files = malloc(count * sizeof(char*));
    }

    count = 0;
    dirInfo(dr, l_flag, f_flag, d_flag, s_flag, files, &count); //Вызов функции

    if (s_flag) { //Если флаг '-s' установлен
        setlocale(LC_COLLATE, ""); //Установка локали для сортировки
        qsort(files, count, sizeof(char*), comparator); //Сортировка массива файлов
        for (int i = 0; i < count; i++) { //Вывод массива файлов на консоль
            printf("%s\n", files[i]);
            free(files[i]); //Очистка памяти
        }
        free(files);
    }

    return 0;
}