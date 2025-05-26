/*
 * File: DatabaseManager.h
 * Description: Handles PostgreSQL database connection and query execution.
 * Author: David Paul Desuyo
 * Date: 2025-05-26
 */

#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <pqxx/pqxx>
#include <string>
#include <map>

class DatabaseManager {
private:
    pqxx::connection* conn;

    // Helper methods for loading and building connection string
    std::map<std::string, std::string> loadConfig(const std::string& filename);
    std::string buildConnectionString(const std::map<std::string, std::string>& config);
    std::string trim(const std::string& str);


public:
    DatabaseManager(const std::string& configFilePath);
    ~DatabaseManager();

    pqxx::result executeQuery(const std::string& query);
    void executeUpdate(const std::string& query);
    pqxx::connection* getConnection();
};

#endif // DATABASEMANAGER_H
