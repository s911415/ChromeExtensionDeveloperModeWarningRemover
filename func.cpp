//
// Created by s911415 on 2018/03/04.
//

#include "func.h"

using namespace std;

int showError() {
    cerr << "Error: " << -errno << ", "
         << strerror(errno) << endl;
    return -errno;
}

string baseName(const char *pathname) {
    char fileName[MAX_PATH_SIZE], ext[MAX_PATH_SIZE];
    _splitpath(pathname, nullptr, nullptr, fileName, ext);
    if (strlen(ext) == 0)
        return string(fileName);

    return string(fileName) + string(ext);
}

string fullPath(const char *relPath) {
    char fullPath[MAX_PATH_SIZE];
    if (_fullpath(fullPath, relPath, MAX_PATH_SIZE) == nullptr) {
        throw "Path error";
    }

    return string(fullPath);
}

void copyFile(const char *dist, const char *src) {
    FILE *infile = fopen(src, "rb");
    FILE *outfile = fopen(dist, "wb");

    if (infile == nullptr || outfile == nullptr)
        throw showError();

    const int size = 16384;
    char buffer[size];

    while (!feof(infile)) {
        size_t n = fread(buffer, 1, size, infile);
        fwrite(buffer, 1, n, outfile);
    }

    fflush(outfile);

    fclose(infile);
    fclose(outfile);
}

void copyFile(const string &dist, const string &src) {
    copyFile(dist.c_str(), src.c_str());
}

int backupFile(const string &targetFilename) {
    time_t now = time(nullptr);
    tm *ltm = localtime(&now);
    char timeStr[4 + 2 + 2 + 1 + 2 + 2 + 2 + 1];
    sprintf(
            timeStr, "%04d%02d%02d_%02d%02d%02d",
            ltm->tm_year + 1900, ltm->tm_mon + 1, ltm->tm_mday,
            ltm->tm_hour, ltm->tm_min, ltm->tm_sec
    );

    string backupFilename = targetFilename + "." + timeStr + ".bak";
    cout << "Backing up file to \"" << backupFilename << "\"" << endl;
    int backupResult = rename(targetFilename.c_str(), backupFilename.c_str());
    if (backupResult != 0) return showError();
    copyFile(targetFilename, backupFilename);

    return 0;
}

void readFileIntoArray(unsigned char *&arrPtr, unsigned int &fileSizePtr, const string &path) {
    arrPtr = nullptr;
    fileSizePtr = 0;

    ifstream inFile;
    unsigned int size = 0; // here

    inFile.open(path, ios::in | ios::binary | ios::ate);
    unsigned char *oData = nullptr;

    inFile.seekg(0, ios::end); // set the pointer to the end
    size = (unsigned int) inFile.tellg(); // get the length of the file
    inFile.seekg(0, ios::beg); // set the pointer to the beginning

    oData = new unsigned char[size];
    inFile.read(reinterpret_cast<char *>(oData), size);
    inFile.close();
    arrPtr = oData;
    fileSizePtr = size;
}


long indexOfData(
        char unsigned *&src, const unsigned int &srcLen,
        short const *searchFor, const unsigned int &searchLen,
        const unsigned int &startAt
) {
    for (unsigned int i = startAt; i < srcLen - searchLen; i++) {
        bool allMatch = true;
        for (unsigned int j = 0; j < searchLen; j++) {
            unsigned int curPos = i + j;
            if (searchFor[j] == -1) continue; //Ignore -1 value
            if (searchFor[j] != src[curPos]) {
                allMatch = false;
                break;
            }
        }
        if (allMatch) {
            return (long) i;
        }
    }

    return -1;
}

long indexOfData(
        unsigned char *&src, const unsigned int &srcLen,
        short const *searchFor, const unsigned int &searchLen
) {
    return indexOfData(src, srcLen, searchFor, searchLen, 0);
}
