#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QCloseEvent>


// Forward declarations for Qt classes
QT_BEGIN_NAMESPACE
class QChartView;
class QChart;
class QBarSeries;
class QBarSet;
class QLineSeries;
class QPieSeries;
class QBarCategoryAxis;
class QValueAxis;
QT_END_NAMESPACE

class InventoryPage;
class QStackedWidget;

/**
 * @class Dashboard
 * @brief A widget for visualizing business data, including financials, inventory, and monthly reports.
 * @authors Kareena Sen and Maneet Chahal
 *
 * The Dashboard class provides a user interface to display and update business data in the form of tables and charts.
 * It includes financial data (cash inflows and outflows), inventory data, and monthly revenue reports.
 * The data is visualized using bar charts, line charts, and pie charts.
 *
 * The class is designed to be modular, with separate methods for updating different sections of the dashboard.
 * It also includes slots for handling button clicks to refresh the data and charts dynamically.
 */
class Dashboard : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a Dashboard widget.
     * @param parent Pointer to the parent QWidget (the default is nullptr).
     *
     * This constructor initializes the UI components, including labels, tables, and charts.
     * It also sets up the layout and connects the update button to the appropriate slots.
     * 
     * @author Kareena Sen
     */
    explicit Dashboard(QWidget* parent = nullptr);

    void setCurrentUserId(const QString& id);
    QString getCurrentUserId() const { return currentUserId; }
    void loadMonthlyRevenueData(const QString& userId);
    void saveMonthlyRevenueData(const QString& userId);

    /**
     * @brief Destructor for the Dashboard class.
     *
     * This destructor cleans up dynamically allocated memory for the charts, series, and chart views.
     * 
     * @author Kareena Sen
     */
    ~Dashboard();

    /**
    * @brief Sets the stacked widget for the dashboard.
    *
    * This function assigns the provided QStackedWidget to the dashboard, allowing
    * for navigation between different pages or views within the application.
    *
    * @param stack A pointer to the QStackedWidget that will be used for page navigation.
    * 
    * @author Maneet Chahal
    */
    void setPageStack(QStackedWidget* stack);

    /**
    * @brief Sets the tab widget for the dashboard.
    *
    * This function assigns the provided QTabWidget to the dashboard, enabling
    * tab-based navigation and organization of content within the application.
    *
    * @param tabs A pointer to the QTabWidget that will be used for tab navigation.
    * 
    * @author Maneet Chahal
    */
    void setTabWidget(QTabWidget* tabs);

    /**
    * @brief Sets the inventory table for the dashboard.
    *
    * This function assigns the provided QTableWidget to the dashboard and configures it
    * for displaying inventory data. It ensures the table is properly integrated into
    * the dashboard's layout and UI hierarchy.
    *
    * @param sharedTable A pointer to the QTableWidget that will be used as the inventory table.
    * 
    * @author Maneet Chahal
    */
    void setInventoryTable(QTableWidget* sharedTable);

    /**
    * @brief Gets the inventory table used by the dashboard.
    *
    * This function returns a pointer to the QTableWidget that is currently being used
    * to display inventory data in the dashboard.
    *
    * @return A pointer to the QTableWidget representing the inventory table.
    * 
    * @author Kareena Sen
    */
    QTableWidget* getInventoryTable() const;


private slots:
    /**
     * @brief Updates all the charts in the dashboard.
     *
     * This slot is triggered when the update button is clicked. It updates the bar chart, line chart,
     * and pie chart with the latest data from the tables.
     * 
     * @author Kareena Sen
     */
    void updateCharts();

    /**
     * @brief Updates the financial data displayed in the dashboard.
     *
     * This slot updates the labels displaying cash inflows and outflows with new values.
     * 
     * @author Kareena Sen
     */
    void updateFinancials();

    /**
     * @brief Updates the inventory data displayed in the dashboard.
     *
     * This slot updates the quantity of the first item in the inventory table.
     * 
     * @author Kareena Sen
     */
    void updateInventory();

    /**
     * @brief Updates the monthly revenue data displayed in the dashboard.
     *
     * This slot updates the revenue for February in the monthly reports table.
     * 
     * @author Kareena Sen
     */
    void updateMonthlyReports();

private:
    /**
     * @brief Sets up the pie chart for visualizing gross margin by category.
     *
     * This method initializes the pie chart and adds data to it based on the current cash inflows and outflows.
     * The pie chart is then added to the right layout of the dashboard.
     * 
     * @author Maneet Chahal
     */
    void setupPieChart();

    QTabWidget* dashboardTabWidget = nullptr; // Pointer to the tab widget for dashboard navigation

    QString currentUserId;
    QTableWidget* monthlyReportsTable;

protected:
    void closeEvent(QCloseEvent* event) override;


    

    // UI Components
    QLabel* financialsLabel;       // Label for the financials section header.
    QLabel* inventoryLabel;        // Label for the inventory section header.
    QLabel* monthlyReportsLabel;   // Label for the monthly reports section header.
    QLabel* cashInflowsLabel;      // Label displaying cash inflows.
    QLabel* cashOutflowsLabel;     // Label displaying cash outflows.

    QTableWidget* inventoryTable;      // Table widget for displaying inventory data.
    //QTableWidget* monthlyReportsTable; // Table widget for displaying monthly revenue data.

    QPushButton* updateButton;         // Button to trigger updates for the dashboard.

    // Chart Views
    QChartView* barChartView;   // View for displaying the bar chart.
    QChartView* lineChartView;  // View for displaying the line chart.
    QChartView* pieChartView;   // View for displaying the pie chart.

    // Chart Objects
    QChart* barChart;   // The bar chart object.
    QChart* lineChart;  // The line chart object.
    QChart* pieChart;   // The pie chart object.

    // Chart Series
    QBarSeries* barSeries;   // Series for the bar chart.
    QLineSeries* lineSeries; // Series for the line chart.
    QPieSeries* pieSeries;   // Series for the pie chart.

    // Layouts
    QVBoxLayout* rightLayout; // Layout for the right side of the dashboard (charts).
    QPushButton* editInventoryButton; // Button to trigger the edit inventory functionality
    QStackedWidget* pageStack; // Stacked widget for managing multiple pages/views in the UI

};

#endif // DASHBOARD_H