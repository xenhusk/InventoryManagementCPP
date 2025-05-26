/*
 * File: Product.cpp
 * Description: Implements the Product class.
 * Author: David Paul Desuyo
 * Date: 2025-05-26
 */

#include "Product.h"

Product::Product(int id, const std::string& name, double price, int quantity)
    : productId(id), productName(name), price(price), quantity(quantity) {}

Product::Product(const std::string& name, double price, int quantity)
    : productId(-1), productName(name), price(price), quantity(quantity) {}
