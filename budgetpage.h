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
    explicit BudgetPage(QWidget *parent = nullptr);
    ~BudgetPage();
    QJsonObject to_JSON();
    void getJSONBudget(const QJsonObject &budget);

public slots:
    void onBudgetChangeSlot(double budget);
    void onExpenseChangedSlot(double delta);
    void onBudgetPeriodTypeChangedSlot(int index);
    void onBudgetPeriodChangeSlot(int index, char period);
    void calculateRemainingBudget();
    void createBudgetPeriodSelector();
    void createBudgetSelector();
    void createExpensesSubPage();
    void changeBudgetPage();
    void newExpense();
    void deleteExpense(BudgetPageExpenses *toDelete);
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
    // Removed: static const bool SHOW_DEBUG_LOGS = true;  // Now using macro from budgetpageexpenses.h
};

#endif // BUDGETPAGE_H
