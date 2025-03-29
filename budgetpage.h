#ifndef BUDGETPAGE_H
#define BUDGETPAGE_H

#include <QMainWindow>
#include <QWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QJsonObject>
#include <QJsonArray>
#include <QChart>
#include <QChartView>
#include <QBarSet>
#include <QStackedBarSeries>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QTableWidget>
#include "budgetpagebudget.h"
#include "budgetpageexpenses.h"

/**
 * @class BudgetPage
 * @brief Main window for managing budgets, expenses, and visualizations.
 *
 * This class provides a user interface for managing budgets, tracking expenses,
 * and displaying data through a bar chart. It includes functionality for
 * creating, updating, and deleting budgets and expenses. The class also handles
 * JSON serialization and deserialization for saving and loading budget data.
 *
 * @author Katherine R
 */
class BudgetPage : public QMainWindow {
    Q_OBJECT

public:
    /**
     * @brief Constructs a BudgetPage object.
     *
     * Initializes the main window and sets up the UI components for managing budgets,
     * expenses, and visualizations. It also connects signals and slots for user interactions.
     *
     * @param parent The parent widget, default is nullptr.
     */
    explicit BudgetPage(QWidget* parent = nullptr);

    /**
     * @brief Destructor for BudgetPage.
     *
     * Cleans up dynamically allocated resources and ensures proper memory management.
     */
    ~BudgetPage();

    /**
     * @brief Converts the current budget data to a JSON object.
     *
     * This method serializes the budget data, including expenses and remaining budget,
     * into a JSON object for storage or transmission.
     *
     * @return QJsonObject containing the budget data.
     */
    QJsonObject to_JSON();

    /**
     * @brief Loads budget data from a JSON object.
     *
     * This method deserializes the budget data from a JSON object and updates the UI
     * to reflect the loaded data.
     *
     * @param budget The JSON object containing budget data.
     */
    void getJSONBudget(const QJsonObject& budget);

    void setCurrentUserId(const QString& id);
    void saveExpenses();
    void loadExpenses();

public slots:
    /**
     * @brief Slot to handle budget changes.
     *
     * This slot is triggered when the user updates the budget amount. It recalculates
     * the remaining budget and updates the UI accordingly.
     *
     * @param budget The new budget value.
     */
    void onBudgetChangeSlot(double budget);

    /**
     * @brief Slot to handle expense changes.
     *
     * This slot is triggered when an expense is added, modified, or deleted. It updates
     * the total expenses and remaining budget.
     *
     * @param delta The change in expense value.
     */
    void onExpenseChangedSlot(double delta);

    /**
     * @brief Slot to handle budget period type changes.
     *
     * This slot is triggered when the user selects a different budget period type
     * (e.g., quarterly, monthly). It updates the UI to reflect the selected period type.
     *
     * @param index The selected index of the period type.
     */
    void onBudgetPeriodTypeChangedSlot(int index);

    /**
     * @brief Slot to handle budget period changes.
     *
     * This slot is triggered when the user selects a specific budget period (e.g., Q1, January).
     * It updates the UI to display the budget and expenses for the selected period.
     *
     * @param index The selected index of the period.
     * @param period The type of period ('Q' for quarterly, 'M' for monthly).
     */
    void onBudgetPeriodChangeSlot(int index, char period);

    /**
     * @brief Calculates and updates the remaining budget.
     *
     * This method calculates the remaining budget by subtracting the total expenses
     * from the total budget and updates the corresponding UI label.
     */
    void calculateRemainingBudget();

    /**
     * @brief Creates the budget period selector UI.
     *
     * This method initializes the UI components for selecting the budget period type
     * (e.g., quarterly, monthly) and specific periods (e.g., Q1, January).
     */
    void createBudgetPeriodSelector();

    /**
     * @brief Creates the budget selector UI.
     *
     * This method initializes the UI components for entering and displaying the budget amount.
     */
    void createBudgetSelector();

    /**
     * @brief Creates the expenses subpage UI.
     *
     * This method initializes the UI components for managing expenses, including the
     * list of expenses, remaining budget, and total expenses labels.
     */
    void createExpensesSubPage();

    /**
     * @brief Updates the budget page based on the selected period.
     *
     * This method refreshes the UI to display the budget and expenses for the currently
     * selected budget period.
     */
    void changeBudgetPage();

    /**
     * @brief Adds a new expense to the current budget.
     *
     * This method creates a new expense entry and adds it to the current budget. It also
     * updates the total expenses and remaining budget.
     */
    void newExpense();

    /**
     * @brief Deletes an expense from the current budget.
     *
     * This method removes the specified expense from the current budget and updates
     * the total expenses and remaining budget.
     *
     * @param toDelete The expense to delete.
     */
    void deleteExpense(BudgetPageExpenses* toDelete);

    /**
     * @brief Updates the bar graph with current data.
     *
     * This method refreshes the bar chart to reflect the current budget and expense data.
     */
    void updateBarGraph();

private:
    // budget period selector
    QGroupBox* budgetPeriod_GroupBox; // group box for budget period selection.
    QLabel* budgetPeriod_Label; // label for budget period selection.
    QComboBox* budgetPeriod_TypeComboBox; // combo box for period type selection.
    QVBoxLayout* budgetPeriod_Vbox; // vertical layout for period selection.
    QComboBox* budgetPeriod_QuarterlyComboBox; // combo box for quarterly periods.
    QComboBox* budgetPeriod_MonthlyComboBox; // combo box for monthly periods.

    // budget selector
    QGroupBox* budgetSelector_group; // group box for budget selection.
    QLabel* budgetSelector_Label; // label for budget selection.
    QDoubleSpinBox* budgetSelector_SpinBox; // spin box for budget input.
    QVBoxLayout* budgetSelector_VBox; // vertical layout for budget selection.

    // expenses subpage
    QGroupBox* expenses_Group; // group box for expenses.
    QVBoxLayout* expenses_vbox; // vertical layout for expenses.
    QPushButton* expenses_addExpenseButton; // button to add new expenses.
    QLabel* expenses_remainingBudgetLabel; // label for remaining budget.
    QLabel* expenses_totalExpensesLabel; // label for total expenses.

    // bar graph
    QWidget* barChart_Widget; // widget for the bar chart.
    QHBoxLayout* barChart_GroupVbox; // layout for the bar chart group.
    QBarSet* barChart_Value; // bar set for positive values.
    QBarSet* barChart_Neg; // bar set for negative values.
    QBarCategoryAxis* barChart_xAxis; // x-axis for the bar chart.
    QStackedBarSeries* barChart_series; // series for the bar chart.
    QChart* barChart_chart; // chart for the bar graph.
    QValueAxis* barChart_yAxis; // y-axis for the bar chart.
    QChartView* barChart_chartView; // view for the bar chart.
    QPushButton* barGraph_updateButton; ///< Button to update the bar graph.

    // core members
    QVector<BudgetPageBudget*> budgets; // list of budgets.
    int budgetPeriodIndex; // index of the current budget period.
    QWidget* centralWidget; // central widget for the main window.
    QGridLayout* budgetLayout; // layout for the budget page.

    // static data for bar chart
    static const QStringList barChart_categories_Quarterly; ///< categories for quarterly periods.
    static const QStringList barChart_categories_Monthly; ///< categories for monthly periods.

    QTableWidget* expensesTable;

    QString currentUserId;
};

#endif // BUDGETPAGE_H