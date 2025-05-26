/*
 * File: InventoryManager.h
 * Description: Provides CRUD operations for products using DatabaseManager.
 * Author: David Paul Desuyo
 * Date: 2025-05-26
 */

#ifndef INVENTORYMANAGER_H
#define INVENTORYMANAGER_H

#include "Product.h"
#include "DatabaseManager.h"
#include <vector>
#include <optional>

class InventoryManager {
private:
    DatabaseManager& dbManager;

public:
    InventoryManager(DatabaseManager& db);

    bool addProduct(const std::string& name, double price, int quantity);
    std::optional<Product> getProductById(int productId);
    std::vector<Product> getAllProductsAlgorithm1(); // Was getAllProductsEfficient
    std::vector<Product> getAllProductsAlgorithm2(); // Was getAllProductsLessEfficient
    bool updateProduct(int productId, const std::string& name, double price, int quantity);
    bool deleteProduct(int productId);
};

#endif // INVENTORYMANAGER_H
