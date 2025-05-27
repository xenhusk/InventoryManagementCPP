#Documents
**Testplan Documentation**
https://docs.google.com/document/d/1q8shJnAazFL923uHcrPQKrJ1DUJWcDyXpX96LOJB3dc/edit?usp=sharing

**Algorithm Documentation**
https://docs.google.com/document/d/1jsVyUu2LXJN5o6hnkjKMnHdqr5QfltgJVwLNpOI46IU/edit?usp=sharing

# Inventory Management System (C++ & PostgreSQL)

This project is a command-line based Inventory Management System built with C++ and using PostgreSQL as the database. It allows users to perform CRUD (Create, Read, Update, Delete) operations on products and includes a comparison of two different algorithms for fetching all products.

## Prerequisites

Before you begin, ensure you have the following installed:

1.  **C++ Compiler:** A modern C++ compiler (e.g., MSVC, GCC, Clang) that supports C++17.
2.  **CMake:** Version 3.15 or higher.
3.  **PostgreSQL:** A running PostgreSQL server instance.
4.  **vcpkg:** Microsoft's C++ package manager. Ensure `libpqxx` is installed via vcpkg and integrated into your development environment.
    *   Install `libpqxx` using vcpkg: `vcpkg install libpqxx:x64-windows` (or your target triplet).
    *   Ensure vcpkg is integrated: `vcpkg integrate install`.

## Setup Instructions

1.  **Clone the Repository (if applicable):**
    ```bash
    git clone <repository-url>
    cd InventoryManagementCPP
    ```

2.  **Database Configuration:**
    *   Create a database in your PostgreSQL instance for this project (e.g., `inventory_db`).
    *   Locate the `db_config.ini.template` file (or create `db_config.ini` if it doesn't exist) in the project root directory.
    *   Copy it to `db_config.ini` if you copied the template.
    *   Edit `db_config.ini` with your PostgreSQL connection details:
        ```ini
        dbname=your_database_name
        user=your_username
        password=your_password
        host=localhost
        port=5432
        ```
    *   **Important:** The `db_config.ini` file is listed in `.gitignore` and should not be committed to version control.

3.  **Create Products Table:**
    Connect to your PostgreSQL database (e.g., using `psql` or a GUI tool like pgAdmin) and execute the following SQL command to create the `Products` table:
    ```sql
    CREATE TABLE Products (
        id SERIAL PRIMARY KEY,
        name VARCHAR(255) NOT NULL,
        price DECIMAL(10, 2) NOT NULL,
        quantity INTEGER NOT NULL
    );
    ```

## Build Instructions

It is recommended to perform an out-of-source build.

1.  **Navigate to the project root directory** (e.g., `c:\Users\xenhu\OneDrive\Documents\GitHub\InventoryManagementCPP`).

2.  **Create a build directory and navigate into it:**
    ```powershell
    mkdir build
    cd build
    ```

3.  **Configure the project with CMake:**
    This command tells CMake where to find the `CMakeLists.txt` (in the parent directory `..`) and your vcpkg toolchain file.
    ```powershell
    cmake .. -DCMAKE_TOOLCHAIN_FILE=[path-to-vcpkg]/scripts/buildsystems/vcpkg.cmake
    ```
    *   Replace `[path-to-vcpkg]` with the actual path to your vcpkg installation (e.g., `C:/vcpkg` or `~/vcpkg`).

4.  **Build the project:**
    This command compiles the source code and creates the executable.
    ```powershell
    cmake --build .
    ```
    Or, if you prefer to specify the configuration (e.g., Debug or Release):
    ```powershell
    cmake --build . --config Debug
    ```

## Running the Application

After a successful build, the executable will be located in the `build\Debug` (or `build\Release`) directory.

1.  **Navigate to the project root directory** (where `db_config.ini` is located):
    ```powershell
    cd ..
    ```
    (If you are currently in the `build` directory).

2.  **Run the executable:**
    ```powershell
    .\build\Debug\InventoryManagementCPP.exe
    ```
    Or, if you built in Release mode:
    ```powershell
    .\build\Release\InventoryManagementCPP.exe
    ```

    **Note:** The application expects `db_config.ini` to be in the same directory from which the executable is run. Running from the project root ensures it can find the configuration file correctly as `main.cpp` specifies `"db_config.ini"` as the path.

## Project Structure

*   `Product.h`/`.cpp`: Defines the `Product` class.
*   `DatabaseManager.h`/`.cpp`: Manages the connection to the PostgreSQL database using `libpqxx` and loads credentials from `db_config.ini`.
*   `InventoryManager.h`/`.cpp`: Handles the business logic for inventory operations (CRUD, algorithm comparison).
*   `main.cpp`: Contains the command-line interface and program entry point.
*   `CMakeLists.txt`: CMake build script.
*   `db_config.ini`: Stores database connection credentials (ignored by Git).
*   `.gitignore`: Specifies intentionally untracked files that Git should ignore.
*   `README.md`: This file.

## Algorithm Comparison

The system implements two algorithms for fetching all products from the database:

1.  **Algorithm 1 (Efficient):** Fetches all products in a single database query.
2.  **Algorithm 2 (Less Efficient - N+1 Problem):** Fetches product IDs first, then retrieves each product individually in a loop, leading to multiple database queries.

The CLI allows you to choose which algorithm to use for displaying all products, and it measures the execution time for comparison.
