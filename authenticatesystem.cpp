/**
 * @file authenticatesystem.cpp
 * @brief Implementation of the AuthenticateSystem class.
 * 
 * This file contains the implementation of the AuthenticateSystem class, which 
 * manages user authentication, including login, logout, and account lockout 
 * mechanisms based on failed attempts.
 * 
 * @author Simran Kullar
 */
#include "authenticatesystem.h"
#include <iostream>  

/**
 * @class AuthenticateSystem
 * @brief Handles user authentication and account management.
 * 
 * The AuthenticateSystem class provides functionality to authenticate users,
 * manage failed login attempts, lock and unlock accounts, and register new users.
 */
 
/**
 * @brief Constructs an AuthenticateSystem object.
 * 
 * Initializes the authentication system and sets up necessary data structures 
 * for tracking users and failed login attempts.
 */
AuthenticateSystem::AuthenticateSystem() {}

/**
 * @brief Authenticates a user based on their user ID.
 * 
 * This function checks whether a user exists in the system, verifies if their 
 * account is locked due to excessive failed login attempts, and allows 
 * authentication if the credentials are valid. If authentication fails, it 
 * increments the failure count and locks the account if necessary.
 * 
 * @param userID The ID of the user attempting to log in.
 * @return true if authentication is successful, false otherwise.
 * 
 * @author Simran Kullar
 */
bool AuthenticateSystem::authenticateUser(const QString& userID) {
    std::cout << "Checking login for " << userID.toStdString()
              << ". Failed attempts: " << failedAttempts[userID] << "/5" << std::endl;

    // Check if the account is locked due to previous failed attempts
    if (isAccountLocked(userID)) {
        std::cout << "⚠ Account " << userID.toStdString() << " is LOCKED! Try again later." << std::endl;
        return false;
    }

    // Verify if the user exists in the system
    if (!userExists(userID)) {
        std::cout << "Invalid ID: No user found with ID " << userID.toStdString() << std::endl;
        failedAttempts[userID]++;
        
        // Lock the account if too many failed attempts occur
        if (failedAttempts[userID] >= 5) {
            lockAccount(userID);
        }
        return false;
    }

    // Attempt user login
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
