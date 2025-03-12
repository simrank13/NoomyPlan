#include "financialreport.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QFileInfo>

FinancialReportGenerator::FinancialReportGenerator() {
    // Explicitly initialize transactions as empty
    transactions = QList<Transaction>();
    qDebug() << "Constructor: FinancialReportGenerator initialized, transactions size =" << transactions.size();
}

FinancialReportGenerator::~FinancialReportGenerator() {
    // QList handles its own cleanup, no action needed
    qDebug() << "Destructor: FinancialReportGenerator destroyed";
}

bool FinancialReportGenerator::loadCSV(const QString& filename) {
    // First check if file exists
    QFileInfo check_file(filename);
    if (!check_file.exists() || !check_file.isFile()) {
        qDebug() << "Error: File does not exist:" << filename;
        return false;
    }
    
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error: Unable to open file:" << filename << ", Error:" << file.errorString();
        return false;
    }

    QTextStream in(&file);

    // Clear transactions safely
    qDebug() << "Before clearing: transactions size =" << transactions.size();
    transactions.clear();
    qDebug() << "After clearing: transactions size =" << transactions.size();

    int lineNum = 0;
    const int maxLines = 100000; // Safety limit

    while (!in.atEnd() && lineNum < maxLines) {
        QString line = in.readLine();
        lineNum++;

        // Skip empty lines
        if (line.trimmed().isEmpty()) {
            continue;
        }

        QStringList values = line.split(',');

        // Ensure correct number of columns
        if (values.size() < 7) {
            qDebug() << "Skipping malformed line " << lineNum << ": " << line;
            continue;
        }

        // Create and populate transaction
        Transaction txn;
        txn.date = values[0].trimmed();
        txn.transactionID = values[1].trimmed();
        txn.type = values[2].trimmed();
        txn.description = values[3].trimmed();

        // Parse amount
        bool isAmountValid;
        txn.amount = values[4].trimmed().toDouble(&isAmountValid);
        if (!isAmountValid) {
            qDebug() << "Invalid amount on line " << lineNum << ": " << values[4];
            continue;
        }

        txn.paymentMethod = values[5].trimmed();
        txn.category = values[6].trimmed();

        // Validate mandatory fields
        if (txn.transactionID.isEmpty() || txn.type.isEmpty() ||
            txn.description.isEmpty() || txn.category.isEmpty()) {
            qDebug() << "Skipping invalid transaction at line " << lineNum;
            continue;
        }

        // Add transaction to list
        transactions.append(txn);
    }

    file.close();
    qDebug() << "Total transactions loaded: " << transactions.size();
    return !transactions.isEmpty();
}

void FinancialReportGenerator::generateProfitLossStatement(const QString& outputFilename) {
    if (transactions.isEmpty()) {
        qDebug() << "Error: No transactions available for generating profit/loss statement";
        return;
    }

    double totalIncome = 0.0;
    double totalExpenses = 0.0;

    for (const auto& txn : transactions) {
        if (txn.type == "Income") {
            totalIncome += txn.amount;
        } else if (txn.type == "Expense") {
            totalExpenses += txn.amount;
        }
    }

    double netProfit = totalIncome - totalExpenses;

    QFile file(outputFilename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Error: Could not write to " << outputFilename << ", Error:" << file.errorString();
        return;
    }

    QTextStream out(&file);
    out << "Profit & Loss Statement\n";
    out << "------------------------\n";
    out << "Total Income: $" << totalIncome << "\n";
    out << "Total Expenses: $" << totalExpenses << "\n";
    out << "Net Profit: $" << netProfit << "\n";
    file.close();

    qDebug() << "Profit & Loss Statement saved to " << outputFilename;
}

void FinancialReportGenerator::generateExpenseBreakdownReport(const QString& outputFilename) {
    if (transactions.isEmpty()) {
        qDebug() << "Error: No transactions available for generating expense breakdown";
        return;
    }

    QMap<QString, double> expenseCategories;

    for (const auto& txn : transactions) {
        if (txn.type == "Expense") {
            expenseCategories[txn.category] += txn.amount;
        }
    }

    if (expenseCategories.isEmpty()) {
        qDebug() << "Warning: No expense transactions found";
    }

    QFile file(outputFilename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Error: Could not write to " << outputFilename << ", Error:" << file.errorString();
        return;
    }

    QTextStream out(&file);
    out << "Expense Breakdown Report\n";
    out << "------------------------\n";
    for (auto it = expenseCategories.constBegin(); it != expenseCategories.constEnd(); ++it) {
        out << it.key() << ": $" << it.value() << "\n";
    }
    file.close();

    qDebug() << "Expense Breakdown Report saved to " << outputFilename;
}

void FinancialReportGenerator::generateBalanceSheetReport(const QString& outputFilename) {
    if (transactions.isEmpty()) {
        qDebug() << "Error: No transactions available for generating balance sheet";
        return;
    }

    double totalAssets = 0.0;
    double totalLiabilities = 0.0;

    for (const auto& txn : transactions) {
        if (txn.type == "Income") {
            totalAssets += txn.amount;
        } else if (txn.type == "Expense") {
            totalLiabilities += txn.amount;
        }
    }

    double netWorth = totalAssets - totalLiabilities;

    QFile file(outputFilename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Error: Could not write to " << outputFilename << ", Error:" << file.errorString();
        return;
    }

    QTextStream out(&file);
    out << "Balance Sheet Report\n";
    out << "----------------------\n";
    out << "Total Assets: $" << totalAssets << "\n";
    out << "Total Liabilities: $" << totalLiabilities << "\n";
    out << "Net Worth: $" << netWorth << "\n";
    file.close();

    qDebug() << "Balance Sheet Report saved to " << outputFilename;
}
