#include "financialreport.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QFileInfo>

/**
 * @class FinancialReportGenerator
 * @brief Generates financial reports such as Profit & Loss Statements, Expense Breakdowns, and Balance Sheets.
 *
 * This class is responsible for loading financial transactions from a CSV file,
 * processing them, and generating various financial reports. It supports operations
 * such as loading transactions, generating profit/loss statements, expense breakdowns,
 * and balance sheets.
 *
 * @author Simran Kullar
 */

/**
 * @brief Constructor for FinancialReportGenerator.
 * 
 * Initializes the FinancialReportGenerator object and sets the transactions list to empty.
 * Logs the initialization message for debugging purposes.
 * 
 * @author Simran Kullar
 */
FinancialReportGenerator::FinancialReportGenerator() {
    // Initialize the transactions list as an empty QList of Transaction objects.
    transactions = QList<Transaction>();
    // Log a debug message to indicate that the constructor has been called
    // and display the initial size of the transactions list (should be 0).
    qDebug() << "Constructor: FinancialReportGenerator initialized, transactions size =" << transactions.size();
}

/**
 * @brief Destructor for FinancialReportGenerator.
 * 
 * Logs the destruction message for debugging purposes.
 * The QList destructor automatically cleans up the transactions list.
 * 
 * @author Simran Kullar
 */
FinancialReportGenerator::~FinancialReportGenerator() {
    // Log a debug message to indicate that the destructor has been called
    // and the FinancialReportGenerator object is being destroyed.
    qDebug() << "Destructor: FinancialReportGenerator destroyed";
}

/**
 * @brief Loads transactions from a CSV file.
 * 
 * This function reads a CSV file, parses each line into a Transaction object,
 * and stores the transactions in the `transactions` list. It skips malformed lines
 * and logs errors or warnings as needed.
 * 
 * @param filename The path to the CSV file to load.
 * @return True if the file was successfully loaded and contains valid transactions, false otherwise.
 * @author Simran Kullar
 */
bool FinancialReportGenerator::loadCSV(const QString& filename) {
    // Check if the file exists and is a valid file
    QFileInfo check_file(filename);
    if (!check_file.exists() || !check_file.isFile()) {
        qDebug() << "Error: File does not exist:" << filename;
        return false; // Return false if the file does not exist or is invalid
    }
    // Open the file for reading
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error: Unable to open file:" << filename << ", Error:" << file.errorString();
        return false; // Return false if the file cannot be opened
    }
    // Create a QTextStream to read the file line by line
    QTextStream in(&file);

    // Log the size of the transactions list before clearing it
    qDebug() << "Before clearing: transactions size =" << transactions.size();
    // Clear the existing transactions list to prepare for new data
    transactions.clear();
    // Log the size of the transactions list after clearing it
    qDebug() << "After clearing: transactions size =" << transactions.size();

    int lineNum = 0; // Track the current line number
    const int maxLines = 100000; // Set a safety limit to prevent infinite loops

    // Read the file line by line until the end or the safety limit is reached
    while (!in.atEnd() && lineNum < maxLines) { 
        QString line = in.readLine(); // Read the next line
        lineNum++; // Increment the line counter

        // Skip empty lines (after trimming whitespace)
        if (line.trimmed().isEmpty()) {
            continue;
        }
        // Split the line into values using commas as the delimiter
        QStringList values = line.split(',');

        // Ensure the line has at least 7 columns 
        if (values.size() < 7) {
            qDebug() << "Skipping incorrectly formed line " << lineNum << ": " << line;
            continue; // Skip lines with insufficient columns
        }

        // Create a new Transaction object and populate its fields
        Transaction txn;
        txn.date = values[0].trimmed(); // Assign the date (first column)
        txn.transactionID = values[1].trimmed(); // Assign the transaction ID (second column)
        txn.type = values[2].trimmed(); // Assign the transaction type (third column)
        txn.description = values[3].trimmed(); // Assign the description (fourth column)

        // Parse the amount (fifth column) and validate it
        bool isAmountValid;
        txn.amount = values[4].trimmed().toDouble(&isAmountValid);
        if (!isAmountValid) {
            qDebug() << "Invalid amount on line " << lineNum << ": " << values[4];
            continue; // Skip lines with invalid amounts
        }
        // Assign the payment method (sixth column) and category (seventh column)
        txn.paymentMethod = values[5].trimmed();
        txn.category = values[6].trimmed();

        // Validate mandatory fields (transactionID, type, description, category)
        if (txn.transactionID.isEmpty() || txn.type.isEmpty() || txn.description.isEmpty() || txn.category.isEmpty()) {
            qDebug() << "Skipping invalid transaction at line " << lineNum;
            continue; // Skip transactions with missing mandatory fields
        }
        // Add the valid transaction to the transactions list
        transactions.append(txn);
    }
    // Close the file after reading
    file.close();
    // Log the total number of transactions loaded
    qDebug() << "Total transactions loaded: " << transactions.size();
    // Return true if at least one transaction was loaded, otherwise false
    return !transactions.isEmpty();
}

/**
 * @brief Generates a Profit & Loss Statement and saves it to a file.
 * 
 * This function calculates the total income, total expenses, and net profit
 * from the loaded transactions and writes the results to the specified output file.
 * 
 * @param outputFilename The path to the file where the Profit & Loss Statement will be saved.
 * @author Simran Kullar
 */
void FinancialReportGenerator::generateProfitLossStatement(const QString& outputFilename) {
    // Check if there are any transactions to process
    if (transactions.isEmpty()) {
        qDebug() << "Error: No transactions available for generating profit/loss statement";
        return; // Exit the function if no transactions are available
    }
    // Initialize variables to store total income and total expenses
    double totalIncome = 0.0;
    double totalExpenses = 0.0;

    // Iterate through all transactions to calculate totals
    for (const auto& txn : transactions) {
        if (txn.type == "Income") {
            // Add the amount to total income if the transaction type is "Income"
            totalIncome += txn.amount;
        } else if (txn.type == "Expense") {
            // Add the amount to total expenses if the transaction type is "Expense"
            totalExpenses += txn.amount;
        }
    }
    // Calculate net profit by subtracting total expenses from total income
    double netProfit = totalIncome - totalExpenses;

    // Open the output file for writing
    QFile file(outputFilename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Error: Could not write to " << outputFilename << ", Error:" << file.errorString();
        return; // Exit the function if the file cannot be opened for writing
    }
    // Create a QTextStream to write the report to the file
    QTextStream out(&file);
    out << "Profit & Loss Statement\n"; // Write the report header
    out << "------------------------\n"; // Write a separator line
    out << "Total Income: $" << totalIncome << "\n"; // Write total income
    out << "Total Expenses: $" << totalExpenses << "\n"; // Write total expenses
    out << "Net Profit: $" << netProfit << "\n"; // Write net profit
    
    file.close(); // Close the file after writing

    // Log a message to confirm that the report was saved successfully
    qDebug() << "Profit & Loss Statement saved to " << outputFilename;
}

/**
 * @brief Generates an Expense Breakdown Report and saves it to a file.
 * 
 * This function categorizes expenses by their category and calculates the total
 * amount spent in each category. The results are written to the specified output file.
 * 
 * @param outputFilename The path to the file where the Expense Breakdown Report will be saved.
 * @author Simran Kullar
 */
void FinancialReportGenerator::generateExpenseBreakdownReport(const QString& outputFilename) {
    // Check if there are any transactions to process
    if (transactions.isEmpty()) {
        qDebug() << "Error: No transactions available for generating expense breakdown";
        return; // Exit the function if no transactions are available
    }
    // Create a QMap to store expense categories and their corresponding totals
    QMap<QString, double> expenseCategories;

    // Iterate through all transactions to categorize expenses
    for (const auto& txn : transactions) {
        if (txn.type == "Expense") {
            // Add the transaction amount to the corresponding category in the QMap
            expenseCategories[txn.category] += txn.amount;
        }
    }
    // Check if any expense transactions were found
    if (expenseCategories.isEmpty()) {
        qDebug() << "Warning: No expense transactions found"; // Log a warning if no expenses were found
    }
    // Open the output file for writing
    QFile file(outputFilename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Error: Could not write to " << outputFilename << ", Error:" << file.errorString();
        return; // Exit the function if the file cannot be opened for writing
    }
    // Create a QTextStream to write the report to the file
    QTextStream out(&file); 
    out << "Expense Breakdown Report\n"; // Write the report header
    out << "------------------------\n"; // Write a separator line

    // Iterate through the QMap and write each category and its total amount to the file
    for (auto it = expenseCategories.constBegin(); it != expenseCategories.constEnd(); ++it) {
        out << it.key() << ": $" << it.value() << "\n"; // Write category and amount
    }
    file.close(); // Close the file after writing

    // Log a message to confirm that the report was saved successfully
    qDebug() << "Expense Breakdown Report saved to " << outputFilename;
}

/**
 * @brief Generates a Balance Sheet Report and saves it to a file.
 * 
 * This function calculates the total assets, total liabilities, and net worth
 * from the loaded transactions and writes the results to the specified output file.
 * 
 * @param outputFilename The path to the file where the Balance Sheet Report will be saved.
 * @author Simran Kullar
 */
void FinancialReportGenerator::generateBalanceSheetReport(const QString& outputFilename) {
    // Check if there are any transactions to process
    if (transactions.isEmpty()) {
        qDebug() << "Error: No transactions available for generating balance sheet";
        return; // Exit the function if no transactions are available
    }
    // Initialize variables to store total assets and total liabilities
    double totalAssets = 0.0;
    double totalLiabilities = 0.0;

    // Iterate through all transactions to calculate totals
    for (const auto& txn : transactions) {
        if (txn.type == "Income") {
            // Add the amount to total assets if the transaction type is "Income"
            totalAssets += txn.amount;
        } else if (txn.type == "Expense") {
            // Add the amount to total liabilities if the transaction type is "Expense"
            totalLiabilities += txn.amount;
        }
    }
    // Calculate net worth by subtracting total liabilities from total assets
    double netWorth = totalAssets - totalLiabilities;

    // Open the output file for writing
    QFile file(outputFilename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Error: Could not write to " << outputFilename << ", Error:" << file.errorString();
        return; // Exit the function if the file cannot be opened for writing
    }
    // Create a QTextStream to write the report to the file
    QTextStream out(&file);
    out << "Balance Sheet Report\n"; // Write the report header
    out << "----------------------\n"; // Write a separator line
    out << "Total Assets: $" << totalAssets << "\n"; // Write total assets
    out << "Total Liabilities: $" << totalLiabilities << "\n"; // Write total liabilities
    out << "Net Worth: $" << netWorth << "\n"; // Write net worth
    file.close(); // Close the file after writing

    // Log a message to confirm that the report was saved successfully
    qDebug() << "Balance Sheet Report saved to " << outputFilename;
}
