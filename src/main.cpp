#include <iostream>
#include <sstream>
#include <cdk.h>
using namespace std;

#include "Profile.h"
#include "display/Terminal.h"
#include "display/Window.h"
#include "views/Welcome.h"
#include "views/Login.h"

void setup()
{
    Profile profile = Profile();

    string filename;
    printf("Enter filename: ");
    cin >> filename;

    string password;
    printf("Enter password: ");
    cin >> password;

    profile.setup(password);

    ifstream t("profiles/lorem_ipsum.txt");
    stringstream buffer;
    buffer << t.rdbuf();

    string loremipsum = buffer.str();
    profile.setData(loremipsum);

    profile.encryptData();
    
    profile.setFilename(filename);
    profile.saveToFile();
}

void load()
{
    Profile profile = Profile();

    string filename;
    printf("Enter filename: ");
    cin >> filename;

    string password;
    printf("Enter password: ");
    cin >> password;

    profile.setFilename(filename);
    profile.loadFromFile();

    if (profile.authenticate(password))
    {
        printf("PASSWORD CORRECT\n");

        profile.decryptData();
        // printf("\n\n%s\n", profile.getData());

        profile.debug();
    }
    else
    {
        printf("PASSWORD INCORRECT!!!\n");
    }
}

int main()
{
    // setup();
    // load();
    // return 0;

    Terminal terminal = Terminal();
    Window window = Window(LINES, COLS, 0, 0);
    
    CDKSCREEN* screen = initCDKScreen(window.window);

    LoginView login = LoginView();
    
    if(!login.activate(screen))
    {
        destroyCDKScreen(screen);
        endCDK();
        return 0;
    }

    destroyCDKScreen(screen);
    endCDK();

    printf("\n\ndata: %i\n\n", (int) login.profile.getDataSize());

    return 0;
}