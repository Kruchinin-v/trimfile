#include <fstream>
#include <iostream>
#include <ctime>
#include <cstring>
#include <sstream>
#include "env.h"

const char availableUnit[10] = {'b', 'k', 'm', 'g'};
int size = -1;
int ResultSize; // итоговый размер лога в байтах
int SkipSize; // размер, который нужно будет пропустить
char unit = '\0';
char*  TargetFile;


int main(int argc, char** argv) {

    int ans = detect(argc, argv);
    if ( ans == -1)
        return -1;
    else if ( ans == 33) {
        return 0;
    }


    std::time_t ttimestamp = std::time(0);  // timestamp
    std::stringstream tempFile;
    tempFile << "/tmp/temp-" << ttimestamp;

    std::string  tempfile = tempFile.str();

    char* TempFile = &tempfile[0u];

    int FileSize = GetFileSize(TargetFile);
    ResultSize = CalculationSize(size, unit);
    SkipSize = FileSize - ResultSize;

    if (SkipSize < 0 ) 
    {
        std::cerr << "Размер файла меньше необходимого размера" << std::endl;
        return 1;
    }


    std::cout << "Путь до файла: " << TargetFile << std::endl;
    
    switch (unit)
    {
    case 'b':
       std::cout << "Текущий размер файла: "  << FileSize << "b"   << std::endl;
       std::cout << "Итоговый размер файла: " << ResultSize << "b" << std::endl;
       std::cout << "Будет очищенно: "        << SkipSize << "b"   << std::endl;
        break;
    case 'k': 
       std::cout << "Текущий размер файла: "  << FileSize / 1024   << "k" << std::endl;
       std::cout << "Итоговый размер файла: " << ResultSize / 1024 << "k" << std::endl;
       std::cout << "Будет очищенно: "        << SkipSize / 1024   << "k" << std::endl;
        break;
    case 'm': 
       std::cout << "Текущий размер файла: "  << FileSize / 1024 / 1024   << "m" << std::endl;
       std::cout << "Итоговый размер файла: " << ResultSize / 1024 / 1024 << "m" << std::endl;
       std::cout << "Будет очищенно: "        << SkipSize / 1024 / 1024   << "m" << std::endl;
        break;
    case 'g': 
       std::cout << "Текущий размер файла: "  << FileSize / 1024 / 1024 / 1024   << "g" << std::endl;
       std::cout << "Итоговый размер файла: " << ResultSize / 1024 / 1024 / 1024 << "g" << std::endl;
       std::cout << "Будет очищенно: "        << SkipSize / 1024 / 1024 / 1024   << "m" << std::endl;
        break;
    default:
        break;
    }
    
    char is_right[] = "";

    std::cout << std::endl << "Все верно? (y/n) ";
    std::cin >> is_right;

    if ( (strcmp(is_right, "y") != 0) && (strcmp(is_right, "Y") != 0)) {
        std::cout << "Отмена"<< std::endl;
        return 0;
    }

    MoveLog(TargetFile, TempFile, SkipSize, FileSize);

    CreateNewLog(TargetFile, TempFile);

    remove(TempFile);

    std::cout << "Successful"<< std::endl;

    return 0;
}