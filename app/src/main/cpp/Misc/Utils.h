//
// Created by Admin on 10/23/2022.
//

#ifndef IMGUI_UNITY_TOUCH_UTILS_H
#define IMGUI_UNITY_TOUCH_UTILS_H

#include <cstdio>
#include <string>
#include <dobby.h>
#include "obfuscate.h"

#define OFC(data) AY_OBFUSCATE_KEY(data, AY_OBFUSCATE_DEFAULT_KEY)

typedef uint64_t DWORD;

static DWORD address;
bool libLoaded;

DWORD findLibrary(const char *library) {
    char filename[0xFF] = {0},
            buffer[1024] = {0};
    FILE *fp = NULL;
    DWORD _address = 0;

    sprintf(filename, OFC("/proc/self/maps"));

    fp = fopen(filename, ("rt"));
    if (fp == NULL) {
        perror(("fopen"));
        goto done;
    }

    while (fgets(buffer, sizeof(buffer), fp)) {
        if (strstr(buffer, library)) {
            address = (DWORD) strtoul(buffer, NULL, 16);
            goto done;
        }
    }

    done:

    if (fp) {
        fclose(fp);
    }

    return _address;
}

DWORD getAbsoluteAddress(const char *libraryName, DWORD relativeAddress) {
    address = findLibrary(libraryName);
    if (address == 0)
        return 0;
    return (reinterpret_cast<DWORD>(address + relativeAddress));
}

bool isLibraryLoaded(const char *libName) {
    char line[512] = {0};
    FILE *fp = fopen(OFC("/proc/self/maps"), OFC("rt"));
    if (fp != NULL) {
        while (fgets(line, sizeof(line), fp)) {
            std::string a = line;
            if (strstr(line, libName)) {
                libLoaded = true;
                return true;
            }
        }
        fclose(fp);
    }
    return false;
}

uintptr_t string2Offset(const char *c) {
    int base = 16;
    // See if this function catches all possibilities.
    // If it doesn't, the function would have to be amended
    // whenever you add a combination of architecture and
    // compiler that is not yet addressed.
    static_assert(sizeof(uintptr_t) == sizeof(unsigned long)
                  || sizeof(uintptr_t) == sizeof(unsigned long long),
                  "Please add string to handle conversion for this architecture.");

    return strtoul(c, nullptr, base);
}

#endif //IMGUI_UNITY_TOUCH_UTILS_H
