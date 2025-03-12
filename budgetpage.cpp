#include "budgetpage.h"
#include "expenses.h"   
#include <QLabel>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QHeaderView>
#include <QJsonObject>
#include <QJsonDocument>
#define SHOW_DEBUG_LOGS true

BudgetPage::BudgetPage(QWidget *parent)
    : QMainWindow(parent) {
   
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

QJsonObject BudgetPage::to_JSON() {
    QJsonObject budgetJson;
    budgetJson.insert("Budget Period", this->budgetLabel->text());
    budgetJson.insert("Total Expenses", this->totalExpenses);
    budgetJson.insert("Remaining Budget", this->remainingBudget);
    budgetJson.insert("Budget", this->budget);
    return budgetJson;
}

void BudgetPage::getJSONBudget(QJsonObject budget) {
    this->budget = budget.value("Budget").toDouble();
    this->totalExpenses = budget.value("Total Expenses").toDouble();
    this->remainingBudget = budget.value("Remaining Budget").toDouble();
    this->budgetPeriodLabel->setText(budget.value("Budget Period").toString());
}


void BudgetPage::onBudgetChangeSlot(double budget) {
    this->budget = budget;
    if (SHOW_DEBUG_LOGS) {
        qDebug() << "Changed Budget - " << budget;
    }
    calculateRemainingBudget();
}

void BudgetPage::onExpenseChangedSlot(double oldExpense, double newExpense) {
    totalExpenses -= oldExpense;
    totalExpenses += newExpense;

    if (SHOW_DEBUG_LOGS) {
        qDebug() << "Old expense:" << oldExpense << " new:" << newExpense << " new Total - " << totalExpenses;
    }
    
    totalExpensesLabel->setText("Total Expenses: $" + QString::number(this->totalExpenses, 'f', 2));
    calculateRemainingBudget();
}

void BudgetPage::onBudgetPeriodChangedSlot(int index) {
    QString labelText; 
    switch (index) {
        case 0: 
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

void BudgetPage::calculateRemainingBudget() {
    this->remainingBudget = this->budget - this->totalExpenses;
    remainingBudgetLabel->setText("Remaining Budget: $" + QString::number(this->remainingBudget));
    if (SHOW_DEBUG_LOGS) {
        qDebug() << "Remaining Budget - " << this->remainingBudget;
    }
}

void BudgetPage::createBudgetPeriodSelector() {
    budgetPeriodGroupBox = new QGroupBox(tr(""));
    budgetPeriodLabel = new QLabel("Select Budget Period:");
    budgetPeriodComboBox = new QComboBox();
    budgetPeriodVbox = new QVBoxLayout;

    budgetPeriodComboBox->addItem(tr("Quarterly"));
    budgetPeriodComboBox->addItem(tr("Monthly"));
    budgetPeriodComboBox->addItem(tr("Yearly"));

    budgetPeriodVbox->addWidget(budgetPeriodLabel);
    budgetPeriodVbox->addWidget(budgetPeriodComboBox);
    budgetPeriodGroupBox->setLayout(budgetPeriodVbox);
    budgetLayout->addWidget(budgetPeriodGroupBox);

    connect(budgetPeriodComboBox, &QComboBox::currentIndexChanged, this, &BudgetPage::onBudgetPeriodChangedSlot);
}

void BudgetPage::createBudgetSelector() {
    budgetGroup = new QGroupBox(tr("Budget Plan"));
    budgetLabel = new QLabel("Quarterly Budget:");
    budgetSpinBox = new QDoubleSpinBox();
    budgetVBox = new QVBoxLayout;

    budgetSpinBox->setPrefix(tr("$ "));
    budgetSpinBox->setMaximum(100000000);
    budgetSpinBox->setDecimals(2);
    budgetSpinBox->setAlignment(Qt::AlignRight);

    budgetVBox->addWidget(budgetLabel);
    budgetVBox->addWidget(budgetSpinBox);
    budgetGroup->setLayout(budgetVBox);
    budgetLayout->addWidget(budgetGroup);

    connect(budgetSpinBox, &QDoubleSpinBox::valueChanged, this, &BudgetPage::onBudgetChangeSlot);
}

void BudgetPage::createExpensesSubPage() {

    expensesGroup = new QGroupBox(tr("Expense Plan"));
    expensesVboxLayout = new QVBoxLayout;
    addExpenseButton = new QPushButton(tr("Add Expense"), this);
    remainingBudgetLabel = new QLabel(tr("Remaining Budget: $0.00"));
    totalExpensesLabel = new QLabel(tr("Total Expenses: $0.00"));
    expensesVboxLayout->addWidget(remainingBudgetLabel);
    expensesVboxLayout->addWidget(totalExpensesLabel);

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

void BudgetPage::addExpense() {
    Expenses *newExpense = new Expenses(this, expensesScrollListVbox);
    connect(newExpense, &Expenses::expenseChangedSignal, this, &BudgetPage::onExpenseChangedSlot);

    updateTotalExpenses(0.0, 0.0);  
}


void BudgetPage::updateTotalExpenses(double oldExpense, double newExpense) {
    totalExpenses -= oldExpense;
    totalExpenses += newExpense;
    
    totalExpensesLabel->setText("Total Expenses: $" + QString::number(totalExpenses, 'f', 2));
    calculateRemainingBudget();  // Ensure the budget is recalculated

    qDebug() << "Updated Total Expenses: $" << totalExpenses;
}
