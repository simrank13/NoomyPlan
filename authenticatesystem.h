#ifndef AUTHENTICATESYSTEM_H
#define AUTHENTICATESYSTEM_H

#include <unordered_map>
#include <QString>
#include <QTimer>
#include "user.h"

class AuthenticateSystem {
private:
    std::unordered_map<QString, User*> users;   
    std::unordered_map<QString, int> failedAttempts;
    std::unordered_map<QString, bool> lockedAccounts;

public:
    AuthenticateSystem();
    bool authenticateUser(const QString& userID);
    void lockAccount(const QString& userID);
    void logoutUser(const QString& userID);
    void addUser(const QString& userID, const QString& role);
    bool userExists(const QString& userID);
    bool isAccountLocked(const QString& userID);
    
};

#endif
