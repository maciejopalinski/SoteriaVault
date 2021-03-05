#include <iostream>
#include <sstream>
using namespace std;

#include "Profile.h"

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

    ifstream t("lorem_ipsum.txt");
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
    setup();
    load();
    return 0;
}