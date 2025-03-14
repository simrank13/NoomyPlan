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
#include<QJsonDocument>
#include<QJsonObject>

#define SHOW_DEBUG_LOGS true
class Expenses : public QObject
{
    Q_OBJECT
public:
    explicit Expenses(QObject *parent = nullptr);

    Expenses(QObject *parent, QString name, QString description, double price,
             double quantity);

    Expenses(QObject *parent, QVBoxLayout *vbox, QJsonObject Expense);


    QJsonObject to_JSON();

    QPushButton* getRemoveButton();

    double getExpense();


    void createExpenseUI(QWidget *parent = nullptr, QVBoxLayout *vbox = nullptr);

    ~Expenses();

signals:
    void expenseChangedSignal(double delta);
public slots:
    void expenseSBChangedSlot(double change, char changedType);

    void onExpenseDescriptionChangedSlot(const QString &newDescription);

    void onExpenseNameChangedSlot(const QString &newName);
private:
    double price;
    double quantity;
    QString *expenseName;
    QString *expenseDescription;
    QWidget *newExpenseWidget;
    QFormLayout *addExpenseForm;
    QLineEdit *expenseNameLineEdit;
    QLineEdit *expenseDescriptionLineEdit;
    QDoubleSpinBox *expenseQuantitySpinBox;
    QDoubleSpinBox *expensePriceSpinBox;
    QVBoxLayout *addexpenseLayout;
    QWidget *removeExpenseButtonWidget;
    QHBoxLayout *removeExpenseHbox;
    QPushButton *removeExpenseButton;

};



#endif // EXPENSES_H
