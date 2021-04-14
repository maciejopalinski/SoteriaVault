// Copyright (C) 2021  Maciej Opaliński. All Rights Reserved.

#include <iostream>
#include <sstream>
#include <cdk.h>
using namespace std;

#include "include/sqlite3/sqlite3.h"
#include "Profile.h"
#include "views/Welcome.h"
#include "views/Login.h"
#include "views/Profile.h"
#include "database/Controller.h"
#include "Version.h"

void setup()
{
    // initialize the profile instance
    Profile profile = Profile();

    string filename;
    printf("Enter filename: ");
    cin >> filename;

    string password;
    printf("Enter password: ");
    cin >> password;

    profile.setup(password);

    // initialize sqlite database
    sqlite3 *db;
    sqlite3_open(":memory:", &db);

    // load example data into database
    ifstream file("profiles/users.25k.sql");
    if(file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            sqlite3_exec(db, line.c_str(), 0, 0, 0);
        }
        file.close();
    }

    // serialize the database
    sqlite_int64 size;
    uint8_t *serialized = sqlite3_serialize(db, "main", &size, 0);

    // and load the profile
    profile.setData((char*) serialized, size);
    profile.encryptData();
    
    // save encrypted profile data
    profile.setFilename(filename);
    profile.saveToFile();
}

void load()
{
    // initialize the profile instance
    Profile profile = Profile();

    string filename;
    printf("Enter filename: ");
    cin >> filename;

    string password;
    printf("Enter password: ");
    cin >> password;

    // load its contents from file
    profile.setFilename(filename);
    profile.loadFromFile();

    // try to authenticate using provided password
    if (profile.authenticate(password))
    {
        printf("PASSWORD CORRECT\n");

        profile.decryptData();
        // printf("\n%s\n", bin_hex(profile.getData(), profile.getDataSize()).c_str());
        // printf("\n%s\n", profile.getData());
        // profile.debug();

        // initialize sqlite database and load its serialized contents
        DatabaseController db = DatabaseController();
        db.getFromProfile(profile);

        // example query
        auto res = db.query("SELECT * FROM users LIMIT 10;");

        int row_i = 1;
        for (auto row : res)
        {
            printf("\n%i:\n", row_i);
            for (auto col : row)
            {
                printf("%s: %s\n", col.first.c_str(), col.second.c_str());
            }
            row_i++;
        }
    }
    else
    {
        printf("PASSWORD INCORRECT!!!\n");
    }
}

int cdk_exit(CDKSCREEN* screen)
{
    destroyCDKScreen(screen);
    endCDK();
    return 0;
}

int main(int argc, char **argv)
{
    // setup();
    load();
    return 0;
    
    initscr();
    raw();
    keypad(stdscr, true);
    noecho();
    cbreak();
    
    CDKSCREEN* screen = initCDKScreen(stdscr);
    refresh();

    WelcomeView welcome = WelcomeView(screen);
    if(!welcome.activate()) return cdk_exit(screen);

    LoginView login = LoginView(screen);
    if(!login.activate()) return cdk_exit(screen);
    
    ProfileView prof_view = ProfileView(screen, login.profile);
    if(!prof_view.activate()) return cdk_exit(screen);

    prof_view.profile.encryptData();
    prof_view.profile.saveToFile();
    
    cdk_exit(screen);
    
    printf("\ndata_size: %i\n", (int) login.profile.getDataSize());
    
    return 0;
}
