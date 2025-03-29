#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include "authenticatesystem.h"
#include "financialreport.h"
#include "cashflowtracking.h"
#include "dashboard.h"
#include "loginpage.h"
#include "inventory.h"


class QPushButton;
class QResizeEvent;
class QLineEdit;
class QTableWidget;
class AuthenticateSystem;
class LoginPage;
class FinancialReportGenerator;
class CashflowTracking;
class Dashboard;

/**
 * @class main.cpp
 * @brief Main application window for the Business Management System.
 *
 * This file contains the implementation of the MainWindow class, which serves as the central
 * interface for managing financial reports, cashflow tracking, budgets, and inventory.
 * It integrates various components such as FinancialReportGenerator, CashflowTracking, and
 * InventoryPage to provide a comprehensive business management solution.
 *
 */

 /**
  * @brief Main application window for the Business Management System.
  *
  * This class manages the main UI of the application, including tabs for Dashboard, Financials,
  * Budget, Cashflow Tracking, and Inventory. It integrates various components to provide
  * functionality for adding transactions, generating reports, and managing inventory.
  *
  * @author Simran Kullar
  *         Abbygail Latimer
  *         Kareena Sen
  *         Maneet Chahal
  *         Katherine R
  */
class MainWindow : public QMainWindow {
    Q_OBJECT
private:
    FinancialReportGenerator reportGenerator; // Handles financial report generation (e.g., profit/loss, balance sheet)
    CashflowTracking cashflowTracking; // Manages cashflow transactions and data entries
    QTableWidget* tableWidget; // Displays cashflow transactions in a table format
    QLineEdit* dateTimeEdit; // Input field for transaction date and time
    QLineEdit* sellerEdit; // Input field for the seller's name
    QLineEdit* buyerEdit; // Input field for the buyer's name
    QLineEdit* merchandiseEdit; // Input field for the merchandise description
    QLineEdit* costEdit; // Input field for the transaction cost
    QLineEdit* categoryEdit; // Input field for the transaction category
    QLineEdit* searchEdit; // Input field for searching transactions by ID
    QPushButton* logoutButton; // Logout button
    AuthenticateSystem* authSystem; // Pointer to the authentication system
    LoginPage* loginPage; // Pointer to the login page
    QString currentUserId;
    Dashboard* dashboardPage;


public:
    explicit MainWindow(AuthenticateSystem* authSystem, QWidget* parent = nullptr);
    ~MainWindow();
    void setCurrentUserId(const QString& id);
    void saveTransactions();
    void loadTransactions(const QString& userId);


private slots:
    /**
     * @brief Adds a new transaction entry.
     */
    void addTransaction();
    /**
     * @brief Updates the transaction table with current data.
     */
    void updateTransactionTable();
    /**
     * @brief Searches for a transaction by ID.
     */
    void searchTransaction();
    /**
     * @brief Exports transactions between two dates to a CSV file.
     */
    void generateReport();

    void handleLogout();

protected:
    void resizeEvent(QResizeEvent* event) override;

};
#endif // MAINWINDOW_H
