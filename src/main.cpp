#include <iostream>
#include <sstream>
#include <cdk.h>
using namespace std;

#include "Profile.h"

#include "display/Terminal.h"
#include "display/Window.h"

#include "views/Welcome.h"
#include "views/Login.h"
#include "views/Profile.h"

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

int exit(CDKSCREEN* screen)
{
    destroyCDKScreen(screen);
    endCDK();
    return 0;
}

int main()
{
    // setup();
    // load();
    // return 0;

    Terminal terminal = Terminal();
    Window window = Window(LINES, COLS, 0, 0);
    CDKSCREEN* screen = initCDKScreen(window.window);


    WelcomeView welcome = WelcomeView();
    if(!welcome.activate(screen)) return exit(screen);

    LoginView login = LoginView();
    if(!login.activate(screen)) return exit(screen);
    
    ProfileView prof_view = ProfileView(login.profile);
    if(!prof_view.activate(screen)) return exit(screen);

    prof_view.profile.encryptData();
    prof_view.profile.saveToFile();
    
    exit(screen);
    
    printf("\ndata_size: %i\n", (int) login.profile.getDataSize());
    
    return 0;
}