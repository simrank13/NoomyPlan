#include "expenses.h"
#include <QDebug>
#define SHOW_DEBUG_LOGS true

Expenses::Expenses(QWidget *parent)
    : QWidget(parent) {
        this->expenseName = new QString("");
        this->expenseDescription = new QString("");
        this->quantity = 1;
        this->price = 0;
        
}

Expenses::Expenses(QWidget *parent, QVBoxLayout *vbox) {
    this->expenseName = new QString("");
    this->expenseDescription = new QString("");
    this->quantity = 1;
    this->price = 0;
    createExpenseUI(parent, vbox);
}

Expenses::Expenses(QWidget *parent, QVBoxLayout *vbox, QString name, QString description, double price, double quantity
) {
    expenseName = new QString(name);
    expenseDescription = new QString(description);
    this->price = price;
    this->quantity = quantity;
    createExpenseUI(parent, vbox);
}

Expenses::Expenses(QWidget *parent, QVBoxLayout *vbox, QJsonObject Expense) {
    expenseName = new QString(Expense.value("name").toString());
    expenseDescription = new QString(Expense.value("description").toString());
    price = Expense.value("price").toDouble();
    quantity = Expense.value("quantity").toDouble();
    createExpenseUI(parent, vbox);
}

void Expenses::createExpenseUI(QWidget *parent, QVBoxLayout *vbox) {
    newExpenseWidget = new QWidget;
    addExpenseForm = new QFormLayout;
    addexpenseLayout = new QVBoxLayout;
    removeExpenseButtonWidget = new QWidget();

    expenseNameLineEdit = new QLineEdit;
    expenseDescriptionLineEdit = new QLineEdit;
    expenseQuantitySpinBox = new QDoubleSpinBox;
    expensePriceSpinBox = new QDoubleSpinBox;

    expenseNameLineEdit->setPlaceholderText(tr("Name"));
    expenseDescriptionLineEdit->setPlaceholderText(tr("Description"));
    expenseQuantitySpinBox->setRange(1, 999);
    expensePriceSpinBox->setPrefix(tr("$\t"));
    expensePriceSpinBox->setMaximum(100000000);
    expensePriceSpinBox->setDecimals(2);
    expenseQuantitySpinBox->setDecimals(0);

    addExpenseForm->addRow(tr("Quantity:"), expenseQuantitySpinBox);
    addExpenseForm->addRow(tr("Name:"), expenseNameLineEdit);
    addExpenseForm->addRow(tr("Description:"), expenseDescriptionLineEdit);
    addExpenseForm->addRow(tr("Price:"), expensePriceSpinBox);

    expenseNameLineEdit->setText(*this->expenseName);
    expenseDescriptionLineEdit->setText(*this->expenseDescription);
    expenseQuantitySpinBox->setValue(this->quantity);
    expensePriceSpinBox->setValue(this->price);

    connect(expenseQuantitySpinBox, &QDoubleSpinBox::valueChanged, this,
            [this](double value) { expenseSBChangedSlot(value, 'Q'); });
    connect(expensePriceSpinBox, &QDoubleSpinBox::valueChanged, this,
            [this](double value) { expenseSBChangedSlot(value, 'P'); });

    connect(expenseNameLineEdit, &QLineEdit::textChanged, this, &Expenses::onExpenseNameChangedSlot);
    connect(expenseDescriptionLineEdit, &QLineEdit::textChanged, this, &Expenses::onExpenseDescriptionChangedSlot);

    removeExpenseHbox = new QHBoxLayout(removeExpenseButtonWidget);
    removeExpenseButton = new QPushButton(tr("Remove"));
    connect(removeExpenseButton, &QPushButton::clicked, newExpenseWidget, &QWidget::deleteLater);

    removeExpenseHbox->addWidget(removeExpenseButton);
    addexpenseLayout->addLayout(addExpenseForm);
    addexpenseLayout->addWidget(removeExpenseButtonWidget);
    newExpenseWidget->setLayout(addexpenseLayout);

    vbox->addWidget(newExpenseWidget);
    QWidget::update();
}

QJsonObject Expenses::to_JSON() {
    QJsonObject expenseJson;
    expenseJson.insert("name", *expenseName);
    expenseJson.insert("description", *expenseDescription);
    expenseJson.insert("price", QString::number(price));
    expenseJson.insert("quantity", quantity);
    return expenseJson;
}

void Expenses::onExpenseNameChangedSlot(const QString &newName) {
    *expenseName = newName;
    if (SHOW_DEBUG_LOGS) {
        qDebug() << "New Expense Name - " << *this->expenseName;
    }
}

void Expenses::onExpenseDescriptionChangedSlot(const QString &newDescription) {
    *expenseDescription = newDescription;
    if (SHOW_DEBUG_LOGS) {
        qDebug() << *this->expenseName << " - New Expense Description - " << *this->expenseDescription;
    }
}



void Expenses::expenseSBChangedSlot(double change, char changedType) {
    double oldExpenses = this->quantity * this->price;
    switch (changedType) {
        case 'Q':
            this->quantity = change;
            break;
        case 'P':
            this->price = change;
            break;
        default:
            break;
    }
    emit expenseChangedSignal(oldExpenses, this->quantity * this->price);
}

void Expenses::updateExpense() {
    *expenseName = expenseNameLineEdit->text();
    *expenseDescription = expenseDescriptionLineEdit->text();
    quantity = expenseQuantitySpinBox->value();
    price = expensePriceSpinBox->value();
    qDebug() << "Expense Updated: " << *expenseName << " - " << *expenseDescription << " - Qty: " << quantity << " - Price: " << price;
}
