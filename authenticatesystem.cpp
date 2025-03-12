#include "authenticatesystem.h"
#include <iostream>  

AuthenticateSystem::AuthenticateSystem() {}

bool AuthenticateSystem::authenticateUser(const QString& userID) {
    std::cout << "Checking login for " << userID.toStdString()
              << ". Failed attempts: " << failedAttempts[userID] << "/5" << std::endl;

    if (isAccountLocked(userID)) {
        std::cout << "⚠ Account " << userID.toStdString() << " is LOCKED! Try again later." << std::endl;
        return false;
    }

    if (!userExists(userID)) {
        std::cout << "Invalid ID: No user found with ID " << userID.toStdString() << std::endl;
        failedAttempts[userID]++;

        if (failedAttempts[userID] >= 5) {
            lockAccount(userID);
        }

        return false;
    }

    if (users[userID]->login()) {
        std::cout << "Successful login for " << userID.toStdString() << std::endl;
        failedAttempts[userID] = 0;
        return true;
    }

    failedAttempts[userID]++;
    std::cout << "Failed login attempt " << failedAttempts[userID] << "/5 for " 
              << userID.toStdString() << std::endl;

    if (failedAttempts[userID] >= 5) {
        lockAccount(userID);
    }
     
    return false;
}

void AuthenticateSystem::lockAccount(const QString& userID) {
    lockedAccounts[userID] = true;
    std::cout << "Account " << userID.toStdString() << " is now LOCKED for 1 minute :(" << std::endl;


    QTimer::singleShot(60000, [this, userID]() {
        lockedAccounts[userID] = false;
        failedAttempts[userID] = 0;
        std::cout << "✔️ Account " << userID.toStdString() << " is now UNLOCKED!" << std::endl;
    });
}
bool AuthenticateSystem::isAccountLocked(const QString& userID) {
    return lockedAccounts.find(userID) != lockedAccounts.end() && lockedAccounts[userID];
}



void AuthenticateSystem::logoutUser(const QString& userID) {
    if (users.find(userID) != users.end()) {
        users[userID]->logout();
    }
}

void AuthenticateSystem::addUser(const QString& userID, const QString& role) {
    if (users.find(userID) != users.end()) {
        std::cout << "User " << userID.toStdString() << " is already signed up. Please log in :)" << std::endl;
        return;
    }

    users[userID] = new User(userID, role);
    std::cout << "New account created for " << userID.toStdString() << ". You can now log in!" << std::endl;
}
bool AuthenticateSystem::userExists(const QString& userID) {
    return users.find(userID) != users.end();
}
