#include "budgetpagebudget.h"

/**
 * @brief default constructor, everything set to 0.
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
    this->budgetGoal = 0;
    //creates area for expenses
    newExpensescrollArea();
}

/**
 * @brief semi parametrized constructor, everything set to 0, except index
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
    this->budgetGoal = 0;
    newExpensescrollArea();
}

/**
 * @brief parametrized constructor 
 * @param parent parent qobject
 * @param budget budget
 * @param totalexpense total expenses
 * @param remainingbudget remaining budget
 * @param goal budget goal
 * @param budgetindex the index for the period of the budget (0=yearly, 1-5=quarterly, 6-18=monthly)
 */
BudgetPageBudget::BudgetPageBudget(QObject *parent, double budget, double totalexpense, double remainingbudget,
                                   int budgetindex, double goal)
    : QObject{parent} {
    this->budget = budget;
    this->totalExpenses = totalexpense;
    this->remainingBudget = remainingbudget;
    this->budgetIndex = budgetindex;
    this->budgetGoal = goal;

    newExpensescrollArea();
}

/**
* @brief parametrized constructor - creates a budget object from QJsonObject
 * @param parent parent QOBject
 * @param json
 * \n JSON needs to be the same format as to_JSON()
 @ \n copydoc BudgetPageBudget::to_JSON()
 * @author Katherine R
 */
BudgetPageBudget::BudgetPageBudget(QObject *parent, const QJsonObject &json) : QObject{parent} {
    if (!json.isEmpty()) {
        this->budget = json.value("Budget").toDouble();
        this->totalExpenses = json.value("Total Expenses").toDouble();
        this->remainingBudget = json.value("Remaining Budget").toDouble();
        this->budgetIndex = json.value("Index").toInt();
        this->budgetGoal = json.value("Goal").toDouble();
        //splits Expenses into JSON array, then adds them
        QJsonArray expensesArray = json.value("Expenses").toArray();
        for (QJsonValue expense: expensesArray) {
            this->expenses.append(new BudgetPageExpenses(this, expense.toObject())); //creates expense from QJsonObject
        }
        newExpensescrollArea();
    } else {
        qDebug() << "empty JSON import-expense";
    }
}

/**
 * @brief getter for budget
 * @return budget
  * @author Katherine R
 */
double BudgetPageBudget::getBudget() const {
    return budget;
}

/**
 * @brief getter for total expenses
 * @return total expenses
  * @author Katherine R
 */
double BudgetPageBudget::getTotalExpenses() const {
    return totalExpenses;
}

/**
 *  @brief getter for remaining budgets
 * @return remaining budget
  * @author Katherine R
 */
double BudgetPageBudget::getRemainingBudget() const {
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
            \n "Goal" the goal for budget net, double
            \n "Expenses" an array with Expense::to_JSON()
*           \n @copydoc BudgetPageExpenses::to_JSON()
  * @author Katherine R
 */
QJsonObject BudgetPageBudget::to_JSON() {
    QJsonObject jsonDoc;
    QJsonArray expensesArray;
    //for every expense, adds them to jsonarray
    for (BudgetPageExpenses *expense: this->expenses) {
        //adds expenses to a QJSonarray
        expensesArray.append(expense->to_JSON());
    }
    //adds the other variables
    jsonDoc.insert("Budget", this->budget);
    jsonDoc.insert("Total Expenses", this->totalExpenses);
    jsonDoc.insert("Remaining Budget", this->remainingBudget);
    jsonDoc.insert("Expenses", expensesArray); //adds expenses jsonarray
    jsonDoc.insert("Index", this->budgetIndex);
    jsonDoc.insert("Goal", this->budgetGoal);
    return jsonDoc;
}

/**
*@Brief getter for scrollwidget 
@author Katherine R
@return the expenses area widget
*/
QWidget *BudgetPageBudget::getExpensescrollwidget() {
    return budgetObj_expenseScrollWidget.first();
}

/**
*@Brief getter for expenses scroll area 
@author Katherine R
@return the expenses Qscrollarea for scroll area
*/
QScrollArea *BudgetPageBudget::getExpensescrollarea() {
    // return budgetObj_expenseScrollArea;
    return budgetObj_expenseScrollArea.first();
}

/**
*@Brief getter for expenses scroll area at index
*@param index the indexfor the qvector
@author Katherine R
@return the expenses Qscrollarea for scroll area at index
*/
QScrollArea *BudgetPageBudget::getExpensescrollarea(int index) {
    if (index >= 0 && index < budgetObj_expenseScrollArea.count()) {
        return budgetObj_expenseScrollArea[index];
    } else { return nullptr; };
}

/**
*@Brief getter for expenses vbox 
@author Katherine R
@return the expenses vbox
*/
QVBoxLayout *BudgetPageBudget::getExpensesscrolllistvbox() {
    return budgetObj_expenseScrollListVbox.first();
}

/**
*@Brief getter for expenses vbox at index
@author Katherine R
@return the expenses vbox
@param index the index
*/
QVBoxLayout *BudgetPageBudget::getExpensesscrolllistvbox(int index) {
    if (index >= 0 && index < budgetObj_expenseScrollListVbox.count() && !budgetObj_expenseScrollListVbox.isEmpty()) {
        return budgetObj_expenseScrollListVbox[index];
    } else return nullptr;
}

/**
  *@Brief getter for budget goal
  @author Katherine R
  @return the budget goal
  */
double BudgetPageBudget::getBudgetGoal() const {
    return budgetGoal;
}

/**
 * @brief setter for budget goal
 * @param newbudgetgoal the new goal
  * @author Katherine R
 */
void BudgetPageBudget::setBudgetGoal(double newbudgetgoal) {
    this->budgetGoal = newbudgetgoal;
}

/**
 *@brief appends a new scroll area, scroll widget, and scroll list vbox to the vectors, for expenses
 */
void BudgetPageBudget::newExpensescrollArea() {
    //creates + appends scroll area new objects to their vectors
    budgetObj_expenseScrollArea.append(new QScrollArea());
    budgetObj_expenseScrollWidget.append(new QWidget());
    budgetObj_expenseScrollListVbox.append(new QVBoxLayout());
    //sets the scroll area as the scroll widget
    this->budgetObj_expenseScrollArea.last()->setWidget(this->budgetObj_expenseScrollWidget.last());
    this->budgetObj_expenseScrollArea.last()->setWidgetResizable(true);
    //sets the layout as the vbox
    budgetObj_expenseScrollWidget.last()->setLayout(budgetObj_expenseScrollListVbox.last());
}
