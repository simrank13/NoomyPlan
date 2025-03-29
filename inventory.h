#ifndef INVENTORYPAGE_H
#define INVENTORYPAGE_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QStackedWidget>
#include <QTabWidget>

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
class InventoryPage : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructor for InventoryPage.
     * @param parent Pointer to the parent QWidget (default is nullptr).
     *
     * Initializes the InventoryPage instance and prepares it for UI setup.
     */
    explicit InventoryPage(QWidget* parent = nullptr);

    /**
     * @brief Destructor for InventoryPage.
     *
     * Ensures all dynamically allocated components are cleaned up to avoid memory leaks.
     */
    ~InventoryPage();

    /**
     * @brief Sets the stacked widget for page navigation.
     * @param stack Pointer to the QStackedWidget used for navigating pages.
     *
     * Allows the class to navigate between different views within the application.
     */
    void setPageStack(QStackedWidget* stack);

    /**
     * @brief Sets the shared inventory table widget.
     * @param table Pointer to the QTableWidget for inventory management.
     *
     * Initializes the inventory table and sets up the UI components accordingly.
     */
    void setInventoryTable(QTableWidget* table);

    /**
    * @brief Sets up the UI components for inventory management.
    *
    * Initalized the layout of the inventory and inpput fields for the user to interact with.
    * Also includes action buttons and integrated the shared inventory display table.
    */
    void setupUI();

    /**
    * @brief Sets the tab widget to enable navigation between tabs.
    * @param tabs Pointer to the QTabWidget.
     *
    * Allows the class to be able to integrate with the tabbed interface.
     */
    void setTabWidget(QTabWidget* tabs);

public slots:
    /**
    * @brief Adds a new item to the inventory table.
    *
    * Validates the user input and then updates the table accordingly.
    * Clear the input fields after successful addiction, and then display a warning if not complete.
    */
    void addItem();

    /**
    * @brief Removes the selected item from the inventory table.
    *
    * Ensures that the item selected before removal and then will alert the user with a warning if there is no selection.
    */
    void removeItem();

    /**
    * @brief Updates the selected inventory item's details.
    *
    * Checks for the selection validity and updates the selected item's details if inputs are provided.
    * Clears the input fields post-update and alerts if no items is selected.
    */
    void updateItem();

private:
    // Input fields
    QLineEdit* itemNameInput;       // Input field for the item's name
    QLineEdit* itemQuantityInput;   // Input field for the item's quantity
    QLineEdit* priceInput;          // Input field for the item's price

    // Buttons
    QPushButton* addItemButton;     // Button to add new items
    QPushButton* removeItemButton;  // Button to remove items
    QPushButton* updateItemButton;  // Button to update the item's info
    QPushButton* backButton;        // Button to back to dashboard

    // Widgets 
    QTableWidget* sharedInventoryTable = nullptr; // Table to show inventory items
    QStackedWidget* pageStack = nullptr;          // Stack widget to navigate between pages
    QTabWidget* tabWidget = nullptr;              // Tab widget for multiple pages
};

#endif // INVENTORYPAGE_H