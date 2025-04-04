/**
* @class authenticatesystem.cpp
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
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QString>
#include <QObject>
#include <QFile>
#include <QTextStream>

const QString USERS = "users.txt";

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
AuthenticateSystem::AuthenticateSystem() {
    loadUsersFromFile();
}

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
 */
bool AuthenticateSystem::authenticateUser(const QString& userID) {
    // Display current login attempt information including the number of previous failures
    std::cout << "Checking login for " << userID.toStdString()
        << ". Failed attempts: " << failedAttempts[userID] << "/5" << std::endl;

    // Check if the account is locked due to previous failed attempts
    if (isAccountLocked(userID)) {
        // If account is locked, notify user and prevent further login attempts
        std::cout << "⚠ Account " << userID.toStdString() << " is LOCKED! Try again later." << std::endl;
        return false; // Authentication fails for locked accounts
    }

    // Verify if the user exists in the system
    if (!userExists(userID)) {
        std::cout << "Invalid ID: No user found with ID " << userID.toStdString() << std::endl;   // Log the failed attempt with detailed error message
        failedAttempts[userID]++;   // Increment failed attempt counter even for non-existent accounts

        // Lock the account if too many failed attempts occur
        if (failedAttempts[userID] >= 5) {
            // After 5 failures, lock the non-existent userID to slow down brute force attempts
            lockAccount(userID);
        }
        return false; // Authentication fails for non-existent users
    }

    // Attempt user login
    if (users[userID]->login()) {
        // Authentication successful - log the event
        std::cout << "Successful login for " << userID.toStdString() << std::endl;
        // Reset the failed attempt counter upon successful authentication
        failedAttempts[userID] = 0;
        return true; // Authentication succeeded
    }
    // Increment the failed attempt counter for this specific userID
    failedAttempts[userID]++;

    // Log the failed attempt with the updated counter
    std::cout << "Failed login attempt " << failedAttempts[userID] << "/5 for "
        << userID.toStdString() << std::endl;

    // Lock the account if too many failed attempts occur
    if (failedAttempts[userID] >= 5) {
        // After 5 failures, lock the non-existent userID to slow down brute force attempts
        lockAccount(userID);
    }

    return false; // Authentication failed
}

/**
 * @brief Locks a user account for a specified duration.
 *
 * This function marks a user account as locked due to excessive failed login attempts.
 * A timer is set to automatically unlock the account after a 60-second timeout period.
 *
 * @param userID The ID of the user whose account should be locked.
 *
 */void AuthenticateSystem::lockAccount(const QString& userID) {
     lockedAccounts[userID] = true;
     failedAttempts[userID] = 0;

     // Create the lockout popup dialog
     QDialog* lockDialog = new QDialog();
     lockDialog->setWindowTitle("Account Locked");
     lockDialog->setModal(true); // Blocks interaction with other windows
     lockDialog->setWindowFlags(lockDialog->windowFlags() & ~Qt::WindowCloseButtonHint); // Disable close button

     QLabel* countdownLabel = new QLabel("ACCOUNT LOCKED\n\nTime remaining: 60 seconds");
     countdownLabel->setAlignment(Qt::AlignCenter);

     QVBoxLayout* layout = new QVBoxLayout(lockDialog);
     layout->addWidget(countdownLabel);
     lockDialog->setLayout(layout);
     lockDialog->resize(300, 150);

     int* remainingTime = new int(60);
     QTimer* timer = new QTimer(lockDialog);
     timer->setInterval(1000); // 1 second

     QObject::connect(timer, &QTimer::timeout, [=]() mutable {
         (*remainingTime)--;

         countdownLabel->setText(QString("ACCOUNT LOCKED\n\nTime remaining: %1 seconds").arg(*remainingTime));

         if (*remainingTime <= 0) {
             timer->stop();
             lockedAccounts[userID] = false;
             delete remainingTime;
             lockDialog->accept(); // Closes the dialog
         }
         });

     timer->start();
     lockDialog->exec(); // Show and block until finished
 }


/**
 * @brief Checks if a user account is currently locked.
 *
 * This function verifies whether a user account has been locked due to
 * excessive failed login attempts.
 *
 * @param userID The ID of the user to check.
 * @return true if the account is locked, false otherwise.
 */
bool AuthenticateSystem::isAccountLocked(const QString& userID) {
    // Check if the userID exists in the lockedAccounts map AND if its value is true
    return lockedAccounts.find(userID) != lockedAccounts.end() && lockedAccounts[userID];
}

/**
 * @brief Logs out a user from the system.
 *
 * This function handles the logout process for a specified user, calling the
 * logout method of the corresponding User object if it exists.
 *
 * @param userID The ID of the user to log out.
 */
void AuthenticateSystem::logoutUser(const QString& userID) {
    // Check if the user exists in our system before attempting logout
    if (users.find(userID) != users.end()) {
        // User exists, so we can safely call the logout method
        users[userID]->logout();
    }
}

/**
 * @brief Adds a new user to the authentication system.
 *
 * This function creates a new user account with the specified ID and role,
 * but only if the user ID is not already registered in the system.
 *
 * @param userID The ID for the new user account.
 * @param role The role or permission level for the new user.
 */
void AuthenticateSystem::addUser(const QString& userID, const QString& role) {
    // Check if a user with this ID already exists in the system
    if (users.find(userID) != users.end()) {
        // User already exists - notify and abort the registration process
        std::cout << "User " << userID.toStdString() << " is already signed up. Please log in :)" << std::endl;
        return; // return to prevent creating duplicate users
    }
    // Create a new User object with the provided ID and role
    users[userID] = new User(userID, role);
    // Notify through console that the user creation was successful
    std::cout << "New account created for " << userID.toStdString() << ". You can now log in!" << std::endl;
    saveUsersToFile();

}

/**
 * @brief Checks if a user exists in the system.
 *
 * This function verifies whether a user with the specified ID has been registered
 * in the authentication system.
 *
 * @param userID The ID of the user to check.
 * @return true if the user exists, false otherwise.
 */
bool AuthenticateSystem::userExists(const QString& userID) {
    // Check if the userID exists in the 'users' container by searching for it.
    // If the userID is found, return true; otherwise, return false.
    return users.find(userID) != users.end();
}

void AuthenticateSystem::saveUsersToFile() {
    QFile file(USERS);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (const auto& pair : users) {
            out << pair.first << "," << pair.second->getRole() << "\n";
        }
        file.close();
    }
}

void AuthenticateSystem::loadUsersFromFile() {
    QFile file(USERS);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(",");
            if (parts.size() == 2) {
                QString id = parts[0];
                QString role = parts[1];
                users[id] = new User(id, role);
            }
        }
        file.close();
    }
}

