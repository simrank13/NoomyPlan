#include "cashflowtracking.h"
#include <iostream>

void CashflowTracking::addData(const std::string& dateTime, const std::string& seller, 
                               const std::string& buyer, const std::string& merchandise, 
                               double cost, const std::string& category) {
    DataEntry data = {dataCounter++, dateTime, seller, buyer, merchandise, cost, category};
    dataEntries.push_back(data);
    categories[category].push_back(data.transid);
}

DataEntry* CashflowTracking::searchDataEntries(int transID) {
    for (auto& data : dataEntries) {
        if (data.transid == transID) {
            return &data;
        }
    }
    return nullptr;
}

void CashflowTracking::listDataEntries() {
    for (const auto& data : dataEntries) {
        std::cout << "ID: " << data.transid << ", Date/Time: " << data.dateTime 
                  << ", Buyer: " << data.buyer << ", Seller: " << data.seller 
                  << ", Merchandise: " << data.merchandise << ", Cost: $" << data.cost 
                  << ", Category: " << data.category << "\n";
    }
}

std::vector<DataEntry> CashflowTracking::getAllDataEntries() {
    return dataEntries;
}
