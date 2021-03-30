#include "pxt.h"

#include "MicroBitFileSystem.h"
#include "MicroBitConfig.h"

using namespace pxt;

namespace logger {
    MicroBitFileSystem fs;

    #define MBFS_START ((FLASH_PROGRAM_END / MICROBIT_CODEPAGESIZE) * MICROBIT_CODEPAGESIZE) + MICROBIT_CODEPAGESIZE
    #define MBFS_LEN   MICROBIT_DEFAULT_SCRATCH_PAGE - (MBFS_START / MICROBIT_CODEPAGESIZE)

    //%
    int test() {
        // Open file
        int fd = fs.open("test.txt", MB_WRITE);

        // Write file
        if(fs.write(fd, "hello!", 7) != 7) {
            return MICROBIT_ERROR;
        }

        // Close file
        if(!fs.close(fd)) {
            return MICROBIT_ERROR;
        }

        return MICROBIT_OK;
    }

    //%
    void init() {
        fs = new MicroBitFileSystem(MBFS_START, MBFS_LEN);
    }
    
    //% create file
    void create_file() {
        int fd = f.open("test.txt", MB_WRITE|MB_CREAT);V
    }
}