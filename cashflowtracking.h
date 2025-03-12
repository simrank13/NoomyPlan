#ifndef CASHFLOWTRACKING_H
#define CASHFLOWTRACKING_H

#include <vector>
#include <string>
#include <unordered_map>

struct DataEntry {
    int transid;
    std::string dateTime;
    std::string seller;
    std::string buyer;
    std::string merchandise;
    double cost;
    std::string category;
};

class CashflowTracking {
private:
    std::vector<DataEntry> dataEntries;
    std::unordered_map<std::string, std::vector<int>> categories;
    int dataCounter = 1;

public:
    void addData(const std::string& dateTime, const std::string& seller, const std::string& buyer, 
                 const std::string& merchandise, double cost, const std::string& category);

    DataEntry* searchDataEntries(int transID);
    void listDataEntries();
    std::vector<DataEntry> getAllDataEntries();  
};

#endif
