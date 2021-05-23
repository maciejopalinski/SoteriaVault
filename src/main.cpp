// Copyright (C) 2021  Maciej Opaliński. All Rights Reserved.

#include <iostream>
#include <gtkmm.h>
using namespace std;

#include "include/sqlite3/sqlite3.h"
#include "database/Controller.h"
#include "Profile.h"

#include "Version.h"

Glib::RefPtr<Gtk::Application> app;

Gtk::Window *ProfileSelect;
Gtk::Window *ProfileView;

Gtk::FileChooserButton *LoginFile;
Gtk::Entry             *LoginPassword;
Gtk::Button            *LoginSubmit;
Gtk::SearchEntry       *ProfileViewSearch;
Gtk::TextView          *ProfileViewText;
Gtk::Button            *ProfileViewCreateNew;

DatabaseController db;
Profile profile;

void on_app_activate()
{
    auto builder = Gtk::Builder::create_from_file("SoteriaVault.glade");

    builder->get_widget("ProfileSelect", ProfileSelect);
    builder->get_widget("ProfileView", ProfileView);

    builder->get_widget("LoginFile", LoginFile);
    builder->get_widget("LoginPassword", LoginPassword);
    builder->get_widget("LoginSubmit", LoginSubmit);
    builder->get_widget("ProfileViewSearch", ProfileViewSearch);
    builder->get_widget("ProfileViewText", ProfileViewText);
    builder->get_widget("ProfileViewCreateNew", ProfileViewCreateNew);

    LoginSubmit->signal_clicked().connect([] () {
                
        profile.setFilename(LoginFile->get_filename());
        profile.loadFromFile();

        if(profile.authenticate(LoginPassword->get_text()))
        {
            profile.decryptData();
            db.getFromProfile(profile);

            ProfileSelect->hide();
            
            app->add_window(*ProfileView);
            ProfileView->show();

            string text = "";
            text += "File: " + LoginFile->get_filename() + "\n";
            text += "Password: " + LoginPassword->get_text() + "\n";

            ProfileViewText->get_buffer()->set_text(text);
        }
        else
        {
            app->quit();
        }
    });

    ProfileViewCreateNew->signal_clicked().connect([] () {
        if(!profile.isAuthenticated()) return;
        
        string query = ProfileViewSearch->get_text();
        string output = "";
        auto res = db.query(query);

        int index = 1;
        for (auto row : res)
        {
            output += to_string(index) + "\n";
            for (auto col : row)
            {
                output += col.first + ": " + col.second + "\n";
            }
            output += "\n";
            index++;
        }

        ProfileViewText->get_buffer()->set_text(output);
    });

    app->add_window(*ProfileSelect);

    ProfileSelect->show();
}

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

int main(int argc, char *argv[])
{
    // setup();
    // return 0;

    app = Gtk::Application::create("io.github.poprostumieciek.SoteriaVault");

    app->signal_activate().connect([] () { on_app_activate(); });

    return app->run(argc, argv);
}
