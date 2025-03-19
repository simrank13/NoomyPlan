#ifndef FINANCIALREPORT_H
#define FINANCIALREPORT_H

#include <QString>
#include <QList>
#include <QMap>

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
class FinancialReportGenerator {
public:
    /**
     * @brief Default constructor for FinancialReportGenerator.
     */
    FinancialReportGenerator();

    /**
     * @brief Destructor for FinancialReportGenerator.
     */
    ~FinancialReportGenerator();

    /**
     * @struct Transaction
     * @brief Represents a financial transaction.
     *
     * This struct holds details of a financial transaction, including date, ID, type,
     * description, amount, payment method, and category.
     */
    struct Transaction {
        QString date; ///< The date of the transaction.
        QString transactionID; ///< The unique ID of the transaction.
        QString type; ///< The type of the transaction (e.g., "Income" or "Expense").
        QString description; ///< A description of the transaction.
        double amount; ///< The amount of the transaction.
        QString paymentMethod; ///< The payment method used for the transaction.
        QString category; ///< The category of the transaction (e.g., "Food", "Transport").
    };

    /**
     * @brief Loads transactions from a CSV file.
     *
     * This function attempts to locate the CSV file in multiple possible locations,
     * including the executable directory, current working directory, and user-specified paths.
     * If the file is found, it reads and parses the transactions, storing them in the `transactions` list.
     *
     * @param filename The name of the CSV file to load.
     * @return True if the file was successfully loaded and contains valid transactions, false otherwise.
     */
    bool loadCSV(const QString& filename);

    /**
     * @brief Generates a Profit & Loss Statement and saves it to a file.
     *
     * This function calculates the total income, total expenses, and net profit
     * from the loaded transactions and writes the results to the specified output file.
     *
     * @param outputFilename The path to the file where the Profit & Loss Statement will be saved.
     */
    void generateProfitLossStatement(const QString& outputFilename);

    /**
     * @brief Generates an Expense Breakdown Report and saves it to a file.
     *
     * This function categorizes expenses by their category and calculates the total
     * amount spent in each category. The results are written to the specified output file.
     *
     * @param outputFilename The path to the file where the Expense Breakdown Report will be saved.
     */
    void generateExpenseBreakdownReport(const QString& outputFilename);

    /**
     * @brief Generates a Balance Sheet Report and saves it to a file.
     *
     * This function calculates the total assets, total liabilities, and net worth
     * from the loaded transactions and writes the results to the specified output file.
     *
     * @param outputFilename The path to the file where the Balance Sheet Report will be saved.
     */
    void generateBalanceSheetReport(const QString& outputFilename);

private:
    QList<Transaction> transactions; ///< List of transactions loaded from the CSV file.
};

#endif // FINANCIALREPORT_H
