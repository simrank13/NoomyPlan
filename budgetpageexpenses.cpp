#include "budgetpageexpenses.h"

/**
 * @brief default constructor for expenses
 * @param parent parent QObject if needed
  * @author Katherine R
 */
BudgetPageExpenses::BudgetPageExpenses(QObject *parent)
    : QObject{parent} {
    this->expenseName = new QString("");
    this->expenseDescription = new QString("");
    this->quantity = 1;
    this->price = 0;

    expenseObj_ExpenseWidget = new QWidget;
    expenseObj_addExpenseForm = new QFormLayout;
    expenseObj_Layout = new QVBoxLayout;
    expenseObj_removeExpenseButtonWidget = new QWidget();
    expenseObj_NameLineEdit = new QLineEdit;
    expenseObj_DescriptionLineEdit = new QLineEdit;
    expenseObj_QuantitySpinBox = new QDoubleSpinBox;
    expenseObj_PriceSpinBox = new QDoubleSpinBox;
    expenseObj_removeExpenseHbox = new QHBoxLayout(expenseObj_removeExpenseButtonWidget);
    expenseObj_removeExpenseButton = new QPushButton(tr("Remove"));
}

/**
 * @brief parametrized constructor for expenses
 * @param parent parent qobject
 * @param name name of expense
 * @param description description of the expense item
 * @param price price of expense
 * @param quantity # of items
  * @author Katherine R
 */
BudgetPageExpenses::BudgetPageExpenses(QObject *parent, const QString &name, const QString &description, double price,
                                       double quantity
) {
    expenseName = new QString(name);
    expenseDescription = new QString(description);
    this->price = price;
    this->quantity = quantity;

    expenseObj_ExpenseWidget = new QWidget;
    expenseObj_addExpenseForm = new QFormLayout;
    expenseObj_Layout = new QVBoxLayout;
    expenseObj_removeExpenseButtonWidget = new QWidget();
    expenseObj_NameLineEdit = new QLineEdit;
    expenseObj_DescriptionLineEdit = new QLineEdit;
    expenseObj_QuantitySpinBox = new QDoubleSpinBox;
    expenseObj_PriceSpinBox = new QDoubleSpinBox;
    expenseObj_removeExpenseHbox = new QHBoxLayout(expenseObj_removeExpenseButtonWidget);
    expenseObj_removeExpenseButton = new QPushButton(tr("Remove"));
}

/**
 * @brief creates an Expense object from a json
 * @param parent
 * @param Expense
*          \n "Name" - Name of Expense
 *         \n "Description" - description of Expense
 *         \n "Price" - price of Expense item
 *         \n "Quantity" - quantity of items
 */
BudgetPageExpenses::BudgetPageExpenses(QObject *parent, const QJsonObject &Expense) {
    expenseName = new QString(Expense.value("name").toString());
    expenseDescription = new QString(Expense.value("description").toString());
    price = Expense.value("price").toDouble();
    quantity = Expense.value("quantity").toDouble();

    expenseObj_ExpenseWidget = new QWidget;
    expenseObj_addExpenseForm = new QFormLayout;
    expenseObj_Layout = new QVBoxLayout;
    expenseObj_removeExpenseButtonWidget = new QWidget();
    expenseObj_NameLineEdit = new QLineEdit;
    expenseObj_DescriptionLineEdit = new QLineEdit;
    expenseObj_QuantitySpinBox = new QDoubleSpinBox;
    expenseObj_PriceSpinBox = new QDoubleSpinBox;
    expenseObj_removeExpenseHbox = new QHBoxLayout(expenseObj_removeExpenseButtonWidget);
    expenseObj_removeExpenseButton = new QPushButton(tr("Remove"));
}

/**
 * @brief creates a json with data from expense item, used for offline mode/saving
 * @return A QJson object with the values for this expense item
 *         \n "Name" - Name of Expense
 *         \n "Description" - description of Expense
 *         \n "Price" - price of Expense item
 *         \n "Quantity" - quantity of items
 */
QJsonObject BudgetPageExpenses::to_JSON() const {
    QJsonObject expenseJson;
    expenseJson.insert("Name", *expenseName);
    expenseJson.insert("Description", *expenseDescription);
    expenseJson.insert("Price", QString::number(price));
    expenseJson.insert("Quantity", quantity);
    return expenseJson;
}

/**
 * 
 * @return the remove button QPushButton
  * @author Katherine R
 */
QPushButton *BudgetPageExpenses::getRemoveButton() {
    return expenseObj_removeExpenseButton;
}

/**
 * 
 * @return price * quantity of expense item
  * @author Katherine R
 */
double BudgetPageExpenses::getExpense() const {
    return this->price * this->quantity;
}

/**
 * @brief Slot detects if the expense name is changed, changes the expenseName to the new one
 * @param newName the new name for the expense
  * @author Katherine R
 */
void BudgetPageExpenses::onExpenseNameChangedSlot(const QString &newName) const {
    *expenseName = newName;
    if (SHOW_DEBUG_LOGS) {
        qDebug() << "New Expense Name - " << *this->expenseName;
    }
}

/**
 * @brief slot detects if the description name is changed, changes the description to the new one
 * @param newDescription the new description
 */
void BudgetPageExpenses::onExpenseDescriptionChangedSlot(const QString &newDescription) const {
    *expenseDescription = newDescription;
    if (SHOW_DEBUG_LOGS) {
        qDebug() << *this->expenseName << " - New Expense Description - " << *this->expenseDescription;
    }
}


/**
 * @brief slot detects if the price or quantity for the expense has changed
 * @param change new value
 * @param changedType 'P' for price 'Q' for quantity
  * @author Katherine R
 */
void BudgetPageExpenses::expenseSBChangedSlot(double change, char changedType) {
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
    if (SHOW_DEBUG_LOGS) {
        qDebug() << "Expense " << *this->expenseName << " changed by - " << (this->quantity * this->price) -
                oldExpenses;
    }

    emit expenseChangedSignal((this->quantity * this->price) - oldExpenses);
}

/**
 * @brief Creates the UI for the expense item
 * @param parent QWidget to add the UI to
 * @param vbox VBox to add the UI to
  * @author Katherine R
 */
void BudgetPageExpenses::createExpenseUI(QWidget *parent, QVBoxLayout *vbox) {
    expenseObj_NameLineEdit->setPlaceholderText(tr("Name"));
    expenseObj_DescriptionLineEdit->setPlaceholderText(tr("Description"));
    expenseObj_QuantitySpinBox->setRange(1, 999); //configures spinbox
    expenseObj_PriceSpinBox->setPrefix(tr("$\t"));
    expenseObj_PriceSpinBox->setMaximum(100000000);
    expenseObj_PriceSpinBox->setDecimals(2);
    expenseObj_QuantitySpinBox->setDecimals(0);
    expenseObj_addExpenseForm->addRow(tr("Quantity:"), expenseObj_QuantitySpinBox);
    expenseObj_addExpenseForm->addRow(tr("Name:"), expenseObj_NameLineEdit); //adds the name,desc,price,qty to the form
    expenseObj_addExpenseForm->addRow(tr("Description:"), expenseObj_DescriptionLineEdit);
    expenseObj_addExpenseForm->addRow(tr("Price:"), expenseObj_PriceSpinBox);
    expenseObj_NameLineEdit->setText(*this->expenseName);
    expenseObj_DescriptionLineEdit->setText(*this->expenseDescription);
    expenseObj_QuantitySpinBox->setValue(this->quantity);
    expenseObj_PriceSpinBox->setValue(this->price);

    //connects spin boxes to the expenseChanged slot - with a char to denote if it's the quantity or price
    connect(expenseObj_QuantitySpinBox, &QDoubleSpinBox::valueChanged, this,
            [this](double value) { expenseSBChangedSlot(value, 'Q'); });
    connect(expenseObj_PriceSpinBox, &QDoubleSpinBox::valueChanged, this,
            [this](double value) { expenseSBChangedSlot(value, 'P'); });

    //connects name and description to set expenseName and expenseDescription
    connect(expenseObj_NameLineEdit, &QLineEdit::textChanged, this, &BudgetPageExpenses::onExpenseNameChangedSlot);
    connect(expenseObj_DescriptionLineEdit, &QLineEdit::textChanged, this,
            &BudgetPageExpenses::onExpenseDescriptionChangedSlot);

    //adds the remove button to the widget
    expenseObj_removeExpenseHbox->addWidget(expenseObj_removeExpenseButton);
    expenseObj_Layout->addLayout(expenseObj_addExpenseForm);
    expenseObj_Layout->addWidget(expenseObj_removeExpenseButtonWidget);
    expenseObj_ExpenseWidget->setLayout(expenseObj_Layout);

    vbox->addWidget(expenseObj_ExpenseWidget); //adds widget to the vbox
}

/**
 * @brief deletes expenses object
 */
BudgetPageExpenses::~BudgetPageExpenses() {
    delete expenseObj_NameLineEdit;
    delete expenseObj_DescriptionLineEdit;
    delete expenseObj_QuantitySpinBox;
    delete expenseObj_PriceSpinBox;
    delete expenseName;
    delete expenseDescription;
    delete expenseObj_removeExpenseButton;
    delete expenseObj_removeExpenseHbox;
    delete expenseObj_Layout;
    delete expenseObj_removeExpenseButtonWidget;
    delete expenseObj_ExpenseWidget;
}
