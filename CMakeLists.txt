cmake_minimum_required(VERSION 3.10)
project(InventoryManagementCPP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True) # Good practice to ensure C++17 is used

# The CMAKE_TOOLCHAIN_FILE provided on the command line handles vcpkg integration.
# No need for manual CMAKE_PREFIX_PATH manipulation here if the toolchain file is correct.

# Find libpqxx using the config file provided by vcpkg.
# This will also find libpq (PostgreSQL) as a dependency.
# Ensure that libpqxx was installed via vcpkg (e.g., vcpkg install libpqxx:x64-windows)
find_package(libpqxx CONFIG REQUIRED)

add_executable(InventoryManagementCPP
    main.cpp
    Product.cpp
    DatabaseManager.cpp
    InventoryManager.cpp
)

# Link against the imported target from libpqxx.
# This target should bring in include directories and library dependencies automatically.
# If find_package(libpqxx) was successful, this target (libpqxx::pqxx) should exist.
target_link_libraries(InventoryManagementCPP PRIVATE libpqxx::pqxx)
