#ifndef AUTHENTICATESYSTEM_H
#define AUTHENTICATESYSTEM_H

#include <unordered_map>
#include <QString>
#include <QTimer>
#include "user.h"


/**
 * @class AuthenticateSystem
 * @brief Manages user authentication and account security.
 *
 * The AuthenticateSystem class handles user authentication, account locking,
 * and user management. It tracks failed login attempts, locks accounts after
 * too many failures, and provides methods to add, authenticate, and log out users.
 *
 * @author Simran Kullar
 */
class AuthenticateSystem {
private slots:
    void onCountdownTick();


private:
    std::unordered_map<QString, User*> users;          // Stores user IDs mapped to their User objects.
    std::unordered_map<QString, int> failedAttempts;   // Tracks the number of failed login attempts per user.
    std::unordered_map<QString, bool> lockedAccounts;  // Tracks whether a user's account is locked.
    const QString USERS = "users.txt";

public:
    /**
     * @brief Default constructor for AuthenticateSystem.
     *
     * Initializes the AuthenticateSystem with empty user, failed attempts, and locked accounts maps.
     */
    AuthenticateSystem();

    /**
     * @brief Authenticates a user based on their user ID.
     *
     * This method checks if the user exists and if their account is locked. If the user is valid,
     * it resets their failed login attempts. Otherwise, it increments the failed attempts counter.
     *
     * @param userID The ID of the user to authenticate.
     * @return True if authentication is successful, false otherwise.
     */
    bool authenticateUser(const QString& userID);

    /**
     * @brief Locks a user's account after too many failed login attempts.
     *
     * This method locks the user's account by setting their entry in the lockedAccounts map to true.
     *
     * @param userID The ID of the user whose account will be locked.
     */
    void lockAccount(const QString& userID);

    /**
     * @brief Logs out a user by removing them from the active users list.
     *
     * This method removes the user from the users map, effectively logging them out.
     *
     * @param userID The ID of the user to log out.
     */
    void logoutUser(const QString& userID);

    /**
     * @brief Adds a new user to the system.
     *
     * This method creates a new User object and adds it to the users map with the provided user ID.
     * It also initializes the failed attempts and locked status for the new user.
     *
     * @param userID The ID of the user to add.
     * @param role The role of the user (e.g., admin, user).
     */
    void addUser(const QString& userID, const QString& role);

    /**
     * @brief Checks if a user exists in the system.
     *
     * This method checks if the provided user ID exists in the users map.
     *
     * @param userID The ID of the user to check.
     * @return True if the user exists, false otherwise.
     */
    bool userExists(const QString& userID);

    /**
     * @brief Checks if a user's account is locked.
     *
     * This method checks if the provided user ID is marked as locked in the lockedAccounts map.
     *
     * @param userID The ID of the user to check.
     * @return True if the account is locked, false otherwise.
     */
    bool isAccountLocked(const QString& userID);

    void saveUsersToFile();
    void loadUsersFromFile();
};

#endif