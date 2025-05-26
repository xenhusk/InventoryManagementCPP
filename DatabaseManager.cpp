/*
 * File: DatabaseManager.cpp
 * Description: Implements the DatabaseManager class for PostgreSQL operations.
 * Author: David Paul Desuyo
 * Date: 2025-05-26
 */

#include "DatabaseManager.h"
#include <iostream>
#include <fstream>      // For std::ifstream
#include <sstream>      // For std::istringstream
#include <algorithm>    // For std::remove_if

// Helper function to trim whitespace from both ends of a string
std::string DatabaseManager::trim(const std::string& str) {
    const std::string whitespace = " \t\n\r\f\v";
    size_t start = str.find_first_not_of(whitespace);
    if (start == std::string::npos) {
        return ""; // Return empty string if only whitespace
    }
    size_t end = str.find_last_not_of(whitespace);
    return str.substr(start, end - start + 1);
}

// Helper function to load configuration from a file
std::map<std::string, std::string> DatabaseManager::loadConfig(const std::string& filename) {
    std::map<std::string, std::string> config;
    std::ifstream configFile(filename);
    std::string line;

    if (!configFile.is_open()) {
        throw std::runtime_error("Failed to open configuration file: " + filename);
    }

    while (std::getline(configFile, line)) {
        // Remove comments (lines starting with # or ;)
        line = trim(line);
        if (line.empty() || line[0] == '#' || line[0] == ';') {
            continue;
        }

        std::istringstream iss(line);
        std::string key, value;
        if (std::getline(iss, key, '=') && std::getline(iss, value)) {
            config[trim(key)] = trim(value);
        }
    }
    configFile.close();
    return config;
}

// Helper function to build the connection string from config map
std::string DatabaseManager::buildConnectionString(const std::map<std::string, std::string>& config) {
    std::string connStr;
    // Order might matter for some drivers, or just for readability
    // You can add error handling here if required keys are missing
    if (config.count("dbname")) connStr += "dbname=" + config.at("dbname") + " ";
    if (config.count("user")) connStr += "user=" + config.at("user") + " ";
    if (config.count("password")) connStr += "password=" + config.at("password") + " ";
    if (config.count("host")) connStr += "host=" + config.at("host") + " ";
    if (config.count("port")) connStr += "port=" + config.at("port") + " ";
    
    // Trim trailing space if any
    if (!connStr.empty() && connStr.back() == ' ') {
        connStr.pop_back();
    }
    return connStr;
}

DatabaseManager::DatabaseManager(const std::string& configFilePath) : conn(nullptr) {
    try {
        std::map<std::string, std::string> config = loadConfig(configFilePath);
        std::string connectionString = buildConnectionString(config);
        
        if (connectionString.empty()) {
            throw std::runtime_error("Failed to build connection string from config.");
        }

        conn = new pqxx::connection(connectionString);
        if (!conn->is_open()) {
            throw std::runtime_error("Failed to open database connection using config.");
        }
        // std::cout << "Database connection successful using config!" << std::endl; // Optional: for debugging
    } catch (const std::exception& e) {
        std::cerr << "Database initialization error: " << e.what() << std::endl;
        throw; // Re-throw to allow main to handle it (e.g., exit)
    }
}

DatabaseManager::~DatabaseManager() {
    if (conn) {
        // connection destructor will close the connection; no explicit disconnect() method in libpqxx v7
        delete conn;
    }
}

pqxx::result DatabaseManager::executeQuery(const std::string& query) {
    pqxx::work txn(*conn);
    pqxx::result res = txn.exec(query);
    txn.commit();
    return res;
}

void DatabaseManager::executeUpdate(const std::string& query) {
    pqxx::work txn(*conn);
    txn.exec(query);
    txn.commit();
}

pqxx::connection* DatabaseManager::getConnection() {
    return conn;
}
