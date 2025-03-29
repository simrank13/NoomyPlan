#include "cashflowtracking.h"
#include <iostream>

/**
 * @class cashflowtracking.cpp
 * @brief Implements the CashflowTracking class for managing financial transactions.
 *
 * This file defines the methods for adding, searching, listing, and retrieving
 * transaction data. The class helps store transaction records and categorizes
 * them based on different attributes.
 *
 * @author Abbygail Latimer
 */

 /**
     * @brief Adds a new transaction record to the system.
     * @param dateTime Date and time of the transaction.
     * @param seller Name of the seller.
     * @param buyer Name of the buyer.
     * @param merchandise List of purchased merchandise.
     * @param cost Cost of the transaction.
     * @param category Category to store transaction under.
     *
     * This function assigns a unique transaction ID to each entry and stores it
     * in the dataEntries vector. It also updates the category mapping.
     */
void CashflowTracking::addData(const std::string& dateTime, const std::string& seller,
    const std::string& buyer, const std::string& merchandise, 
    double cost, const std::string& category) {
    // Create a new DataEntry object and add it to the dataEntries vector
    DataEntry data = { dataCounter++, dateTime, seller, buyer, merchandise, cost, category };
    dataEntries.push_back(data);
    categories[category].push_back(data.transid); // Add the transaction ID to the category's list
}
/**
     * @brief Searches for a transaction by its ID.
     * @param transID Unique transaction ID to search for.
     * @return Pointer to the found DataEntry, or nullptr if not found.
     *
     * This function iterates through the stored transactions and returns
     * a pointer to the matching transaction if found.
     */
DataEntry* CashflowTracking::searchDataEntries(int transID) {
    for (auto& data : dataEntries) {
        if (data.transid == transID) { // Check if the current entry matches the transaction ID
            return &data; // Return a pointer to the matching entry
        }
    }
    return nullptr; // Return nullptr if no matching entry is found
}
/**
     * @brief Lists all stored transactions.
     *
     * Iterates through the transaction records and prints details.
     */
void CashflowTracking::listDataEntries() {
    for (const auto& data : dataEntries) {
        // Print details of each transaction in the dataEntries vector
        std::cout << "ID: " << data.transid << ", Date/Time: " << data.dateTime
            << ", Buyer: " << data.buyer << ", Seller: " << data.seller
            << ", Merchandise: " << data.merchandise << ", Cost: $" << data.cost
            << ", Category: " << data.category << "\n";
    }
}
/**
     * @brief Retrieves all stored transaction records.
     * @return A vector containing all DataEntry objects.
     *
     * This function provides external access to the full transaction list
     */
std::vector<DataEntry> CashflowTracking::getAllDataEntries() {
    return dataEntries;
}