
#ifndef EXPENSES_H
#define EXPENSES_H


#include <QApplication>
#include <QPushButton>
#include <QLineEdit>
#include<QFormLayout>
#include<QSpinBox>
#include<QDebug>
#include<QMainWindow>
#include<QWidget>
#include<QComboBox>
#include<QObject>
#include<QJsonObject>

#define SHOW_DEBUG_LOGS true

class BudgetPageExpenses : public QObject {
    Q_OBJECT

public:
    explicit BudgetPageExpenses(QObject *parent = nullptr);

    BudgetPageExpenses(QObject *parent, const QString &name, const QString &description, double price,
                       double quantity);

    BudgetPageExpenses(QObject *parent, const QJsonObject &Expense);

    QJsonObject to_JSON() const;

    QPushButton *getRemoveButton();

    double getExpense() const;


    void createExpenseUI(QWidget *parent = nullptr, QVBoxLayout *vbox = nullptr);

    ~BudgetPageExpenses();

signals:
    void expenseChangedSignal(double delta);

public slots:
    void expenseSBChangedSlot(double change, char changedType);

    void onExpenseDescriptionChangedSlot(const QString &newDescription) const;

    void onExpenseNameChangedSlot(const QString &newName) const;

private:
    double price;
    double quantity;
    QString *expenseName;
    QString *expenseDescription;
    QWidget *expenseObj_ExpenseWidget;
    QFormLayout *expenseObj_addExpenseForm;
    QLineEdit *expenseObj_NameLineEdit;
    QLineEdit *expenseObj_DescriptionLineEdit;
    QDoubleSpinBox *expenseObj_QuantitySpinBox;
    QDoubleSpinBox *expenseObj_PriceSpinBox;
    QVBoxLayout *expenseObj_Layout;
    QWidget *expenseObj_removeExpenseButtonWidget;
    QHBoxLayout *expenseObj_removeExpenseHbox;
    QPushButton *expenseObj_removeExpenseButton;
};


#endif // EXPENSES_H
