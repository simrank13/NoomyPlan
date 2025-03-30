#ifndef EXPENSES_H
#define EXPENSES_H


#include <QApplication>
#include <QPushButton>
#include <QLineEdit>
#include<QFormLayout>
#include<QSpinBox>
#include<QMainWindow>
#include<QComboBox>
#include<QJsonObject>

#define SHOW_DEBUG_LOGS true

/**
* this class stores information for a single expense item
* \n it stores a name, description, quantity and price
* \n it also has some UI elements that allow changing those values
* \n casts expenseChangedSignal when the expense quantity or price is changed
* @author Katherine R
*/
class BudgetPageExpenses : public QObject {
    Q_OBJECT

public:
    /**
     * @brief default constructor for expenses
     * quantity and price is set to 1, everything set to 0 or null string
     * @param parent parent QObject if need\ed
      * @author Katherine R
     */
    explicit BudgetPageExpenses(QObject * parent = nullptr);

    /**
     * @brief parametrized constructor for expenses
     * @param parent parent qobject
     * @param name name of expense
     * @param description description of the expense item
     * @param price price of expense
     * @param quantity # of items
     * @param categoryIndex the index of category
      * @author Katherine R
     */
    BudgetPageExpenses(QObject * parent, const QString &name, const QString &description, double price, double quantity,
                       int       categoryIndex);

    /**
     * @brief creates an Expense object from a json
     * @param parent
     * @param Expense
    *          \n "Name" - Name of Expense
     *         \n "Description" - description of Expense
     *         \n "Price" - price of Expense item
     *         \n "Quantity" - quantity of items
     *         \n "Category Index" - the index for categorization
     */
    BudgetPageExpenses(QObject * parent, const QJsonObject &Expense);

    /**
     * @brief creates a json with data from expense item, used for offline mode/saving
     * @return A QJson object with the values for this expense item
     *         \n "Name" - Name of Expense
     *         \n "Description" - description of Expense
     *         \n "Price" - price of Expense item
     *         \n "Quantity" - quantity of items
     *         \n "Category Index" - the index for categorization
     */
    QJsonObject to_JSON();

    /**
     * 
     * @return the remove button QPushButton
      * @author Katherine R
     */
    QPushButton * getRemoveButton();

    /**
     * @brief getter for expense value
     * @return price * quantity of expense item
      * @author Katherine R
     */
    double getExpense() const;

    /**
     * @brief Creates the UI for the expense item
     *
     *creates spinboxes for price and quantity
     *creates forms for name and description
     *connects them to expensesSBchangedslot, onExpenseDescriptionChangedSlot, onExpenseNameChangedSlot
     *
     * @param vbox VBox to add the UI to
      * @author Katherine R
     */
    void createExpenseUI(QVBoxLayout * vbox = nullptr);

    ~BudgetPageExpenses();

    /**
     * sets the index for the category
     * used to know which category expense is a member of when export/import json
     * @param index category index
     */
    void setCategoryIndex(int index);

    /**
     * @brief creates a csv entry with the expenses values
     * @return a QString with
     * name, description, quantity, price, category index
     */
    QString to_CSV();

    /**
     * @brief setter for name
     * @param name
     */
    void setExpenseName(QString name);

    /**
 * @brief setter for description
 * @param description
 */
    void setExpenseDescription(QString description);

    /**
 * @brief setter for price
 * @param price the price
 */
    void setExpensePrice(double price);

    /**
 * @brief setter for quantity
 * @param quantity the quantity
 */
    void setExpenseQuantity(double quantity);

    /**
    * @brief setter for categoryindex
    * @param categoryindex the category index
    */
    void setCategoryindex(int categoryindex);

signals:
    void expenseChangedSignal(double delta);

public slots:
    /**
    * @brief slot detects if the price or quantity for the expense has changed
    * @param change new value
    * @param changedType 'P' for price 'Q' for quantity
    * @author Katherine R
    */
    void expenseSBChangedSlot(double change, char changedType);

    /**
     * @brief slot detects if the description name is changed, changes the description to the new one
     * @param newDescription the new description
     @author Katherine R
     */
    void onExpenseDescriptionChangedSlot(const QString &newDescription) const;

    /**
     * @brief Slot detects if the expense name is changed, changes the expenseName to the new one
     * @param newName the new name for the expense
      * @author Katherine R
     */
    void onExpenseNameChangedSlot(const QString &newName) const;

private:
    double price;
    double quantity;
    int    categoryIndex;

private:
    QString *        expenseName;
    QString *        expenseDescription;
    QWidget *        expenseObj_ExpenseWidget;
    QFormLayout *    expenseObj_addExpenseForm;
    QLineEdit *      expenseObj_NameLineEdit;
    QLineEdit *      expenseObj_DescriptionLineEdit;
    QDoubleSpinBox * expenseObj_QuantitySpinBox;
    QDoubleSpinBox * expenseObj_PriceSpinBox;
    QVBoxLayout *    expenseObj_Layout;
    QWidget *        expenseObj_removeExpenseButtonWidget;
    QHBoxLayout *    expenseObj_removeExpenseHbox;
    QPushButton *    expenseObj_removeExpenseButton;
    /**
     *@brief creates the UI objects for BudgetPageExpenses
     *used to eliminate repeated code on constructor
     */
    void createUIobjects();
};


#endif // EXPENSES_H
