#ifndef FINANCIALREPORT_H
#define FINANCIALREPORT_H

#include <QString>
#include <QList>
#include <QMap>

class FinancialReportGenerator {
public:
    // Add constructor and destructor
    FinancialReportGenerator();
    ~FinancialReportGenerator();
    
    struct Transaction {
        QString date;
        QString transactionID;
        QString type;
        QString description;
        double amount;
        QString paymentMethod;
        QString category;
    };

    bool loadCSV(const QString& filename);
    void generateProfitLossStatement(const QString& outputFilename);
    void generateExpenseBreakdownReport(const QString& outputFilename);
    void generateBalanceSheetReport(const QString& outputFilename);

private:
    QList<Transaction> transactions;
};

#endif // FINANCIALREPORT_H
