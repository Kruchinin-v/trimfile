#include <iostream>  // cout
#include <fcntl.h>  // for stat
#include <sys/types.h>  // for stat
#include <sys/stat.h>  // for stat
#include <sstream>  // for stringstream
#include <map>  // map
#include <unistd.h> //access()
#include <string.h>
#include <fstream>
#include <algorithm> // find()
#include "env.h"

    
/**
 * Получить размер файла в байтах
 * */
long GetFileSize(std::string filename) {
        struct stat stat_buf;
        int rc = stat(filename.c_str(), &stat_buf);
        return rc == 0 ? stat_buf.st_size : -1;
}

/**
 * переместить лог, пропуская то, что нужно удалить в mb
*/
int MoveLog(char* TargetFile, char* TmpFile, int SkipSize, int FileSize) 
{

    // const int SIZE = 5000;
    const int SIZE = 1048576;
    const int sz = 1048576;  //по сколько байт считывать
    char buf[SIZE];
    int length;

    std::fstream infile; //файл источник
    std::fstream outfile; //программа


    infile.open(TargetFile, std::ios::binary | std::ios::in); //открытие файла источника

    if (!infile) {
        std::cerr << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
        std::cerr << "File '" << TargetFile << "' cant't be oppened." << std::endl;
        std::cerr << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
        return 1;
    }//if

    outfile.open(TmpFile, std::ios::binary | std::ios::out); //открытие файла для записи программы
    //проверка создания файла шапки
    if (!outfile) 
    {
        std::cerr << "~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
        std::cerr << "Error creating "<< TmpFile <<" file!" << std::endl;
        std::cerr << "~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
        return 1;
    }//if
    
    infile.seekg(SkipSize, std::ios::beg);

    length = FileSize - SkipSize;
    
    while (length >= sz) //считывать, пока хватает длины необходимого размера 
    {
        infile.read(buf, sz);
        outfile.write(buf, sz);
        length -= sz;
    }

    if (length > 0) //если осталась длина, считать и ее
    {
        infile.read(buf, length);
        outfile.write(buf, length);
    }

    outfile.close();
    infile.close();
    return 0;
    
}

/**
 * Пересносит обратно обрезанный лог
 * */
int CreateNewLog(char* TargetFile, char* TmpFile) 
{

    // const int SIZE = 5000;
    const int SIZE = 1048576;
    const int sz = 1048576;  //по сколько байт считывать
    char buf[SIZE];
    int length;

    int FileSize = GetFileSize(TmpFile);

    std::fstream infile; //файл источник
    std::fstream outfile; //программа


    infile.open(TmpFile, std::ios::binary | std::ios::in); //открытие файла источника

    if (!infile) {
        std::cerr << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
        std::cerr << "File '" << TargetFile << "' cant't be oppened." << std::endl;
        std::cerr << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
        return 1;
    }//if

    outfile.open(TargetFile, std::ios::binary | std::ios::out); //открытие файла для записи программы
    //проверка создания файла шапки
    if (!outfile) 
    {
        std::cerr << "~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
        std::cerr << "Error creating "<< TmpFile <<" file!" << std::endl;
        std::cerr << "~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
        return 1;
    }//if
    

    length = FileSize;
    
    while (length >= sz) //считывать, пока хватает длины необходимого размера 
    {
        infile.read(buf, sz);
        outfile.write(buf, sz);
        length -= sz;
    }

    if (length > 0) //если осталась длина, считать и ее
    {
        infile.read(buf, length);
        outfile.write(buf, length);
    }

    infile.close();
    outfile.close();
    return 0;
    
}


/**
 * вывод подсказки
 * */
int PrintHelp(char* progname) {
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<std::endl;
    std::cout << "Обрезает файл до указаного в мегабайтах размера"<<std::endl;
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<std::endl;
    std::cout << "usage: " << progname << " --size <size> --file <path>"<< std::endl << std::endl;
    std::cout << "      --siz,-s size<unit> -  размер, до которого необходимо обрезать файл." <<std::endl;
    std::cout << "                             с указанием единицы измерения" <<std::endl;
    std::cout << "                             unit - b,k,m,g" <<std::endl;
    std::cout << "      --file,-f path      -  путь до файла, который нужно обрезать" <<std::endl;
    std::cout << "      --help,-h           -  вывод подсказки" <<std::endl;
    std::cout << "Примеры:" <<std::endl;
    std::cout << "./trimfile -s 10m -f file" <<std::endl;
    std::cout << "./trimfile -s 1g -f file" <<std::endl;
    std::cout << "./trimfile -s 1020b -f file" <<std::endl;
    std::cout << "./trimfile --size 512m --file ./log/file" <<std::endl;
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<std::endl;
    return 0;
}

char * del_char(const char * src, char * res, char c) {
    char *tmp = res;
    do
        if (*src != c)
            *res++ = *src;
    while (*src++);
    return tmp;
}

int detect(int argc, char** argv) {

    std::map <std::string, int> arg;
	arg["--size"] = 0; // размер 
	arg["-s"] = 0; // размер 
	arg["--file"] = 1; // путь до файла
	arg["-f"] = 1; // путь до файла
    arg["--help"] = 2; // вывести подсказку
    arg["-h"] = 2; // вывести подсказку


    int i = 1;
    std::map <std::string, int> ::iterator it, it_2;
    while (i < argc) {
        it = arg.find(argv[i]);

        switch (it->second) {
            // --size
            case 0: {
                // Если размер не указан(больше нет аргументов)
                if (argc-1 == i) {
                    std::cerr << "Не указан размер" << '\n';    
                    PrintHelp(argv[0]);
                    return -1;
                }

                char* allsize = argv[i+1];


                unit = allsize[strlen(allsize)-1];

                char buf[256];
                int countUnit = strlen(availableUnit);

                char* only_size = del_char(allsize, buf, unit);
                // возвращает 0 если там не число
                size = strtol(only_size, NULL, 10);

                if ( (std::distance(availableUnit, std::find(availableUnit, availableUnit + countUnit, unit))) >= countUnit) {
                    std::cerr << "Не верно указана еденица измерения: " << unit << std::endl;
                    std::cerr << "Возможные варианты: " << availableUnit << std::endl;
                    return -1;
                }

                // соотвественно проверка
                if ( size < 0 || (size == 0 && only_size != std::string("0")) ) {
                    std::cerr << "Не верно указан размер: " << only_size << '\n';
                    return -1;
                }

                i++;
                break;
            } 
            
            // --file
            case 1:  {
                if (argc-1 == i) {
                    std::cerr << "Не указан путь к файлу" << std::endl;    
                    PrintHelp(argv[0]);
                    return -1;
                }
                TargetFile = argv[i+1];

                if (access(TargetFile, 0)) {
                    std::cerr << "Файл " << TargetFile << " не найден" << std::endl;    
                    return -1;
                }

                i++;
                break;
            }
            
            // --help
            case 2:  {
                PrintHelp(argv[0]);
                return 33;
                break;
            }
            
            default:
                std::cerr << "Верный аргумент: "<< argv[i] << std::endl;
                return -1;
                break;
        }//switch

        i++;
    }//while


   
    if (size == -1 || TargetFile == "") {
        PrintHelp(argv[0]);
        return -1;
    }


    return 0;
}

/**
 * Расчитывает итоговый размер лока в байтах
 * ResultSize: указаный через параметр размер
 * unit: указаная через параметр единица измерения
*/
int CalculationSize(int ResultSize, char unit) {
    switch (unit) {
        case 'b':
            size = ResultSize;
            break;
        case 'k':
            size = ResultSize * 1024;
            break;
        case 'm':
            size = ResultSize * 1024 * 1024;
            break;
        case 'g':
            size = ResultSize * 1024 * 1024 * 1024;
            break;
    }

    return size;
}

