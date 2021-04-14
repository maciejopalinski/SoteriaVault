#ifndef DB_CONTROLLER_H
#define DB_CONTROLLER_H

#include "../include/sqlite3/sqlite3.h"
#include <vector>
#include <map>
using namespace std;

#include "../Profile.h"

class DatabaseController
{
    private:
        sqlite3 *db;

        static int query_callback(void* result, int argc, char **argv, char **cols);

    public:
        vector<map<string, string>> query(string query);
        
        void getFromProfile(Profile profile);
        void saveToProfile(Profile profile);
};

#endif