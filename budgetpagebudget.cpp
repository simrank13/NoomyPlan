#include "budgetpagebudget.h"

/**
 * @brief default construtor, everything set to 0.
 *also creates necessary UI objects
 * @param parent parent qobject
  * @author Katherine R
 */
BudgetPageBudget::BudgetPageBudget(QObject *parent)
    : QObject{parent} {
    this->budget = 0;
    this->totalExpenses = 0;
    this->remainingBudget = 0;
    this->budgetIndex = 0;
    //creates area for expenses
    this->budgetObj_expenseScrollArea = new QScrollArea();//for scrolling expenses
    this->budgetObj_expenseScrollWidget = new QWidget();
    this->budgetObj_expenseScrollListVbox = new QVBoxLayout();
    //sets the widget of the scroll area
    this->budgetObj_expenseScrollArea->setWidget(this->budgetObj_expenseScrollWidget);
    this->budgetObj_expenseScrollArea->setWidgetResizable(true);
    this->budgetObj_expenseScrollWidget->setLayout(this->budgetObj_expenseScrollListVbox);
}

/**
 * @brief semi parametrized construtor, everything set to 0, except index
 *also creates necessary UI objects
 * @param parent parent qobject
 * @param budgetindex the index (0 = year, 1-5 = Q1-Q4, 6-18 = jan-dec) for to_JSON
  * @author Katherine R
 */
BudgetPageBudget::BudgetPageBudget(QObject *parent, int budgetindex)
    : QObject{parent} {
    this->budget = 0;
    this->totalExpenses = 0;
    this->remainingBudget = 0;
    this->budgetIndex = budgetindex;
     //creates area for expenses
    this->budgetObj_expenseScrollArea = new QScrollArea();//for scrolling expenses
    this->budgetObj_expenseScrollWidget = new QWidget();
    this->budgetObj_expenseScrollListVbox = new QVBoxLayout();
    //sets the widget of the scroll area
    this->budgetObj_expenseScrollArea->setWidget(this->budgetObj_expenseScrollWidget);
    this->budgetObj_expenseScrollArea->setWidgetResizable(true);
    this->budgetObj_expenseScrollWidget->setLayout(this->budgetObj_expenseScrollListVbox);
}

/**
 * @brief parametrized constructor 
 * @param parent parent qobject
 * @param budget budget
 * @param totalexpense total expenses
 * @param remainingbudget remaining budget
 */
BudgetPageBudget::BudgetPageBudget(QObject *parent, double budget, double totalexpense, double remainingbudget,
                                   int budgetindex)
    : QObject{parent} {
    this->budget = budget;
    this->totalExpenses = totalexpense;
    this->remainingBudget = remainingbudget;
    this->budgetIndex = budgetindex;
    //creates area for expenses
    this->budgetObj_expenseScrollArea = new QScrollArea();//for scrolling expenses
    this->budgetObj_expenseScrollWidget = new QWidget();
    this->budgetObj_expenseScrollListVbox = new QVBoxLayout();
    //sets the widget of the scroll area
    this->budgetObj_expenseScrollArea->setWidget(this->budgetObj_expenseScrollWidget);
    this->budgetObj_expenseScrollArea->setWidgetResizable(true);
    this->budgetObj_expenseScrollWidget->setLayout(this->budgetObj_expenseScrollListVbox);
}

/**
* @brief parametized constructor - creates a budget object from QJSONobject
 * @param parent parent Qobject
 * @param json
 * \n JSON needs to be the same format as to_JSON()
 @ \n copydoc BudgetPageBudget::to_JSON()
 * @author Katherine R
 */
BudgetPageBudget::BudgetPageBudget(QObject *parent, const QJsonObject &json) : QObject{parent} {
    this->budget = json.value("Budget").toDouble();
    this->totalExpenses = json.value("Total Expenses").toDouble();
    this->remainingBudget = json.value("Remaining Budget").toDouble();
    this->budgetIndex = json.value("Index").toInt();
    //splits Expenses into JSON array, then adds them
    QJsonArray expensesArray = json.value("Expenses").toArray();
    for (QJsonValue expense: expensesArray) {
        this->expenses.append(new BudgetPageExpenses(this, expense.toObject()));//creates expense from QJsonObject
    }
    //creates area for expenses
    this->budgetObj_expenseScrollArea = new QScrollArea();//for scrolling expenses
    this->budgetObj_expenseScrollWidget = new QWidget();
    this->budgetObj_expenseScrollListVbox = new QVBoxLayout();
    //sets the widget of the scroll area
    this->budgetObj_expenseScrollArea->setWidget(this->budgetObj_expenseScrollWidget);
    this->budgetObj_expenseScrollArea->setWidgetResizable(true);
    this->budgetObj_expenseScrollWidget->setLayout(this->budgetObj_expenseScrollListVbox);
}

/**
 * @brief getter for budget
 * @return budget
  * @author Katherine R
 */
double BudgetPageBudget::getBudget() {
    return budget;
}

/**
 * @brief getter for total expenses
 * @return total expenses
  * @author Katherine R
 */
double BudgetPageBudget::getTotalExpenses() {
    return totalExpenses;
}

/**
 *  @brief getter for remaining budgets
 * @return remaining budget
  * @author Katherine R
 */
double BudgetPageBudget::getRemainingBudget() {
    return remainingBudget;
}

/**
 * @brief getter for expenses qvector
 * @return expenses qvector
  * @author Katherine R
 */
QVector<BudgetPageExpenses *> *BudgetPageBudget::getExpenses() {
    return &expenses;
}

/**
 * @brief setter for budget
 * @param newbudget new budget
  * @author Katherine R
 */
void BudgetPageBudget::setBudget(double newbudget) {
    this->budget = newbudget;
}

/**
 * @brief setter for total expense
 * @param newtotalexpense
  * @author Katherine R
 */
void BudgetPageBudget::setTotalExpenses(double newtotalexpense) {
    totalExpenses = newtotalexpense;
}

/**
 * @brief setter for remaining budget
 * @param newremainingbudget
  * @author Katherine R
 */
void BudgetPageBudget::setRemainingBudget(double newremainingbudget) {
    remainingBudget = newremainingbudget;
}

/**
 * @brief setter for index
 * @param index
  * @author Katherine R
 */
void BudgetPageBudget::setIndex(int index) {
    this->budgetIndex = index;
}

/**
 * @brief changes total expenses by delta
 * \n new total =  old total + delta
 * @param delta the change
  * @author Katherine R
 */
void BudgetPageBudget::changeTotalExpenses(double delta) {
    this->totalExpenses += delta;
}

/**
 * @brief Creates a json object of the budget object
 * @return json object
            \n "Budget" - the budget
            \n "Total Expenses" - the total expenses
            \n "Remaining Budget" - the remaining budget
            \n "Index" 0-18, the budget period (0-yearly) (1-5 Q1-Q4) (6-18 jan-dec)
            \n "Expenses" an array with Expense::to_JSON()
*           \n @copydoc BudgetPageExpenses::to_JSON()
  * @author Katherine R
 */
QJsonObject BudgetPageBudget::to_JSON() {
    QJsonObject jsonDoc;
    QJsonArray expensesArray;
    //for every expense, adds them to jsonarray
    for (BudgetPageExpenses *expense: this->expenses) {//adds expenses to a QJSonarray
        expensesArray.append(expense->to_JSON());
    }
    //adds the other variables
    jsonDoc.insert("Budget", this->budget);
    jsonDoc.insert("Total Expenses", this->totalExpenses);
    jsonDoc.insert("Remaining Budget", this->remainingBudget);
    jsonDoc.insert("Expenses", expensesArray);//adds expenses jsonarray
    jsonDoc.insert("Index", this->budgetIndex);
    return jsonDoc;
}
/**
*@Brief getter for scrollwidget 
@author Katherine R
@return the expenses widget
*/
QWidget *BudgetPageBudget::getExpensescrollwidget() {
    return budgetObj_expenseScrollWidget;
}
/**
*@Brief getter for expenses scroll area 
@author Katherine R
@return the expenses Qscrollarea for scroll area
*/
QScrollArea *BudgetPageBudget::getExpensescrollarea() {
    return budgetObj_expenseScrollArea;
}
/**
*@Brief getter for expenses vbox 
@author Katherine R
@return the expenses vbox
*/
QVBoxLayout *BudgetPageBudget::getExpensesscrolllistvbox() {
    return budgetObj_expenseScrollListVbox;
}