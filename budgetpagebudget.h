#ifndef BUDGETPAGEBUDGET_H
#define BUDGETPAGEBUDGET_H

#include <QObject>
#include <BudgetPageExpenses.h>
#include <QVector>
#include <QJsonArray>
#include <QJsonObject>
#include <QScrollArea>

class BudgetPageBudget : public QObject {
    Q_OBJECT

public:
    explicit BudgetPageBudget(QObject *parent = nullptr);

    BudgetPageBudget(QObject *parent, int budgetindex);

    BudgetPageBudget(QObject *parent, double budget, double totalexpense, double remainingbudget, int budgetindex);

    BudgetPageBudget(QObject *parent, const QJsonObject& json);


    double getBudget();

    double getTotalExpenses();

    double getRemainingBudget();

    QVector<BudgetPageExpenses *> *getExpenses();

    void setBudget(double newbudget);

    void setTotalExpenses(double newtotalexpense);

    void setRemainingBudget(double newremainingbudget);

    void setIndex(int index);

    void changeTotalExpenses(double delta);

    QJsonObject to_JSON();

    QWidget *getExpensescrollwidget();

    QScrollArea *getExpensescrollarea();

    QVBoxLayout *getExpensesscrolllistvbox();

private:
    QVector<BudgetPageExpenses *> expenses;
    double budget;
    double totalExpenses;
    double remainingBudget;
    int budgetIndex; // index 0 = yearly 1-5 = Q1-Q4, 6-18 = jan-dec
    QWidget *budgetObj_expenseScrollWidget;
    QScrollArea *budgetObj_expenseScrollArea;
    QVBoxLayout *budgetObj_expenseScrollListVbox;
};

#endif // BUDGETPAGEBUDGET_H
