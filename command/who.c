#include <command.h>

int
main(string arg)
{
    object *list;
    int j;

    //printf("%-25s idle\n", "name (*edit, +input)");
    printf("%-25s idle\n", "name");
    printf("--------------------      ----\n");
    for (list = users(), j = 0; j < sizeof(list); j++) {
        printf("%-25s %4d\n", (string)list[j]->query_name()
/*        +
        (in_edit(this_player()) ? "*" : "") +
        (in_input(this_player()) ? "+" : "")
*/        
        ,
        query_idle(this_player()) / 60
        );
    }
    return 1;
}

string help() {
    return @help
Lists the players currently
online.
help;
}
