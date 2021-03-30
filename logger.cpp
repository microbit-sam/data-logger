#include "pxt.h"

#include "MicroBitFileSystem.h"
#include "MicroBitConfig.h"

#ifndef MICROBIT_DEFAULT_SCRATCH_PAGE
#define MICROBIT_DEFAULT_SCRATCH_PAGE DEFAULT_SCRATCH_PAGE
#endif

#ifndef MICROBIT_CODEPAGESIZE
#define MICROBIT_CODEPAGESIZE PAGE_SIZE
#endif

#define MBFS_START ((FLASH_PROGRAM_END / MICROBIT_CODEPAGESIZE) * MICROBIT_CODEPAGESIZE) + MICROBIT_CODEPAGESIZE
#define MBFS_LEN   MICROBIT_DEFAULT_SCRATCH_PAGE - (MBFS_START / MICROBIT_CODEPAGESIZE)

using namespace pxt;

namespace logger {
    MicroBitFileSystem *fs;

    //% blockId=test_filesystem block="test file write" blockGap=8
    int test() {
        // Open file
        int fd = fs->open("test.txt", MB_WRITE);

        // Write file
        if(fs->write(fd,(uint8_t *)"hello!", 7) != 7) {
            return MICROBIT_NO_RESOURCES;
        }

        // Close file
        if(!fs->close(fd)) {
            return MICROBIT_NO_RESOURCES;
        }

        return MICROBIT_OK;
    }

    //%
    void init() {
        fs = new MicroBitFileSystem(MBFS_START, MBFS_LEN);
    }
    
    //% create file
    void create_file() {
        int fd = fs->open("test.txt", MB_WRITE|MB_CREAT);
    }
}
