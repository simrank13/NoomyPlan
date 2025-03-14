#ifndef BUDGETPAGE_H
#define BUDGETPAGE_H

#include <QMainWindow>
#include <QApplication>
#include <QGridLayout>
#include <QLabel>
#include<QGroupBox>
#include<QDebug>
#include<QWidget>
#include<QScrollArea>
#include <QObject>
#include <QListWidget>
#include <QJsonObject>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include "expenses.h"

#define SHOW_DEBUG_LOGS true

class BudgetPage : public QMainWindow {
    Q_OBJECT

public:
    explicit BudgetPage(QWidget *parent = nullptr);

    QJsonObject to_JSON();

    void getJSONBudget(QJsonObject budget);

private slots:
    void onBudgetChangeSlot(double budget);

    void onExpenseChangedSlot(double delta);

    void onBudgetPeriodChangedSlot(int index);

    void newExpense();

    void newExpense(QWidget *widget, QVBoxLayout *layout);

    void deleteExpense(int index);

    void deleteExpense(Expenses *toDelete);

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

    QVector<Expenses *> expenses;

    void calculateRemainingBudget();

    void createBudgetPeriodSelector();

    void createBudgetSelector();

    void createExpensesSubPage();
};

#endif // BUDGETPAGE_H
