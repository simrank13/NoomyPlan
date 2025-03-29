#include "financialreport.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QFileInfo>
#include <QCoreApplication>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

/**
 * @class FinancialReportGenerator
 * @brief Generates financial reports such as Profit & Loss Statements, Expense Breakdowns, and Balance Sheets.
 *
 * This class is responsible for loading financial transactions from a CSV file,
 * processing them, and generating various financial reports. It supports operations
 * such as loading transactions, generating profit/loss statements, expense breakdowns,
 * and balance sheets.
 * 
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
    // Try multiple possible locations for the file
    QStringList possiblePaths;

    // Add the original path provided by the user
    possiblePaths << filename;

    // Add the executable directory path (where the application is running from)
    possiblePaths << QCoreApplication::applicationDirPath() + "/" + filename;

    // Add the current working directory path
    possiblePaths << QDir::currentPath() + "/" + filename;

    // Add the absolute path of the file (resolves relative paths to absolute paths)
    possiblePaths << QFileInfo(filename).absoluteFilePath();

    // Add parent and sibling directories
    QDir currentDir = QDir::current(); // Start with the current directory
    possiblePaths << currentDir.absolutePath() + "/" + filename; // Current directory
    currentDir.cdUp(); // Move to the parent directory
    possiblePaths << currentDir.absolutePath() + "/" + filename; // Parent directory
    possiblePaths << currentDir.absolutePath() + "/test/" + filename; // "test" subdirectory in the parent directory

    // Debug: Print current directory and executable directory for troubleshooting
    qDebug() << "Current directory: " << QDir::currentPath();
    qDebug() << "Executable directory: " << QCoreApplication::applicationDirPath();

    QString filePath; // Variable to store the path of the found file
    bool fileFound = false; // Flag to track if the file was found

    // Check each possible location for the file
    for (const QString& path : possiblePaths) {
        QFileInfo check_file(path); // Create a QFileInfo object to check the file
        qDebug() << "Checking path: " << path << " exists: " << check_file.exists() << " isFile: " << check_file.isFile();

        // If the file exists and is a valid file, set the file path and flag
        if (check_file.exists() && check_file.isFile()) {
            filePath = path;
            fileFound = true;
            qDebug() << "SUCCESS: File found at:" << filePath;
            break; // Exit the loop once the file is found
        }
    }

    // If the file was not found in any of the checked locations
    if (!fileFound) {
        qDebug() << "ERROR: File not found in any of the checked locations:" << filename;
        qDebug() << "Checked paths:";
        for (const QString& path : possiblePaths) {
            qDebug() << " - " << path; // Log all checked paths for debugging
        }

        // Try prompting the user to manually select the file
        QString lastAttemptPath = QFileDialog::getOpenFileName(
            nullptr, // No parent window
            "Select the business_financials.csv file", // Dialog title
            QDir::currentPath(), // Default directory
            "CSV files (*.csv);;All files (*.*)" // File filter
        );

        // If the user selected a file, check if it exists and is valid
        if (!lastAttemptPath.isEmpty()) {
            QFileInfo check_file(lastAttemptPath);
            if (check_file.exists() && check_file.isFile()) {
                filePath = lastAttemptPath;
                fileFound = true;
                qDebug() << "SUCCESS: User selected file:" << filePath;
            }
        }

        // If the file is still not found, return false
        if (!fileFound) {
            return false;
        }
    }

    // Open the file for reading
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error: Unable to open file:" << filePath << ", Error:" << file.errorString();
        return false; // Return false if the file cannot be opened
    }

    QTextStream in(&file); // Create a QTextStream to read the file line by line

    // Clear the existing transactions list safely
    qDebug() << "Before clearing: transactions size =" << transactions.size();
    transactions.clear(); // Clear the list
    qDebug() << "After clearing: transactions size =" << transactions.size();

    int lineNum = 0; // Track the current line number
    const int maxLines = 100000; // Safety limit to prevent infinite loops

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

        // Ensure the line has at least 7 columns (expected format)
        if (values.size() < 7) {
            qDebug() << "Skipping malformed line " << lineNum << ": " << line;
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

        txn.paymentMethod = values[5].trimmed(); // Assign the payment method (sixth column)
        txn.category = values[6].trimmed(); // Assign the category (seventh column)

        // Validate mandatory fields (transactionID, type, description, category)
        if (txn.transactionID.isEmpty() || txn.type.isEmpty() ||
            txn.description.isEmpty() || txn.category.isEmpty()) {
            qDebug() << "Skipping invalid transaction at line " << lineNum;
            continue; // Skip transactions with missing mandatory fields
        }

        // Add the valid transaction to the transactions list
        transactions.append(txn);
    }

    file.close(); // Close the file after reading
    qDebug() << "Total transactions loaded: " << transactions.size(); // Log the total number of transactions loaded
    return !transactions.isEmpty(); // Return true if at least one transaction was loaded, otherwise false
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
        }
        else if (txn.type == "Expense") {
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

    // Close the file after writing
    file.close();

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

    // Close the file after writing
    file.close();

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
        }
        else if (txn.type == "Expense") {
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

    // Close the file after writing
    file.close();

    // Log a message to confirm that the report was saved successfully
    qDebug() << "Balance Sheet Report saved to " << outputFilename;
}