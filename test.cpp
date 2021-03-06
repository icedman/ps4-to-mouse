#include <IOKit/hid/IOHIDManager.h>
#include <unistd.h>
#include <stdio.h>
#include "gamepad.h"
#include "emumice.hpp"

using namespace Emumice;

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

static float move_x = 0;
static float move_y = 0;

enum {
    UNKNOWN,
    CLICK,
    DRAG,
    KEY
};

enum {
    MOVE,
    FIRE,
    FIRE_A,
    FIRE_B,
    FIRE_C,
    FIRE_D,
    FIRE_E,
    FIRE_F,
    SKILL_A,
    SKILL_B,
    SKILL_C,
    UPGRADE_A,
    UPGRADE_B,
    UPGRADE_C
};

struct Mapping {
    int key;
    int x;
    int y;
    int r;
    int event;
};

const Mapping mapping[] = {
    { MOVE, 380,785, 40, KEY },
    { FIRE, 1600,860, 0, CLICK },       // main ~ cross
    { FIRE_A, 1500,940, 0, CLICK },     // minions ~ square
    { FIRE_B, 1700,745, 0, CLICK },     // turrent ~ circle
    { FIRE_C, 1360,870, 0, CLICK },     // skill A ~ left finger
    { FIRE_D, 1456,700, 0, CLICK },     // skill B ~ right finger
    { FIRE_E, 1610,610, 0, CLICK },     // special ~ right trigger
    { FIRE_F, 1730,230, 0, CLICK },     // item ~ triangle
    { SKILL_A, 975,875, 0, CLICK },     // skill ~ L
    { SKILL_B, 1090,875, 0, CLICK },     // skill ~ D
    { SKILL_C, 1215,875, 0, CLICK },     // skill ~ R
    { UPGRADE_A, 1290,788, 0, CLICK },     // upgrade
    { UPGRADE_B, 1374,623, 0, CLICK },     // upgrade
    { UPGRADE_C, 1550,540, 0, CLICK },     // upgrade
    { -1 }
};

static bool hasFireDir = false;
static int cad = 0;
const char *cad1 = "11111111";
const char *cad2 = "11011011";
const char *cad3 = "10101010";

const char* get_cadence(int k) {
    if (k < 0) k *= -1;
    k = k / 10;
    if (k <= 1) return cad3;
    if (k == 2) return cad2;
    return cad1;
}

void fire(int x, int y, bool hasDir) {
    if (hasFireDir && hasDir) {
        // drag!
        int radius = 80;
        int tx = x + (radius * (RAnalog_X - 128) / 128);
        int ty = y + (radius * (RAnalog_Y - 128) / 128);
        printf("Fire Direction:%d %d %d %d\n", x, y, tx, ty);
        mouse_drag(x, y, tx, ty);
        hasFireDir = false;
    } else {
        mouse_set(x, y);
        mouse_click(btn_left);
        printf("Fire:%d %d\n", x, y);
    }
}

static void send()
{
    cad++;
    if (cad >= 8) cad = 0;

    const Mapping *m;
    m = &mapping[MOVE];

    /**************************
       MOVE
    **************************/
    if (move_x == 0 && move_y == 0) {
        move_x = m->x;
        move_y = m->y;
    }

    int tx = 0;
    int ty = 0;
    if (LAnalog_X != 128 || LAnalog_Y != 128) {
        tx = m->x + (m->r * (LAnalog_X - 128) / 128);
        ty = m->y + (m->r * (LAnalog_Y - 128) / 128);
    } else {
        tx = m->x;
        ty = m->y;
    }

    float mx = (tx - move_x)/4;
    float my = (ty - move_y)/4;

    move_x += mx;
    move_y += my;

    float dcx = move_x - m->x;
    float dcy = move_y - m->y;
    float dc = dcx * dcx + dcy * dcy;

    if (dc > 10) {
        if (m->event == KEY) {
            if (dcx * dcx > 10) {
                if (get_cadence(dcx)[cad] == '1') {
                    printf("X:%f\n", dcx);
                    key_type(keyCodeForKeyString(dcx < 0 ? "a":"d"));
                }
            }
            if (dcy * dcy > 10) {
                if (get_cadence(dcy)[cad] == '1') {
                    printf("Y:%f\n", dcy);
                    key_type(keyCodeForKeyString(dcy < 0 ? "w":"s"));
                }
            }
        } else if (m->event == DRAG) {
            mouse_drag(move_x, move_y, move_x + mx/4, move_y + my/4);
        } else {
            mouse_set(move_x, move_y);
            mouse_click(btn_left);
        }
        // printf("%f %d %d - MOVE_X:%f MOVE_Y:%f\n", dc, LAnalog_X, LAnalog_Y, move_x, move_y);
    }


    /**************************
       FIRE
    **************************/
    hasFireDir = false;
    if (RAnalog_X != 128 || RAnalog_Y != 128) {
        if ((RAnalog_X > 200 || RAnalog_X < 55) ||
            (RAnalog_Y > 200 || RAnalog_Y < 55)) {
            hasFireDir = true;
            // printf("RX:%d RY:%d\n", RAnalog_X, RAnalog_Y);
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
        m = NULL;
        if (LButton == 6) {
            m = &mapping[SKILL_A];
        }
        if (LButton == 4) {
            m = &mapping[SKILL_B];
        }
        if (LButton == 2) {
            m = &mapping[SKILL_C];
        }
        if (m) {
            // mouse_set(m->x, m->y);
            // mouse_click(btn_left);
            fire(m->x, m->y, true);
        }

        if (LButton == 0) {
            m = &mapping[UPGRADE_A];
            fire(m->x, m->y, false);
            m = &mapping[UPGRADE_B];
            fire(m->x, m->y, false);
            m = &mapping[UPGRADE_C];
            fire(m->x, m->y, false);

        }
    }
    if (RButton != 0) {
        /*
            1 - SQUARE
            2 - CROSS
            3 - CIRCLE
            4 - TRIANGLE
        */
        // printf("R:%d\n", RButton);

        m = NULL;
        if (RButton == 2) {
            m = &mapping[FIRE];
        }
        if (RButton == 1) {
            m = &mapping[FIRE_A];
        }
        if (RButton == 3) {
            m = &mapping[FIRE_B];
        }
        if (RButton == 4) {
            m = &mapping[FIRE_F];
        }

        if (m) {
            // mouse_set(m->x, m->y);
            // mouse_click(btn_left);
            fire(m->x, m->y, true);
        }
    }

    m = NULL;
    if (LFButton) {
        printf("LF\n");
        m = &mapping[FIRE_C];
        if (m) {
            fire(m->x, m->y, true);
        }
    }

    m = NULL;
    if (RFButton) {
        printf("RF\n");
        m = &mapping[FIRE_D];
        if (m) {
            fire(m->x, m->y, true);
        }
    }

    m = NULL;
    if (LTButton) {
        // printf("LT:%d\n", LTButton);
    }
    m = NULL;
    if (RTButton) {
        // printf("RT:%d\n", RTButton);
        m = &mapping[FIRE_E];
        if (m) {
            fire(m->x, m->y, true);
        }
    }
}

static void callback(int type, int page, int usage, int value)
{
    if (page == 65280) {
        send();
        return;
    }

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
    // if (2 == type && 7 == page && 41 == usage && 0 == value) {
    //     CFRunLoopStop(CFRunLoopGetCurrent());
    // }
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
