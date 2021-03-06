#include <cassert>
#include <CoreFoundation/CoreFoundation.h>
#include <ApplicationServices/ApplicationServices.h>


#include "emumice.hpp"

//------------------------------------------------------------------------------

namespace Emumice {

//------------------------------------------------------------------------------

namespace Detail {

// this function posts a mouse event in a specified mouse location:
void mouse_event(CGMouseButton button, CGEventType type, CGPoint location)
{
    CGEventRef event = CGEventCreateMouseEvent(NULL, type, location, button);
    CGEventSetType(event, type);
    CGEventPost(kCGHIDEventTap, event);
    CFRelease(event);
}

}; // namespace Detail

//------------------------------------------------------------------------------

// returns current mouse location:
Point get_mouse_loc()
{
    CGEventRef event = CGEventCreate(NULL);
    CGPoint cursor = CGEventGetLocation(event);
    CFRelease(event);
    return Point(cursor.x, cursor.y);
}

//------------------------------------------------------------------------------

// sets mouse location at Point p:
void set_mouse_loc(Point p)
{
    CGPoint location = CGPointMake(p.x, p.y);
    Detail::mouse_event(kCGMouseButtonLeft, kCGEventMouseMoved, location);
}

//------------------------------------------------------------------------------

// sets mouse button state, if is_press==true the button b is pressed
// otherwise is released:
void mouse_button(Button b, bool is_press)
{
    CGEventType type;
    CGMouseButton button;

    switch (b) {
    case btn_left:
        type = (is_press ? kCGEventLeftMouseDown : kCGEventLeftMouseUp);
        button = kCGMouseButtonLeft;
        break;

    case btn_right:
        type = (is_press ? kCGEventRightMouseDown : kCGEventRightMouseUp);
        button = kCGMouseButtonRight;
        break;
    };

    CGEventRef event = CGEventCreate(NULL);
    CGPoint location = CGEventGetLocation(event); // current mouse location
    CFRelease(event);

    Detail::mouse_event(button, type, location);
}

//------------------------------------------------------------------------------

// clicks at the current mouse location:
void mouse_click(Button b)
{
    mouse_button(b, true);  // press
    mouse_button(b, false); // release
}

int keyCodeForKeyString(const char * keyString)
{
    if (strcmp(keyString, "a") == 0) return 0;
    if (strcmp(keyString, "s") == 0) return 1;
    if (strcmp(keyString, "d") == 0) return 2;
    if (strcmp(keyString, "f") == 0) return 3;
    if (strcmp(keyString, "h") == 0) return 4;
    if (strcmp(keyString, "g") == 0) return 5;
    if (strcmp(keyString, "z") == 0) return 6;
    if (strcmp(keyString, "x") == 0) return 7;
    if (strcmp(keyString, "c") == 0) return 8;
    if (strcmp(keyString, "v") == 0) return 9;
    // what is 10?
    if (strcmp(keyString, "b") == 0) return 11;
    if (strcmp(keyString, "q") == 0) return 12;
    if (strcmp(keyString, "w") == 0) return 13;
    if (strcmp(keyString, "e") == 0) return 14;
    if (strcmp(keyString, "r") == 0) return 15;
    if (strcmp(keyString, "y") == 0) return 16;
    if (strcmp(keyString, "t") == 0) return 17;
    if (strcmp(keyString, "1") == 0) return 18;
    if (strcmp(keyString, "2") == 0) return 19;
    if (strcmp(keyString, "3") == 0) return 20;
    if (strcmp(keyString, "4") == 0) return 21;
    if (strcmp(keyString, "6") == 0) return 22;
    if (strcmp(keyString, "5") == 0) return 23;
    if (strcmp(keyString, "=") == 0) return 24;
    if (strcmp(keyString, "9") == 0) return 25;
    if (strcmp(keyString, "7") == 0) return 26;
    if (strcmp(keyString, "-") == 0) return 27;
    if (strcmp(keyString, "8") == 0) return 28;
    if (strcmp(keyString, "0") == 0) return 29;
    if (strcmp(keyString, "]") == 0) return 30;
    if (strcmp(keyString, "o") == 0) return 31;
    if (strcmp(keyString, "u") == 0) return 32;
    if (strcmp(keyString, "[") == 0) return 33;
    if (strcmp(keyString, "i") == 0) return 34;
    if (strcmp(keyString, "p") == 0) return 35;
    if (strcmp(keyString, "RETURN") == 0) return 36;
    if (strcmp(keyString, "l") == 0) return 37;
    if (strcmp(keyString, "j") == 0) return 38;
    if (strcmp(keyString, "'") == 0) return 39;
    if (strcmp(keyString, "k") == 0) return 40;
    if (strcmp(keyString, ";") == 0) return 41;
    if (strcmp(keyString, "\\") == 0) return 42;
    if (strcmp(keyString, ",") == 0) return 43;
    if (strcmp(keyString, "/") == 0) return 44;
    if (strcmp(keyString, "n") == 0) return 45;
    if (strcmp(keyString, "m") == 0) return 46;
    if (strcmp(keyString, ".") == 0) return 47;
    if (strcmp(keyString, "TAB") == 0) return 48;
    if (strcmp(keyString, "SPACE") == 0) return 49;
    if (strcmp(keyString, "`") == 0) return 50;
    if (strcmp(keyString, "DELETE") == 0) return 51;
    if (strcmp(keyString, "ENTER") == 0) return 52;
    if (strcmp(keyString, "ESCAPE") == 0) return 53;

    // some more missing codes abound, reserved I presume, but it would
    // have been helpful for Apple to have a document with them all listed

    if (strcmp(keyString, ".") == 0) return 65;

    if (strcmp(keyString, "*") == 0) return 67;

    if (strcmp(keyString, "+") == 0) return 69;

    if (strcmp(keyString, "CLEAR") == 0) return 71;

    if (strcmp(keyString, "/") == 0) return 75;
    if (strcmp(keyString, "ENTER") == 0) return 76;  // numberpad on full kbd

    if (strcmp(keyString, "=") == 0) return 78;

    if (strcmp(keyString, "=") == 0) return 81;
    if (strcmp(keyString, "0") == 0) return 82;
    if (strcmp(keyString, "1") == 0) return 83;
    if (strcmp(keyString, "2") == 0) return 84;
    if (strcmp(keyString, "3") == 0) return 85;
    if (strcmp(keyString, "4") == 0) return 86;
    if (strcmp(keyString, "5") == 0) return 87;
    if (strcmp(keyString, "6") == 0) return 88;
    if (strcmp(keyString, "7") == 0) return 89;

    if (strcmp(keyString, "8") == 0) return 91;
    if (strcmp(keyString, "9") == 0) return 92;

    if (strcmp(keyString, "F5") == 0) return 96;
    if (strcmp(keyString, "F6") == 0) return 97;
    if (strcmp(keyString, "F7") == 0) return 98;
    if (strcmp(keyString, "F3") == 0) return 99;
    if (strcmp(keyString, "F8") == 0) return 100;
    if (strcmp(keyString, "F9") == 0) return 101;

    if (strcmp(keyString, "F11") == 0) return 103;

    if (strcmp(keyString, "F13") == 0) return 105;

    if (strcmp(keyString, "F14") == 0) return 107;

    if (strcmp(keyString, "F10") == 0) return 109;

    if (strcmp(keyString, "F12") == 0) return 111;

    if (strcmp(keyString, "F15") == 0) return 113;
    if (strcmp(keyString, "HELP") == 0) return 114;
    if (strcmp(keyString, "HOME") == 0) return 115;
    if (strcmp(keyString, "PGUP") == 0) return 116;
    if (strcmp(keyString, "DELETE") == 0) return 117;
    if (strcmp(keyString, "F4") == 0) return 118;
    if (strcmp(keyString, "END") == 0) return 119;
    if (strcmp(keyString, "F2") == 0) return 120;
    if (strcmp(keyString, "PGDN") == 0) return 121;
    if (strcmp(keyString, "F1") == 0) return 122;
    if (strcmp(keyString, "LEFT") == 0) return 123;
    if (strcmp(keyString, "RIGHT") == 0) return 124;
    if (strcmp(keyString, "DOWN") == 0) return 125;
    if (strcmp(keyString, "UP") == 0) return 126;

    fprintf(stderr, "keyString %s Not Found. Aborting...\n", keyString);
    exit(EXIT_FAILURE);
}

void key_press(int key) {
    // Create an HID hardware event source
    CGEventSourceRef src = CGEventSourceCreate(kCGEventSourceStateHIDSystemState);

    // Create a new keyboard key press event
    CGEventRef evt = CGEventCreateKeyboardEvent(src, (CGKeyCode) key, true);

    // Post keyboard event and release
    CGEventPost (kCGHIDEventTap, evt);
    CFRelease (evt); CFRelease (src);

    usleep(60);
}

void key_release(int key) {
    // Create an HID hardware event source
    CGEventSourceRef src = CGEventSourceCreate(kCGEventSourceStateHIDSystemState);

    // Create a new keyboard key release event
    CGEventRef evt = CGEventCreateKeyboardEvent(src, (CGKeyCode) key, false);

    // Post keyboard event and release
    CGEventPost (kCGHIDEventTap, evt);
    CFRelease (evt); CFRelease (src);

    usleep(60);
}

void key_type(int key)
{
    key_press(key);
    key_release(key);
}

//------------------------------------------------------------------------------

}; // namespace Emumice

//------------------------------------------------------------------------------
