// 01010101

#include <cstdint>

#define PAGE_SIZE 128
#define PACKET_HEADER 0x55
#define PACKET_SIZE   PAGE_SIZE+4

enum class PACKET_TYPE{
    PACKET_STOP,
    PACKET_LEDTOGGLE,
    PACKET_FORWARDMSG,
    PACKET_FORWARDRAWMSG,
    PACKET_BOOTPAGE
};

enum class message_type_t{
    NORMAL = 0,
    GPS,
    SPECIAL = 0x80,
    BOOT = 0x80,
    BOOTPGM_PAGE,
    BOOTPGM_SIZE,
    RESET,
    SLEEP,
    WAKEUP,
    CHARGE,
    VOLTAGE,
    RUN,
    READUID,
    CALIB
} ;

enum class CALIB{
    CALIB_SAVE,
    CALIB_UID,
    CALIB_TURN_LEFT,
    CALIB_TURN_RIGHT,
    CALIB_STRAIGHT
};
