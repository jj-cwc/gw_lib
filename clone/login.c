#include <globals.h>

/* ensure_passwords_folder(), password_file(), user_exists(), and create_user_object()
 * are from https://startfromlil.wordpress.com/2012/04/04/users-and-passwords/
 * In fact, as of 3/29/22,  most of this is from that URL.
 */
// A function to ensure the passwords folder exists.
// This will only matter the first time a user is created,
// but let's go ahead and write robust code.
private void ensure_passwords_folder () {
    if ( sizeof( stat( "/u/passwords" ) ) == 0 )
        mkdir( "/u/passwords" );
}

// A function that embodies the password file naming convention.
// The parameter is the character's name (e.g., "Dave").
private string password_file ( string name ) {
    return "/u/passwords/" + name;
}

// A function that determines if the user exists, based on
// whether a password file for that user exists.
private int user_exists ( string name ) {
    return sizeof( stat( password_file( name ) ) ) > 0;
}

private void create_user_object ( string name ) {
    object user;

    user = find_player(name);
    if(user) {
        remove_interactive(user);
        destruct(user);
    }
    user = new("/clone/user");
    //user->set_name(name + "_" + getoid(user));
    user->set_name(name);
    exec(user, this_object());
    user->setup();
#ifndef __NO_ENVIRONMENT__
    user->move(VOID_OB);
#endif
    destruct(this_object());
}

#ifdef __INTERACTIVE_CATCH_TELL__
void catch_tell(string str) {
    receive(str);
}
#endif

void logon() {
#ifdef __NO_ADD_ACTION__
    set_this_player(this_object());
#endif
#ifdef __PACKAGE_UIDS__
    seteuid(getuid(this_object()));
#endif
    cat("etc/title");
    write("\nWhat is your username?\n");
    input_to("get_username", 2);
}

void get_username(string username) {
    if(username == "") {
        write("Perhaps another time, then.");
        remove_interactive(this_object());
        destruct(this_object());
    } else {
        username = lower_case(username);
        if(user_exists(username)) {
            write("Welcome back!\nWhat is your password?\n");
            input_to("get_password", 3, username);
        } else {
            write("Player not found.\n" +
                  "Enter a password to create new\n" +
                  "player, or [Return] to re-enter\n" +
                  "username.");
            input_to("create_password", 3, username);
        }
    }
}

void get_password(string password, string username) {
    if (read_file(password_file(username)) != crypt(password, "$6$"+username)) {
        write("Incorrect password, please try\n" +
              "again.\n\n" +
              "What is your username?");
        input_to("get_username");
    } else {
        write("Welcome back, " + username + "!\n");
        create_user_object(username);
    }
}

void create_password(string password, string username) {
    if(password == "") {
        write("Please enter your username.\n\n");
        input_to("get_username");
    } else {
        write("Please re-enter password.");
        input_to("repeat_password", 3, username, password);
    }
}

void repeat_password(string password2, string username, string password) {
    if(password != password2) {
        write("[" + password + "] vs [" + password2 + "]\n");
        write("Passwords do not match. Please\n" +
              "try again.\n\n" +
              "What is your username?");
        input_to("get_username");
    } else {
        ensure_passwords_folder();
        write_file(password_file(username), crypt(password, "$6$"+username), 1);
        create_user_object(username);
    }
}
