#ifndef USER_H
#define USER_H

#include <QString>

/**
 * @brief The User class represents a system user with authentication capabilities.
 *
 * This class encapsulates user information including user ID and role.
 * It provides basic functionality for user authentication such as login and logout
 * operations, as well as accessor methods for retrieving user properties.
 *
 * @author Simran Kullar
 */
class User {
private:
    /** @brief The unique identifier for the user */
    QString userID;

    /** @brief The role or permission level assigned to the user */
    QString role;

public:
    /**
     * @brief Constructor for the User class.
     *
     * Initializes a new User object with the provided ID and role.
     *
     * @param id The unique identifier for the user.
     * @param role The role or permission level assigned to the user.
     *
     */
    User(const QString& id, const QString& role);

    /**
     * @brief Gets the user's ID.
     *
     * Retrieves the unique identifier string associated with this user instance.
     *
     * @return QString The unique identifier of the user.
     *
     */
    QString getUserID();

    /**
     * @brief Gets the user's role.
     *
     * Retrieves the role or permission level assigned to this user.
     *
     * @return QString The role or permission level of the user.
     *
     */
    QString getRole();

    /**
     * @brief Simulates a login operation for the user.
     *
     * Checks if the user has a valid (non-empty) ID to determine
     * if login is possible.
     *
     * @return bool True if the user has a valid ID, false otherwise.
     *
     */
    bool login();

    /**
     * @brief Logs out the current user.
     *
     * Clears the user ID to indicate that no user is logged in.
     *
     */
    void logout();
};

#endif // USER_H