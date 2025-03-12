#ifndef EXPENSES_H
#define EXPENSES_H

#include <QWidget>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QJsonObject>

class Expenses : public QWidget {
    Q_OBJECT

public:
    explicit Expenses(QWidget *parent = nullptr);
    explicit Expenses(QWidget *parent, QVBoxLayout *vbox);
    explicit Expenses(QWidget *parent, QVBoxLayout *vbox, QString name, QString description, double price, double quantity);
    explicit Expenses(QWidget *parent, QVBoxLayout *vbox, QJsonObject Expense);
    
    QJsonObject to_JSON();
    
private:
    QHBoxLayout *removeExpenseHbox;
    QWidget *newExpenseWidget;
    QFormLayout *addExpenseForm;
    QVBoxLayout *addexpenseLayout;
    QWidget *removeExpenseButtonWidget;

    QPushButton *removeExpenseButton;
    QLineEdit *expenseNameLineEdit;
    QLineEdit *expenseDescriptionLineEdit;
    QDoubleSpinBox *expenseQuantitySpinBox;
    QDoubleSpinBox *expensePriceSpinBox;

    QString *expenseName;
    QString *expenseDescription;
    double quantity;
    double price;

    void createExpenseUI(QWidget *parent, QVBoxLayout *vbox);

public slots:
    void onExpenseNameChangedSlot(const QString &newName);
    void onExpenseDescriptionChangedSlot(const QString &newDescription);
    void expenseSBChangedSlot(double change, char changedType);
    void updateExpense(); 

signals:
    void expenseChangedSignal(double oldExpense, double newExpense);
};

#endif 
