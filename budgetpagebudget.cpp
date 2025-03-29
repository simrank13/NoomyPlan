#include "budgetpagebudget.h"

/**
 * @class BudgetPageBudget
 * @brief Represents a budget with associated expenses and calculations.
 *
 * This class manages a budget, including its total expenses, remaining budget,
 * and a list of expenses. It provides functionality to serialize and deserialize
 * budget data to/from JSON. The class also supports dynamic updates to expenses
 * and recalculates the remaining budget accordingly.
 * 
 * @author Katherine R
 **/

/**
 * @brief Default constructor for BudgetPageBudget.
 *
 * Initializes all budget-related values to 0 and sets up the scroll area for expenses.
 * The scroll area is configured to be resizable and contains a widget with a vertical layout
 * to display the list of expenses.
 *
 * @param parent The parent QObject, default is nullptr.
 * 
 */
BudgetPageBudget::BudgetPageBudget(QObject* parent)
    : QObject{ parent } {
    // initialize budget, total expenses, remaining budget, budget index to 0 
    this->budget = 0; 
    this->totalExpenses = 0; 
    this->remainingBudget = 0; 
    this->budgetIndex = 0; 

    //creates area for expenses
    this->budgetObj_expenseScrollArea = new QScrollArea();
    this->budgetObj_expenseScrollWidget = new QWidget(); 
    this->budgetObj_expenseScrollListVbox = new QVBoxLayout(); 

    //sets the widget of the scroll area
    this->budgetObj_expenseScrollArea->setWidget(this->budgetObj_expenseScrollWidget); 
    this->budgetObj_expenseScrollArea->setWidgetResizable(true); 
    this->budgetObj_expenseScrollWidget->setLayout(this->budgetObj_expenseScrollListVbox); 
}

/**
 * @brief Parameterized constructor for BudgetPageBudget.
 *
 * Initializes all budget-related values to 0 except for the budget index, which is set to the provided value.
 * The scroll area is configured to be resizable and contains a widget with a vertical layout
 * to display the list of expenses.
 *
 * @param parent The parent QObject.
 * @param budgetindex The index of the budget (0 = yearly, 1-4 = Q1-Q4, 5-16 = Jan-Dec).
 * 
 */
BudgetPageBudget::BudgetPageBudget(QObject* parent, int budgetindex)
    : QObject{ parent } {
    // initialize budget, total expenses, remaining budget, budget index to 0 
    this->budget = 0; 
    this->totalExpenses = 0; 
    this->remainingBudget = 0; 
    this->budgetIndex = budgetindex; 

    //creates area for expenses
    this->budgetObj_expenseScrollArea = new QScrollArea(); 
    this->budgetObj_expenseScrollWidget = new QWidget();
    this->budgetObj_expenseScrollListVbox = new QVBoxLayout(); 

    //sets the widget of the scroll area
    this->budgetObj_expenseScrollArea->setWidget(this->budgetObj_expenseScrollWidget); 
    this->budgetObj_expenseScrollArea->setWidgetResizable(true); 
    this->budgetObj_expenseScrollWidget->setLayout(this->budgetObj_expenseScrollListVbox); 
}

/**
 * @brief Parameterized constructor for BudgetPageBudget.
 *
 * Initializes the budget with the provided values for budget, total expenses, remaining budget, and budget index.
 * The scroll area is configured to be resizable and contains a widget with a vertical layout
 * to display the list of expenses.
 *
 * @param parent The parent QObject.
 * @param budget The total budget amount.
 * @param totalexpense The total expenses.
 * @param remainingbudget The remaining budget.
 * @param budgetindex The index of the budget (0 = yearly, 1-4 = Q1-Q4, 5-16 = Jan-Dec).
 * 
 */
BudgetPageBudget::BudgetPageBudget(QObject* parent, double budget, double totalexpense, double remainingbudget,
    int budgetindex)
    : QObject{ parent } {
    // initialize budget, total expenses, remaining budget, budget index to 0 
    this->budget = budget; 
    this->totalExpenses = totalexpense; 
    this->remainingBudget = remainingbudget;
    this->budgetIndex = budgetindex; 

    //creates area for expenses
    this->budgetObj_expenseScrollArea = new QScrollArea(); 
    this->budgetObj_expenseScrollWidget = new QWidget(); 
    this->budgetObj_expenseScrollListVbox = new QVBoxLayout(); 

    //sets the widget of the scroll area
    this->budgetObj_expenseScrollArea->setWidget(this->budgetObj_expenseScrollWidget); 
    this->budgetObj_expenseScrollArea->setWidgetResizable(true); 
    this->budgetObj_expenseScrollWidget->setLayout(this->budgetObj_expenseScrollListVbox); 
}

/**
 * @brief Constructs a BudgetPageBudget object from a JSON object.
 *
 * Initializes the budget by deserializing data from a JSON object. The JSON object should contain
 * the total budget, total expenses, remaining budget, and budget index. It also loads the expenses
 * from the JSON array.
 *
 * @param parent The parent QObject.
 * @param json The JSON object containing budget data.
 * 
 */
BudgetPageBudget::BudgetPageBudget(QObject* parent, const QJsonObject& json) : QObject{ parent } {
    // load budget, total expenses, remaining budget, budget index, expenses array from JSON.
    this->budget = json.value("Budget").toDouble(); 
    this->totalExpenses = json.value("Total Expenses").toDouble(); 
    this->remainingBudget = json.value("Remaining Budget").toDouble(); 
    this->budgetIndex = json.value("Index").toInt();

    //splits Expenses into JSON array, then adds them.
    QJsonArray expensesArray = json.value("Expenses").toArray();

    for (QJsonValue expense : expensesArray) {
        this->expenses.append(new BudgetPageExpenses(this, expense.toObject())); // creates expense from QJsonObject and add each expense to the list.
    }
    //creates area for expenses
    this->budgetObj_expenseScrollArea = new QScrollArea(); 
    this->budgetObj_expenseScrollWidget = new QWidget();
    this->budgetObj_expenseScrollListVbox = new QVBoxLayout(); 

    //sets the widget of the scroll area
    this->budgetObj_expenseScrollArea->setWidget(this->budgetObj_expenseScrollWidget); 
    this->budgetObj_expenseScrollArea->setWidgetResizable(true); 
    this->budgetObj_expenseScrollWidget->setLayout(this->budgetObj_expenseScrollListVbox); 
}

/**
 * @brief Gets the total budget amount.
 *
 * This method returns the total budget amount set for this budget.
 *
 * @return The total budget.
 * 
 */
double BudgetPageBudget::getBudget() {
    return budget;
}

/**
 * @brief Gets the total expenses.
 *
 * This method returns the total expenses recorded for this budget.
 *
 * @return The total expenses.
 * 
 */
double BudgetPageBudget::getTotalExpenses() {
    return totalExpenses;
}

/**
 * @brief Gets the remaining budget.
 *
 * This method returns the remaining budget, calculated as the difference
 * between the total budget and total expenses.
 *
 * @return The remaining budget.
 * 
 */
double BudgetPageBudget::getRemainingBudget() {
    return remainingBudget;
}

/**
 * @brief Gets the list of expenses.
 *
 * This method returns a pointer to the vector containing all expenses
 * associated with this budget.
 *
 * @return A pointer to the vector of expenses.
 * 
 */
QVector<BudgetPageExpenses*>* BudgetPageBudget::getExpenses() {
    return &expenses;
}

/**
 * @brief Sets the total budget amount.
 *
 * This method updates the total budget amount and recalculates the
 * remaining budget.
 *
 * @param newbudget The new budget amount.
 * 
 */
void BudgetPageBudget::setBudget(double newbudget) {
    this->budget = newbudget;
}

/**
 * @brief Sets the total expenses.
 *
 * This method updates the total expenses and recalculates the remaining budget.
 *
 * @param newtotalexpense The new total expenses.
 * 
 */
void BudgetPageBudget::setTotalExpenses(double newtotalexpense) {
    totalExpenses = newtotalexpense;
}

/**
 * @brief Sets the remaining budget.
 *
 * This method directly sets the remaining budget. It is typically used
 * when loading data from a JSON object.
 *
 * @param newremainingbudget The new remaining budget.
 * @author Katherine R
 */
void BudgetPageBudget::setRemainingBudget(double newremainingbudget) {
    remainingBudget = newremainingbudget;
}

/**
 * @brief Sets the budget index.
 *
 * This method updates the budget index, which determines the period
 * (e.g., yearly, quarterly, monthly) this budget represents.
 *
 * @param index The new budget index.
 * 
 */
void BudgetPageBudget::setIndex(int index) {
    this->budgetIndex = index;
}

/**
 * @brief Updates the total expenses by a delta value.
 *
 * This method adjusts the total expenses by the specified delta value
 * and recalculates the remaining budget.
 *
 * @param delta The change in expenses.
 * 
 */
void BudgetPageBudget::changeTotalExpenses(double delta) {
    this->totalExpenses += delta;
}

/**
 * @brief Converts the budget data to a JSON object.
 *
 * This method serializes the budget data, including total budget, total expenses,
 * remaining budget, and budget index, into a JSON object. It also includes the
 * list of expenses as a JSON array.
 *
 * @return The JSON object containing budget data.
 *          \n "Budget" - the budget
            \n "Total Expenses" - the total expenses
            \n "Remaining Budget" - the remaining budget
            \n "Index" 0-18, the budget period (0-yearly) (1-5 Q1-Q4) (6-18 jan-dec)
            \n "Expenses" an array with Expense::to_JSON()
*           \n @copydoc BudgetPageExpenses::to_JSON()
 * 
 */
QJsonObject BudgetPageBudget::to_JSON() {
    QJsonObject jsonDoc;
    QJsonArray expensesArray;
    for (BudgetPageExpenses* expense : this->expenses) {
        expensesArray.append(expense->to_JSON()); ///< Add each expense to the JSON array.
    }
    jsonDoc.insert("Budget", this->budget); ///< Add budget to JSON.
    jsonDoc.insert("Total Expenses", this->totalExpenses); ///< Add total expenses to JSON.
    jsonDoc.insert("Remaining Budget", this->remainingBudget); ///< Add remaining budget to JSON.
    jsonDoc.insert("Expenses", expensesArray); ///< Add expenses array to JSON.
    jsonDoc.insert("Index", this->budgetIndex); ///< Add budget index to JSON.
    return jsonDoc;
}

/**
 * @brief Gets the widget for the expenses scroll area.
 *
 * This method returns the widget used to display the list of expenses
 * in a scrollable area.
 *
 * @return The expenses scroll widget.
 * 
 */
QWidget* BudgetPageBudget::getExpensescrollwidget() {
    return budgetObj_expenseScrollWidget;
}

/**
 * @brief Gets the scroll area for expenses.
 *
 * This method returns the scroll area used to display the list of expenses.
 *
 * @return The expenses scroll area.
 * 
 */
QScrollArea* BudgetPageBudget::getExpensescrollarea() {
    return budgetObj_expenseScrollArea;
}

/**
 * @brief Gets the vertical layout for the expenses list.
 *
 * This method returns the vertical layout used to organize the list of expenses
 * within the scroll area.
 *
 * @return The expenses scroll list layout.
 * 
 */
QVBoxLayout* BudgetPageBudget::getExpensesscrolllistvbox() {
    return budgetObj_expenseScrollListVbox;
}