#include "pxt.h"

#include "CodalFS.h"
#include "MicroBitConfig.h"

#define MICROBIT_LOGGER 0x73
#define LOG 0x1

using namespace pxt;

namespace logger {
    CodalFS *fs = NULL;

    //% default log
    void log(String str) {

        ManagedString s = MSTR(str);

        if(fs == NULL) {
            fs = new CodalFS(uBit.flash, 128);
        }

        int fd = fs->open("log.csv", FS_APPEND|FS_WRITE|FS_CREAT);
        if(fd != MICROBIT_OK) {
            uBit.serial.printf("result %d", fd);
        }

        if(fs->write(fd,(uint8_t *)s.toCharArray(), s.length()) != s.length()) {
            // Should this fire a failed evt? How to handle in MakeCode
            uBit.serial.send("Write failed.. ");
            return;
        }

        fs->close(fd);
    }


    //%    
    void function_every(int period, int time, Action handler) {
#ifdef MICROBIT_DEFAULT_SCRATCH_PAGE // Very basic check for V2
        system_timer_event_every_us(period * 1000, MICROBIT_LOGGER, LOG);
#endif
        registerWithDal(MICROBIT_LOGGER, LOG, handler);
    }


    // Unused
    //%
    void test() {
        // Open file
        int fd = fs->open("test.txt", FS_WRITE);

        // Write file
        if(fs->write(fd,(uint8_t *)"hello!", 7) != 7) {
            return;
        }

        // Close file
        if(!fs->close(fd)) {
            return;
        }

        return;
    }

    //% delete_file
    void delete_file() {
        int fd = fs->remove("log.csv");
    }

    //% create file
    void create_file() {
        int fd = fs->open("test.txt", FS_WRITE|FS_CREAT);
    }

    //% dump log
    void dump_log() {
        // Open file
        int fd = fs->open("log.csv", FS_READ);

        uBit.serial.send("Start log dump \r\n");
        
        uint8_t buffer[100];
        while(fs->read(fd, buffer, 100) == 100) {
            uBit.serial.printf("%s",(char *) buffer);
        }

        // Print final
        uBit.serial.printf("%s \r\n",(char *) buffer);
        
        // Close file
        int i = fs->close(fd);
        if(i == MICROBIT_INVALID_PARAMETER) {
            uBit.serial.printf("MICROBIT_INVALID_PARAMETER \r\n");
        } else if(i == MICROBIT_NOT_SUPPORTED) {
            uBit.serial.printf("MICROBIT_NOT_SUPPORTED \r\n");
        } else if(i > 0) {
            uBit.serial.printf("Unknown failure \r\n");
        }
        return;
    }

    //%
    void read_file() {

        // Open file
        int fd = fs->open("test.txt", FS_READ);

        uint8_t *buffer;
        if(fs->read(fd, buffer, 100) != 0) {
            uBit.display.scroll(ManagedString("fail"));
            return;
        }

        // Close file
        if(!fs->close(fd)) {
            uBit.display.scroll(ManagedString("fail"));
            return;
        }

        uBit.display.scroll(ManagedString((char *)buffer));
            
        return;
    }
}
