/*
 * File: Product.h
 * Description: Defines the Product class representing a product entity in the inventory.
 * Author: David Paul Desuyo
 * Date: 2025-05-26
 */

#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>

class Product {
public:
    int productId;
    std::string productName;
    double price;
    int quantity;

    Product(int id, const std::string& name, double price, int quantity);
    Product(const std::string& name, double price, int quantity);
};

#endif // PRODUCT_H
