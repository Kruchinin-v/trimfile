#ifndef ENV_H
#define ENV_H

#include <string>

extern const char availableUnit[10];
extern int size;
extern char unit;
extern char*  TargetFile;

long GetFileSize(std::string filename);

int MoveLog(char* TargetFile, char* TmpFile, int SkipSize, int FileSize);

int CreateNewLog(char* TargetFile, char* TmpFile);

int PrintHelp(char* progname);

int detect(int argc, char** argv);

int CalculationSize(int ResultSize, char unit);

#endif