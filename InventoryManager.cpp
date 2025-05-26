/*
 * File: InventoryManager.cpp
 * Description: Implements the InventoryManager class for CRUD operations.
 * Author: David Paul Desuyo
 * Date: 2025-05-26
 */

#include "InventoryManager.h"
#include <iostream>

InventoryManager::InventoryManager(DatabaseManager& db) : dbManager(db) {}

bool InventoryManager::addProduct(const std::string& name, double price, int quantity) {
    try {
        // Use a parameterized query to prevent SQL injection
        pqxx::work txn(*dbManager.getConnection());
        txn.exec_params( // Reverted to exec_params
            "INSERT INTO Products (product_name, price, quantity) VALUES ($1, $2, $3)",
            name, price, quantity
        );
        txn.commit();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error adding product: " << e.what() << std::endl;
        return false;
    }
}

std::optional<Product> InventoryManager::getProductById(int productId) {
    try {
        pqxx::work txn(*dbManager.getConnection());
        pqxx::result res = txn.exec_params( // Reverted to exec_params
            "SELECT product_id, product_name, price, quantity FROM Products WHERE product_id = $1",
            productId
        );
        txn.commit();
        if (!res.empty()) {
            const auto& row = res[0];
            Product product(
                row[0].as<int>(),
                row[1].as<std::string>(),
                row[2].as<double>(),
                row[3].as<int>()
            );
            return product;
        }
        return std::nullopt;
    } catch (const std::exception& e) {
        std::cerr << "Error retrieving product: " << e.what() << std::endl;
        return std::nullopt;
    }
}

/* // Original getAllProducts - REMOVED
std::vector<Product> InventoryManager::getAllProducts() {
    std::vector<Product> products;
    try {
        pqxx::work txn(*dbManager.getConnection());
        // This one uses exec and is fine as it has no parameters
        pqxx::result res = txn.exec("SELECT product_id, product_name, price, quantity FROM Products ORDER BY product_id");
        txn.commit();
        for (const auto& row : res) {
            products.emplace_back(
                row[0].as<int>(),
                row[1].as<std::string>(),
                row[2].as<double>(),
                row[3].as<int>()
            );
        }
    } catch (const std::exception& e) {
        std::cerr << "Error retrieving products: " << e.what() << std::endl;
    }
    return products;
}
*/

// Algorithm 1 (Originally efficient: single query, reserves memory)
std::vector<Product> InventoryManager::getAllProductsAlgorithm1() {
    std::vector<Product> products;
    try {
        pqxx::work txn(*dbManager.getConnection());
        pqxx::result res = txn.exec("SELECT product_id, product_name, price, quantity FROM Products ORDER BY product_id");
        txn.commit();
        products.reserve(res.size()); // Pre-allocate memory
        for (const auto& row : res) {
            products.emplace_back(
                row[0].as<int>(),
                row[1].as<std::string>(),
                row[2].as<double>(),
                row[3].as<int>()
            );
        }
    } catch (const std::exception& e) {
        std::cerr << "Error retrieving products (Algorithm 1): " << e.what() << std::endl;
    }
    return products;
}

// Algorithm 2 (Originally less efficient: N+1 queries)
std::vector<Product> InventoryManager::getAllProductsAlgorithm2() {
    std::vector<Product> products;
    try {
        pqxx::work count_txn(*dbManager.getConnection());
        pqxx::result count_res = count_txn.exec("SELECT product_id FROM Products");
        count_txn.commit();

        for (const auto& id_row : count_res) {
            int current_id = id_row[0].as<int>();
            pqxx::work product_txn(*dbManager.getConnection()); 
            pqxx::result product_res = product_txn.exec_params(
                "SELECT product_id, product_name, price, quantity FROM Products WHERE product_id = $1",
                current_id
            );
            product_txn.commit();

            if (!product_res.empty()) {
                const auto& row = product_res[0];
                products.emplace_back(
                    row[0].as<int>(),
                    row[1].as<std::string>(),
                    row[2].as<double>(),
                    row[3].as<int>()
                );
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error retrieving products (Algorithm 2): " << e.what() << std::endl;
    }
    return products;
}


bool InventoryManager::updateProduct(int productId, const std::string& name, double price, int quantity) {
    try {
        pqxx::work txn(*dbManager.getConnection());
        pqxx::result res = txn.exec_params( // Reverted to exec_params
            "UPDATE Products SET product_name = $1, price = $2, quantity = $3 WHERE product_id = $4",
            name, price, quantity, productId
        );
        txn.commit();
        // Check if any row was updated
        return res.affected_rows() > 0;
    } catch (const std::exception& e) {
        std::cerr << "Error updating product: " << e.what() << std::endl;
        return false;
    }
}

bool InventoryManager::deleteProduct(int productId) {
    try {
        pqxx::work txn(*dbManager.getConnection());
        pqxx::result res = txn.exec_params( // Reverted to exec_params
            "DELETE FROM Products WHERE product_id = $1",
            productId
        );
        txn.commit();
        // Check if any row was deleted
        return res.affected_rows() > 0;
    } catch (const std::exception& e) {
        std::cerr << "Error deleting product: " << e.what() << std::endl;
        return false;
    }
}
