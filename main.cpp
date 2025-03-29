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
#include <QDialog>
#include <QFormLayout>
#include <QFileDialog>
#include <QDateTime>
#include <QTextStream>
#include <QStackedWidget>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include "loginpage.h"
#include "authenticatesystem.h"
#include "financialreport.h"
#include "cashflowtracking.h"
#include "budgetpage.h"
#include "dashboard.h"
#include "inventory.h"
#include "mainwindow.h"

/**
 * @brief Constructor for MainWindow.
 *
 * Initializes the main application window, sets up the UI components, and connects signals
 * and slots for navigation and functionality. It also configures the tab widget and integrates
 * various pages such as Dashboard, Financials, Budget, and Cashflow Tracking.
 *
 * @param parent The parent widget, default is nullptr.
 *
 * @author Simran Kullar
  *         Abbygail Latimer
  *         Kareena Sen
  *         Maneet Chahal
  *         Katherine R
 */
MainWindow::MainWindow(AuthenticateSystem* authSystem, QWidget* parent)
    : QMainWindow(parent), authSystem(authSystem) {
    setWindowTitle("Business Management System"); // Set window title and size
    resize(900, 650);

    QWidget* centralWidget = new QWidget(this); // Create a central widget for the main window
    setCentralWidget(centralWidget); // Set the central widget for the main window

    QTabWidget* tabWidget = new QTabWidget(centralWidget); // Create a tab widget for navigation
    QTableWidget* sharedInventoryTable = new QTableWidget(0, 3); // Create a shared inventory table with 0 rows and 3 columns

    dashboardPage = new Dashboard();  // Create the dashboard widget
    QWidget* financialsPage = new QWidget(); // Create the financials page widget
    BudgetPage* budgetPage = new BudgetPage(); // Create the budget page widget
    QWidget* cashflowTrackingPage = new QWidget(); // Create the cashflow tracking page widget

    dashboardPage->setCurrentUserId(currentUserId); // assuming it was set before launching MainWindow
    dashboardPage->loadMonthlyRevenueData(currentUserId);

    budgetPage->setCurrentUserId(currentUserId);

    dashboardPage->setInventoryTable(sharedInventoryTable); // Set the shared inventory table for the dashboard


    InventoryPage* inventoryPage = new InventoryPage(); // Create the inventory page widget
    inventoryPage->setTabWidget(tabWidget); // Set the tab widget for the inventory page
    inventoryPage->setInventoryTable(sharedInventoryTable); // Set the shared inventory table for the inventory page

    // Let Dashboard switch tabs
    dashboardPage->setTabWidget(tabWidget); // Allow the dashboard to control tab navigation

    tabWidget->setCurrentIndex(0); // Set the default tab to the first one (Dashboard)
    tabWidget->setMovable(false); // Disable the ability to move tabs
    tabWidget->setTabsClosable(false); // Disable the ability to close tabs

    // Add tabs to the tab widget with appropriate labels
    tabWidget->addTab(dashboardPage, "Dashboard"); // Add the Dashboard tab
    tabWidget->addTab(financialsPage, "Financials"); // Add the Financials tab
    tabWidget->addTab(budgetPage, "Budget"); // Add the Budget tab
    tabWidget->addTab(cashflowTrackingPage, "Cashflow Tracking"); // Add the Cashflow Tracking tab
    tabWidget->addTab(inventoryPage, "Inventory"); // Add the Inventory tab

    // Create a vertical layout for the main window
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(tabWidget); // Add the tab widget to the layout
    centralWidget->setLayout(layout); // Set the layout for the central widget


    dashboardPage->setTabWidget(tabWidget);  // Pass the tab widget to the dashboard for navigation control

    // Financials tab
    QVBoxLayout* financialsLayout = new QVBoxLayout(financialsPage); // Main vertical layout

    // Title
    QLabel* financialsLabel = new QLabel("Financial Reports", financialsPage);
    financialsLabel->setAlignment(Qt::AlignCenter);
    financialsLabel->setStyleSheet("font-size: 21px; font-weight: 600;");
    financialsLayout->addSpacing(8);
    financialsLayout->addWidget(financialsLabel);
    financialsLayout->addSpacing(25);

    // Buttons (in a row)
    QPushButton* profitLossButton = new QPushButton("Generate Profit & Loss Report", financialsPage);
    QPushButton* expenseBreakdownButton = new QPushButton("Generate Expense Breakdown", financialsPage);
    QPushButton* balanceSheetButton = new QPushButton("Generate Balance Sheet", financialsPage);

    profitLossButton->setFixedWidth(220);
    expenseBreakdownButton->setFixedWidth(220);
    balanceSheetButton->setFixedWidth(220);


    QHBoxLayout* buttonRow = new QHBoxLayout();
    buttonRow->addStretch(); // optional: push buttons to center
    buttonRow->addWidget(profitLossButton);
    buttonRow->addSpacing(20); // space between buttons
    buttonRow->addWidget(expenseBreakdownButton);
    buttonRow->addSpacing(20);
    buttonRow->addWidget(balanceSheetButton);
    buttonRow->addStretch();

    financialsLayout->addLayout(buttonRow);

    // Add spacer to push everything up
    financialsLayout->addStretch();

    financialsPage->setLayout(financialsLayout);


    // Create a button in the top-left corner
    logoutButton = new QPushButton("Logout", this);
    
    // Position it in the top-right corner
    int buttonWidth = 100;
    int buttonHeight = 25;
    int margin = 10;
    logoutButton->setGeometry(width() - buttonWidth - margin, margin - 5, buttonWidth, buttonHeight);
    connect(logoutButton, &QPushButton::clicked, this, &MainWindow::handleLogout);


    // Connect the profit/loss button to generate and display a profit/loss report
    connect(profitLossButton, &QPushButton::clicked, this, [this]() {
        try {
            if (reportGenerator.loadCSV("business_financials.csv")) {
                reportGenerator.generateProfitLossStatement("profit_loss_statement.csv");
                QMessageBox::information(this, "Success", "Profit & Loss statement generated");
            }
            else {
                QMessageBox::warning(this, "Error", "Failed to load financial data");
            }
        }
        catch (const std::exception& e) {
            QMessageBox::critical(this, "Error", QString("An error occurred: %1").arg(e.what()));
        }
        });

    // Connect the expense breakdown button to generate and display a expense breakdown report
    connect(expenseBreakdownButton, &QPushButton::clicked, this, [this]() {
        try {
            if (reportGenerator.loadCSV("business_financials.csv")) {
                reportGenerator.generateExpenseBreakdownReport("expense_breakdown_report.csv");
                QMessageBox::information(this, "Success", "Expense breakdown report generated");
            }
            else {
                QMessageBox::warning(this, "Error", "Failed to load financial data");
            }
        }
        catch (const std::exception& e) {
            QMessageBox::critical(this, "Error", QString("An error occurred: %1").arg(e.what()));
        }
        });

    // Connect the balance sheet button to generate and display a balance sheet report
    connect(balanceSheetButton, &QPushButton::clicked, this, [this]() {
        try {
            if (reportGenerator.loadCSV("business_financials.csv")) {
                reportGenerator.generateBalanceSheetReport("balance_sheet_report.csv");
                QMessageBox::information(this, "Success", "Balance sheet report generated");
            }
            else {
                QMessageBox::warning(this, "Error", "Failed to load financial data");
            }
        }
        catch (const std::exception& e) {
            QMessageBox::critical(this, "Error", QString("An error occurred: %1").arg(e.what()));
        }
        });

    // Connect tab changes to update the budget page when the Budget tab is selected
    connect(tabWidget, &QTabWidget::currentChanged, this, [=](int index) {
        qDebug() << "Switched to tab:" << index;
        if (index == 3) {  // Budget tab
            budgetPage->update();
            budgetPage->repaint();
            budgetPage->layout()->update();
        }
        });


    // Cashflow Tracking tab
    //create cashflow tracking tab and table to store all transactions
    QVBoxLayout* cashflowLayout = new QVBoxLayout(cashflowTrackingPage);

    //two buttons that run the addtransaction and searchtransaction functions and export button for exportinh
    QPushButton* addButton = new QPushButton("Add Transaction");
    QPushButton* searchButton = new QPushButton("Search Transaction");
    QPushButton* exportButton = new QPushButton("Generate Report");

    QLabel* titleLabel = new QLabel("Cashflow Tracking", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 21px; font-weight: 600;");
    cashflowLayout->addSpacing(8);
    cashflowLayout->addWidget(titleLabel);

    tableWidget = new QTableWidget(cashflowTrackingPage);
    tableWidget->setColumnCount(7);
    tableWidget->setHorizontalHeaderLabels({ "ID", "Date/Time", "Seller", "Buyer", "Merchandise", "Cost", "Category" });
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // transaction input fields
    dateTimeEdit = new QLineEdit();
    sellerEdit = new QLineEdit();
    buyerEdit = new QLineEdit();
    merchandiseEdit = new QLineEdit();
    costEdit = new QLineEdit();
    categoryEdit = new QLineEdit();
    searchEdit = new QLineEdit();

    //input widgets and layout
    // Create a horizontal layout for buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(searchButton);
    buttonLayout->addWidget(exportButton);

    // Add the button layout to the main vertical layout
    cashflowLayout->addLayout(buttonLayout);

    cashflowLayout->addWidget(tableWidget);
    cashflowTrackingPage->setLayout(cashflowLayout);

    //connect buttons
    connect(addButton, &QPushButton::clicked, this, &MainWindow::addTransaction);
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::searchTransaction);
    connect(exportButton, &QPushButton::clicked, this, &MainWindow::generateReport);
}

/**
 * @brief Destructor for MainWindow.
 *
 * Cleans up resources used by the MainWindow instance. Qt automatically handles the deletion
 * of child widgets, so no manual cleanup is required.
 *
 * @author Simran Kullar
 */
MainWindow::~MainWindow() {
    // Nothing needs to be deleted manually because Qt handles child QObjects
}

void MainWindow::handleLogout() {
    QMessageBox::StandardButton confirm = QMessageBox::question(
        this, "Logout", "Are you sure you want to logout?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (confirm == QMessageBox::Yes) {
        this->hide();  // 👈 don't close MainWindow

        LoginPage* loginPage = new LoginPage(authSystem, this); // 👈 pass THIS main window back
        loginPage->show();
    }
}



/**
 * @brief Adds a new transaction to the system.
 *
 * Opens a dialog where users can enter transaction details. If all required fields are filled,
 * the transaction is stored in CashflowTracking and the transaction table is updated.
 *
 * @author Abbygail Latimer
 */
void MainWindow::addTransaction() {
    //open add transaction dialogue
    QDialog dialog(this);
    dialog.setWindowTitle("Add Transaction");
    QFormLayout form(&dialog);

    //allow user to input date
    QLineEdit dateTimeEdit, sellerEdit, buyerEdit, merchandiseEdit, costEdit,
        categoryEdit; form.addRow("Enter date and time (yyyy-mm-dd, hh:mm):", &dateTimeEdit);
    form.addRow("Enter seller:", &sellerEdit); form.addRow("Enter buyer:", &buyerEdit);
    form.addRow("Enter purchased merchandise:", &merchandiseEdit); form.addRow("Enter cost:", &costEdit);
    form.addRow("Enter category:", &categoryEdit);

    //button to add transaction
    QPushButton submitButton("Add", &dialog);
    form.addRow(&submitButton);

    //connect button
    connect(&submitButton, &QPushButton::clicked, [&]() {
        QString dateTime = dateTimeEdit.text().trimmed();
        QString seller = sellerEdit.text().trimmed();
        QString buyer = buyerEdit.text().trimmed();
        QString merchandise = merchandiseEdit.text().trimmed();
        double cost = costEdit.text().toDouble();
        QString category = categoryEdit.text().trimmed();

        //if any data is empty give error
        if (dateTime.isEmpty() || seller.isEmpty() || buyer.isEmpty() || merchandise.isEmpty() || category.isEmpty()) {
            QMessageBox::warning(&dialog, "Input Error", "All fields must be filled!");
            return;
        }

        //update table
        cashflowTracking.addData(dateTime.toStdString(), seller.toStdString(), buyer.toStdString(), merchandise.toStdString(), cost, category.toStdString());
        updateTransactionTable();
        dialog.accept();
        });

    saveTransactions();

    dialog.exec();
}
/**
 * @brief Updates the transaction table with the latest data.
 *
 * Retrieves all transactions from CashflowTracking and populates the table widget
 * with the transaction details.
 *
 * @author Abbygail Latimer
 */
void MainWindow::updateTransactionTable() {
    // Populate the transaction table with data from CashflowTracking
    tableWidget->setRowCount(0); // Clear the table by setting the row count to 0

    // Retrieve all data entries from the CashflowTracking object
    std::vector<DataEntry> transactions = cashflowTracking.getAllDataEntries();

    // Iterate through each transaction entry
    for (const auto& entry : transactions) {
        // Get the current row count and insert a new row at the end
        int row = tableWidget->rowCount();
        tableWidget->insertRow(row);

        // Populate the table with data from the current entry
        tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(entry.transid))); // Transaction ID
        tableWidget->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(entry.dateTime))); // Date and Time
        tableWidget->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(entry.seller))); // Seller
        tableWidget->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(entry.buyer))); // Buyer
        tableWidget->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(entry.merchandise))); // Merchandise
        tableWidget->setItem(row, 5, new QTableWidgetItem(QString::number(entry.cost, 'f', 2))); // Cost (formatted to 2 decimal places)
        tableWidget->setItem(row, 6, new QTableWidgetItem(QString::fromStdString(entry.category))); // Category
    }
    saveTransactions();

}
/**
 * @brief Searches for a transaction by ID.
 *
 * Opens a dialog where users can input a transaction ID to search for a specific transaction.
 * If found, the transaction details are displayed in a message box.
 *
 * @author Abbygail Latimer
 */
void MainWindow::searchTransaction() {
    // Create a QDialog window with 'this' as its parent
    QDialog dialog(this);
    dialog.setWindowTitle("Search Transaction"); // Set the title of the dialog window

    // Create a form layout to organize the widgets in the dialog
    QFormLayout form(&dialog);

    // Create a QLineEdit widget for entering the transaction ID
    QLineEdit searchEdit;
    form.addRow("Transaction ID:", &searchEdit); // Add the QLineEdit to the form with a label

    // Create a QPushButton for initiating the search
    QPushButton searchButton("Search", &dialog);
    form.addRow(&searchButton); // Add the search button to the form

    // Connect the search button's clicked signal to a lambda function
    connect(&searchButton, &QPushButton::clicked, [&]() {
        // Retrieve the text from the QLineEdit and convert it to an integer (transaction ID)
        int searchID = searchEdit.text().toInt();

        // Search for the transaction in the CashflowTracking object using the entered ID
        DataEntry* transaction = cashflowTracking.searchDataEntries(searchID);

        // Check if the transaction was found
        if (!transaction) {
            // If not found, show a warning message box
            QMessageBox::warning(&dialog, "Search Error", "Transaction not found!");
        }
        else {
            // If found, format the transaction details into a QString
            QString details = QString("ID: %1\nDate: %2\nSeller: %3\nBuyer: %4\nMerchandise: %5\nCost: %6\nCategory: %7")
                .arg(transaction->transid) // Transaction ID
                .arg(QString::fromStdString(transaction->dateTime)) // Date and Time
                .arg(QString::fromStdString(transaction->seller)) // Seller
                .arg(QString::fromStdString(transaction->buyer)) // Buyer
                .arg(QString::fromStdString(transaction->merchandise)) // Merchandise
                .arg(transaction->cost, 0, 'f', 2) // Cost (formatted to 2 decimal places)
                .arg(QString::fromStdString(transaction->category)); // Category

            // Show the transaction details in an information message box
            QMessageBox::information(&dialog, "Transaction Found", details);
        }

        // Close the dialog after the search is complete
        dialog.accept();
        });

    saveTransactions();

    // Display the dialog and wait for user interaction
    dialog.exec();
}

void MainWindow::setCurrentUserId(const QString& id) {
    currentUserId = id;

    dashboardPage->setCurrentUserId(id);
    dashboardPage->loadMonthlyRevenueData(id);

    loadTransactions(id);  // load transactions when user logs in
}


/**
 * @brief Generates a financial report for cashflow tracking.
 *
 * Allows users to select a date range and category, then exports transactions within that range
 * to a CSV file. The exported file contains details such as ID, date/time, seller, buyer,
 * merchandise, cost, and category.
 *
 * @author Abbygail Latimer
 */
void MainWindow::generateReport() {
    // Create a QDialog window with 'this' as its parent
    QDialog dialog(this);
    dialog.setWindowTitle("Generate Report"); // Set the title of the dialog window

    // Create a form layout to organize the widgets in the dialog
    QFormLayout form(&dialog);

    // Create QLineEdit widgets for start date, end date, and category filter
    QLineEdit startDateEdit, endDateEdit, categoryEdit;

    // Set placeholder text for the input fields
    startDateEdit.setPlaceholderText("YYYY-MM-DD"); // Hint for start date format
    endDateEdit.setPlaceholderText("YYYY-MM-DD");   // Hint for end date format
    categoryEdit.setPlaceholderText("(Optional)");  // Hint that category is optional

    // Add the input fields to the form with labels
    form.addRow("Start Date:", &startDateEdit);
    form.addRow("End Date:", &endDateEdit);
    form.addRow("Category:", &categoryEdit);

    // Create a QPushButton for exporting the report
    QPushButton exportButton("Export", &dialog);
    form.addRow(&exportButton); // Add the export button to the form

    // Connect the export button's clicked signal to a lambda function
    connect(&exportButton, &QPushButton::clicked, [&]() {
        // Retrieve and trim the input values
        QString startDate = startDateEdit.text().trimmed(); // Start date input
        QString endDate = endDateEdit.text().trimmed();     // End date input
        QString categoryFilter = categoryEdit.text().trimmed().toLower(); // Category filter (optional)

        // Validate that both date fields are filled
        if (startDate.isEmpty() || endDate.isEmpty()) {
            QMessageBox::warning(&dialog, "Input Error", "Both date fields must be filled!");
            return; // Exit the function if validation fails
        }

        // Parse the start and end dates from the input strings
        QDate start = QDate::fromString(startDate, "yyyy-MM-dd");
        QDate end = QDate::fromString(endDate, "yyyy-MM-dd");

        // Validate the parsed dates
        if (!start.isValid() || !end.isValid() || start > end) {
            QMessageBox::warning(&dialog, "Date Error", "Invalid date range!");
            return; // Exit the function if the date range is invalid
        }

        // Open a file dialog to let the user choose where to save the CSV file
        QString filePath = QFileDialog::getSaveFileName(this, "Save CSV", "", "CSV Files (*.csv)");
        if (filePath.isEmpty()) return; // Exit if the user cancels the file dialog

        // Open the selected file for writing
        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(&dialog, "File Error", "Unable to open file for writing.");
            return; // Exit if the file cannot be opened
        }

        // Create a QTextStream to write to the file
        QTextStream out(&file);
        out << "ID,Date/Time,Seller,Buyer,Merchandise,Cost,Category\n"; // Write the CSV header

        // Retrieve all transactions from the CashflowTracking object
        std::vector<DataEntry> transactions = cashflowTracking.getAllDataEntries();

        // Iterate through each transaction
        for (const auto& entry : transactions) {
            // Extract the date from the transaction's dateTime string
            QDate entryDate = QDate::fromString(QString::fromStdString(entry.dateTime).split(",")[0], "yyyy-MM-dd");
            // Convert the transaction's category to lowercase for comparison
            QString entryCategory = QString::fromStdString(entry.category).toLower();

            // Check if the transaction falls within the specified date range
            bool inDateRange = entryDate.isValid() && entryDate >= start && entryDate <= end;
            // Check if the transaction matches the category filter (if provided)
            bool categoryMatches = categoryFilter.isEmpty() || entryCategory == categoryFilter;

            // If both conditions are met, write the transaction to the CSV file
            if (inDateRange && categoryMatches) {
                out << entry.transid << ","
                    << QString::fromStdString(entry.dateTime) << ","
                    << QString::fromStdString(entry.seller) << ","
                    << QString::fromStdString(entry.buyer) << ","
                    << QString::fromStdString(entry.merchandise) << ","
                    << entry.cost << ","
                    << QString::fromStdString(entry.category) << "\n";
            }
        }

        // Close the file after writing
        file.close();

        // Show a success message
        QMessageBox::information(&dialog, "Success", "CSV file saved successfully!");

        // Close the dialog
        dialog.accept();
        });

    // Display the dialog and wait for user interaction
    dialog.exec();
}

void MainWindow::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event); // Call base class implementation

    int buttonWidth = 100; // Button width
    int buttonHeight = 26; // Button height
    int margin = 10;       // Margin from the top and right

    // Reposition the button dynamically when resizing
    logoutButton->setGeometry(width() - buttonWidth - margin, margin - 7, buttonWidth, buttonHeight);
}

void MainWindow::saveTransactions() {
    if (currentUserId.isEmpty()) return;

    QJsonArray transactionsArray;
    std::vector<DataEntry> transactions = cashflowTracking.getAllDataEntries();
    for (const auto& entry : transactions) {
        QJsonObject obj;
        obj["id"] = QString::number(entry.transid);
        obj["datetime"] = QString::fromStdString(entry.dateTime);
        obj["seller"] = QString::fromStdString(entry.seller);
        obj["buyer"] = QString::fromStdString(entry.buyer);
        obj["merchandise"] = QString::fromStdString(entry.merchandise);
        obj["cost"] = QString::number(entry.cost, 'f', 2); // 👈 formatted
        obj["category"] = QString::fromStdString(entry.category);
        transactionsArray.append(obj);
    }


    QJsonObject root;
    root["transactions"] = transactionsArray;

    QFile file("data/" + currentUserId + "_transactions.json");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(root).toJson());
        file.close();
    }
}

void MainWindow::loadTransactions(const QString& userId) {
    QFile file("data/" + userId + "_transactions.json");
    if (!file.open(QIODevice::ReadOnly)) return;

    tableWidget->setRowCount(0);
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    QJsonArray transactionsArray = doc["transactions"].toArray();
    for (const QJsonValue& val : transactionsArray) {
        QJsonObject obj = val.toObject();
        int row = tableWidget->rowCount();
        tableWidget->insertRow(row);
        tableWidget->setItem(row, 0, new QTableWidgetItem(obj["id"].toString()));
        tableWidget->setItem(row, 1, new QTableWidgetItem(obj["datetime"].toString()));
        tableWidget->setItem(row, 2, new QTableWidgetItem(obj["seller"].toString()));
        tableWidget->setItem(row, 3, new QTableWidgetItem(obj["buyer"].toString()));
        tableWidget->setItem(row, 4, new QTableWidgetItem(obj["merchandise"].toString()));
        tableWidget->setItem(row, 5, new QTableWidgetItem(obj["cost"].toString()));
        tableWidget->setItem(row, 6, new QTableWidgetItem(obj["category"].toString()));

        // 👇 This ensures persistence in memory
        cashflowTracking.addData(
            obj["datetime"].toString().toStdString(),
            obj["seller"].toString().toStdString(),
            obj["buyer"].toString().toStdString(),
            obj["merchandise"].toString().toStdString(),
            obj["cost"].toString().toDouble(),
            obj["category"].toString().toStdString()
        );
    }

}



/**
 * @brief Entry point of the application.
 *
 * This function initializes the Qt application, creates an instance of the authentication system,
 * the main window, and the login page. It then displays the login page and starts the application
 * event loop.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return int The exit code of the application.
 *
 * @author Simran Kullar
 */
/*int main(int argc, char* argv[]) {
    QApplication app(argc, argv); // Initialize the Qt application
    AuthenticateSystem authSystem; // Create the authentication system
    MainWindow mainWindow; // Create the main window
    LoginPage loginPage(&authSystem, &mainWindow); // Create the login page with references to auth system and main window
    loginPage.show(); // Display the login page

    return app.exec(); // Start the application event loop


}*/
int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    AuthenticateSystem authSystem;
    MainWindow mainWindow(&authSystem);                 // ✅ create it here
    LoginPage loginPage(&authSystem, &mainWindow);      // ✅ pass it to LoginPage

    loginPage.show();                                   // show login page first

    return app.exec();
}

