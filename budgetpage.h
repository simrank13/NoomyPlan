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
#include <QLineEdit>
#include "budgetpagebudget.h"
#include "budgetpageexpenses.h"
/**
* This is the UI class for the budgetpage
* \n it allows users to pick a budget period, set a budget for that period, and add expenses
* \n it also allows users to view a bar graph that summarizes the total net budget for the fisical period
* \n it uses budgetpagebudget to store the budgets for different time periods (Quarters, months, and a yearly one)
* \n relies on budgetpagebudget and budgetpage expenses
* @copydoc budgetpagebudget
*/
class BudgetPage : public QMainWindow {
    Q_OBJECT

public:
    /**
    *  @brief default constructor for the budget page
    \n sets budget index to Quarterly and Q1, creates budget objects, displays UI
     * @param parent qwidget
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
     *
     * 
     * @return JSON with the budget data
     * \n "Budgets" JsonArray contaiting JSONs of budgetpagebudget
     * \n specified in - @copydoc BudgetPageBudget::to_JSON()
      * @author - Katherine R
     */
    QJsonObject to_JSON();

    /**
     * @brief adds json budget for offline/saves
     * imports JSON budget according to to_JSON() and adds them to the page
     * @param JSON with budget data - needs to be the same as to_JSON()
     * "Budgets" Array with budgetpagebudget JSONS
     * \n defined in @copydoc BudgetPageBudget::to_JSON()
    * @author - Katherine R
     */
    void getJSONBudget(const QJsonObject &budget);

    /**
     * @brief calculates the remaining budget for budgets[budgetPeriodIndex]
     * for budgets[budgetPeriodIndex] :budget - total expenses
     * \n if the remaining budget is below the financial surplus goal, the remaining budget text changes to red to notify user
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
     * \n uses a spinbox to set the budget, connects to onBudgetChangeSlot
      * @author - Katherine
     */
    void createBudgetSelector();

    /**
     * @brief creates a scrollable, dynamic list of expenses
     * \n creates a separate expense area for every budgetpagebudget, (so that each budget period can have it's own list of expenses)
     * \n can set the name, desc, price, and count
     * \n calculates total and remaining budget automatically using calculateRemainingBudget
     * @author - Katherine R
     */
    void createExpensesSubPage();

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
     * \n then calculates the new total by adding the change
     * \n after that, calculates the remaining budget using calculateRemainingBudget()
     * @param delta - change in expense
     * @author - Katherine R
     */
    void onExpenseChangedSlot(double delta);

    /**
     * @brief Slot for when the user changes the budget period type (monthly , quarterly, yearly)
     *\n then changes the text for the budget spinbox
     *\n then swaps the period selector combo box to the selected one (so for the Q1-Q4 spinbox if quarterly is selected)
     *\n and swaps the budgets to the selected one
     * @author - Katherine R
     */
    void onBudgetPeriodTypeChangedSlot(int index);

    /**
    * @brief slot detects when budget periods are changed
    * changes the budget object when another budget period is selected I.E (jan, feb, Q1..)
    * changes the budget label to X Budget - Y, X being quarterly/monthly/yrly, Y being Q1-Q5 jan-dec
    * @param index index of changed Combobox
    * @param period M for monthly Q for quarterly
    * @author - Katherine R
    */
    void onBudgetPeriodChangeSlot(int index, char period);


    /**
     * @brief Changes the budget page variables to the new budget period
     * swaps some expense UI elements
     * \n as many UI elements as possible are renamed instead of replaced
      * @author - Katherine R
     */
    void changeBudgetPage();

    /**
     * @brief creates a new expense item, adds it to the budget object and adds GUI
     * the expense item is created for the budgetpagebudget at the selected budget period
      * @author - Katherine R
     */
    void newExpense();

    /**
     * @brief deletes the expense item provided, removes it from the expenses QVector
     * deletes it from the budgetpagebudget for the selected budget period
     * \n and calculates new total expense
     * @param toDelete expense item to delete
      * @author - Katherine R
     */
    void deleteExpense(BudgetPageExpenses *toDelete);

    /**
     * @brief "updates" the bar graph
     * creates a new graph object, replacing the old one
     * the graph contains X bars, 1 per every budget period (4 for quarterly, 12 for monthly, 1 for yearly)
     * negative budgets show as a red bar, while positive budgets show as a black bar
     * \n trying to actually update the variables wouldn't work automatically,
     * \n so i decided to just add a button to "update" (create a new graph to replace)
      * @author - Katherine R
     */
    void updateBarGraph();

    /**
     * @brief QT Slot to detect changes in the budget goal SpinBox
     * \n changes the budget goal variable and calculates new remaining budget
     * @param goal new budget goal
     * @author - Katherine R
     */
    void onBudgetGoalChangedSlot(double goal);

    void addExpenseCategory();

  void changeExpenseCategory(int index);

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
    QVector<BudgetPageBudget *> budgets;
    int budgetPeriodIndex;
    QObject *budgetsObject;
    QLabel *budgetSelector_GoalLabel;
    QDoubleSpinBox *budgetSelector_goalSpinBox;

    // Expenses subpage
    QGroupBox *expenses_Group;
    QVBoxLayout *expenses_vbox;
    QPushButton *expenses_addExpenseButton;
    QLabel *expenses_remainingBudgetLabel;
    QLabel *expenses_totalExpensesLabel;
    QComboBox *expenses_categoriesComboBox;
    QLineEdit *expenses_categoryLineEdit;
    int expenses_categoriesComboBox_index;

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
    QStringList *barChart_categories_Quarterly;
    QStringList *barChart_categories_Monthly;

    QWidget *centralWidget;
    QGridLayout *budgetLayout;
};

#endif // BUDGETPAGE_H
