#include "inventory.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>


/**
 * @class InventoryPage
 * @brief Provides a management for the inventory UI.
 *
 * This class has elements to use for functionalities of the inventory management.
 * It handles adding, removing, and updating inventory items.
 * The class will interact with the dashboard as a shared table.
 *
 * @author Maneet Chahal
 */

 /**
  * @brief Constructs the InventoryPage widget.
  * @param parent Pointer to the parent QWidget (default nullptr).
  *
  * Initializes the InventoryPage instance and prepares it for UI setup.
  */
InventoryPage::InventoryPage(QWidget* parent) : QWidget(parent)
{
}

/**
 * @brief Sets the tab widget to enable navigation between tabs.
 * @param tabs Pointer to the QTabWidget.
 *
 * Allows the class to be able to integrate with the tabbed interface.
 */
void InventoryPage::setTabWidget(QTabWidget* tabs) {
    tabWidget = tabs;
}

/**
 * @brief Sets up the UI components for inventory management.
 *
 * Initalized the layout of the inventory and inpput fields for the user to interact with.
 * Also includes action buttons and integrated the shared inventory display table.
 */
void InventoryPage::setupUI()
{
    // Title label for the main layout of the inventory section
    QVBoxLayout* mainLayout = new QVBoxLayout();
    this->setLayout(mainLayout);

    // Input fields layout
    QLabel* titleLabel = new QLabel("Inventory Management", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 21px; font-weight: 600;");
    mainLayout->addSpacing(8);
    mainLayout->addWidget(titleLabel);

    // Input field for the Item Name
    QHBoxLayout* inputLayout = new QHBoxLayout();
    itemNameInput = new QLineEdit(this);
    itemNameInput->setPlaceholderText("Enter item name");

    // Input field for the Quantity Input
    itemQuantityInput = new QLineEdit(this);
    itemQuantityInput->setPlaceholderText("Enter quantity");

    // Input field for the Price Input
    priceInput = new QLineEdit(this);
    priceInput->setPlaceholderText("Enter price (e.g. 9.99)");

    // adds the input widget layouts
    inputLayout->addWidget(itemNameInput);
    inputLayout->addWidget(itemQuantityInput);
    inputLayout->addWidget(priceInput);
    mainLayout->addLayout(inputLayout);

    // Action buttons for the operations of the inventory
    addItemButton = new QPushButton("Add Item", this); // button for adding item
    removeItemButton = new QPushButton("Remove Selected Item", this); // button to remove item
    updateItemButton = new QPushButton("Update Selected Item", this); // button to update item
    backButton = new QPushButton("Back to Dashboard", this); // button to go back to dashboard

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(addItemButton);
    buttonLayout->addWidget(removeItemButton);
    buttonLayout->addWidget(updateItemButton);
    buttonLayout->addWidget(backButton);
    mainLayout->addLayout(buttonLayout);

    // Inventory table integration for the widget
    if (sharedInventoryTable) {
        mainLayout->addWidget(sharedInventoryTable);
    }

    // Connecting buttons to slots
    connect(addItemButton, &QPushButton::clicked, this, &InventoryPage::addItem);
    connect(removeItemButton, &QPushButton::clicked, this, &InventoryPage::removeItem);
    connect(updateItemButton, &QPushButton::clicked, this, &InventoryPage::updateItem);
    connect(backButton, &QPushButton::clicked, [this]() {
        if (tabWidget) {
            tabWidget->setCurrentIndex(0);
        }
        });

}

/**
 * @brief Adds a new item to the inventory table.
 *
 * Validates the user input and then updates the table accordingly.
 * Clear the input fields after successful addiction, and then display a warning if not complete.
 */
void InventoryPage::addItem()
{
    // get input from user
    qDebug() << "Add item clicked!";
    QString name = itemNameInput->text();
    QString quantity = itemQuantityInput->text();
    QString price = priceInput->text();

    // validate the input fields
    if (name.isEmpty() || quantity.isEmpty() || price.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please fill in all fields.");
        return;
    }

    if (!sharedInventoryTable) return;

    // add new row to the inventory table
    int row = sharedInventoryTable->rowCount();
    sharedInventoryTable->insertRow(row);

    // populate the table with the new data
    sharedInventoryTable->setItem(row, 0, new QTableWidgetItem(name));
    sharedInventoryTable->setItem(row, 1, new QTableWidgetItem(quantity));
    sharedInventoryTable->setItem(row, 2, new QTableWidgetItem("$" + price));

    // clear the input fields after item is added
    itemNameInput->clear();
    itemQuantityInput->clear();
    priceInput->clear();

    /*if (tabWidget) {
        tabWidget->setCurrentIndex(0); // switch to dashboard
    }*/
}

/**
 * @brief Removes the selected item from the inventory table.
 *
 * Ensures that the item selected before removal and then will alert the user with a warning if there is no selection.
 */
void InventoryPage::removeItem()
{
    // get user input
    qDebug() << "Remove item clicked!";
    if (!sharedInventoryTable) return;
    int currentRow = sharedInventoryTable->currentRow(); // determined the selected row
    if (currentRow >= 0) { // remove if valid
        sharedInventoryTable->removeRow(currentRow);
    }
    // otherwise if not valid give warning message
    else {
        QMessageBox::warning(this, "Selection Error", "Please select an item to remove.");
    }

    /*if (tabWidget) {
        tabWidget->setCurrentIndex(0); // switch to dashboard
    }*/

}

/**
 * @brief Updates the selected inventory item's details.
 *
 * Checks for the selection validity and updates the selected item's details if inputs are provided.
 * Clears the input fields post-update and alerts if no items is selected.
 */
void InventoryPage::updateItem()
{
    // determine which one to update
    qDebug() << "Update item clicked!";
    if (!sharedInventoryTable) return;
    int currentRow = sharedInventoryTable->currentRow();

    // update the selected row with the user inputs
    if (currentRow >= 0) {
        QString name = itemNameInput->text();
        QString quantity = itemQuantityInput->text();
        QString price = priceInput->text();
        if (!name.isEmpty())
            sharedInventoryTable->setItem(currentRow, 0, new QTableWidgetItem(name));
        if (!quantity.isEmpty())
            sharedInventoryTable->setItem(currentRow, 1, new QTableWidgetItem(quantity));
        if (!price.isEmpty())
            sharedInventoryTable->setItem(currentRow, 2, new QTableWidgetItem("$" + price));

        // clear input fields after updating
        itemNameInput->clear();
        itemQuantityInput->clear();
        priceInput->clear();
    }

    // give user warning message
    else {
        QMessageBox::warning(this, "Selection Error", "Please select an item to update.");
    }

    /*if (tabWidget) {
        tabWidget->setCurrentIndex(0); // switch to dashboard
    }*/

}

/**
 * @brief Sets the page stack for navigation.
 * @param stack Pointer to QStackedWidget used to manage the multiple views.
 *
 * Allows the class to navigate between different views within the application.
 */
void InventoryPage::setPageStack(QStackedWidget* stack) {
    pageStack = stack;
}

/**
 * @brief Sets the shared inventory table.
 * @param table Pointer to the QTableWidget used for displaying inventory.
 *
 * Initializes the inventory table and sets up the UI components accordingly.
 */
void InventoryPage::setInventoryTable(QTableWidget* table) {
    if (!sharedInventoryTable) {
        sharedInventoryTable = table;
        setupUI();
    }
}


/**
 * @brief Destructor to clear up allocated resources.
 *
 * Ensures all dynamically allocated components are cleaned up to avoid memory leaks.
 */
InventoryPage::~InventoryPage() {
}

