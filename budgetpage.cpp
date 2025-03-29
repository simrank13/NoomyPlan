#include "budgetpage.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

// Define static members declared in budgetpage.h
const QStringList BudgetPage::barChart_categories_Quarterly = { "Q1", "Q2", "Q3", "Q4" };
const QStringList BudgetPage::barChart_categories_Monthly = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

/**
 * @class BudgetPage
 * @brief A class that manages budget planning with different time periods (yearly, quarterly, monthly)
 *
 * BudgetPage provides an interface for users to set budgets, add expenses, and visualize
 * their budget data through various time periods. It supports yearly, quarterly, and monthly
 * budget management with dynamic expense tracking and remaining budget calculations.
 *
 * @author Katherine R
 */

 /**
  * @brief Default constructor for the budget page
  * @param parent Parent widget for this QMainWindow
  *
  * Initializes the budget page with default values, creates budget objects,
  * and sets up the UI elements including budget selectors, expense tracking,
  * and visualization components.
  */
BudgetPage::BudgetPage(QWidget* parent)
    : QMainWindow(parent) {
    //sets variables
    this->budgetPeriodIndex = 1;
    this->setWindowTitle("Budget plan");

    //creates budget objects for every possible budget period
    for (int i = 0; i <= 17; i++) {
        budgets.append(new BudgetPageBudget(this, i)); // creating 18 budget objects and adding them to the budgets list
    }

    centralWidget = new QWidget(parent); //creates the budget period selector UI
    budgetLayout = new QGridLayout(centralWidget); //creates a UI area to set budget
    setCentralWidget(centralWidget); //creates an UI area for expenses

    createBudgetPeriodSelector(); //creates the bar graph UI
    createBudgetSelector(); //creates a UI area to set budget
    createExpensesSubPage(); //creates an UI area for expenses
    updateBarGraph(); //creates the bar graph UI

    //adds bar graph update button
    barGraph_updateButton = new QPushButton("Update Graph");
    budgetLayout->addWidget(barGraph_updateButton, 1, 1);

    // connects the button's clicked signal to the updateBarGraph method of this BudgetPage class
    connect(barGraph_updateButton, &QPushButton::clicked, this, &BudgetPage::updateBarGraph);
}

/**
 * @brief Saves data of the budget page for offline mode
 * @return JSON object with the budget data
 *
 * Creates a JSON representation of all budget objects and their associated data
 * for persistent storage.
 * @see BudgetPageBudget::to_JSON()
 */
QJsonObject BudgetPage::to_JSON() {
    QJsonArray budgetJson; // creates an empty JSON array to store budget objects
    QJsonObject budgetJSonObj; // creates an empty JSON object that will be the container

    //saves all the budget objects using budgetpagebudget::to_JSON()
    for (int i = 0; i <= 17; i++) {
        budgetJson.append(budgets.at(i)->to_JSON()); // converts each budget object to JSON and adds to the array
    }

    budgetJSonObj.insert("Budgets", budgetJson); // inserts the JSON array into the main JSON object with key "Budgets"
    return budgetJSonObj; // returns the completed JSON object containing all budget data
}

/**
 * @brief Loads budget data from a JSON object
 * @param budget JSON object with the budget data
 *               (Budget-double)(Total Expenses-double)(Remaining Budget-double)(Budget Period- String)
 *
 * Populates the budget objects with data from the provided JSON object,
 * enabling restoration of saved budget information.
 */
void BudgetPage::getJSONBudget(const QJsonObject& budget) {
    //splits JSON into buget JSON array
    for (QJsonValueRef budgetObj : budget["Budgets"].toArray()) {
        // creates a new BudgetPageBudget from each JSON object and adds to the budgets list
        budgets.append(new BudgetPageBudget(this, budgetObj.toObject()));
    }

    // sets the text of the budgetPeriod_Label to the value associated with "Budget Period" key in the budget JSON object
    this->budgetPeriod_Label->setText(budget.value("Budget Period").toString());
}

/**
 * @brief QT Slot to detect changes in the budget SpinBox
 * @param budget New budget value
 *
 * Updates the budget variable for the current budget period and
 * recalculates the remaining budget based on the new value.
 */
void BudgetPage::onBudgetChangeSlot(double budget) {
    this->budgets[budgetPeriodIndex]->setBudget(budget); // sets the budget amount for the current period's budget object

    if (SHOW_DEBUG_LOGS) {
        qDebug() << "Changed Budget - " << budget; // outputs debug message with the budget value if debug logs are enabled
    }

    calculateRemainingBudget(); // recalculates the remaining budget after the change
}

/**
 * @brief QT Slot to detect changes in Expense object
 * @param delta Change in expense amount
 *
 * Updates the total expenses for the current budget period based on
 * the provided delta value and recalculates the remaining budget.
 */
void BudgetPage::onExpenseChangedSlot(double delta) {
    budgets[budgetPeriodIndex]->changeTotalExpenses(delta); // updates the total expenses of the current budget period by adding delta

    if (SHOW_DEBUG_LOGS) {
        qDebug() << "new delta in expense " << delta; // prints debug information about the expense change if debugging is enabled
    }

    expenses_totalExpensesLabel->setText(
        "Total Expenses: $0" + QString::number(budgets.at(budgetPeriodIndex)->getTotalExpenses())); // Updates the expense label with new total

    calculateRemainingBudget(); // recalculates remaining budget after expenses have been updated
}

/**
 * @brief Slot for when the user changes the budget period type
 * @param index Index of the selected budget period type (0=Quarterly, 1=Monthly, 2=Yearly)
 *
 * Handles switching between different budget period types (monthly, quarterly, yearly),
 * updates the UI to display the appropriate period selector, and loads the selected budget data.
 */
void BudgetPage::onBudgetPeriodTypeChangedSlot(int index) {
    barChart_xAxis->clear(); // clears the x-axis of the bar chart

    // hides and disables the expense scroll area for the current budget period
    budgets[budgetPeriodIndex]->getExpensescrollarea()->hide();
    budgets[budgetPeriodIndex]->getExpensescrollarea()->setDisabled(true);

    QString labelText; //updates the budget text according to the set budget period
    switch (index) {
    case 0: // Quarterly
        labelText = tr("Quarterly Budget - ") + budgetPeriod_QuarterlyComboBox->currentText(); // sets label text with quarter name
        budgetPeriodIndex = budgetPeriod_QuarterlyComboBox->currentIndex() + 1; // update budget period index (Q1=1, Q2=2, etc.)
        budgetPeriod_MonthlyComboBox->setHidden(true); // hide monthly selection
        budgetPeriod_QuarterlyComboBox->setHidden(false); // show quarterly selection
        if (SHOW_DEBUG_LOGS) {
            qDebug() << "Changed Budget Period - Quarterly" << budgetPeriodIndex; // debug log for quarterly selection
        }
        break;
    case 1: // Monthly
        budgetPeriodIndex = budgetPeriod_MonthlyComboBox->currentIndex() + 6; // update budget period index (offset by 6)
        budgetPeriod_MonthlyComboBox->setHidden(false); // show monthly selection
        budgetPeriod_QuarterlyComboBox->setHidden(true); // hide quarterly selection
        labelText = tr("Monthly Budget - ") + budgetPeriod_MonthlyComboBox->currentText(); // sets label with month name
        if (SHOW_DEBUG_LOGS) {
            qDebug() << "Changed Budget Period - Monthly" << budgetPeriodIndex; // debug log for monthly selection
        }
        break;
    case 2: // Yearly
        budgetPeriodIndex = 0; // set index to 0 for yearly budget
        budgetPeriod_MonthlyComboBox->setHidden(true); // hide monthly selection
        budgetPeriod_QuarterlyComboBox->setHidden(true); // hide quarterly selection
        labelText = tr("Yearly Budget"); // sets label text for yearly budget
        if (SHOW_DEBUG_LOGS) {
            qDebug() << "Changed Budget Period - Yearly"; // debug log for yearly selection
        }
        break;
    default:
        if (SHOW_DEBUG_LOGS) {
            qDebug() << "Changed Budget Period - Unknown"; // debug log for unknown selection
        }
        break;
    }
    budgetSelector_Label->setText(labelText); // updates the budget selector label with new text
    changeBudgetPage(); // updates the budget page based on the new selection
}

/**
 * @brief Detects when specific budget periods are changed
 * @param index Index of changed ComboBox item
 * @param period Period type identifier ('M' for monthly, 'Q' for quarterly)
 *
 * Handles selection of specific budget periods within a period type (e.g., Jan-Dec for monthly,
 * Q1-Q4 for quarterly) and updates the UI to display the corresponding budget data.
 */
void BudgetPage::onBudgetPeriodChangeSlot(int index, char period) {
    QString labelText; // declare a QString variable to store the label text

    // hide and disable the expenses scroll area for the current budget period
    budgets[budgetPeriodIndex]->getExpensescrollarea()->hide();
    budgets[budgetPeriodIndex]->getExpensescrollarea()->setDisabled(true);

    // switch statement to handle different budget periods
    switch (period) {
    case 'Q': // case for Quarterly budget period (Q1)
        if (SHOW_DEBUG_LOGS) { // check if debug logs are enabled
            qDebug() << "Changed Budget Period - Quarterly " << budgetPeriod_QuarterlyComboBox->currentText(); // log the quarterly period change
        }
        labelText = tr("Quarterly Budget - ") + budgetPeriod_QuarterlyComboBox->currentText(); // set label text for quarterly budget
        budgetPeriodIndex = budgetPeriod_QuarterlyComboBox->currentIndex() + 1; // update budget period index for quarterly
        break;

    case 'M': // case for Monthly budget period
        if (SHOW_DEBUG_LOGS) { // check if debug logs are enabled
            qDebug() << "Changed Budget Period - Monthly" << budgetPeriod_MonthlyComboBox->currentText(); // log the monthly period change
        }
        labelText = tr("Monthly Budget - ") + budgetPeriod_MonthlyComboBox->currentText(); // set label text for monthly budget
        budgetPeriodIndex = budgetPeriod_MonthlyComboBox->currentIndex() + 6; // update budget period index for monthly
        break;

    default: // default case for unknown budget periods
        if (SHOW_DEBUG_LOGS) { // check if debug logs are enabled
            qDebug() << "Unknown budget period changed"; // log an unknown period change
        }
        break;
    }

    // update the label text in the UI with the new budget period
    budgetSelector_Label->setText(labelText);

    // call a function to change the budget page based on the new period
    changeBudgetPage();
}

/**
 * @brief Calculates the remaining budget for the current budget period
 *
 * Computes the difference between the budget amount and total expenses,
 * updates the remaining budget value, and refreshes the UI display.
 */
void BudgetPage::calculateRemainingBudget() {
    // calculate and set the remaining budget for the current budget period
    budgets[budgetPeriodIndex]->setRemainingBudget(
        budgets.at(budgetPeriodIndex)->getBudget() - budgets.at(budgetPeriodIndex)->getTotalExpenses());

    // update the UI label to display the remaining budget
    expenses_remainingBudgetLabel->setText(
        "Remaining Budget: $" + QString::number(budgets.at(budgetPeriodIndex)->getRemainingBudget()));

    // log the remaining budget if debug logs are enabled
    if (SHOW_DEBUG_LOGS) {
        qDebug() << "Remaining Budget - " << budgets.at(budgetPeriodIndex)->getRemainingBudget();
    }
}

/**
 * @brief Creates the budget period selector widget
 *
 * Sets up the UI components for selecting budget periods, including
 * type selectors (Monthly, Yearly, Quarterly) and specific period
 * selectors (Q1-Q4, Jan-Dec).
 */
void BudgetPage::createBudgetPeriodSelector() {
    //adds the option to select the budget period
    budgetPeriod_GroupBox = new QGroupBox(tr(""));
    budgetPeriod_Label = new QLabel("Select Budget Period:");
    budgetPeriod_TypeComboBox = new QComboBox();
    budgetPeriod_Vbox = new QVBoxLayout;
    budgetPeriod_TypeComboBox->addItem(tr("Quarterly"));
    budgetPeriod_TypeComboBox->addItem(tr("Monthly"));
    budgetPeriod_TypeComboBox->addItem(tr("Yearly"));

    //adds the budget period selector to layout
    budgetPeriod_Vbox->addWidget(budgetPeriod_Label);
    budgetPeriod_Vbox->addWidget(budgetPeriod_TypeComboBox);
    budgetPeriod_GroupBox->setLayout(budgetPeriod_Vbox);
    budgetLayout->addWidget(budgetPeriod_GroupBox);

    //connects the combobox to change the budget period
    connect(budgetPeriod_TypeComboBox, &QComboBox::currentIndexChanged, this,
        &BudgetPage::onBudgetPeriodTypeChangedSlot);

    //creates comboboxes for the budget periods
    budgetPeriod_QuarterlyComboBox = new QComboBox();
    budgetPeriod_QuarterlyComboBox->addItem(tr("Q1"));
    budgetPeriod_QuarterlyComboBox->addItem(tr("Q2"));
    budgetPeriod_QuarterlyComboBox->addItem(tr("Q3"));
    budgetPeriod_QuarterlyComboBox->addItem(tr("Q4"));

    budgetPeriod_MonthlyComboBox = new QComboBox();
    budgetPeriod_MonthlyComboBox->addItem(tr("January"));
    budgetPeriod_MonthlyComboBox->addItem(tr("February"));
    budgetPeriod_MonthlyComboBox->addItem(tr("March"));
    budgetPeriod_MonthlyComboBox->addItem(tr("April"));
    budgetPeriod_MonthlyComboBox->addItem(tr("May"));
    budgetPeriod_MonthlyComboBox->addItem(tr("June"));
    budgetPeriod_MonthlyComboBox->addItem(tr("July"));
    budgetPeriod_MonthlyComboBox->addItem(tr("August"));
    budgetPeriod_MonthlyComboBox->addItem(tr("September"));
    budgetPeriod_MonthlyComboBox->addItem(tr("October"));
    budgetPeriod_MonthlyComboBox->addItem(tr("November"));
    budgetPeriod_MonthlyComboBox->addItem(tr("December"));

    budgetPeriod_Vbox->addWidget(budgetPeriod_QuarterlyComboBox); //adds them to the vbox
    budgetPeriod_Vbox->addWidget(budgetPeriod_MonthlyComboBox);
    budgetPeriod_MonthlyComboBox->setHidden(true); //hides monthly since default is quarterly

    //connects them to the slot
    connect(budgetPeriod_MonthlyComboBox, &QComboBox::currentIndexChanged, this, [this](int index) {
        onBudgetPeriodChangeSlot(index, 'M');
        });
    connect(budgetPeriod_QuarterlyComboBox, &QComboBox::currentIndexChanged, this, [this](int index) {
        onBudgetPeriodChangeSlot(index, 'Q');
        });
}

/**
 * @brief Creates a budget selector for BudgetPage
 *
 * Sets up the UI components for setting and displaying the budget amount,
 * including a SpinBox for numeric input with appropriate formatting.
 */
void BudgetPage::createBudgetSelector() {
    //spinbox to set the budget
    budgetSelector_group = new QGroupBox(tr("Budget Plan"));
    budgetSelector_Label = new QLabel("Quarterly Budget:");
    budgetSelector_SpinBox = new QDoubleSpinBox(budgetSelector_Label);
    budgetSelector_VBox = new QVBoxLayout;

    //set budget spinbox config
    budgetSelector_SpinBox->setPrefix(tr("$\t")); //adds a $ sign
    budgetSelector_SpinBox->setMaximum(100000000); //sets a max value
    budgetSelector_SpinBox->setDecimals(2);
    budgetSelector_SpinBox->setAlignment(Qt::AlignRight);

    //adds to layout
    budgetSelector_VBox->addWidget(budgetSelector_Label);
    budgetSelector_VBox->addWidget(budgetSelector_SpinBox);
    budgetSelector_group->setLayout(budgetSelector_VBox);
    budgetLayout->addWidget(budgetSelector_group);

    //connects the spinbox to set the budget variable
    connect(budgetSelector_SpinBox, &QDoubleSpinBox::valueChanged, this, &BudgetPage::onBudgetChangeSlot);
}

/**
 * @brief Creates a scrollable, dynamic list of expenses
 *
 * Sets up the UI components for tracking and displaying expenses,
 * including a scrollable list with add/remove functionality and
 * automatic calculation of total and remaining budget.
 */
void BudgetPage::createExpensesSubPage() {
    //creates a group and vbox for the expense plan area
    expenses_Group = new QGroupBox(tr("Expense Plan"));
    expenses_vbox = new QVBoxLayout;
    //creates buttons to add expense, and text w remaining budget and total expenses
    expenses_addExpenseButton = new QPushButton(tr("Add Expense"), this);//creates a button to add new expense
    expenses_remainingBudgetLabel = new QLabel(tr("Remaining Budget: $0.00"));//lists remainign budget
    expenses_totalExpensesLabel = new QLabel(tr("Total Expenses: $0.00"));//lists total expenses
    //adds them to vbox
    expenses_vbox->addWidget(expenses_remainingBudgetLabel);
    expenses_vbox->addWidget(expenses_totalExpensesLabel);
    expenses_vbox->addWidget(expenses_addExpenseButton);
    // creates a scroll area for the list of expenses for every budgetpage, then hides and disables them
    for (BudgetPageBudget* budget : budgets) {
        expenses_vbox->addWidget(budget->getExpensescrollarea());
        budget->getExpensescrollarea()->hide();
        budget->getExpensescrollarea()->setDisabled(true);
    }
    //shows/enables the current budget page's expenses scroll area
    budgets[budgetPeriodIndex]->getExpensescrollarea()->show();
    budgets[budgetPeriodIndex]->getExpensescrollarea()->setDisabled(false);

    expenses_Group->setLayout(expenses_vbox);
    //connects the add button to newExpense
    connect(expenses_addExpenseButton, &QPushButton::clicked, this, &BudgetPage::newExpense);
    budgetLayout->addWidget(expenses_Group);
}

/**
 * @brief Changes the budget page variables to the new budget period
 *
 * Updates UI elements to reflect the current budget period data, displays
 * the correct expense list, and recalculates budget values.
 */
void BudgetPage::changeBudgetPage() {
    if (this->budgetPeriodIndex >= 0 && this->budgetPeriodIndex <= 17) {
        qDebug() << "Changed budget page";
        //changes budget setter spinbox to the value of the changed page
        budgetSelector_SpinBox->setValue(budgets[budgetPeriodIndex]->getBudget());
        //changes the total expenses text to the value of the changed page
        expenses_totalExpensesLabel->setText(
            "Total Expenses: $0" + QString::number(budgets.at(budgetPeriodIndex)->getTotalExpenses()));
        //shows and enables new expense area
        budgets[budgetPeriodIndex]->getExpensescrollarea()->show();
        budgets[budgetPeriodIndex]->getExpensescrollarea()->setDisabled(false);
        calculateRemainingBudget();
    }
    else {
        if (SHOW_DEBUG_LOGS) {
            qDebug() << "Budget period index out of range";
        }
    }
}

/**
 * @brief Creates a new expense item
 *
 * Adds a new expense to the current budget period, creates its UI representation,
 * and connects signals for expense changes and deletion.
 */
void BudgetPage::newExpense() {
    //adds a new expense object to the current budget page
    budgets[budgetPeriodIndex]->getExpenses()->append(new BudgetPageExpenses());
    BudgetPageExpenses* tempExpense = budgets[budgetPeriodIndex]->getExpenses()->last();

    // connects signal to know when expense is changed
    connect(tempExpense, &BudgetPageExpenses::expenseChangedSignal, this, &BudgetPage::onExpenseChangedSlot);

    //creates the expense UI and adds it to the budget page's vbox
    tempExpense->createExpenseUI(this, budgets[budgetPeriodIndex]->getExpensesscrolllistvbox());

    //connects the remove expense button to deleteExpense,
    connect(tempExpense->getRemoveButton(), &QPushButton::clicked, this,
        [this, tempExpense]() {
            deleteExpense(tempExpense);
        });
}

/**
 * @brief Deletes an expense item
 * @param toDelete Pointer to the expense item to be deleted
 *
 * Removes the specified expense from the expenses vector, updates
 * the total expenses amount, and recalculates the remaining budget.
 */
void BudgetPage::deleteExpense(BudgetPageExpenses* toDelete) {
    //gets index of expense to delete
    long long index = budgets[budgetPeriodIndex]->getExpenses()->indexOf(toDelete);

    //gets the the expense value of the object to be deleted
    double tempExpenseTotal = budgets[budgetPeriodIndex]->getExpenses()->at(index)->getExpense();

    //deletes object, then removes it from the budget page's QVector
    budgets[budgetPeriodIndex]->getExpenses()->at(index)->deleteLater();
    budgets[budgetPeriodIndex]->getExpenses()->removeAt(index);

    onExpenseChangedSlot(-tempExpenseTotal);//removes from total expenses
}

/**
 * @brief Updates the bar graph visualization
 *
 * Creates a new chart/graph representation of budget data showing
 * remaining budgets across different periods. Positive values are shown
 * in black, negative values (over budget) in dark red.
 */
void BudgetPage::updateBarGraph() {
    //creates new qwidget, hbox
    barChart_Widget = new QWidget();
    barChart_GroupVbox = new QHBoxLayout();
    barChart_Widget->setLayout(barChart_GroupVbox);

    //creates new QBarset and QStringList for labeling X axis(necessary for updating graph)
    barChart_Value = new QBarSet("Remaining Budget");
    barChart_Neg = new QBarSet("Remaining Budget");
    barChart_xAxis = new QBarCategoryAxis;
    int rangelow; //lowest budgets index to graph budget from
    int rangehigh; //highset budgets index to graph budget from

    //determines if it should display yearly/quarterly/monthly from current index
    if (budgetPeriodIndex == 0) {
        rangelow = 0;
        rangehigh = 0;
    }
    else if (budgetPeriodIndex <= 5) {
        rangelow = 1;
        rangehigh = 5;
        barChart_xAxis->append(barChart_categories_Quarterly);
    }
    else {
        rangelow = 6;
        rangehigh = 17;
        barChart_xAxis->append(barChart_categories_Monthly);
    }

    //adds values from budgets to graph QBarset 
    for (int i = rangelow; i <= rangehigh; i++) {
        if (budgets.at(i)->getRemainingBudget() < 0) { // if the remaining budget is negativ
            barChart_Neg->append(budgets.at(i)->getRemainingBudget()); //adds value to negative cha
            barChart_Value->append(0);  //adds 0 to positive char
        }
        else {
            barChart_Value->append(budgets.at(i)->getRemainingBudget()); //adds value to positive char
            barChart_Neg->append(0); //adds value to negative cha
        }
    }
    //sets the colors for the bar chart ba
    barChart_Value->setColor(Qt::black);
    barChart_Neg->setColor(QColor(97, 24, 8)); //red color

    //adds values to the serie
    barChart_series = new QStackedBarSeries;
    barChart_series->append(barChart_Value);
    barChart_series->append(barChart_Neg);

    //creates the chart and adds the series(values)
    barChart_chart = new QChart();
    barChart_chart->addSeries(barChart_series);
    barChart_chart->setTitle("Budget Graph");;
    barChart_chart->addAxis(barChart_xAxis, Qt::AlignBottom);
    barChart_series->attachAxis(barChart_xAxis);
    barChart_yAxis = new QValueAxis;
    barChart_yAxis->setLabelFormat("%5d");
    barChart_chart->addAxis(barChart_yAxis, Qt::AlignLeft);
    barChart_series->attachAxis(barChart_yAxis);
    barChart_chart->legend()->setVisible(false);

    //creates chartview and adds to layout
    // barChart_chart->legend()->setAlignment(Qt::AlignBottom);
    barChart_chartView = new QChartView(barChart_chart);
    barChart_chartView->setLayout(barChart_GroupVbox);
    barChart_GroupVbox->setAlignment(Qt::AlignRight);
    budgetLayout->addWidget(barChart_chartView, 2, 1);
}

void BudgetPage::setCurrentUserId(const QString& id) {
    currentUserId = id;
    loadExpenses();  // Automatically load expenses for this user
}

void BudgetPage::saveExpenses() {
    if (currentUserId.isEmpty()) return;

    QJsonArray budgetsArray;
    for (BudgetPageBudget* budget : budgets) {
        budgetsArray.append(budget->to_JSON());  // assumes each BudgetPageBudget has this method
    }

    QJsonObject root;
    root["Budgets"] = budgetsArray;

    QFile file("data/" + currentUserId + "_expenses.json");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(root).toJson());
        file.close();
    }
}
void BudgetPage::loadExpenses() {
    QFile file("data/" + currentUserId + "_expenses.json");
    if (!file.open(QIODevice::ReadOnly)) return;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    QJsonObject obj = doc.object();
    getJSONBudget(obj);  // uses your already-written getJSONBudget
    changeBudgetPage();  // refresh UI
}



/**
 * @brief Destructor for the BudgetPage class
 *
 * Cleans up all allocated resources including UI elements and budget data
 */
BudgetPage::~BudgetPage() {
    // clear budgets list
    budgets.clear();

    for (auto budget : budgets) {
        delete budget;
    }
    budgets.clear();
}
