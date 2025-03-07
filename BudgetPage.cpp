#include "BudgetPage.h"


/**
 * constructor for the budget page
 * @param parent
 */
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
void BudgetPage::onExpenseChangedSlot(double oldExpense, double newExpense) {
    totalExpenses -= oldExpense;
    totalExpenses += newExpense;
    if (SHOW_DEBUG_LOGS) {
        qDebug() << "Old expense:" << oldExpense << " new:" << newExpense << " new Total - " << totalExpenses;
    }
    totalExpensesLabel->setText("Total Expenses: $0" + QString::number(this->totalExpenses));
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


    // connect(addExpenseButton, &QPushButton::clicked, this, &BudgetPage::newExpense);

    connect(addExpenseButton, &QPushButton::clicked, this,
            [expensesScrollListVbox=this->expensesScrollListVbox, this,expensesScrollWidget=this->expensesScrollWidget
            ]() {
                new Expenses(this, expensesScrollListVbox);
            });

    budgetLayout->addWidget(expensesGroup);
}

void BudgetPage::newExpense() {
//     Expenses *expenses = new Expenses();
//         connect(expenses, &Expenses::expenseChangedSignal, this, &BudgetPage::onExpenseChangedSlot);
//
}
