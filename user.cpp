#include "user.h"

/**
 * @brief Constructor for the User class.
 *
 * Initializes a new User object with the provided ID and role. This constructor
 * sets the initial state of the user with their identification and permission level.
 *
 * @param id The unique identifier for the user.
 * @param role The role or permission level assigned to the user.
 *
 * @author Simran Kullar
 */
User::User(const QString& id, const QString& role) : userID(id), role(role) {}

/**
 * @brief Gets the user's ID.
 *
 * Retrieves the unique identifier string associated with this user instance.
 * This ID is used throughout the system to identify and reference the user.
 *
 * @return QString The unique identifier of the user.
 *
 */
QString User::getUserID() { 
    return userID;  // return user's id
}

/**
 * @brief Gets the user's role.
 *
 * Retrieves the role or permission level assigned to this user. Roles determine
 * what actions and resources the user can access within the application.
 *
 * @return QString The role or permission level of the user.
 *
 */
QString User::getRole() { 
    return role; // return user's role
}

/**
 * @brief Simulates a login operation for the user.
 *
 * This method checks if the user has a valid (non-empty) ID to determine
 * if login is possible. A user with an empty ID is considered invalid
 * and cannot be logged in.
 *
 * @return bool True if the user has a valid ID, false otherwise.
 *
 */
bool User::login() {
    return !userID.isEmpty(); // return true if there is valid user id and false otherwise
}

/**
 * @brief Logs out the current user.
 *
 * This method clears the user ID to indicate that no user is logged in.
 * After calling this method, the user will have an empty ID and subsequent
 * calls to login() will return false.
 *
 */
void User::logout() {
    userID = ""; // set empty user id
}