/*
 * File: main.cpp
 * Description: Entry point and CLI for Inventory Management System.
 * Author: David Paul Desuyo
 * Date: 2025-05-27 (Updated)
 */

#include <iostream>
#include <vector>      // For std::vector
#include <string>      // For std::string
#include <iomanip>     // For std::setw, std::left, std::fixed, std::setprecision
#include <limits>      // For std::numeric_limits
#include <algorithm>   // For std::max
#include <chrono>      // For timing

#include "InventoryManager.h"
#include "DatabaseManager.h"

// Function to print a horizontal line for table
void printHorizontalLine(int idWidth, int nameWidth, int priceWidth, int quantityWidth) {
    std::cout << "+-" << std::string(idWidth, '-') << "-+-"
              << std::string(nameWidth, '-') << "-+-"
              << std::string(priceWidth, '-') << "-+-"
              << std::string(quantityWidth, '-') << "-+\n";
}

// Function to print the product table
void printProductTable(const std::vector<Product>& products) {
    if (products.empty()) {
        std::cout << "No products found.\n";
        return;
    }

    // Determine column widths
    int idWidth = 4; // "ID"
    int nameWidth = 10; // "Name"
    int priceWidth = 10; // "Price"
    int quantityWidth = 10; // "Quantity"

    for (const auto& p : products) {
        idWidth = std::max(idWidth, static_cast<int>(std::to_string(p.productId).length()));
        nameWidth = std::max(nameWidth, static_cast<int>(p.productName.length()));
        // For price, consider a fixed precision format
        // For quantity, consider the length of the number
        quantityWidth = std::max(quantityWidth, static_cast<int>(std::to_string(p.quantity).length()));
    }
    // Add a little padding
    idWidth += 2;
    nameWidth += 2;
    priceWidth +=2; // Default width for price, e.g., "XX.YY" + padding
    quantityWidth += 2;


    // Print table header
    printHorizontalLine(idWidth, nameWidth, priceWidth, quantityWidth);
    std::cout << "| " << std::left << std::setw(idWidth) << "ID"
              << "| " << std::left << std::setw(nameWidth) << "Name"
              << "| " << std::left << std::setw(priceWidth) << "Price"
              << "| " << std::left << std::setw(quantityWidth) << "Quantity" << " |\n";
    printHorizontalLine(idWidth, nameWidth, priceWidth, quantityWidth);

    // Print table rows
    for (const auto& p : products) {
        std::cout << "| " << std::left << std::setw(idWidth) << p.productId
                  << "| " << std::left << std::setw(nameWidth) << p.productName
                  << "| " << std::left << std::fixed << std::setprecision(2) << std::setw(priceWidth) << p.price
                  << "| " << std::left << std::setw(quantityWidth) << p.quantity << " |\n";
    }
    printHorizontalLine(idWidth, nameWidth, priceWidth, quantityWidth);
}


void printMenu() {
    std::cout << "\n+--------------------------------------+\n";
    std::cout << "|   Inventory Management System        |\n";
    std::cout << "+--------------------------------------+\n";
    std::cout << "| 1. Add Product                       |\n";
    std::cout << "| 2. View All Products                 |\n";
    std::cout << "| 3. View Product by ID                |\n";
    std::cout << "| 4. Update Product                    |\n";
    std::cout << "| 5. Delete Product                    |\n";
    std::cout << "| 6. Exit                              |\n";
    std::cout << "+--------------------------------------+\n";
    std::cout << "Enter your choice: ";
}

// Function to get a valid integer input
int getIntegerInput(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.good()) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear buffer
            return value;
        } else {
            std::cout << "Invalid input. Please enter a number.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

// Function to get a valid double input
double getDoubleInput(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.good()) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear buffer
            return value;
        } else {
            std::cout << "Invalid input. Please enter a valid price.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}


int main() {
    // Initialize the database manager with the configuration file path
    std::string configFilePath = "db_config.ini"; // Expect db_config.ini to be in the CWD
    DatabaseManager dbManager(configFilePath);
    InventoryManager inventory(dbManager);

    int choice = 0;
    while (choice != 6) {
        printMenu();
        // More robust choice input
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear(); // clear error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard invalid input
            choice = 0; // reset choice to trigger invalid option path
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear valid input buffer
        }


        switch (choice) {
            case 1: {
                std::string name;
                double price;
                int quantity;
                std::cout << "Enter product name: ";
                std::getline(std::cin, name);
                price = getDoubleInput("Enter product price: ");
                quantity = getIntegerInput("Enter product quantity: ");

                if (inventory.addProduct(name, price, quantity)) {
                    std::cout << "Product added successfully.\n";
                } else {
                    std::cout << "Failed to add product.\n";
                }
                break;
            }
            case 2: {
                std::cout << "\nWhich version to run?\n";
                std::cout << "1. Algorithm 1 (Single Query)\n";
                std::cout << "2. Algorithm 2 (N+1 Queries)\n";
                std::cout << "Enter choice: ";
                int algo_choice;
                std::cin >> algo_choice;
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    algo_choice = 0; 
                } else {
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }

                std::vector<Product> products;
                auto start_time = std::chrono::high_resolution_clock::now();
                size_t initial_memory = 0; // Placeholder for actual memory measurement

                if (algo_choice == 1) {
                    std::cout << "\nRunning Algorithm 1 (Single Query)...\n";
                    products = inventory.getAllProductsAlgorithm1();
                } else if (algo_choice == 2) {
                    std::cout << "\nRunning Algorithm 2 (N+1 Queries)...\n";
                    products = inventory.getAllProductsAlgorithm2();
                } else {
                    std::cout << "Invalid algorithm choice.\n";
                    break;
                }

                auto end_time = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
                size_t final_memory = 0; // Placeholder for actual memory measurement

                std::cout << "\n--- All Products ---\n";
                printProductTable(products);
                
                std::cout << "\n--- Performance ---\n";
                std::cout << "Time taken: " << duration.count() << " microseconds.\n";
                // Note: Actual memory usage measurement is complex and platform-dependent.
                // This is a conceptual placeholder.
                std::cout << "Estimated memory usage (conceptual): " << (products.capacity() * sizeof(Product)) << " bytes for vector capacity.\n";
                std::cout << "Number of products: " << products.size() << "\n";
                break;
            }
            case 3: {
                int id = getIntegerInput("Enter product ID: ");
                auto product = inventory.getProductById(id);
                if (product) {
                    std::cout << "\n--- Product Details ---\n";
                    // You can use the printProductTable for a single product too if you adapt it
                    // or just print plainly:
                    std::cout << "ID        : " << product->productId << "\n";
                    std::cout << "Name      : " << product->productName << "\n";
                    std::cout << "Price     : " << std::fixed << std::setprecision(2) << product->price << "\n";
                    std::cout << "Quantity  : " << product->quantity << '\n';
                    std::cout << "----------------------\n";
                } else {
                    std::cout << "Product with ID " << id << " not found.\n";
                }
                break;
            }
            case 4: {
                int id = getIntegerInput("Enter product ID to update: ");
                // Check if product exists before asking for new details
                auto existing_product = inventory.getProductById(id);
                if (!existing_product) {
                    std::cout << "Product with ID " << id << " not found. Cannot update.\n";
                    break;
                }

                std::string name;
                double price;
                int quantity;

                std::cout << "Enter new product name (current: " << existing_product->productName << "): ";
                std::getline(std::cin, name);
                price = getDoubleInput("Enter new product price (current: " + std::to_string(existing_product->price) + "): ");
                quantity = getIntegerInput("Enter new product quantity (current: " + std::to_string(existing_product->quantity) + "): ");

                if (inventory.updateProduct(id, name, price, quantity)) {
                    std::cout << "Product updated successfully.\n";
                } else {
                    std::cout << "Failed to update product.\n";
                }
                break;
            }
            case 5: {
                int id = getIntegerInput("Enter product ID to delete: ");
                 // Optional: Confirm before deleting
                char confirm;
                std::cout << "Are you sure you want to delete product with ID " << id << "? (y/n): ";
                std::cin >> confirm;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                if (tolower(confirm) == 'y') {
                    if (inventory.deleteProduct(id)) {
                        std::cout << "Product deleted successfully.\n";
                    } else {
                        std::cout << "Failed to delete product (it might not exist).\n";
                    }
                } else {
                    std::cout << "Deletion cancelled.\n";
                }
                break;
            }
            case 6:
                std::cout << "Exiting Inventory Management System. Goodbye!\n";
                break;
            default:
                std::cout << "Invalid choice. Please enter a number between 1 and 6.\n";
                break;
        }
        if (choice != 6) {
            std::cout << "\nPress Enter to continue...";
            std::cin.get(); // Wait for user to press Enter
        }
    }
    return 0;
}