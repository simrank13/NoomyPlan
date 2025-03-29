/**
 * @class dashboard.cpp
 * @brief Implementation of the Dashboard class for visualizing business data.
 * @author Kareena Sen (data implementation, tables, bar graph, and line chart)
 * @author Maneet Chahal (pie chart implementation and testing)
 */

#include "dashboard.h"
#include "inventory.h"
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QChart>
#include <QtCharts>
#include <QChartView>
#include <QBarSeries>
#include <QBarSet>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QLineSeries>
#include <QPieSeries>
#include <QPieSlice>
#include <QPainter>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

 /**
  * @class Dashboard
  * @brief A widget for visualizing business data including financials, inventory, and monthly reports.
  *
  * The Dashboard class provides an interactive and visually appealing user interface to display and update business data in the form of tables and charts.
  * It includes financial data (cash inflows and outflows), inventory data, and monthly revenue reports.
  * The data is visualized using bar charts, line charts, and pie charts.
  */

  /**
   * @brief Constructs the Dashboard widget and initializes the UI.
   * @param parent Pointer to the parent QWidget.
   *
   * This constructor sets up the main layout of the dashboard, which is divided into two sections:
   * - Left side: Displays financial data, inventory data, and monthly revenue reports in tables.
   * - Right side: Displays data visualization using bar charts, line charts, and pie charts.
   *
   * The constructor also initializes the UI components, including labels, tables, and charts, and connects
   * the update button to the appropriate slots for updating the data and charts.
   * 
   * @author Kareena Sen
   *         Maneet Chahal
   */
Dashboard::Dashboard(QWidget* parent) : QWidget(parent)
{
    setWindowTitle("Business Dashboard"); // Set window title

    // Main layout: Left side (tables) and right side (charts)
    QHBoxLayout* mainLayout = new QHBoxLayout(this);

    // Left side layout (tables)
    QVBoxLayout* leftLayout = new QVBoxLayout();

    QLabel* overviewLabel = new QLabel("Dashboard Overview", this);
    overviewLabel->setAlignment(Qt::AlignCenter);
    overviewLabel->setStyleSheet("font-size: 21px; font-weight: 600; margin-bottom: 10px;");
    leftLayout->addWidget(overviewLabel);

    // Financial Data and Labels
    financialsLabel = new QLabel("Financials: Cash Inflows and Outflows", this);
    financialsLabel->setAlignment(Qt::AlignCenter);
    cashInflowsLabel = new QLabel("Inflows: $5000", this);
    cashOutflowsLabel = new QLabel("Outflows: $3000", this);
    QVBoxLayout* financialsLayout = new QVBoxLayout();
    financialsLayout->addWidget(cashInflowsLabel);
    financialsLayout->addWidget(cashOutflowsLabel);
    leftLayout->addWidget(financialsLabel);
    leftLayout->addLayout(financialsLayout);

    // Inventory Data
    inventoryLabel = new QLabel("Inventory Data", this);
    inventoryLabel->setAlignment(Qt::AlignCenter);
    leftLayout->addWidget(inventoryLabel);


    inventoryTable = nullptr; // set inventoryTable to null pointer as a place holder


    //inventoryTable = new QTableWidget(this); 
    leftLayout->addWidget(inventoryTable);

    editInventoryButton = new QPushButton("Edit Inventory", this); // Make edit inventory button
    leftLayout->addWidget(editInventoryButton);

    // connect edit Inventory button to Dashboard page
    connect(editInventoryButton, &QPushButton::clicked, [this]() {
        if (dashboardTabWidget) {
            dashboardTabWidget->setCurrentIndex(4);
        }
        });

    // Monthly Reports
    monthlyReportsLabel = new QLabel("Monthly Revenue", this); // Create a label for the monthly revenue section
    monthlyReportsLabel->setAlignment(Qt::AlignCenter); // Center-align the label text
    leftLayout->addWidget(monthlyReportsLabel); // Add the label to the left layout

    // Create a table to display monthly revenue data
    monthlyReportsTable = new QTableWidget(3, 2, this); // Initialize a 3x2 table
    monthlyReportsTable->setHorizontalHeaderLabels({ "Month", "Revenue" }); // Set column headers

    // Populate the table with sample data
    monthlyReportsTable->setItem(0, 0, new QTableWidgetItem("January")); // Add January data
    monthlyReportsTable->setItem(0, 1, new QTableWidgetItem("$10000")); // Add January revenue
    monthlyReportsTable->setItem(1, 0, new QTableWidgetItem("February")); // Add February data
    monthlyReportsTable->setItem(1, 1, new QTableWidgetItem("$25000")); // Add February revenue
    monthlyReportsTable->setItem(2, 0, new QTableWidgetItem("March")); // Add March data
    monthlyReportsTable->setItem(2, 1, new QTableWidgetItem("$35000")); // Add March revenue

    leftLayout->addWidget(monthlyReportsTable); // Add the table to the left layout

    // Add left layout to main layout
    mainLayout->addLayout(leftLayout);

    // Right side layout (Charts)
    rightLayout = new QVBoxLayout();

    // Bar Chart
    barChart = new QChart();
    barChart->setTitle("Revenue Earned (Monthly)");
    barSeries = new QBarSeries();
    barChart->addSeries(barSeries);

    QBarCategoryAxis* barAxisX = new QBarCategoryAxis();
    barChart->addAxis(barAxisX, Qt::AlignBottom);
    barSeries->attachAxis(barAxisX);

    QValueAxis* barAxisY = new QValueAxis();
    barAxisY->setLabelFormat("$%.0f");
    barChart->addAxis(barAxisY, Qt::AlignLeft);
    barSeries->attachAxis(barAxisY);

    barChartView = new QChartView(barChart);
    barChartView->setRenderHint(QPainter::Antialiasing);
    rightLayout->addWidget(barChartView);

    // Line Chart Implementation
    lineChart = new QChart();
    lineChart->setTitle("Revenue Growth (Monthly)");
    lineSeries = new QLineSeries();
    lineChart->addSeries(lineSeries);

    QValueAxis* lineAxisX = new QValueAxis();
    lineAxisX->setTitleText("Month");
    QValueAxis* lineAxisY = new QValueAxis();
    lineAxisY->setLabelFormat("$%.0f");
    lineChart->addAxis(lineAxisX, Qt::AlignBottom);
    lineChart->addAxis(lineAxisY, Qt::AlignLeft);
    lineSeries->attachAxis(lineAxisX);
    lineSeries->attachAxis(lineAxisY);

    lineChartView = new QChartView(lineChart);
    lineChartView->setRenderHint(QPainter::Antialiasing);
    rightLayout->addWidget(lineChartView);

    // Pie Chart
    setupPieChart();

    // Add layouts to main layout
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);

    // Update Button
    updateButton = new QPushButton("Update Dashboard", this);
    leftLayout->addWidget(updateButton);

    // Connects signals
    connect(updateButton, &QPushButton::clicked, this, &Dashboard::updateFinancials);
    connect(updateButton, &QPushButton::clicked, this, &Dashboard::updateInventory);
    connect(updateButton, &QPushButton::clicked, this, &Dashboard::updateMonthlyReports);
    connect(updateButton, &QPushButton::clicked, this, &Dashboard::updateCharts);

    updateCharts();
}

/**
 * @brief Sets the tab widget for the dashboard and configures navigation.
 *
 * This function assigns the provided QTabWidget to the dashboard and sets up a connection
 * for the `editInventoryButton` to switch to a specific tab (e.g., the inventory tab) when clicked.
 * The tab index is hardcoded, so ensure it matches the intended tab in the QTabWidget.
 *
 * @param tabs A pointer to the QTabWidget that the dashboard will use for navigation.
 * @note The tab index (e.g., 4) should be updated to match the actual index of the target tab.
 * 
 * @author Maneet Chahal
 */
void Dashboard::setTabWidget(QTabWidget* tabs) {
    dashboardTabWidget = tabs; // Assign the provided tab widget to the dashboard

    // Connect the edit inventory button to switch to the specified tab
    connect(editInventoryButton, &QPushButton::clicked, [this]() {
        if (dashboardTabWidget) {
            dashboardTabWidget->setCurrentIndex(4);  // Switch to the tab at index 4 (e.g., inventory tab)
        }
        });
}


/**
 * @brief Sets up the pie chart for visualizing gross margin by category.
 *
 * This method initializes the pie chart and adds data to it based on the current cash inflows and outflows.
 * The pie chart is then added to the right layout of the dashboard.
 * 
 * @author Maneet Chahal
 */
void Dashboard::setupPieChart()
{
    pieChart = new QChart();
    pieChart->setTitle("Gross Margin by Category");

    pieSeries = new QPieSeries();

    // Extract the current inflows and outflows from labels
    double inflows = cashInflowsLabel->text().split('$').last().toDouble();
    double outflows = cashOutflowsLabel->text().split('$').last().toDouble();

    // Add data to the pie series
    pieSeries->append("Inflows", inflows);
    pieSeries->append("Outflows", outflows);

    // Add data to the pie chart
    pieChart->addSeries(pieSeries);
    pieChartView = new QChartView(pieChart);
    pieChartView->setRenderHint(QPainter::Antialiasing);
    rightLayout->addWidget(pieChartView);
}

/**
 * @brief Destructor for the Dashboard class.
 *
 * This destructor cleans up the dynamically allocated memory for the charts, series, and chart views (data visualization).
 * 
 * @author Kareena Sen
 */
Dashboard::~Dashboard()
{
    // Safely remove and delete bar series
    if (barSeries && barChart) {
        barChart->removeSeries(barSeries);
        delete barSeries;
        barSeries = nullptr;
    }

    // Safely remove and delete line series
    if (lineSeries && lineChart) {
        lineChart->removeSeries(lineSeries);
        delete lineSeries;
        lineSeries = nullptr;
    }

    // Safely remove pie series (optional since replaced)
    if (pieSeries && pieChart) {
        pieChart->removeSeries(pieSeries);
        delete pieSeries;
        pieSeries = nullptr;
    }

    // Delete charts and views
    delete barChart;        barChart = nullptr;
    delete lineChart;       lineChart = nullptr;
    delete pieChart;        pieChart = nullptr;
    delete barChartView;    barChartView = nullptr;
    delete lineChartView;   lineChartView = nullptr;
    delete pieChartView;    pieChartView = nullptr;
}



/**
 * @brief Updates the financial data displayed in the dashboard.
 *
 * This method updates the labels displaying cash inflows and outflows with new values.
 * 
 * @author Kareena Sen
 */
void Dashboard::updateFinancials()
{
    // display cash inflow and outflow for testing purposes 
    financialsLabel->setText("Financials: Cash Inflows and Outflows Updated");

    cashInflowsLabel->setText("Inflows: $5500");
    cashOutflowsLabel->setText("Outflows: $3200");
}

/**
 * @brief Updates the inventory data displayed in the dashboard.
 *
 * This method updates the quantity of the first item in the inventory table.
 * 
 * @author Kareena Sen
 */
void Dashboard::updateInventory()
{
    // Test if updated inventory item is displayed
    inventoryTable->setItem(0, 1, new QTableWidgetItem("120"));
}

/**
 * @brief Updates the monthly revenue data displayed in the dashboard.
 *
 * This method updates the revenue for February in the monthly reports table.
 */
void Dashboard::updateMonthlyReports()
{
    /*monthlyReportsTable->setItem(1, 1, new QTableWidgetItem("$27000"));*/
    if (!currentUserId.isEmpty()) {
        saveMonthlyRevenueData(currentUserId);
    }
}

/**
 * @brief Gets the inventory table used by the dashboard.
 *
 * This function returns a pointer to the QTableWidget that is currently being used
 * to display inventory data in the dashboard. The table can be used to view or
 * modify inventory items.
 *
 * @return A pointer to the QTableWidget representing the inventory table.
 * 
 * @author Maneet Chahal
 */
QTableWidget* Dashboard::getInventoryTable() const {
    return inventoryTable; // Return the pointer to the inventory table
}


/**
 * @brief Updates all the charts in the dashboard.
 *
 * This method updates the bar chart, line chart, and pie chart with the latest data from the tables.
 * It extracts data from the monthly reports table and updates the charts accordingly.
 * 
 * @author Kareena Sen
 */
void Dashboard::updateCharts()
{

    if (!barSeries || !lineSeries || !pieChart || !monthlyReportsTable) return;

    // Clear existing data
    barSeries->clear();
    lineSeries->clear();

    // Extract data from the monthly reports table
    QStringList categories;
    double maxRevenue = 0;

    for (int row = 0; row < monthlyReportsTable->rowCount(); ++row) {
        if (monthlyReportsTable->item(row, 0) && monthlyReportsTable->item(row, 1)) {
            QString month = monthlyReportsTable->item(row, 0)->text();
            QString revenueStr = monthlyReportsTable->item(row, 1)->text().remove('$');
            double revenue = revenueStr.toDouble();

            // Add data to bar chart
            QBarSet* barSet = new QBarSet(month);
            *barSet << revenue;
            barSeries->append(barSet);

            // Add data to line chart
            lineSeries->append(row, revenue);

            // Update maximum for scaling
            if (revenue > maxRevenue) maxRevenue = revenue;

            // Store the month for X axis
            categories << month;
        }
    }


    // Update bar chart axes
    QBarCategoryAxis* barAxisX = qobject_cast<QBarCategoryAxis*>(barChart->axes(Qt::Horizontal).first());
    if (barAxisX) {
        barAxisX->clear();
        barAxisX->append(categories);
    }

    QValueAxis* barAxisY = qobject_cast<QValueAxis*>(barChart->axes(Qt::Vertical).first());
    if (barAxisY) {
        barAxisY->setRange(0, maxRevenue * 1.2);
    }

    // Update line chart axes
    QValueAxis* lineAxisX = qobject_cast<QValueAxis*>(lineChart->axes(Qt::Horizontal).first());
    if (lineAxisX) {
        lineAxisX->setRange(0, categories.size() - 1);
        lineAxisX->setTickCount(categories.size());
    }

    QValueAxis* lineAxisY = qobject_cast<QValueAxis*>(lineChart->axes(Qt::Vertical).first());
    if (lineAxisY) {
        lineAxisY->setRange(0, maxRevenue * 1.2);
    }

    // Update pie chart
    pieChart->removeAllSeries();

    // Create pie series 
    pieSeries = new QPieSeries();
    double inflows = cashInflowsLabel->text().split('$').last().toDouble();
    double outflows = cashOutflowsLabel->text().split('$').last().toDouble();

    // Add inflows and outflows to pie Series
    pieSeries->append("Inflows", inflows);
    pieSeries->append("Outflows", outflows);

    // Add pie series to pie chart
    pieChart->addSeries(pieSeries);
}

/**
 * @brief Sets the inventory table for the dashboard and configures its display.
 *
 * This function assigns the provided QTableWidget to the dashboard and configures it
 * with appropriate headers and optional demo data. It also ensures the table is added
 * to the UI hierarchy and positioned correctly within the layout.
 *
 * @param sharedTable A pointer to the QTableWidget that will be used as the inventory table.
 * @note The table is added to the left layout at a specific position (e.g., index 3). Adjust
 *       the position as needed based on your layout structure.
 * 
 * @author Maneet Chahal
 */
void Dashboard::setInventoryTable(QTableWidget* sharedTable) {
    inventoryTable = sharedTable; // Assign the provided table to the dashboard
    inventoryTable->setParent(this);  // Ensure it's part of the UI hierarchy

    // Configure table headers and resize behavior
    inventoryTable->setHorizontalHeaderLabels({ "Item", "Quantity", "Price" });
    inventoryTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Add some demo data if the table is empty
    if (inventoryTable->rowCount() == 0) {
        inventoryTable->insertRow(0);
        inventoryTable->setItem(0, 0, new QTableWidgetItem("Item 1"));
        inventoryTable->setItem(0, 1, new QTableWidgetItem("100"));
        inventoryTable->setItem(0, 2, new QTableWidgetItem("$10"));

        inventoryTable->insertRow(1);
        inventoryTable->setItem(1, 0, new QTableWidgetItem("Item 2"));
        inventoryTable->setItem(1, 1, new QTableWidgetItem("200"));
        inventoryTable->setItem(1, 2, new QTableWidgetItem("$5"));
    }

    // Insert into the left layout after the inventoryLabel
    QVBoxLayout* leftLayout = qobject_cast<QVBoxLayout*>(this->layout()->itemAt(0)->layout());
    if (leftLayout) {
        // Ensure it's not already added (to prevent duplication)
        if (leftLayout->indexOf(inventoryTable) == -1) {
            leftLayout->insertWidget(3, inventoryTable); 
        }
    }
}

void Dashboard::saveMonthlyRevenueData(const QString& userId) {
    QJsonArray revenueArray;

    for (int row = 0; row < monthlyReportsTable->rowCount(); ++row) {
        QJsonObject entry;
        entry["month"] = monthlyReportsTable->item(row, 0)->text();
        entry["revenue"] = monthlyReportsTable->item(row, 1)->text();
        revenueArray.append(entry);
    }

    QJsonObject root;
    root["revenueData"] = revenueArray;

    QJsonDocument doc(root);
    QFile file("data/" + userId + "_revenue.json");
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to open file for writing!";
        return;
    }

    file.write(doc.toJson());
    file.close();
}

void Dashboard::loadMonthlyRevenueData(const QString& userId) {
    QFile file("data/" + userId + "_revenue.json");
    if (!file.open(QIODevice::ReadOnly)) return;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonArray revenueArray = doc["revenueData"].toArray();
    file.close();

    monthlyReportsTable->setRowCount(0);
    for (const QJsonValue& val : revenueArray) {
        QJsonObject obj = val.toObject();
        int row = monthlyReportsTable->rowCount();
        monthlyReportsTable->insertRow(row);
        monthlyReportsTable->setItem(row, 0, new QTableWidgetItem(obj["month"].toString()));
        monthlyReportsTable->setItem(row, 1, new QTableWidgetItem(obj["revenue"].toString()));
    }

    updateCharts();  // Refresh the visualizations
}

void Dashboard::setCurrentUserId(const QString& id) {
    currentUserId = id;
}

void Dashboard::closeEvent(QCloseEvent* event)
{
    if (!currentUserId.isEmpty()) {
        saveMonthlyRevenueData(currentUserId);
    }
    QWidget::closeEvent(event);
}



