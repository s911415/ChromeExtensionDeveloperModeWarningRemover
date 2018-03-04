//
// Created by s911415 on 2018/03/04.
//

#ifndef CHROMEEXTENSIONDEVELOPERMODEWARNINGREMOVER_FUNC_H
#define CHROMEEXTENSIONDEVELOPERMODEWARNINGREMOVER_FUNC_H
#define MAX_PATH_SIZE 260

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <cerrno>
#include <chrono>
#include <ctime>

using namespace std;

int showError();

string baseName(const char *pathname);

string fullPath(const char *relPath);

void copyFile(const char *dist, const char *src);

void copyFile(const string &dist, const string &src);

int backupFile(const string &targetFilename);

void readFileIntoArray(unsigned char *&arrPtr, unsigned int &fileSizePtr, const string &path);

long indexOfData(
        char unsigned *&src, const unsigned int &srcLen,
        short const *searchFor, const unsigned int &searchLen,
        const unsigned int &startAt
);

long indexOfData(
        unsigned char *&src, const unsigned int &srcLen,
        short const *searchFor, const unsigned int &searchLen
);

#endif //CHROMEEXTENSIONDEVELOPERMODEWARNINGREMOVER_FUNC_H
