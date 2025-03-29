#include "budgetpageexpenses.h"

/**
    * @brief default constructor for expenses
    * quantity and price is set to 1, everything set to 0 or null string
    * @param parent parent QObject if needed
     * @author Katherine R
    */
BudgetPageExpenses::BudgetPageExpenses(QObject *parent)
    : QObject{parent} {
    this->expenseName = new QString("");
    this->expenseDescription = new QString("");
    this->quantity = 1;
    this->price = 1;
    this->categoryIndex = 0;
    createUIobjects();
}


/**
 * @brief parametrized constructor for expenses
 * @param parent parent qobject
 * @param name name of expense
 * @param description description of the expense item
 * @param price price of expense
 * @param quantity # of items
 * @param categoryIndex the index of category
  * @author Katherine R
 */
BudgetPageExpenses::BudgetPageExpenses(QObject *parent, const QString &name, const QString &description, double price,
                                       double quantity, int categoryIndex
): QObject{parent} {
    expenseName = new QString(name);
    expenseDescription = new QString(description);
    this->price = price;
    this->quantity = quantity;
    this->categoryIndex = categoryIndex;

    createUIobjects();
}

/**
     * @brief creates an Expense object from a json
     * @param parent
     * @param Expense
    *          \n "Name" - Name of Expense
     *         \n "Description" - description of Expense
     *         \n "Price" - price of Expense item
     *         \n "Quantity" - quantity of items
     *         \n "Category Index" - the index for categorization
     */
BudgetPageExpenses::BudgetPageExpenses(QObject *parent, const QJsonObject &Expense): QObject{parent} {
    if (!Expense.isEmpty()) {
        expenseName = new QString(Expense.value("name").toString());
        expenseDescription = new QString(Expense.value("description").toString());
        price = Expense.value("price").toDouble();
        quantity = Expense.value("quantity").toDouble();
        categoryIndex = Expense.value("Category Index").toInt();
        createUIobjects();
    } else {
        qDebug() << "empty JSON import-expense";
    }
}

/** @brief creates a json with data from expense item, used for offline mode/saving
     * @return A QJson object with the values for this expense item
     *         \n "Name" - Name of Expense
     *         \n "Description" - description of Expense
     *         \n "Price" - price of Expense item
     *         \n "Quantity" - quantity of items
     *         \n "Category Index" - the index for categorization
     */
QJsonObject BudgetPageExpenses::to_JSON() {
    QJsonObject expenseJson;
    //inserts expense item values to json
    if (!expenseName->isEmpty()) {
        expenseJson.insert("Name", *expenseName);
    } else {
        expenseJson.insert("Name", "\0");
    }
    if (!expenseDescription->isEmpty()) {
        expenseJson.insert("Description", *expenseDescription);
    } else {
        expenseJson.insert("Description", "\0");
    }
    expenseJson.insert("Price", QString::number(price));
    expenseJson.insert("Quantity", quantity);
    expenseJson.insert("Category Index", categoryIndex);
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
     * @brief getter for expense value
     * @return price * quantity of expense item
      * @author Katherine R
     */
double BudgetPageExpenses::getExpense() const {
    if (quantity <= 0 || price < 0) {
        return 0;
    }
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
   @author Katherine R
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
    //determines the type that was changed (price or quantity)
    switch (changedType) {
        case 'Q': //if quantity
            this->quantity = change;
            break;
        case 'P': //if price
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
     *
     *creates spinboxes for price and quantity
     *creates forms for name and description
     *connects them to expensesSBchangedslot, onExpenseDescriptionChangedSlot, onExpenseNameChangedSlot
     *
     * @param vbox VBox to add the UI to
      * @author Katherine R
     */
void BudgetPageExpenses::createExpenseUI(QVBoxLayout *vbox) {
    //creates a line edit for name and description
    expenseObj_NameLineEdit->setPlaceholderText(tr("Name"));
    expenseObj_DescriptionLineEdit->setPlaceholderText(tr("Description"));

    //creates a spinbox for price and quantity
    expenseObj_QuantitySpinBox->setRange(1, 999); //configures spinbox
    expenseObj_PriceSpinBox->setPrefix(tr("$\t"));
    expenseObj_PriceSpinBox->setMaximum(100000000);
    expenseObj_PriceSpinBox->setDecimals(2);
    expenseObj_QuantitySpinBox->setDecimals(0);

    //adds them to a form
    expenseObj_addExpenseForm->addRow(tr("Quantity:"), expenseObj_QuantitySpinBox);
    expenseObj_addExpenseForm->addRow(tr("Name:"), expenseObj_NameLineEdit); //adds the name,desc,price,qty to the form
    expenseObj_addExpenseForm->addRow(tr("Description:"), expenseObj_DescriptionLineEdit);
    expenseObj_addExpenseForm->addRow(tr("Price:"), expenseObj_PriceSpinBox);

    //sets text and value for name, desc, quantity, price to the values from the object
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

/**
     * sets the index for the category
     * used to know which category expense is a member of when export/import json
     * @param index category index
     */
void BudgetPageExpenses::setCategoryIndex(int index) {
    this->categoryIndex = index;
}

/**
 *@brief creates the UI objects for BudgetPageExpenses
 *used to eliminate repeated code on constructor
 *
 */
void BudgetPageExpenses::createUIobjects() {
    //creates UI objects for the expense item
    expenseObj_ExpenseWidget = new QWidget; //the widget for the expense
    expenseObj_addExpenseForm = new QFormLayout; //the form layout for the expense
    expenseObj_Layout = new QVBoxLayout;
    expenseObj_removeExpenseButtonWidget = new QWidget(); //the widget for the expense remove button
    expenseObj_NameLineEdit = new QLineEdit; //expense name
    expenseObj_DescriptionLineEdit = new QLineEdit;
    expenseObj_QuantitySpinBox = new QDoubleSpinBox;
    expenseObj_PriceSpinBox = new QDoubleSpinBox;
    //creates UI objects for removing expense buttons/hbox
    expenseObj_removeExpenseHbox = new QHBoxLayout(expenseObj_removeExpenseButtonWidget);
    //hbox for the expense remove button
    expenseObj_removeExpenseButton = new QPushButton(tr("Remove")); //the expense remove button
}