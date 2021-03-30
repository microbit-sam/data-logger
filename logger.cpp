#include "pxt.h"

#include "MicroBitFileSystem.h"
#include "MicroBitConfig.h"

#define MICROBIT_LOGGER 0x73
#define LOG 0x1

using namespace pxt;

namespace logger {
    MicroBitFileSystem *fs = NULL;

    //% default log
    void log(String str) {

        ManagedString s = MSTR(str);

        if(fs == NULL) {
            fs = new MicroBitFileSystem(0, 0);
        }

        int fd = fs->open("log.csv", MB_APPEND|MB_WRITE|MB_CREAT);
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
    void function_every(int period, int timeMeasure, Action handler) {
    
        switch(timeMeasure) {
                case 0:
                        period = period * 1000;
                        break;
                case 1:
                        period = period * 1000 * 1000;
                        break;
                case 2:
                        period = period * 1000 * 1000 * 60;
                        break;
                case 3:
                        period = period * 1000 * 1000 * 60 * 60;
                        break;
        }

#ifdef MICROBIT_DEFAULT_SCRATCH_PAGE // Very basic check for V2
        system_timer_event_every_us(period , MICROBIT_LOGGER, LOG);
#endif
        registerWithDal(MICROBIT_LOGGER, LOG, handler);
    }


    // Unused
    //%
    void test() {
        // Open file
        int fd = fs->open("test.txt", MB_WRITE);

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

    //%
    void init() {
        fs = new MicroBitFileSystem(0, 0);
    }
    
    //% delete_file
    void delete_file() {
        int fd = fs->remove("log.csv");
    }

    //% create file
    void create_file() {
        int fd = fs->open("test.txt", MB_WRITE|MB_CREAT);
    }

    //% dump log
    void dump_log() {
        // Open file
        int fd = fs->open("log.csv", MB_READ);

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
        int fd = fs->open("test.txt", MB_READ);

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
