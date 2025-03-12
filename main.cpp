#include <QApplication>
#include <QMainWindow>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QLineEdit>
#include <QMessageBox>
#include <QHeaderView>
#include "loginpage.h"
#include "authenticatesystem.h"
#include "financialreport.h"
#include "cashflowtracking.h"
#include "budgetpage.h"

class MainWindow : public QMainWindow {
private:
    // Make reportGenerator a member variable so it persists throughout the object's lifetime
    FinancialReportGenerator reportGenerator;

public:
    MainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        setWindowTitle("Business Management System");
        resize(900, 650);

        QWidget *centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);

        QTabWidget *tabWidget = new QTabWidget(centralWidget);

        QWidget *dashboardPage = new QWidget();
        QWidget *financialsPage = new QWidget();
        QWidget *inventoryPage = new QWidget();
        BudgetPage *budgetPage = new BudgetPage();
        QWidget *cashflowTrackingPage = new QWidget();

        tabWidget->setCurrentIndex(0);
        tabWidget->setMovable(false);
        tabWidget->setTabsClosable(false);

        tabWidget->addTab(dashboardPage, "Dashboard");
        tabWidget->addTab(financialsPage, "Financials");
        tabWidget->addTab(inventoryPage, "Inventory");
        tabWidget->addTab(budgetPage, "Budget");
        tabWidget->addTab(cashflowTrackingPage, "Cashflow Tracking");

        QVBoxLayout *layout = new QVBoxLayout(centralWidget);
        layout->addWidget(tabWidget);
        centralWidget->setLayout(layout);

        QLabel *dashboardLabel = new QLabel("Dashboard Content", dashboardPage);
        QVBoxLayout *dashboardLayout = new QVBoxLayout(dashboardPage);
        dashboardLayout->addWidget(dashboardLabel);
        dashboardPage->setLayout(dashboardLayout);

        QLabel *financialsLabel = new QLabel("Financial Reports", financialsPage);
        QPushButton *profitLossButton = new QPushButton("Generate Profit & Loss Report", financialsPage);
        QPushButton *expenseBreakdownButton = new QPushButton("Generate Expense Breakdown", financialsPage);
        QPushButton *balanceSheetButton = new QPushButton("Generate Balance Sheet", financialsPage);

        QVBoxLayout *financialsLayout = new QVBoxLayout(financialsPage);
        financialsLayout->addWidget(financialsLabel);
        financialsLayout->addWidget(profitLossButton);
        financialsLayout->addWidget(expenseBreakdownButton);
        financialsLayout->addWidget(balanceSheetButton);
        financialsPage->setLayout(financialsLayout);
        
        // Using a member pointer to reportGenerator so it lives as long as MainWindow
        connect(profitLossButton, &QPushButton::clicked, this, [this]() {
            try {
                if (reportGenerator.loadCSV("business_financials.csv")) {
                    reportGenerator.generateProfitLossStatement("profit_loss_statement.csv");
                    QMessageBox::information(this, "Success", "Profit & Loss statement generated");
                } else {
                    QMessageBox::warning(this, "Error", "Failed to load financial data");
                }
            } catch (const std::exception& e) {
                QMessageBox::critical(this, "Error", QString("An error occurred: %1").arg(e.what()));
            }
        });
        
        connect(expenseBreakdownButton, &QPushButton::clicked, this, [this]() {
            try {
                if (reportGenerator.loadCSV("business_financials.csv")) {
                    reportGenerator.generateExpenseBreakdownReport("expense_breakdown_report.csv");
                    QMessageBox::information(this, "Success", "Expense breakdown report generated");
                } else {
                    QMessageBox::warning(this, "Error", "Failed to load financial data");
                }
            } catch (const std::exception& e) {
                QMessageBox::critical(this, "Error", QString("An error occurred: %1").arg(e.what()));
            }
        });
        
        connect(balanceSheetButton, &QPushButton::clicked, this, [this]() {
            try {
                if (reportGenerator.loadCSV("business_financials.csv")) {
                    reportGenerator.generateBalanceSheetReport("balance_sheet_report.csv");
                    QMessageBox::information(this, "Success", "Balance sheet report generated");
                } else {
                    QMessageBox::warning(this, "Error", "Failed to load financial data");
                }
            } catch (const std::exception& e) {
                QMessageBox::critical(this, "Error", QString("An error occurred: %1").arg(e.what()));
            }
        });

        connect(tabWidget, &QTabWidget::currentChanged, this, [=](int index) {
            qDebug() << "Switched to tab:" << index;
            if (index == 3) {  // Budget tab index
                budgetPage->update();
                budgetPage->repaint();
                budgetPage->layout()->update();
            }
        });
        
        // --- Cashflow Tracking ---
        QVBoxLayout *cashflowLayout = new QVBoxLayout(cashflowTrackingPage);
        QTableWidget *tableWidget = new QTableWidget(cashflowTrackingPage);
        tableWidget->setColumnCount(6);
        tableWidget->setHorizontalHeaderLabels({"ID", "Date/Time", "Seller", "Buyer", "Merchandise", "Cost", "Category"});
        tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        QLineEdit *dateTimeEdit = new QLineEdit();
        QLineEdit *sellerEdit = new QLineEdit();
        QLineEdit *buyerEdit = new QLineEdit();
        QLineEdit *merchandiseEdit = new QLineEdit();
        QLineEdit *costEdit = new QLineEdit();
        QLineEdit *categoryEdit = new QLineEdit();
        QLineEdit *searchEdit = new QLineEdit();

        QPushButton *addButton = new QPushButton("Add Transaction");
        QPushButton *searchButton = new QPushButton("Search Transaction");

        cashflowLayout->addWidget(tableWidget);
        cashflowLayout->addWidget(new QLabel("Date/Time (yyyy-mm-dd, hh:mm):"));
        cashflowLayout->addWidget(dateTimeEdit);
        cashflowLayout->addWidget(new QLabel("Seller:"));
        cashflowLayout->addWidget(sellerEdit);
        cashflowLayout->addWidget(new QLabel("Buyer:"));
        cashflowLayout->addWidget(buyerEdit);
        cashflowLayout->addWidget(new QLabel("Merchandise:"));
        cashflowLayout->addWidget(merchandiseEdit);
        cashflowLayout->addWidget(new QLabel("Cost:"));
        cashflowLayout->addWidget(costEdit);
        cashflowLayout->addWidget(new QLabel("Category:"));
        cashflowLayout->addWidget(categoryEdit);
        cashflowLayout->addWidget(addButton);
        cashflowLayout->addWidget(new QLabel("Search Transaction ID:"));
        cashflowLayout->addWidget(searchEdit);
        cashflowLayout->addWidget(searchButton);

        cashflowTrackingPage->setLayout(cashflowLayout);
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    AuthenticateSystem authSystem;
    MainWindow mainWindow;
    LoginPage loginPage(&authSystem, &mainWindow);
    loginPage.show();

    return app.exec();
}
