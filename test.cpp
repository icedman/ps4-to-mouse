#include <IOKit/hid/IOHIDManager.h>
#include <unistd.h>
#include <stdio.h>
#include "gamepad.h"

static int LAnalog_X = 128;
static int LAnalog_Y = 128;
static int RAnalog_X = 128;
static int RAnalog_Y = 128;

static int LButton = 8;
static int RButton = 0;
static int LFButton = 0;
static int RFButton = 0;
static int LTButton = 0;
static int RTButton = 0;

enum {
    _unknown,
    _click,
    _move,
    _drag
};

struct Mapping {
    int key;
    int ax;
    int ay;
    int bx;
    int by;
    int event;
};

const Mapping mapping[] = {
    { 'M', 380,785, 0,0, _click },
    { -1 }
};

static void send()
{
    if (LAnalog_X != 128 || LAnalog_Y != 128) {
        printf("LX:%d LY:%d\n", -128 + LAnalog_X, -128 + LAnalog_Y);
        // key - 'M'
    }
    if (RAnalog_X != 128 || RAnalog_Y != 128) {
        if ((RAnalog_X > 200 || RAnalog_X < 55) ||
            (RAnalog_Y > 200 || RAnalog_Y < 55)) {
            printf("RX:%d RY:%d\n", RAnalog_X, RAnalog_Y);
        }
    }
    if (LButton != 8) {
        /*
            0 - UP
            2 - RIGHT
            4 - DOWN
            6 - LEFT
        */
        printf("L:%d\n", LButton);
    }
    if (RButton != 0) {
        /*
            1 - SQUARE
            2 - CROSS
            3 - CIRCLE
            4 - TRIANGLE
        */
        printf("R:%d\n", RButton);
    }
    if (LFButton) {
        printf("LF\n");
    }
    if (RFButton) {
        printf("RF\n");
    }
    if (LTButton) {
        printf("LT:%d\n", LTButton);
    }
    if (RTButton) {
        printf("RT:%d\n", RTButton);
    }
}

static void callback(int type, int page, int usage, int value)
{
    if (page == 65280) return;

    if (usage == 48) {
        LAnalog_X = value;
    }
    if (usage == 49) {
        LAnalog_Y = value;
    }
    if (usage == 50) {
        RAnalog_X = value;
    }
    if (usage == 53) {
        RAnalog_Y = value;
    }
    if (usage == 57) {
        LButton = value;
    }
    if (usage == 1 || usage == 2 || usage == 3 || usage == 4) {
        RButton = value ? usage : 0;
    }
    if (usage == 5) {
        LFButton = value;
    }
    if (usage == 6) {
        RFButton = value;
    }
    if (usage == 51) {
        LTButton = value;
    }
    if (usage == 52) {
        RTButton = value;
    }

    send();

    // printf("type=%d, page=%d, usage=%d, value=%d\n", type, page, usage, value);

    /* end main loop if push esc key */
    if (2 == type && 7 == page && 41 == usage && 0 == value) {
        CFRunLoopStop(CFRunLoopGetCurrent());
    }
}

int main()
{
    void* ctx;

    /* initialize gamepad */
    ctx = gamepad_init(1, 1, 0);
    if (!ctx) {
        puts("init failed");
        return -1;
    }

    /* set callback */
    gamepad_set_callback(ctx, callback);

    CFRunLoopRun();

    /* terminate gamepad */
    gamepad_term(ctx);
    return 0;
}
