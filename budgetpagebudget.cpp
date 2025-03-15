#include "budgetpagebudget.h"

/**
 * default construtor, everything seet to 0.
 * @param parent parent qobject
 */
BudgetPageBudget::BudgetPageBudget(QObject *parent)
    : QObject{parent} {
    this->budget = 0;
    this->totalExpenses = 0;
    this->remainingBudget = 0;
    this->budgetIndex = 0;
    this->budgetObj_expenseScrollArea = new QScrollArea();
    this->budgetObj_expenseScrollWidget = new QWidget();
    this->budgetObj_expenseScrollListVbox = new QVBoxLayout();
    this->budgetObj_expenseScrollArea->setWidget(this->budgetObj_expenseScrollWidget);
    this->budgetObj_expenseScrollArea->setWidgetResizable(true);
    this->budgetObj_expenseScrollWidget->setLayout(this->budgetObj_expenseScrollListVbox);
}

/**
 * parametrized construtor, everything set to 0, except index
 * @param parent parent qobject
 * @param budgetindex the index (0 = year, 1-5 = Q1-Q4, 6-18 = jan-dec) for to_JSON
 */
BudgetPageBudget::BudgetPageBudget(QObject *parent, int budgetindex)
    : QObject{parent} {
    this->budget = 0;
    this->totalExpenses = 0;
    this->remainingBudget = 0;
    this->budgetIndex = budgetindex;
    this->budgetObj_expenseScrollArea = new QScrollArea();
    this->budgetObj_expenseScrollWidget = new QWidget();
    this->budgetObj_expenseScrollListVbox = new QVBoxLayout();
    this->budgetObj_expenseScrollArea->setWidget(this->budgetObj_expenseScrollWidget);
    this->budgetObj_expenseScrollArea->setWidgetResizable(true);
    this->budgetObj_expenseScrollWidget->setLayout(this->budgetObj_expenseScrollListVbox);
}

/**
 * parametrized constructor
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
    this->budgetObj_expenseScrollArea = new QScrollArea();
    this->budgetObj_expenseScrollWidget = new QWidget();
    this->budgetObj_expenseScrollListVbox = new QVBoxLayout();
    this->budgetObj_expenseScrollArea->setWidget(this->budgetObj_expenseScrollWidget);
    this->budgetObj_expenseScrollArea->setWidgetResizable(true);
    this->budgetObj_expenseScrollWidget->setLayout(this->budgetObj_expenseScrollListVbox);
}

/**
* creates a budget object from QJSONobject
 * @param parent parent object
 * @param json
 */
BudgetPageBudget::BudgetPageBudget(QObject *parent, const QJsonObject &json) : QObject{parent} {
    this->budget = json.value("Budget").toDouble();
    this->totalExpenses = json.value("Total Expenses").toDouble();
    this->remainingBudget = json.value("Remaining Budget").toDouble();
    this->budgetIndex = json.value("Index").toInt();
    QJsonArray expensesArray = json.value("Expenses").toArray();
    for (QJsonValue expense: expensesArray) {
        this->expenses.append(new BudgetPageExpenses(this, expense.toObject()));
    }
    this->budgetObj_expenseScrollArea = new QScrollArea();
    this->budgetObj_expenseScrollWidget = new QWidget();
    this->budgetObj_expenseScrollListVbox = new QVBoxLayout();
    this->budgetObj_expenseScrollArea->setWidget(this->budgetObj_expenseScrollWidget);
    this->budgetObj_expenseScrollArea->setWidgetResizable(true);
    this->budgetObj_expenseScrollWidget->setLayout(this->budgetObj_expenseScrollListVbox);
}

/**
 * getter for budget
 * @return budget
 */
double BudgetPageBudget::getBudget() {
    return budget;
}

/**
 * getter for total expenses
 * @return total expenses
 */
double BudgetPageBudget::getTotalExpenses() {
    return totalExpenses;
}

/**
 *  getter for remaining budgets
 * @return remaining budget
 */
double BudgetPageBudget::getRemainingBudget() {
    return remainingBudget;
}

/**
 * getter for expenses qvector
 * @return expenses qvector
 */
QVector<BudgetPageExpenses *> *BudgetPageBudget::getExpenses() {
    return &expenses;
}

/**
 * setter for budget
 * @param newbudget new budget
 */
void BudgetPageBudget::setBudget(double newbudget) {
    this->budget = newbudget;
}

/**
 * setter for total expense
 * @param newtotalexpense
 */
void BudgetPageBudget::setTotalExpenses(double newtotalexpense) {
    totalExpenses = newtotalexpense;
}

/**
 * setter for remaining budget
 * @param newremainingbudget
 */
void BudgetPageBudget::setRemainingBudget(double newremainingbudget) {
    remainingBudget = newremainingbudget;
}

/**
 *  setter for index
 * @param index
 */
void BudgetPageBudget::setIndex(int index) {
    this->budgetIndex = index;
}

/**
 * changes total expenses by delta
 * \n new total =  old total + delta
 * @param delta the change
 */
void BudgetPageBudget::changeTotalExpenses(double delta) {
    this->totalExpenses += delta;
}

/**
 * Creates a json object of the budget object
 * @return json object
            \n "Budget" - the budget
            \n "Total Expenses" - the total expenses
            \n "Remaining Budget" - the remaining budget
            \n "Expenses" an array with Expense::to_JSON()
            \n "Index" 0-18, the budget period (0-yearly) (1-5 Q1-Q4) (6-18 jan-dec)
 *          \n @copydoc BudgetPageExpenses::to_JSON()
 */
QJsonObject BudgetPageBudget::to_JSON() {
    QJsonObject jsonDoc;
    QJsonArray expensesArray;
    for (BudgetPageExpenses *expense: this->expenses) {
        expensesArray.append(expense->to_JSON());
    }
    jsonDoc.insert("Budget", this->budget);
    jsonDoc.insert("Total Expenses", this->totalExpenses);
    jsonDoc.insert("Remaining Budget", this->remainingBudget);
    jsonDoc.insert("Expenses", expensesArray);
    jsonDoc.insert("Index", this->budgetIndex);
    return jsonDoc;
}

QWidget *BudgetPageBudget::getExpensescrollwidget() {
    return budgetObj_expenseScrollWidget;
}

QScrollArea *BudgetPageBudget::getExpensescrollarea() {
    return budgetObj_expenseScrollArea;
}

QVBoxLayout *BudgetPageBudget::getExpensesscrolllistvbox() {
    return budgetObj_expenseScrollListVbox;
}
