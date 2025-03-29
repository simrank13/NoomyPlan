#ifndef BUDGETPAGEBUDGET_H
#define BUDGETPAGEBUDGET_H

#include <QObject>
#include "budgetpageexpenses.h"
#include <QVector>
#include <QJsonArray>
#include <QJsonObject>
#include <QScrollArea>

/**
 * @class BudgetPageBudget
 * @brief Represents a budget with associated expenses and calculations.
 *
 * This class manages a budget, including its total expenses, remaining budget,
 * and a list of expenses. It also provides functionality to serialize and
 * deserialize budget data to/from JSON. The class supports dynamic updates to
 * expenses and recalculates the remaining budget accordingly.
 *
 * @author Katherine R
 */
class BudgetPageBudget : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Constructs a BudgetPageBudget object with default values.
     *
     * Initializes the budget with default values for total budget, total expenses,
     * and remaining budget. The budget index is set to 0 (yearly).
     *
     * @param parent The parent QObject, default is nullptr.
     */
    explicit BudgetPageBudget(QObject* parent = nullptr);

    /**
     * @brief Constructs a BudgetPageBudget object with a specified index.
     *
     * Initializes the budget with default values for total budget, total expenses,
     * and remaining budget. The budget index is set to the provided value.
     *
     * @param parent The parent QObject.
     * @param budgetindex The index of the budget.
     */
    BudgetPageBudget(QObject* parent, int budgetindex);

    /**
     * @brief Constructs a BudgetPageBudget object with specified values.
     *
     * Initializes the budget with the provided values for total budget, total expenses,
     * remaining budget, and budget index.
     *
     * @param parent The parent QObject.
     * @param budget The total budget amount.
     * @param totalexpense The total expenses.
     * @param remainingbudget The remaining budget.
     * @param budgetindex The index of the budget.
     */
    BudgetPageBudget(QObject* parent, double budget, double totalexpense, double remainingbudget, int budgetindex);

    /**
     * @brief Constructs a BudgetPageBudget object from a JSON object.
     *
     * Initializes the budget by deserializing data from a JSON object. The JSON object
     * should contain the total budget, total expenses, remaining budget, and budget index.
     *
     * @param parent The parent QObject.
     * @param json The JSON object containing budget data.
     */
    BudgetPageBudget(QObject* parent, const QJsonObject& json);

    /**
     * @brief Gets the total budget amount.
     *
     * This method returns the total budget amount set for this budget.
     *
     * @return The total budget.
     */
    double getBudget();

    /**
     * @brief Gets the total expenses.
     *
     * This method returns the total expenses recorded for this budget.
     *
     * @return The total expenses.
     */
    double getTotalExpenses();

    /**
     * @brief Gets the remaining budget.
     *
     * This method returns the remaining budget, calculated as the difference
     * between the total budget and total expenses.
     *
     * @return The remaining budget.
     */
    double getRemainingBudget();

    /**
     * @brief Gets the list of expenses.
     *
     * This method returns a pointer to the vector containing all expenses
     * associated with this budget.
     *
     * @return A pointer to the vector of expenses.
     */
    QVector<BudgetPageExpenses*>* getExpenses();

    /**
     * @brief Sets the total budget amount.
     *
     * This method updates the total budget amount and recalculates the
     * remaining budget.
     *
     * @param newbudget The new budget amount.
     */
    void setBudget(double newbudget);

    /**
     * @brief Sets the total expenses.
     *
     * This method updates the total expenses and recalculates the remaining budget.
     *
     * @param newtotalexpense The new total expenses.
     */
    void setTotalExpenses(double newtotalexpense);

    /**
     * @brief Sets the remaining budget.
     *
     * This method directly sets the remaining budget. It is typically used
     * when loading data from a JSON object.
     *
     * @param newremainingbudget The new remaining budget.
     */
    void setRemainingBudget(double newremainingbudget);

    /**
     * @brief Sets the budget index.
     *
     * This method updates the budget index, which determines the period
     * (e.g., yearly, quarterly, monthly) this budget represents.
     *
     * @param index The new budget index.
     */
    void setIndex(int index);

    /**
     * @brief Updates the total expenses by a delta value.
     *
     * This method adjusts the total expenses by the specified delta value
     * and recalculates the remaining budget.
     *
     * @param delta The change in expenses.
     */
    void changeTotalExpenses(double delta);

    /**
     * @brief Converts the budget data to a JSON object.
     *
     * This method serializes the budget data, including total budget, total expenses,
     * remaining budget, and budget index, into a JSON object.
     *
     * @return The JSON object containing budget data.
     */
    QJsonObject to_JSON();

    /**
     * @brief Gets the widget for the expenses scroll area.
     *
     * This method returns the widget used to display the list of expenses
     * in a scrollable area.
     *
     * @return The expenses scroll widget.
     */
    QWidget* getExpensescrollwidget();

    /**
     * @brief Gets the scroll area for expenses.
     *
     * This method returns the scroll area used to display the list of expenses.
     *
     * @return The expenses scroll area.
     */
    QScrollArea* getExpensescrollarea();

    /**
     * @brief Gets the vertical layout for the expenses list.
     *
     * This method returns the vertical layout used to organize the list of expenses
     * within the scroll area.
     *
     * @return The expenses scroll list layout.
     */
    QVBoxLayout* getExpensesscrolllistvbox();

private:
    QVector<BudgetPageExpenses*> expenses; // list of expenses associated with this budget.
    double budget; // total budget amount.
    double totalExpenses; // total expenses recorded for this budget.
    double remainingBudget; // remaining budget, calculated as total budget minus total expenses.
    int budgetIndex; // index of the budget (0 = yearly, 1-4 = Q1-Q4, 5-16 = Jan-Dec).
    QWidget* budgetObj_expenseScrollWidget; // widget for the expenses scroll area.
    QScrollArea* budgetObj_expenseScrollArea; // scroll area for displaying expenses.
    QVBoxLayout* budgetObj_expenseScrollListVbox; // vertical layout for organizing expenses in the scroll area.
};

#endif // BUDGETPAGEBUDGET_H