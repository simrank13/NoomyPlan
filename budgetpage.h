#ifndef BUDGETPAGE_H
#define BUDGETPAGE_H

#include <QMainWindow>
#include <QGroupBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QScrollArea>
#include <QJsonObject>
#include "expenses.h" 

class BudgetPage : public QMainWindow {
    Q_OBJECT

public:
    explicit BudgetPage(QWidget *parent = nullptr);

    QJsonObject to_JSON();
    void getJSONBudget(QJsonObject budget);

public slots:
    void onBudgetChangeSlot(double budget);
    void onExpenseChangedSlot(double oldExpense, double newExpense);
    void onBudgetPeriodChangedSlot(int index);
    void addExpense();
    void updateTotalExpenses(double oldExpense, double newExpense);

private:
    void createBudgetPeriodSelector();
    void createBudgetSelector();
    void createExpensesSubPage();
    void calculateRemainingBudget();

    QWidget *centralWidget;
    QGridLayout *budgetLayout;
    QGroupBox *budgetPeriodGroupBox;
    QLabel *budgetPeriodLabel;
    QComboBox *budgetPeriodComboBox;
    QVBoxLayout *budgetPeriodVbox;

    QGroupBox *budgetGroup;
    QLabel *budgetLabel;
    QDoubleSpinBox *budgetSpinBox;
    QVBoxLayout *budgetVBox;

    QGroupBox *expensesGroup;
    QVBoxLayout *expensesVboxLayout;
    QPushButton *addExpenseButton;
    QLabel *remainingBudgetLabel;
    QLabel *totalExpensesLabel;
    
    QScrollArea *expenseScrollArea;
    QWidget *expensesScrollWidget;
    QVBoxLayout *expensesScrollListVbox;

    double budget;
    double totalExpenses;
    double remainingBudget;
};

#endif
