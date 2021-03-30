/**
 * Use this file to define custom functions and blocks.
 * Read more at https://makecode.microbit.org/blocks/custom
 */

enum TimeMeasure
{
    //% block="milliseconds"
    Millisecond = 0,
    //% block="seconds"
    Second = 1,
    //% block="minutes"
    Minute = 2,
    //% block="hours"
    Hour = 3
}

namespace logger {
    let timestamp_enabled: boolean = false;

    //% block blockId=log_string block="log %data"
    //% shim=logger::log
    //% data.shadowOptions.toString=true
    export function log(data: string): void {
        return;
    }
    
    let columns: string[] = []
    let data: string[] = [];
    let headers_stale: boolean = true;
    let timestamp = false;

    function process_key(key: string): void {
        if(columns.indexOf(key) == -1) {
                columns.push(key);
                headers_stale = true;
        }
    }

    function process_key_value(key: string, value: string): void {
        process_key(key)
        data[columns.indexOf(key)] = value;
    }

    //% blockId=log_set_key block="set log columns %key1 ||, %key2 |, %key3 |, %key4 |, %key5 | include timestamp %timestamp"
    //% expandableArgumentMode="enabled"
    //% inlineInputMode=inline
    //% key1.shadowOptions.toString=true key2.shadowOptions.toString=true key3.shadowOptions.toString=true key4.shadowOptions.toString=true key5.shadowOptions.toString=true
    export function log_set_key(key1: string = "",
                                  key2: string = "",
                                  key3: string = "",
                                  key4: string = "",
                                  key5: string = "",
                                  timestamp: boolean = false
                                 ): void {
            process_key(key1)
            process_key(key2)
            process_key(key3)
            process_key(key4)
            process_key(key5)
            timestamp_enabled = timestamp
    }


    //% blockId=log_key_value block="log data %key1 to %value1 ||, %key2 to %value2 |, %key3 to %value3 |, %key4 to %value4 |, %key5 to %value5"
    //% expandableArgumentMode="enabled"
    //% inlineInputMode=inline
    //% key1.shadowOptions.toString=true key2.shadowOptions.toString=true key3.shadowOptions.toString=true key4.shadowOptions.toString=true key5.shadowOptions.toString=true
    //% value1.shadowOptions.toString=true value2.shadowOptions.toString=true value3.shadowOptions.toString=true value4.shadowOptions.toString=true value5.shadowOptions.toString=true
    export function log_key_value(key1: string = "", value1: string = "",  
                                  key2: string = "", value2: string = "", 
                                  key3: string = "", value3: string = "", 
                                  key4: string = "", value4: string = "", 
                                  key5: string = "", value5: string = ""
                                 ): void {
        data = []

        // Key 1
        process_key_value(key1, value1)
        process_key_value(key2, value2)
        process_key_value(key3, value3)
        process_key_value(key4, value4)
        process_key_value(key5, value5)

        if(headers_stale) {
            log("headers updated \r\n")

            let s: string = timestamp_enabled ? "timestamp," : ""
            s += columns.join(',') + "\r\n"
            log(s)

            headers_stale = false
        }
            
        let d: string = timestamp_enabled ? convertToText(control.millis()) : ""
        d += data.join(',') + "\r\n"
        log(d)

    }
    
    //% block
    //% blockId=dump_log block="dump log to serial" blockGap=8
    //% shim=logger::dump_log
    export function dump_log(): void {
    }
    
    //% shim=logger::delete_file
    function delete_file(): void {
    }

    //% block
    //% blockId=delete_log block="clear log" blockGap=8
    export function delete_log(): void {
            headers_stale = true;
            delete_file();
    }

    //% block
    //% blockId=function_every block="every %period %timeMeasure" blockGap=8
    //% shim=logger::function_every
    export function function_every(period: number, timeMeasure: TimeMeasure, handler: Action): void{
        return;
    }

    //% block
    //% blockId=enter_deep_sleep block="enter deep sleep" blockGap=8
    export function enter_deep_sleep(): void{
        return;
    }


}
