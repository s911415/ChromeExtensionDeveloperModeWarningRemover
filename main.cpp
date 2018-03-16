#include <cstdlib>
#include <cstdio>
#include <iostream>
#include "func.h"

using namespace std;

int crackChrome(const string &path);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << "Usage:" << endl;
        cout << "  " << baseName(argv[0]) << " " << "chrome.dll" << endl;
        return -1;
    }

    string targetFilename = fullPath(argv[1]);
    int backupResult = backupFile(targetFilename);
    if (backupResult != 0) return backupResult;
    if (crackChrome(targetFilename) == 0) {
        return 0;
    } else {
        cerr << "Something went wrong lol." << endl;
        return -1;
    }
}


int crackChrome(const string &path) {

    unsigned char *byteData = nullptr;
    unsigned int fileSize = 0;
    readFileIntoArray(byteData, fileSize, path);

    if (byteData == nullptr) {
        showError();
        return -2;
    }

    short searchFor[] = {
            0xc3, // ret
            0xe8, -1, -1, -1, -1, // call chrome.xxxxxxxxxxxx
            0x83, 0xf8, -1, //cmp eax, xx
            0x0f, -1, -1, -1, -1, -1, //jg chrome.xxxxxxxxxxxx
            0x83, 0x3d, -1, -1, -1, -1, 0x01, // cmp dword ptr ds:[xxxx], 1
            0x0f, -1, -1, -1, -1, -1, // je chrome.xxxxxxxxxxxx
            0xe8, -1, -1, -1, -1, // call chrome.xxxxxxxxxxxx
            0x48, -1, -1, //mov xxx, xxx
    };
    const unsigned char PatchTarget[] = {0x7f};
    const unsigned int PatchTargetLen = sizeof(PatchTarget) / sizeof(PatchTarget[0]);

    unsigned int searchLen = sizeof(searchFor) / sizeof(searchFor[0]);
    long searchIdx = indexOfData(
            byteData, fileSize,
            searchFor, searchLen
    );

    if (searchIdx != -1) {
        // Make sure just one pattern matched.
        if (indexOfData(byteData, fileSize, searchFor, searchLen, (unsigned int) searchIdx + 1) >= 0)
            searchIdx = -9;
    }

    delete[] byteData;

    if (searchIdx == -1) {
        cerr << "Pattern not found" << endl;
        return -1;
    } else if (searchIdx == -9) {
        cerr << "Pattern too many" << endl;
        return -9;
    } else {

        cout << "Find pattern at 0x" << flush;
        printf("%016X", (unsigned int) searchIdx);
        cout << endl;

        //Do patch
        cout << "Patching" << endl;

        FILE *target = fopen(path.c_str(), "r+");
        if (target == nullptr) {
            showError();
            return -3;
        }

        fseek(target, (long) (searchIdx + 8), SEEK_SET);
        if (fwrite(PatchTarget, sizeof(PatchTarget[0]), PatchTargetLen, target) <= 0) {
            showError();
            fclose(target);
            return -4;
        } else {
            cout << "Patch done." << endl;
        }
        fclose(target);
        return 0;
    }
}
