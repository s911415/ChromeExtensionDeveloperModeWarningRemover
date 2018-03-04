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
            0x4c, 0x89, 0xe0, //mov rax, r12
            0x48, 0x81, 0xc4, 0x80, 0x00, 0x00, 0x00, // add rsp, 80
            0x5b, // pop rbx
            0x5f, // pop rdi
            0x5e, // pop rsi
            0x41, 0x5c, // pop r12
            0x41, 0x5d, // pop r13
            0x41, 0x5e, // pop r14
            0x41, 0x5f, // pop r15
            0xc3, // ret
            0xe8, -1, -1, -1, -1, // call chrome.xxxxxxxxxxxx
            0x83, 0xf8, -1, //cmp eax, xx
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
            searchIdx = -1;
    }

    delete[] byteData;

    if (searchIdx == -1) {
        cerr << "Pattern not found" << endl;
        return -1;
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

        fseek(target, (long) (searchIdx + searchLen - 1), SEEK_SET);
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
