
#ifndef EXPENSES_H
#define EXPENSES_H


#include <QApplication>
#include <QPushButton>
#include <QLineEdit>
#include<QFormLayout>
#include<QSpinBox>
#include<QDebug>
#include<QMainWindow>
#include<QWidget>
#include<QComboBox>
#include<QObject>
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
     * quanity is set to 1, everything set to 0 or null string
     * @param parent parent QObject if needed
      * @author Katherine R
     */
    explicit BudgetPageExpenses(QObject *parent = nullptr);
    /**
     * @brief parametrized constructor for expenses
     * @param parent parent qobject
     * @param name name of expense
     * @param description description of the expense item
     * @param price price of expense
     * @param quantity # of items
      * @author Katherine R
     */
    BudgetPageExpenses(QObject *parent, const QString &name, const QString &description, double price,
                       double quantity);
    /**
     * @brief creates an Expense object from a json
     * @param parent
     * @param Expense
    *          \n "Name" - Name of Expense
     *         \n "Description" - description of Expense
     *         \n "Price" - price of Expense item
     *         \n "Quantity" - quantity of items
     */
    BudgetPageExpenses(QObject *parent, const QJsonObject &Expense);
    /**
     * @brief creates a json with data from expense item, used for offline mode/saving
     * @return A QJson object with the values for this expense item
     *         \n "Name" - Name of Expense
     *         \n "Description" - description of Expense
     *         \n "Price" - price of Expense item
     *         \n "Quantity" - quantity of items
     */
    QJsonObject to_JSON() const;
    /**
     * 
     * @return the remove button QPushButton
      * @author Katherine R
     */
     QPushButton *getRemoveButton();
    
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
     * @param parent QWidget to add the UI to
     * @param vbox VBox to add the UI to
      * @author Katherine R
     */
    void createExpenseUI(QWidget *parent = nullptr, QVBoxLayout *vbox = nullptr);

    ~BudgetPageExpenses();

signals:
    void expenseChangedSignal(double delta);

public slots:
    /**
     * @brief Slot detects if the expense name is changed, changes the expenseName to the new one
     * @param newName the new name for the expense
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
     * @brief slot detects if the price or quantity for the expense has changed
     * @param change new value
     * @param changedType 'P' for price 'Q' for quantity
     * @author Katherine R
     */
    void onExpenseNameChangedSlot(const QString &newName) const;

private:
    double price;
    double quantity;
    QString *expenseName;
    QString *expenseDescription;
    QWidget *expenseObj_ExpenseWidget;
    QFormLayout *expenseObj_addExpenseForm;
    QLineEdit *expenseObj_NameLineEdit;
    QLineEdit *expenseObj_DescriptionLineEdit;
    QDoubleSpinBox *expenseObj_QuantitySpinBox;
    QDoubleSpinBox *expenseObj_PriceSpinBox;
    QVBoxLayout *expenseObj_Layout;
    QWidget *expenseObj_removeExpenseButtonWidget;
    QHBoxLayout *expenseObj_removeExpenseHbox;
    QPushButton *expenseObj_removeExpenseButton;
};


#endif // EXPENSES_H
