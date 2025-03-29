#ifndef CASHFLOWTRACKING_H
#define CASHFLOWTRACKING_H

#include <vector>
#include <string>
#include <unordered_map>

/**
 * @class cashflowtracking.h
 * @brief Defines the CashflowTracking class for managing and storing transactions.
 *
 * This file contains the declaration of the CashflowTracking class, which provides
 * functionalities for adding, searching, and listing transactions.
 * Transactions stored in vector, and categories mapped to their respective transaction IDs.
 *
 * * @author Abbygail Latimer
 */

 /**
 * @struct DataEntry
 * @brief Represents a single transaction.
 *
 * Each DataEntry struct stores details about a transaction, including transaction ID,
 * date and time, seller, buyer, merchandise details, cost, and category.

 */
struct DataEntry {
    int transid;                ///< Unique transaction ID.
    std::string dateTime;       ///< Date and time of the transaction.
    std::string seller;         ///< Name of the seller.
    std::string buyer;          ///< Name of the buyer.
    std::string merchandise;    ///< Description of the merchandise involved.
    double cost;                ///< Cost of the transaction.
    std::string category;       ///< Category of the transaction (e.g., sales, expenses).
};

/**
 * @class CashflowTracking
 * @brief Manages transactions and tracks cash flow.
 *
 * The class provides methods to add, search, and list transactions.
 * It maintains a record of transactions and categorizes them.
 */
class CashflowTracking {
private:
    std::vector<DataEntry> dataEntries;
    std::unordered_map<std::string, std::vector<int>> categories;
    int dataCounter = 1;

public:
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
    void addData(const std::string& dateTime, const std::string& seller, const std::string& buyer,
        const std::string& merchandise, double cost, const std::string& category);

    /**
     * @brief Searches for a transaction by its ID.
     * @param transID Unique transaction ID to search for.
     * @return Pointer to the found DataEntry, or nullptr if not found.
     *
     * This function iterates through the stored transactions and returns
     * a pointer to the matching transaction if found.
     */
    DataEntry* searchDataEntries(int transID);
    /**
     * @brief Lists all stored transactions.
     *
     * Iterates through the transaction records and prints details.
     */
    void listDataEntries();
    /**
     * @brief Retrieves all stored transaction records.
     * @return A vector containing all DataEntry objects.
     *
     * This function provides external access to the full transaction list
     */
    std::vector<DataEntry> getAllDataEntries();
};

#endif
