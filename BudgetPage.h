#ifndef BUDGETPAGE_H
#define BUDGETPAGE_H

#include <QApplication>
#include <QGridLayout>
#include <QLabel>
#include<QGroupBox>
#include<QDebug>
#include<QMainWindow>
#include<QWidget>
#include<QScrollArea>
#include <QObject>
#include <QListWidget>
#include "Expenses.h"

#define SHOW_DEBUG_LOGS true



class BudgetPage : public QMainWindow {
    // Q_OBJECT
public:
    explicit BudgetPage(QWidget *parent = nullptr);

private slots:
    void onBudgetChangeSlot(double budget);

    void onExpenseChangedSlot(double oldExpense, double newExpense);

    void onBudgetPeriodChangedSlot(int index);

    void newExpense();

signals:

private:
    double budget;
    double totalExpenses;
    double remainingBudget;
    QWidget *centralWidget;
    QWidget *expensesScrollWidget{};
    QGroupBox *expensesGroup{};
    QGridLayout *budgetLayout;
    QVBoxLayout *expensesVboxLayout{};
    QVBoxLayout *expensesScrollListVbox{};
    QVBoxLayout *budgetVBox{};
    QScrollArea *expenseScrollArea{};
    QGroupBox *budgetPeriodGroupBox{};
    QComboBox *budgetPeriodComboBox{};
    QVBoxLayout *budgetPeriodVbox{};
    QGroupBox *budgetGroup{};
    QDoubleSpinBox *budgetSpinBox{};
    QLabel *totalExpensesLabel{};
    QLabel *remainingBudgetLabel{};
    QLabel *budgetPeriodLabel{};
    QLabel *budgetLabel{};
    QPushButton *addExpenseButton{};
    // QVector<Expenses *> expenses;
    void calculateRemainingBudget();

    void createBudgetPeriodSelector();

    void createBudgetSelector();

    void createExpensesSubPage();
};

#endif //BUDGETPAGE_H
