#ifndef BUDGETPAGEBUDGET_H
#define BUDGETPAGEBUDGET_H

#include <QObject>
#include "budgetpageexpenses.h"
#include <QVector>
#include <QJsonArray>
#include <QJsonObject>
#include <QScrollArea>
/**
*This class stores budget and expenses information 
* it stores the budget, remaining and total expenses
* and stores a dynamic vector array w budgetpageexpenses
* it also stores some UI elements for the expenses area
* @copydoc BudgetPageExpenses
* @author Katherine R
**/
class BudgetPageBudget : public QObject {
    Q_OBJECT

public:
    /**
     * @brief default constructor, everything set to 0.
     *also creates necessary UI objects
     * @param parent parent qobject
      * @author Katherine R
     */
    explicit BudgetPageBudget(QObject *parent = nullptr);

    /**
     * @brief semi parametrized constructor, everything set to 0, except index
     *also creates necessary UI objects
     * @param parent parent qobject
     * @param budgetindex the index (0 = year, 1-5 = Q1-Q4, 6-18 = jan-dec) for to_JSON
      * @author Katherine R
     */
    BudgetPageBudget(QObject *parent, int budgetindex);

    /**
    * @brief parametrized constructor
    * @param parent parent qobject
    * @param budget budget
    * @param totalexpense total expenses
    * @param remainingbudget remaining budget
    * @param goal budget goal
    * @param budgetindex the index for the period of the budget (0=yearly, 1-5=quarterly, 6-18=monthly)
    */
    BudgetPageBudget(QObject *parent, double budget, double totalexpense, double remainingbudget, int budgetindex,
                     double goal);

    /**
    * @brief parametrized constructor - creates a budget object from QJSONobject
     * @param parent parent object
     * @param json
     * JSON needs to be the same as to_JSON()
     @copydoc BudgetPageBudget::to_JSON()
      * @author Katherine R
     */
    BudgetPageBudget(QObject *parent, const QJsonObject &json);

    /**
     * @brief getter for budget
     * @return budget
      * @author Katherine R
     */
    double getBudget() const;

    /**
     * @brief getter for total expenses
     * @return total expenses
      * @author Katherine R
     */
    double getTotalExpenses() const;

    /**
     *  @brief getter for remaining budgets
     * @return remaining budget
      * @author Katherine R
     */
    double getRemainingBudget() const;

    /**
     * @brief getter for expenses qvector
     * @return expenses qvector
      * @author Katherine R
     */
    QVector<BudgetPageExpenses *> *getExpenses();

    /**
     * @brief setter for budget
     * @param newbudget new budget
      * @author Katherine R
     */
    void setBudget(double newbudget);

    /**
     * @brief setter for total expense
     * @param newtotalexpense
      * @author Katherine R
     */
    void setTotalExpenses(double newtotalexpense);

    /**
     * @brief setter for remaining budget
     * @param newremainingbudget
      * @author Katherine R
     */
    void setRemainingBudget(double newremainingbudget);

    /**
     * @brief setter for index
     * @param index
      * @author Katherine R
     */
    void setIndex(int index);

    /**
     * @brief changes total expenses by delta
     * \n new total =  old total + delta
     * @param delta the change
      * @author Katherine R
     */
    void changeTotalExpenses(double delta);

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
    QJsonObject to_JSON();

    /**
    *@Brief getter for scrollwidget
    @author Katherine R
    @return the expenses widget
    */
    QWidget *getExpensescrollwidget();

    /**
    *@Brief getter for expenses scroll area
    @author Katherine R
    @return the expenses Qscrollarea for scroll area
    */
    QScrollArea *getExpensescrollarea();

    /**@Brief getter for expenses scroll area at index
   *@param index - the index
   @author Katherine R
   @return the expenses scroll area
   */
    QScrollArea *getExpensescrollarea(int index);

    /**
    *@Brief getter for expenses vbox
    @author Katherine R
    @return the expenses vbox
    */
    QVBoxLayout *getExpensesscrolllistvbox();

    /**
    *@Brief getter for expenses vbox at index
    *@param index - the index
    @author Katherine R
    @return the expenses vbox
    */
    QVBoxLayout *getExpensesscrolllistvbox(int index);

    /**
      *@Brief getter for budget goal
      @author Katherine R
      @return the budget goal
      */
    double getBudgetGoal() const;

    /**
     * @brief setter for budget goal
     * @param newbudgetgoal the new goal
      * @author Katherine R
     */
    void setBudgetGoal(double newbudgetgoal);

    /**
     *@brief appends a new scroll area, scroll widget, and scroll list vbox to the vectors, for expenses
     */
    void newExpensescrollArea();

private:
    QVector<BudgetPageExpenses *> expenses; //stores expense items
    double budget;
    double totalExpenses;
    double remainingBudget;
    int budgetIndex; // index 0 = yearly 1-5 = Q1-Q4, 6-18 = jan-dec
    double budgetGoal;
    QVector<QWidget *> budgetObj_expenseScrollWidget; //vector storing a qwidget per expense category
    QVector<QScrollArea *> budgetObj_expenseScrollArea; //vector storing qscrollarea per expense category
    QVector<QVBoxLayout *> budgetObj_expenseScrollListVbox; //vector storing a vbox per expense category
};

#endif // BUDGETPAGEBUDGET_H
