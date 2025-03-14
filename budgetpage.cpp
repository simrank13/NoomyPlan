#include "budgetpage.h"

BudgetPage::BudgetPage(QWidget *parent)
    : QMainWindow(parent) {
    //sets variables
    this->budget = 0;
    this->totalExpenses = 0;
    this->remainingBudget = 0;
    this->setWindowTitle("Budget plan");

    centralWidget = new QWidget(parent);
    budgetLayout = new QGridLayout(centralWidget);
    setCentralWidget(centralWidget);

    createBudgetPeriodSelector();
    createBudgetSelector();
    createExpensesSubPage();
}

/**
 * saves data to json for offline
 * @return JSON with the budget data
 */
QJsonObject BudgetPage::to_JSON() {
    QJsonObject budgetJson;
    budgetJson.insert("Time", QTime::currentTime().toString());
    budgetJson.insert("Budget Period", this->budgetLabel->text());
    budgetJson.insert("Total Expenses", this->totalExpenses);
    budgetJson.insert("Remaining Budget", this->remainingBudget);
    budgetJson.insert("Budget", this->budget);
    return budgetJson;
}

/**
 *
 * @param budget JSON with the budget data (Budget-double)(Total Expenses-double)(Remaining Budget-double)(Budget Period- String)
 */
void BudgetPage::getJSONBudget(QJsonObject budget) {
    this->budget = budget.value("Budget").toDouble();
    this->totalExpenses = budget.value("Total Expenses").toDouble();
    this->remainingBudget = budget.value("Remaining Budget").toDouble();
    this->budgetPeriodLabel->setText(budget.value("Budget Period").toString());
}


/**
 * QT Slot to detect changes in the budget SpinBox
 * changes the budget variable and calculates new remaining budget
 * @param budget new budget
 */
void BudgetPage::onBudgetChangeSlot(double budget) {
    this->budget = budget;
    if (SHOW_DEBUG_LOGS) {
        qDebug() << "Changed Budget - " << budget;
    }
    calculateRemainingBudget();
}

/**
 * QT Slot to detect changes in Expense object
 * @param oldExpense old expense
 * @param newExpense new expense
 */
void BudgetPage::onExpenseChangedSlot(double delta) {
    totalExpenses += delta;
    if (SHOW_DEBUG_LOGS) {
        qDebug() << "new delta in expense " << delta;
    }
    totalExpensesLabel->setText("Total Expenses: $0" + QString::number(this->totalExpenses));
    calculateRemainingBudget();
}

/*
 *Slot for when the user changes the budget period
 *changes the text for the budget spinbox
 */
void BudgetPage::onBudgetPeriodChangedSlot(int index) {
    QString labelText; //updates the budget text according to the set budget period
    switch (index) {
    case 0: // Quarterly
        labelText = tr("Quarterly Budget");
        if (SHOW_DEBUG_LOGS) {
            qDebug() << "Changed Budget Period - Quarterly";
        }
        break;
    case 1: // Monthly
        labelText = tr("Monthly Budget");
        if (SHOW_DEBUG_LOGS) {
            qDebug() << "Changed Budget Period - Monthly";
        }
        break;
    case 2: // Yearly
        labelText = tr("Yearly Budget");
        if (SHOW_DEBUG_LOGS) {
            qDebug() << "Changed Budget Period - Yearly";
        }
        break;
    default:
        if (SHOW_DEBUG_LOGS) {
            qDebug() << "Changed Budget Period - Unknown";
        }
        break;
    }
    budgetLabel->setText(labelText);
}

/**
 * calculates the remaining budget
 */
void BudgetPage::calculateRemainingBudget() {
    this->remainingBudget = this->budget - this->totalExpenses;
    remainingBudgetLabel->setText("Remaining Budget: $" + QString::number(this->remainingBudget));
    if (SHOW_DEBUG_LOGS) {
        qDebug() << "Remaining Budget - " << this->remainingBudget;
    }
}

/**
 * allows selection of budget period (Monthly, Yearly, Quarterly) for BudgetPage
 */
void BudgetPage::createBudgetPeriodSelector() {
    //adds the option to select the budget period
    budgetPeriodGroupBox = new QGroupBox(tr(""));
    budgetPeriodLabel = new QLabel("Select Budget Period:");
    budgetPeriodComboBox = new QComboBox();
    budgetPeriodVbox = new QVBoxLayout;
    budgetPeriodComboBox->addItem(tr("Quarterly"));
    budgetPeriodComboBox->addItem(tr("Monthly"));
    budgetPeriodComboBox->addItem(tr("Yearly"));

    //adds the budget period selector to layout
    budgetPeriodVbox->addWidget(budgetPeriodLabel);
    budgetPeriodVbox->addWidget(budgetPeriodComboBox);
    budgetPeriodGroupBox->setLayout(budgetPeriodVbox);
    budgetLayout->addWidget(budgetPeriodGroupBox);
    //connects the combobox to change the budget period
    connect(budgetPeriodComboBox, &QComboBox::currentIndexChanged, this, &BudgetPage::onBudgetPeriodChangedSlot);
}

/**
 * creates a budget selector for BudgetPage
 * uses a spinbox to set the budget
 */
void BudgetPage::createBudgetSelector() {
    //spinbox to set the budget
    budgetGroup = new QGroupBox(tr("Budget Plan"));
    budgetLabel = new QLabel("Quarterly Budget:");
    budgetSpinBox = new QDoubleSpinBox(budgetLabel);
    budgetVBox = new QVBoxLayout;

    //set budget spinbox config
    budgetSpinBox->setPrefix(tr("$\t"));
    budgetSpinBox->setMaximum(100000000);
    budgetSpinBox->setDecimals(2);
    budgetSpinBox->setAlignment(Qt::AlignRight);
    //adds to layout
    budgetVBox->addWidget(budgetLabel);
    budgetVBox->addWidget(budgetSpinBox);
    budgetGroup->setLayout(budgetVBox);
    budgetLayout->addWidget(budgetGroup);
    //connects the spinbox to set the budget variable
    connect(budgetSpinBox, &QDoubleSpinBox::valueChanged, this, &BudgetPage::onBudgetChangeSlot);
}

/**
 * creates a scrollable, dynamic list of expenses
 * can set the name, desc, price, and count
 * calculates total and remaining budget automatically
 */
void BudgetPage::createExpensesSubPage() {
    //expenses plan
    expensesGroup = new QGroupBox(tr("Expense Plan"));
    expensesVboxLayout = new QVBoxLayout;
    addExpenseButton = new QPushButton(tr("Add Expense"), this);
    remainingBudgetLabel = new QLabel(tr("Remaining Budget: $0.00"));
    totalExpensesLabel = new QLabel(tr("Total Expenses: $0.00"));
    expensesVboxLayout->addWidget(remainingBudgetLabel);
    expensesVboxLayout->addWidget(totalExpensesLabel);
    //creates a scroll area for the list of expenses
    expensesScrollWidget = new QWidget;
    expensesScrollListVbox = new QVBoxLayout;
    expenseScrollArea = new QScrollArea;
    expensesVboxLayout->addWidget(addExpenseButton);
    expensesVboxLayout->addWidget(expenseScrollArea);
    expensesScrollWidget->setLayout(expensesScrollListVbox);
    expenseScrollArea->setWidget(expensesScrollWidget);
    expenseScrollArea->setWidgetResizable(true);
    expensesGroup->setLayout(expensesVboxLayout);

    connect(addExpenseButton, &QPushButton::clicked, this,
               [expensesScrollListVbox=this->expensesScrollListVbox, this
       ]() {
                   newExpense(this,expensesScrollListVbox);
               });


    budgetLayout->addWidget(expensesGroup);
}
void BudgetPage::newExpense() {

}
void BudgetPage::newExpense(QWidget *widget, QVBoxLayout *layout) {
    expenses.append(new Expenses());
    Expenses *tempExpense=expenses.last();
    // //connects signal to know when expense is changed
    connect(tempExpense, &Expenses::expenseChangedSignal, this, &BudgetPage::onExpenseChangedSlot);
    tempExpense->createExpenseUI(widget, layout);
    //connects the expense button to delete
    connect(tempExpense->getRemoveButton(), &QPushButton::clicked, this,
        [index=this->expenses.count()-1, this, tempExpense]() {
        deleteExpense(tempExpense);
    });
}

void BudgetPage::deleteExpense(int index) {
    double tempExpenseTotal = expenses.at(index)->getExpense();
    expenses.at(index)->deleteLater();
    expenses.removeAt(index);
    onExpenseChangedSlot(-tempExpenseTotal);
}
void BudgetPage::deleteExpense(Expenses* toDelete) {
    long long index = expenses.indexOf(toDelete);
    double tempExpenseTotal = expenses.at(index)->getExpense();
    expenses.at(index)->deleteLater();
    expenses.removeAt(index);
    onExpenseChangedSlot(-tempExpenseTotal);
}
