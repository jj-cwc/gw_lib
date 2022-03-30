#include <command.h>

int main(string arg) {
    if(!arg) {
#ifndef __NO_ADD_ACTION__
        return notify_fail("cat what?\n");
#else
        write("cat what?\n");
        return 1;
#endif
    }
    cat(arg);
    write("\n");
    return 1;
}