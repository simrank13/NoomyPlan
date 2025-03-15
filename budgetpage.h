#ifndef BUDGETPAGE_H
#define BUDGETPAGE_H

#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include<QGroupBox>
#include <QListWidget>
#include <QtCharts/QBarSet>
#include <QtCharts/QChartview>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QChart>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
// #include<QDebug>
// #include<QWidget>
// #include<QScrollArea>s
// #include <QObject>
// #include <QJsonObject>
// #include <QPushButton>
// #include <QJsonArray>
#include "BudgetPageExpenses.h"
#include "budgetpagebudget.h"

#define SHOW_DEBUG_LOGS true

class BudgetPage : public QMainWindow {
    Q_OBJECT

public:
    explicit BudgetPage(QWidget *parent = nullptr);

    QJsonObject to_JSON();

    void getJSONBudget(const QJsonObject &budget);

    ~BudgetPage();

private slots:
    void onBudgetChangeSlot(double budget);

    void onExpenseChangedSlot(double delta);

    void onBudgetPeriodTypeChangedSlot(int index);

    void onBudgetPeriodChangeSlot(int index, char period);

    void newExpense();

    void deleteExpense(BudgetPageExpenses *toDelete);

    void updateBarGraph();


signals:

private:
    QWidget *centralWidget;
    QGridLayout *budgetLayout;

    QVBoxLayout *budgetSelector_VBox{};
    QGroupBox *budgetSelector_group{};
    QDoubleSpinBox *budgetSelector_SpinBox{};
    QLabel *budgetSelector_Label{};

    QGroupBox *expenses_Group{};
    QVBoxLayout *expenses_vbox{};
    QLabel *expenses_totalExpensesLabel{};
    QLabel *expenses_remainingBudgetLabel{};
    QPushButton *expenses_addExpenseButton{};

    QLabel *budgetPeriod_Label{};
    QComboBox *budgetPeriod_QuarterlyComboBox;
    QVBoxLayout *budgetPeriod_Vbox{};
    QGroupBox *budgetPeriod_GroupBox{};
    QComboBox *budgetPeriod_TypeComboBox{};
    QComboBox *budgetPeriod_MonthlyComboBox;

    QVector<BudgetPageBudget *> budgets;
    int budgetPeriodIndex;

    QWidget *barChart_Widget;
    QHBoxLayout *barChart_GroupVbox;
    QBarSet *barChart_Value;
    QBarSet *barChart_Neg;
    QStackedBarSeries *barChart_series;
    QChart *barChart_chart;
    QValueAxis *barChart_yAxis;
    QBarCategoryAxis *barChart_xAxis;
    QChartView *barChart_chartView;
    QStringList const barChart_categories_Quarterly{"Q1", "Q2", "Q3", "Q4"};
    QStringList const barChart_categories_Monthly{
        "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };
    QPushButton *barGraph_updateButton;


    void calculateRemainingBudget();

    void createBudgetPeriodSelector();

    void createBudgetSelector();

    void createExpensesSubPage();

    void changeBudgetPage();
};

#endif // BUDGETPAGE_H

//https://doc.qt.io/qt-6/qtcharts-stackedbarchart-example.html
