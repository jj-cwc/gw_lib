/* Initial version from:
 * https://startfromlil.wordpress.com/2012/04/05/create-a-command-help/
 */

#include <command.h>
#include <globals.h>
 
string* add_action_commands () {
    return map( commands(), (: $1[0] :) ) - ({ "" });
}
string* commands_in_filesystem () {
    return map( get_dir( COMMAND_PREFIX + "*.c" ),
                (: $1[0..sizeof($1)-3] :) );
}
 
int main(string arg)
{
    object cobj;
    string help;
    string* cmds;
    int i, col;
 
    if ( arg ) {
        if ( member_array( arg, commands_in_filesystem() ) > -1 ) {
            cobj = load_object( COMMAND_PREFIX + arg + ".c" );
            help = cobj->help();
            if ( !help )
                help = "No help available on that\ncommand.";
            if ( help[sizeof(help)-1..] != "\n" )
                help += "\n";
            write( help );
        } else if ( member_array( arg, add_action_commands() ) > -1 ) {
            write( "No help available on that\ncommand.\n" );
        } else {
            write( "There is no such command to\nget help on.\n" );
        }
        return 1;
    } else {
        write( "You have access to the\n"
             + "following commands. You can ask\n"
             + "for help on any one by typing:\n"
             + "\"help <command>\".\n\n" );
        cmds = add_action_commands() + commands_in_filesystem();
        //write( implode( sort_array( cmds, 1 ), ", " ) + "\n" );
        cmds = sort_array(cmds, 1);
        col = 0;
        for(i=0; i<sizeof(cmds); i++) {
            printf("%-10s", cmds[i]);
            col++;
            if(col==3) {
                write("\n");
                col = 0;
            }
        }
        write("\n");
        
        return 1;
    }
}
 
string help() {
    return @help
Lists the commands available to
you, if you just type "help."

Gives help on individual
commands, if you type 
"help <command>."
help;
}