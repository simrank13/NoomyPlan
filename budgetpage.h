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
#include "budgetpagebudget.h"
#include "budgetpageexpenses.h"

class BudgetPage : public QMainWindow {
    Q_OBJECT

public:
    /**
    *  @brief default constructor for the budget page
    \n sets budget index to Quarterly and Q1, creates budget objects, displays UI
     * @param parent 
     * @author - Katherine R
     */
    explicit BudgetPage(QWidget *parent = nullptr);
    /**
     * @brief destructor for budgetpage
      * @author Katherine R
     */
    ~BudgetPage();
    /**
     * @brief saves data of the budget page for offline mode
     * saves a JSON with any relevant data
     * @return JSON with the budget data
     * \n @copydoc BudgetPageBudget::to_JSON()
      * @author - Katherine R
     */
    QJsonObject to_JSON();
    /**
     * @brief adds json budget for offline/saves
     imports JSON budget according to to_JSON()
     * @param JSON with budget data - needs to be the same as to_JSON()
     * \n @copydoc BudgetPageBudget::to_JSON()
    * @author - Katherine R
     */
    void getJSONBudget(const QJsonObject &budget);

public slots:
    /**
     * @brief QT Slot to detect changes in the budget SpinBox
     * \n changes the budget variable and calculates new remaining budget
     * @param budget new budget
     * @author - Katherine R
     */
    void onBudgetChangeSlot(double budget);
    /**
     * @brief QT Slot to detect changes in Expense object
     * \n then calculates the new total and remaining budget
     * @param delta - change in expense
     * @author - Katherine R
     */
    void onExpenseChangedSlot(double delta);
    /**
     * @brief Slot for when the user changes the budget period type (monthly , quarterly, yearly)
     *\n then changes the text for the budget spinbox
     *\n and swaps the budgets to the selected one
     * @author - Katherine R
     */
    void onBudgetPeriodTypeChangedSlot(int index);
    /**
     * @brief detects when budget periods are changed
     * changes the budget object when another budget period is selected I.E (jan, feb, Q1..)
     * @param index index of changed Combobox
     * @param period M for monthly Q for quarterly
     * @author - Katherine R
     */
    void onBudgetPeriodChangeSlot(int index, char period);
    /**
     * @brief calculates the remaining budget for budgets[budgetPeriodIndex]
    * for budgets[budgetPeriodIndex] :budget - total expenses
     * @author - Katherine R
     */
    void calculateRemainingBudget();
    /**
     * @brief creates the budget period selector widget
     * \n allows selection of budget period (Monthly, Yearly, Quarterly) for BudgetPage using a combobox
     * \n and (Q1-Q4) (jan-dec)
     * @author - Katherine R
     */
    void createBudgetPeriodSelector();
    /**
     * @brief creates a budget selector for BudgetPage
     * \n uses a spinbox to set the budget
      * @author - Katherine 
     */
    void createBudgetSelector();
    /**
     * @brief creates a scrollable, dynamic list of expenses
     * \n can set the name, desc, price, and count
     * \n calculates total and remaining budget automatically using calculateRemainingBudget 
     * @author - Katherine R
     */
    void createExpensesSubPage();
    /**
     * @brief Changes the budget page variables to the new budget period
     * \n as many UI elements as possible are renamed instead of replaced
      * @author - Katherine R
     */
    void changeBudgetPage();
    /**
     * @brief creates a new expense item, adds it to the budget object and adds GUI
      * @author - Katherine R
     */
    void newExpense();
    /**
     * @brief deletes the expense item provided, removes it from the expenses QVector
     * \n and calculates new total expense
     * @param toDelete expense item to delete
      * @author - Katherine R
     */
    void deleteExpense(BudgetPageExpenses *toDelete);
    /**
     * @brief "updates" the bar graph
     * \n trying to actually update the variables wouldn't work automatically,
     * \n so i decided to just add a button to "update" (create a new graph to replace)
      * @author - Katherine R
     */
    void updateBarGraph();

private:
    // Budget period selector
    QGroupBox *budgetPeriod_GroupBox;
    QLabel *budgetPeriod_Label;
    QComboBox *budgetPeriod_TypeComboBox;
    QVBoxLayout *budgetPeriod_Vbox;
    QComboBox *budgetPeriod_QuarterlyComboBox;
    QComboBox *budgetPeriod_MonthlyComboBox;

    // Budget selector
    QGroupBox *budgetSelector_group;
    QLabel *budgetSelector_Label;
    QDoubleSpinBox *budgetSelector_SpinBox;
    QVBoxLayout *budgetSelector_VBox;

    // Expenses subpage
    QGroupBox *expenses_Group;
    QVBoxLayout *expenses_vbox;
    QPushButton *expenses_addExpenseButton;
    QLabel *expenses_remainingBudgetLabel;
    QLabel *expenses_totalExpensesLabel;

    // Bar graph
    QWidget *barChart_Widget;
    QHBoxLayout *barChart_GroupVbox;
    QBarSet *barChart_Value;
    QBarSet *barChart_Neg;
    QBarCategoryAxis *barChart_xAxis;
    QStackedBarSeries *barChart_series;
    QChart *barChart_chart;
    QValueAxis *barChart_yAxis;
    QChartView *barChart_chartView;
    QPushButton *barGraph_updateButton;

    // Core members
    QVector<BudgetPageBudget *> budgets;
    int budgetPeriodIndex;
    QWidget *centralWidget;
    QGridLayout *budgetLayout;

    // Static data for bar chart
    static const QStringList barChart_categories_Quarterly;
    static const QStringList barChart_categories_Monthly;
};

#endif // BUDGETPAGE_H
