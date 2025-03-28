#include "budgetpage.h"


/**
*  @brief default constructor for the budget page
\n sets budget index to Quarterly and Q1, creates budget objects, then displays UI
 * @param parent QWidget
 * @author - Katherine R
 */
BudgetPage::BudgetPage(QWidget *parent)
    : QMainWindow(parent) {
    //sets variables
    this->budgetPeriodIndex = 1;
    this->setWindowTitle("Budget plan");

    //creates budget objects for every possible budget period
    budgetsObject = new QObject();
    for (int i = 0; i <= 17; i++) {
        budgets.append(new BudgetPageBudget(budgetsObject, i)); //creates budget object
    }
    centralWidget = new QWidget(parent);
    budgetLayout = new QGridLayout(centralWidget);
    setCentralWidget(centralWidget);

    showExpenseExceedPopup = true;

    createBudgetPeriodSelector(); //creates the budget period selector UI
    createBudgetSelector(); //creates a UI area to set budget
    createExpensesSubPage(); //creates a UI area for expenses
    updateBarGraph(); //creates the bar graph UI
    //adds bar graph update button
    barGraph_updateButton = new QPushButton("Update Graph");
    budgetLayout->addWidget(barGraph_updateButton, 1, 1);
    connect(barGraph_updateButton, &QPushButton::clicked, this, &BudgetPage::updateBarGraph); //connects it to update
}

/**
 * @brief saves data of the budget page for offline mode
 * saves a JSON with any relevant data
 *
 * @return JSON with the budget data
 * \n "Budgets" JsonArray contaiting JSONs of BudgetPageBudget
 * \n "Categories" A QJSonArray containing strings of the categories
 * \n specified in - @copydoc BudgetPageBudget::to_JSON()
  * @author - Katherine R
 */
QJsonObject BudgetPage::to_JSON() {
    QJsonArray budgetJson;
    QJsonObject budgetJSonObj;
    QJsonArray categoriesJson;
    //saves all the budget objects using BudgetPageBudget::to_JSON()
    for (int i = 0; i <= 17; i++) {
        budgetJson.append(budgets.at(i)->to_JSON()); //adds the budget @index to json
    }
    for (int i = 0; i < expenses_categoriesComboBox->count(); i++) {
        categoriesJson.append(expenses_categoriesComboBox->itemText(i));
    }
    budgetJSonObj.insert("Budgets", budgetJson);
    budgetJSonObj.insert("Categories", categoriesJson);
    return budgetJSonObj;
}

/**
 * @brief adds json budget for offline/saves
 * imports JSON budget according to to_JSON() and adds them to the page
 * @param budget JSON Object with budget data - needs to be the same as to_JSON()
 * "Budgets" Array with BudgetPageBudget JSONS
 * \n defined in @copydoc BudgetPageBudget::to_JSON()
* @author - Katherine R
 */
void BudgetPage::getJSONBudget(const QJsonObject &budget) {
    //splits JSON into budget JSON array
    budgets.clear();
    if (!budget.isEmpty() && budget.contains("Budgets")) {
        if (budget.value("Budgets").toArray().count() == 17) {
            //if there is enough
            for (QJsonValueRef budgetObj: budget["Budgets"].toArray()) {
                budgets.append(new BudgetPageBudget(this, budgetObj.toObject())); //adds them to the page
            }
        }
    }

    this->budgetPeriod_Label->setText(budget.value("Budget Period").toString());
}

/**
 * @brief QT Slot to detect changes in the budget SpinBox
 * \n changes the budget variable and calculates new remaining budget
 * @param budget new budget
 * @author - Katherine R
 */
void BudgetPage::onBudgetChangeSlot(double budget) {
    this->budgets[budgetPeriodIndex]->setBudget(budget);
    if (SHOW_DEBUG_LOGS) {
        qDebug() << "Changed Budget - " << budget;
    }
    calculateRemainingBudget();
}

/**
 * @brief QT Slot to detect changes in the budget goal SpinBox
 * \n changes the budget goal variable and calculates new remaining budget
 * @param goal new budget goal
 * @author - Katherine R
 */
void BudgetPage::onBudgetGoalChangedSlot(double goal) {
    this->budgets[budgetPeriodIndex]->setBudgetGoal(goal);
    if (SHOW_DEBUG_LOGS) {
        qDebug() << "Changed Budget goal - " << goal;
    }

    calculateRemainingBudget();
}


/**
 * @brief QT Slot to detect changes in Expense object
 * \n then calculates the new total by adding the change
 * \n after that, calculates the remaining budget using calculateRemainingBudget()
 * @param delta - change in expense
 * @author - Katherine R
 */
void BudgetPage::onExpenseChangedSlot(double delta) {
    budgets[budgetPeriodIndex]->changeTotalExpenses(delta);
    //changes the expense for the budget object by adding the change
    if (SHOW_DEBUG_LOGS) {
        qDebug() << "new delta in expense " << delta;
    }
    expenses_totalExpensesLabel->setText(
        "Total Expenses: $0" + QString::number(budgets.at(budgetPeriodIndex)->getTotalExpenses()));
    //updates the total expense text
    calculateRemainingBudget();
}

/**
 * @brief Slot for when the user changes the budget period type (monthly , quarterly, yearly)
 *\n then changes the text for the budget spinbox
 *\n then swaps the period selector combo box to the selected one (so for the Q1-Q4 spinbox if quarterly is selected)
 *\n and swaps the budgets to the selected one
 * @author - Katherine R
 */
void BudgetPage::onBudgetPeriodTypeChangedSlot(int index) {
    barChart_xAxis->clear(); //removes x-axis values from graph
    //hides and disables the prev budget shown
    budgets[budgetPeriodIndex]->getExpensescrollarea(expenses_categoriesComboBox_index)->hide();
    //hides old expense area
    budgets[budgetPeriodIndex]->getExpensescrollarea(expenses_categoriesComboBox_index)->setDisabled(true);
    //disables old expense area
    QString labelText; //updates the budget text according to the set budget period
    switch (index) {
        case 0: // Quarterly
            labelText = tr("Quarterly Budget - ") + budgetPeriod_QuarterlyComboBox->currentText();
        //sets text to Quarterly Budget - QX
            budgetPeriodIndex = budgetPeriod_QuarterlyComboBox->currentIndex() + 1;
        //changes the budgets index to the last one saved for quarter
            budgetPeriod_MonthlyComboBox->setHidden(true); //hides monthly selector combobox
            budgetPeriod_QuarterlyComboBox->setHidden(false); //shows quarter selector
            if (SHOW_DEBUG_LOGS) {
                qDebug() << "Changed Budget Period - Quarterly" << budgetPeriodIndex;
            }
            break;
        case 1: // Monthly
            budgetPeriodIndex = budgetPeriod_MonthlyComboBox->currentIndex() + 6;
        //changes the budgets index to the last one saved for monthly
            budgetPeriod_MonthlyComboBox->setHidden(false); //shows month selector combobox
            budgetPeriod_QuarterlyComboBox->setHidden(true); //hides quarter selector combobox
            labelText = tr("Monthly Budget - ") + budgetPeriod_MonthlyComboBox->currentText();
        //sets text to monthly budget - (month)
            if (SHOW_DEBUG_LOGS) {
                qDebug() << "Changed Budget Period - Monthly" << budgetPeriodIndex;
            }
            break;
        case 2: // Yearly
            budgetPeriodIndex = 0;
        //hides both month and quarter selector combobox
            budgetPeriod_MonthlyComboBox->setHidden(true);
            budgetPeriod_QuarterlyComboBox->setHidden(true);
            labelText = tr("Yearly Budget"); //sets text to yearly budget
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
    budgetSelector_Label->setText(labelText);
    changeBudgetPage();
}

/**
 * @brief slot detects when budget periods are changed
 * changes the budget object when another budget period is selected I.E (jan, feb, Q1..)
 * changes the budget label to X Budget - Y, X being quarterly/monthly/yrly, Y being Q1-Q5 jan-dec
 * @param index index of changed Combobox
 * @param period M for monthly Q for quarterly
 * @author - Katherine R
 */
void BudgetPage::onBudgetPeriodChangeSlot(int index, char period) {
    QString labelText;
    budgets[budgetPeriodIndex]->getExpensescrollarea(expenses_categoriesComboBox_index)->hide();
    //hides old expense area
    budgets[budgetPeriodIndex]->getExpensescrollarea(expenses_categoriesComboBox_index)->setDisabled(true);
    //disables old expense area
    switch (period) {
        case 'Q': //Q1
            if (SHOW_DEBUG_LOGS) {
                qDebug() << "Changed Budget Period - Quarterly " << budgetPeriod_QuarterlyComboBox->currentText();
            }
            labelText = tr("Quarterly Budget - ") + budgetPeriod_QuarterlyComboBox->currentText();
        //sets new text as Quarterly Budget- QX
            budgetPeriodIndex = budgetPeriod_QuarterlyComboBox->currentIndex() + 1;
        //changes budgets index to the selected one

            break;
        case 'M':
            if (SHOW_DEBUG_LOGS) {
                qDebug() << "Changed Budget Period - Monthly" << budgetPeriod_MonthlyComboBox->currentText();
            }

            labelText = tr("Monthly Budget - ") + budgetPeriod_MonthlyComboBox->currentText();
        //sets text as monthly budget - month
            budgetPeriodIndex = budgetPeriod_MonthlyComboBox->currentIndex() + 6;
        //changes budgets index to the selected one
            break;
        default:
            if (SHOW_DEBUG_LOGS) {
                qDebug() << "Unknown budget period changed";
            }
            break;
    }
    budgetSelector_Label->setText(labelText);


    changeBudgetPage();
}


/**
 * @brief calculates the remaining budget for budgets[budgetPeriodIndex]
 * for budgets[budgetPeriodIndex] :budget - total expenses
 * \n if the remaining budget is below the financial surplus goal, the remaining budget text changes to red to notify user
 * \n and gives popup
 * @author - Katherine R
 */
void BudgetPage::calculateRemainingBudget() {
    //sets the remaining budget at budgets at index to budget - totalexpenses
    budgets[budgetPeriodIndex]->setRemainingBudget(
        budgets.at(budgetPeriodIndex)->getBudget() - budgets.at(budgetPeriodIndex)->getTotalExpenses());
    //updates remaining budget text
    expenses_remainingBudgetLabel->setText(
        "Remaining Budget: $" + QString::number(budgets.at(budgetPeriodIndex)->getRemainingBudget()));
    //changes text color to red to notify user of negative budget
    if (budgets.at(budgetPeriodIndex)->getRemainingBudget() < budgets.at(budgetPeriodIndex)->getBudgetGoal()) {
        expenses_remainingBudgetLabel->setStyleSheet("Color: #611808"); //red color to notify user of negative budget
        //gives popup warning w option to ignore
        if (showExpenseExceedPopup) {
            QMessageBox budgetExceedpopup;
            budgetExceedpopup.setText("Warning! \n Expenses Exceed financial surplus goal!");
            budgetExceedpopup.setIcon(QMessageBox::NoIcon);
            budgetExceedpopup.setStandardButtons(QMessageBox::Ok | QMessageBox::Ignore);
            int x = budgetExceedpopup.exec();
            if (x == QMessageBox::Ignore) {
                showExpenseExceedPopup = false;
            }
        }
    } else {
        expenses_remainingBudgetLabel->setStyleSheet("Color: auto"); //default color if it's not negative
    }
    if (SHOW_DEBUG_LOGS) {
        qDebug() << "Remaining Budget - " << budgets.at(budgetPeriodIndex)->getRemainingBudget();
    }
}

/**
 * @brief creates the budget period selector widget
 * \n allows selection of budget period (Monthly, Yearly, Quarterly) for BudgetPage using a combobox
 * \n and (Q1-Q4) (jan-dec)
 * @author - Katherine R
 */
void BudgetPage::createBudgetPeriodSelector() {
    //adds the option to select the budget period
    budgetPeriod_GroupBox = new QGroupBox(tr(""));
    budgetPeriod_Label = new QLabel("Select Budget Period:");
    budgetPeriod_TypeComboBox = new QComboBox();
    budgetPeriod_TypeComboBox->addItem(tr("Quarterly")); //adds the available budget period types to the combobox
    budgetPeriod_TypeComboBox->addItem(tr("Monthly"));
    budgetPeriod_TypeComboBox->addItem(tr("Yearly"));

    //adds the budget period type selector to layout
    budgetPeriod_Vbox = new QVBoxLayout;
    budgetPeriod_Vbox->addWidget(budgetPeriod_Label);
    budgetPeriod_Vbox->addWidget(budgetPeriod_TypeComboBox);
    budgetPeriod_GroupBox->setLayout(budgetPeriod_Vbox);
    budgetLayout->addWidget(budgetPeriod_GroupBox);
    //connects the combobox to change the budget period
    connect(budgetPeriod_TypeComboBox, &QComboBox::currentIndexChanged, this,
            &BudgetPage::onBudgetPeriodTypeChangedSlot);

    //creates comboboxes for the quarters
    budgetPeriod_QuarterlyComboBox = new QComboBox();
    budgetPeriod_QuarterlyComboBox->addItem(tr("Q1")); //adds q1-q4 to combobox
    budgetPeriod_QuarterlyComboBox->addItem(tr("Q2"));
    budgetPeriod_QuarterlyComboBox->addItem(tr("Q3"));
    budgetPeriod_QuarterlyComboBox->addItem(tr("Q4"));
    //creates a monthly budget period selector
    budgetPeriod_MonthlyComboBox = new QComboBox();
    budgetPeriod_MonthlyComboBox->addItem(tr("January")); //adds jan-dec to combobox
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
    connect(budgetPeriod_MonthlyComboBox, &QComboBox::currentIndexChanged, this, [=,this](int index) {
        onBudgetPeriodChangeSlot(index, 'M');
    });
    connect(budgetPeriod_QuarterlyComboBox, &QComboBox::currentIndexChanged, this, [=,this](int index) {
        onBudgetPeriodChangeSlot(index, 'Q');
    });
}

/**
 * @brief creates a budget selector for BudgetPage
 * \n uses a spinbox to set the budget, connects to onBudgetChangeSlot
 * \n also uses a spinbox to set the budget goal, connects to onBudgeGoalChangedSlot
  * @author - Katherine
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

    //creates a financial surplus goal selector - when remaining budget is under the surplus, it notifies user
    budgetSelector_GoalLabel = new QLabel(tr("Financial Surplus Goal:"));
    budgetSelector_goalSpinBox = new QDoubleSpinBox(budgetSelector_Label);
    //sets the spinbox config
    budgetSelector_goalSpinBox->setPrefix(tr("$\t"));
    budgetSelector_goalSpinBox->setMaximum(100000000);
    budgetSelector_goalSpinBox->setDecimals(2);
    budgetSelector_goalSpinBox->setAlignment(Qt::AlignRight);
    //adds them to the layout
    budgetSelector_VBox->addWidget(budgetSelector_GoalLabel);
    budgetSelector_VBox->addWidget(budgetSelector_goalSpinBox);
    //connects it to onBudgetGoalChangedSlot
    connect(budgetSelector_goalSpinBox, &QDoubleSpinBox::valueChanged, this, &BudgetPage::onBudgetGoalChangedSlot);
}


/**
 * @brief creates a scrollable, dynamic list of expenses
 * \n creates a separate expense area for every BudgetPageBudget, (so that each budget period can have its own list of expenses)
 * \n can set the name, desc, price, and count
 * \n calculates total and remaining budget automatically using calculateRemainingBudget
 * @author - Katherine R
 */
void BudgetPage::createExpensesSubPage() {
    //creates a group and vbox for the expense plan area
    expenses_Group = new QGroupBox(tr("Expense Plan"));
    expenses_vbox = new QVBoxLayout;
    //creates buttons to add expense, and text w remaining budget and total expenses
    expenses_addExpenseButton = new QPushButton(tr("Add Expense"), this); //creates a button to add new expense
    expenses_remainingBudgetLabel = new QLabel(tr("Remaining Budget: $0.00")); //lists remaining budget
    expenses_totalExpensesLabel = new QLabel(tr("Total Expenses: $0.00")); //lists total expenses
    //adds them to vbox
    expenses_vbox->addWidget(expenses_remainingBudgetLabel);
    expenses_vbox->addWidget(expenses_totalExpensesLabel);

    //category part
    expenses_categoriesComboBox = new QComboBox();
    expenses_categoryLineEdit = new QLineEdit();
    //creates a QLineEdit to add categories, add with enter
    //the categories are selectable w the combobox
    expenses_categoriesComboBox->addItem(tr("Default Category"));
    expenses_categoryLineEdit->setPlaceholderText("Add Expense Category");
    //adds category when enter is pressed
    connect(expenses_categoryLineEdit, &QLineEdit::returnPressed, this, &BudgetPage::addExpenseCategory);
    //combobox swaps expenses with category
    connect(expenses_categoriesComboBox, &QComboBox::currentIndexChanged, this, &BudgetPage::changeExpenseCategory);
    //adds to widget
    expenses_vbox->addWidget(expenses_categoriesComboBox);
    expenses_vbox->addWidget(expenses_categoryLineEdit);
    expenses_categoriesComboBox_index = 0;

    expenses_vbox->addWidget(expenses_addExpenseButton);

    // creates a scroll area for the list of expenses for every budgetpage, then hides and disables them
    for (BudgetPageBudget *budget: budgets) {
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
 * @brief getter for the set budget at the current period
 * @return the budgeted amount, double
 */
double BudgetPage::getBudget() {
    return this->budgetSelector_SpinBox->value();
}

/**
 * @brief getter for the total expense for the budget period
 * @return the total expense, double
 */
double BudgetPage::getTotalExpenses() {
    return this->budgets[budgetPeriodIndex]->getTotalExpenses();
}

/**
 * @brief getter for the budget surplus goal at the current period
 * @return the surplus goal, double
 */
double BudgetPage::getSurplusGoal() {
    return this->budgetSelector_goalSpinBox->value();
}

/**
* @brief Slot called when a new expense category is detected and creates it
* \n Doesnt do anything if it's empty
* \n shows text saying category already exists if duplicate entry is attempted
* \n categories are shared with all budget configurations
* \n creates an expense category UI for every budgetpagebudget
*/
void BudgetPage::addExpenseCategory() {
    //category
    QString Category = expenses_categoryLineEdit->text(); //gets category text
    if (!Category.isEmpty()) {
        //if the text isn't empty
        if (expenses_categoriesComboBox->findText(Category) == -1) {
            //if the text isn't already a category
            expenses_categoriesComboBox->addItem(Category); //adds category
            expenses_categoryLineEdit->clear(); //clears text on the line edit
            //creates a new vbox/scrollarea for every budget page's category's expenses, hides them
            for (BudgetPageBudget *budget: budgets) {
                budget->newExpensescrollArea();
                expenses_vbox->addWidget(budget->getExpensescrollarea(expenses_categoriesComboBox->count() - 1));
                budget->getExpensescrollarea(expenses_categoriesComboBox->count() - 1)->hide();
                budget->getExpensescrollarea(expenses_categoriesComboBox->count() - 1)->setDisabled(true);
            }
            //shows the selected category on the current budget page
            budgets[budgetPeriodIndex]->getExpensescrollarea(expenses_categoriesComboBox->count() - 1)->show();
            budgets[budgetPeriodIndex]->getExpensescrollarea(expenses_categoriesComboBox->count() - 1)->
                    setDisabled(false);
            //changes the category combobox selection to the new one
            expenses_categoriesComboBox->setCurrentIndex(expenses_categoriesComboBox->count() - 1);
        } else {
            //gives error if it's already a category
            expenses_categoryLineEdit->setText("Error! category already exists");
            // expenses_categoryLineEdit->setStyleSheet("background-color: red");
        }
    }
}

/**
 * @brief changes to the selected category's expense scroll area
 * \n shows the expenses for the selected category and hides the ones for the prev selected one.
 * @param index category index
 */
void BudgetPage::changeExpenseCategory(int index) {
    //hides prev selected category
    budgets[budgetPeriodIndex]->getExpensescrollarea(expenses_categoriesComboBox_index)->hide();
    budgets[budgetPeriodIndex]->getExpensescrollarea(expenses_categoriesComboBox_index)->setDisabled(true);
    //shows the selected category
    budgets[budgetPeriodIndex]->getExpensescrollarea(index)->setDisabled(false);
    budgets[budgetPeriodIndex]->getExpensescrollarea(index)->show();
    expenses_categoriesComboBox_index = index; //updates the category index var to the new one
}


/**
 * @brief Changes the budget page variables to the new budget period
 * swaps some expense UI elements
 * \n as many UI elements as possible are renamed instead of replaced
  * @author - Katherine R
 */
void BudgetPage::changeBudgetPage() {
    if (this->budgetPeriodIndex >= 0 && this->budgetPeriodIndex <= 17) {
        qDebug() << "Changed budget page";
        //changes budget setter spinbox to the value of the changed page
        budgetSelector_SpinBox->setValue(budgets[budgetPeriodIndex]->getBudget());
        //changes budget goal setter spinbox to the value of the changed page
        budgetSelector_goalSpinBox->setValue(budgets.at(budgetPeriodIndex)->getBudgetGoal());
        //changes the total expenses text to the value of the changed page
        expenses_totalExpensesLabel->setText(
            "Total Expenses: $0" + QString::number(budgets.at(budgetPeriodIndex)->getTotalExpenses()));
        //shows and enables new expense area
        budgets[budgetPeriodIndex]->getExpensescrollarea(expenses_categoriesComboBox_index)->show();
        budgets[budgetPeriodIndex]->getExpensescrollarea(expenses_categoriesComboBox_index)->setDisabled(false);
        calculateRemainingBudget();
    } else {
        if (SHOW_DEBUG_LOGS) {
            qDebug() << "Budget period index out of range";
        }
    }
}

/**
 * @brief creates a new expense item, adds it to the budget object and adds GUI
 * the expense item is created for the BudgetPageBudget at the selected budget period
  * @author - Katherine R
 */
void BudgetPage::newExpense() {
    //adds a new expense object to the current budget page
    budgets[budgetPeriodIndex]->getExpenses()->append(new BudgetPageExpenses());
    BudgetPageExpenses *tempExpense = budgets[budgetPeriodIndex]->getExpenses()->last();
    onExpenseChangedSlot(tempExpense->getExpense());
    // connects signal to know when expense is changed
    connect(tempExpense, &BudgetPageExpenses::expenseChangedSignal, this, &BudgetPage::onExpenseChangedSlot);
    //creates the expense UI and adds it to the budget page's vbox
    tempExpense->createExpenseUI(
        this, budgets[budgetPeriodIndex]->getExpensesscrolllistvbox(expenses_categoriesComboBox_index));
    //connects the remove expense button to deleteExpense,
    connect(tempExpense->getRemoveButton(), &QPushButton::clicked, this,
            [this, tempExpense]() {
                deleteExpense(tempExpense);
            });
}


/**
 * @brief deletes the expense item provided, removes it from the expenses QVector
 * deletes it from the BudgetPageBudget for the selected budget period
 * \n and calculates snew total expense
 * @param toDelete expense item to delete
  * @author - Katherine R
 */
void BudgetPage::deleteExpense(BudgetPageExpenses *toDelete) {
    //gets index of expense to delete
    long long index = budgets[budgetPeriodIndex]->getExpenses()->indexOf(toDelete);
    //gets the the expense value of the object to be deleted
    double tempExpenseTotal = budgets[budgetPeriodIndex]->getExpenses()->at(index)->getExpense();
    //deletes object, then removes it from the budget page's QVector
    budgets[budgetPeriodIndex]->getExpenses()->at(index)->deleteLater();
    budgets[budgetPeriodIndex]->getExpenses()->removeAt(index);
    onExpenseChangedSlot(-tempExpenseTotal); //removes from total expenses
}


/**
 * @brief "updates" the bar graph
 * creates a new graph object, replacing the old one
 * the graph contains X bars, 1 per every budget period (4 for quarterly, 12 for monthly, 1 for yearly)
 * negative budgets show as a red bar, while positive budgets show as a black bar
 * \n trying to actually update the variables wouldn't work automatically,
 * \n so i decided to just add a button to "update" (create a new graph to replace)
 * \n plots the financial goal as a green line
  * @author - Katherine R
 */
void BudgetPage::updateBarGraph() {
    //creates new qwidget, hbox
    barChart_Widget = new QWidget();
    barChart_GroupVbox = new QHBoxLayout();
    barChart_Widget->setLayout(barChart_GroupVbox);
    //creates new QBarset and QStringList for labeling X axis(necessary for updating graph)
    barChart_Value = new QBarSet("Remaining Budget", new QObject());
    barChart_Neg = new QBarSet("Remaining Budget-Negative", new QObject());
    barChart_categories_Quarterly = new QStringList{"Q1", "Q2", "Q3", "Q4"};
    barChart_categories_Monthly = new QStringList{
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };
    barChart_xAxis = new QBarCategoryAxis;
    auto line = new QLineSeries;
    line->setName("Financial Goal");
    int rangelow; //lowest budgets index to graph budget from
    int rangehigh; //highset budgets index to graph budget from
    //determines if it should display yearly/quarterly/monthly from current index
    if (budgetPeriodIndex == 0) {
        rangelow = 0;
        rangehigh = 0;
    } else if (budgetPeriodIndex <= 5) {
        rangelow = 1;
        rangehigh = 5;
        barChart_xAxis->append(*barChart_categories_Quarterly); //adds quarters to x axis labeling
    } else {
        rangelow = 6;
        rangehigh = 17;
        barChart_xAxis->append(*barChart_categories_Monthly); //adds months to x axis labeling
    }
    //adds values from budgets to graph QBarset
    for (int i = rangelow; i <= rangehigh; i++) {
        line->append(i - rangelow, budgets.at(i)->getBudgetGoal());
        if (budgets.at(i)->getRemainingBudget() < 0) {
            // if the remaining budget is negative
            barChart_Neg->append((qreal) budgets.at(i)->getRemainingBudget()); //adds value to negative chart
            barChart_Value->append((qreal) 0); //adds 0 to the positive chart
        } else {
            barChart_Neg->append((qreal) 0); //adds 0 to the negative chart
            barChart_Value->append((qreal) budgets.at(i)->getRemainingBudget()); //adds value to the positive chart
        }
    }
    //sets the colors for the bar chart bars
    barChart_Value->setColor(Qt::black);
    barChart_Neg->setColor(QColor(97, 24, 8)); //red color
    //adds values to the series
    barChart_series = new QStackedBarSeries();
    barChart_series->append(barChart_Value);
    barChart_series->append(barChart_Neg);
    //creates the chart and adds the series(values)
    barChart_chart = new QChart();
    barChart_chart->addSeries(barChart_series);
    barChart_chart->addSeries(line); //adds the finacial goal line
    barChart_chart->setTitle("Budget Graph");;
    //adds x and y axis
    barChart_chart->addAxis(barChart_xAxis, Qt::AlignBottom);
    barChart_series->attachAxis(barChart_xAxis);
    line->attachAxis(barChart_xAxis);
    barChart_yAxis = new QValueAxis;
    barChart_yAxis->setLabelFormat("%5d");
    barChart_chart->addAxis(barChart_yAxis, Qt::AlignLeft);
    line->attachAxis(barChart_yAxis);
    barChart_series->attachAxis(barChart_yAxis);
    barChart_chart->legend()->setVisible(true);
    //creates chartview and adds to layout
    barChart_chartView = new QChartView(barChart_chart);
    barChart_chartView->setLayout(barChart_GroupVbox);
    barChart_GroupVbox->setAlignment(Qt::AlignRight);
    budgetLayout->addWidget(barChart_chartView, 2, 1);
}


/**
 * @brief destructor for budgetpage
  * @author Katherine R
 */
BudgetPage::~BudgetPage() {
    delete budgetSelector_SpinBox;
    delete budgetSelector_Label;
    delete budgetPeriod_QuarterlyComboBox;
    delete budgetPeriod_TypeComboBox;
    delete budgetPeriod_MonthlyComboBox;
    delete budgetSelector_VBox;
    delete budgetSelector_group;
    // delete budgetSelector_SpinBox;

    delete expenses_vbox;
    delete expenses_totalExpensesLabel;
    delete expenses_remainingBudgetLabel;
    delete expenses_addExpenseButton;
    delete expenses_Group;

    budgets.clear();
    delete barChart_Widget;
    // delete barChart_Value;
    // delete barChart_Neg;
    delete barChart_series;
    delete barChart_yAxis;
    delete barChart_xAxis;
    delete barChart_chart;
    delete barChart_chartView;
    delete barGraph_updateButton;
    // delete barChart_GroupVbox;
}
