#include "Controller.h"

vector<map<string, string>> DatabaseController::query(string query)
{
    vector<map<string, string>> result;
    
    sqlite3_stmt *sql_query;
    sqlite3_prepare_v2(db, query.c_str(), query.size(), &sql_query, 0);

    int rc;
    map<string, string> row;
    while ((rc = sqlite3_step(sql_query)) == SQLITE_ROW)
    {
        int argc = sqlite3_column_count(sql_query);
        for (int i = 0; i < argc; i++)
        {
            row[sqlite3_column_name(sql_query, i)] = (char*) sqlite3_column_text(sql_query, i);
        }
        result.push_back(row);
    }

    sqlite3_finalize(sql_query);
    return result;
}

void DatabaseController::getFromProfile(Profile profile)
{
    sqlite3_close(db);
    sqlite3_open(":memory:", &db);
    sqlite3_deserialize(db, "main", profile.getData(), profile.getDataSize(), 0, 0);
}

void DatabaseController::saveToProfile(Profile profile)
{
    sqlite_int64 size;
    uint8_t *serialized = sqlite3_serialize(db, "main", &size, 0);

    profile.setData((char*) serialized, size);

    sqlite3_close(db);
}