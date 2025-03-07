//
// Created by Katherine Rivera on 2025-03-05.
//

#include "Expenses.h"
/**
 *creates an expenses object
 * @param parent constructor for the expenses widget for budget page
 * @param vbox
 */
Expenses::Expenses(QWidget *parent, QVBoxLayout *vbox) {
    this->expenseName = new QString("");
    this->expenseDescription = new QString("");
    this->quantity = 1;
    this->price = 0;
    createExpenseUI(parent, vbox);
}

/**
 * creates an expense object with the given parameters
 * intended for offline mode
 * @param parent
 * @param name expense name
 * @param description expense description
 * @param price expense price
 * @param quantity expense quantity
 */
Expenses::Expenses(QWidget *parent, QVBoxLayout *vbox, QString name, QString description, double price, double quantity
) {
    expenseName = new QString(name);
    expenseDescription = new QString(description);
    this->price = price;
    this->quantity = quantity;
    createExpenseUI(parent, vbox);
}

/**
 * creates an Expense object from a json
 * @param parent
 * @param Expense
 *         needs to have "name", "description" price (double),quantity (double)
 * @param vbox 
 */
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
    expenseQuantitySpinBox->setRange(1, 999); //configures spinbox
    expensePriceSpinBox->setPrefix(tr("$\t"));
    expensePriceSpinBox->setMaximum(100000000);
    expensePriceSpinBox->setDecimals(2);
    expenseQuantitySpinBox->setDecimals(0);
    addExpenseForm->addRow(tr("Quantity:"), expenseQuantitySpinBox);
    addExpenseForm->addRow(tr("Name:"), expenseNameLineEdit); //adds the name,desc,price,qty to the form
    addExpenseForm->addRow(tr("Description:"), expenseDescriptionLineEdit);
    addExpenseForm->addRow(tr("Price:"), expensePriceSpinBox);
    expenseNameLineEdit->setText(*this->expenseName);
    expenseDescriptionLineEdit->setText(*this->expenseDescription);
    expenseQuantitySpinBox->setValue(this->quantity);
    expensePriceSpinBox->setValue(this->price);

    //connects spin boxes to the expenseChanged slot - with a char to denote if it's the quantity or price
    connect(expenseQuantitySpinBox, &QDoubleSpinBox::valueChanged, this,
            [this](double value) { expenseSBChangedSlot(value, 'Q'); });
    connect(expensePriceSpinBox, &QDoubleSpinBox::valueChanged, this,
            [this](double value) { expenseSBChangedSlot(value, 'P'); });

    //connects name and description to set expenseName and expenseDescription
    connect(expenseNameLineEdit, &QLineEdit::textChanged, this, &Expenses::onExpenseNameChangedSlot);
    connect(expenseDescriptionLineEdit, &QLineEdit::textChanged, this, &Expenses::onExpenseDescriptionChangedSlot);

    //connects the removeExpense button to deletelater, doesn't send a signal
    //TODO: fix so that it can both delete itself and send a signal expenseChanged to '0.0'
    removeExpenseHbox = new QHBoxLayout(removeExpenseButtonWidget);
    removeExpenseButton = new QPushButton(tr("Remove"));
    connect(removeExpenseButton, &QPushButton::clicked, newExpenseWidget, &QWidget::deleteLater);
    //  connect(removeExpenseButton, &QPushButton::clicked, [this]() {
    //     emit expensesChangedSignal(this->price * this->quantity, 0.0);\
    //     deleteLater();
    // });

    //adds the remove button to the widget
    removeExpenseHbox->addWidget(removeExpenseButton);
    addexpenseLayout->addLayout(addExpenseForm);
    addexpenseLayout->addWidget(removeExpenseButtonWidget);
    newExpenseWidget->setLayout(addexpenseLayout);

    vbox->addWidget(newExpenseWidget); //adds widget to the vbox
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

/**
 * slot detects if the expense name is changed, changes the expenseName to the new one
 * @param newName the new name
 */
void Expenses::onExpenseNameChangedSlot(const QString &newName) {
    expenseName->assign(newName);
    if (SHOW_DEBUG_LOGS) {
        qDebug() << "New Expense Name - " << *this->expenseName;
    }
}

/**
 * slow detectects if the description name is changed, changes the description to the new one
 * @param newDescription the new description
 */
void Expenses::onExpenseDescriptionChangedSlot(const QString &newDescription) {
    expenseDescription->assign(newDescription);
    if (SHOW_DEBUG_LOGS) {
        qDebug() << *this->expenseName << " - New Expense Description - " << *this->expenseDescription;
    }
}

/**
* sends signal with new and old expense
*/
void Expenses::expenseChangedSignal(double oldExpense, double newExpense) {
    if (SHOW_DEBUG_LOGS) {
        qDebug() << *this->expenseName << " - Old expense:" << oldExpense << " new:" << newExpense;
    }
}

/**
 * slot detects if the price or quantity for the expense has changed
 * @param change new value
 * @param changedType 'P' for price 'Q' for quantity
 */
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
