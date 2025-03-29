#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "authenticatesystem.h"
#include "mainwindow.h"

/**
 * @brief The LoginPage class provides a user interface for login and signup functionality.
 *
 * This class creates a window with input fields and buttons for user authentication.
 * It handles login and signup operations by interacting with the authentication system.
 * The class is responsible for validating user input, showing appropriate messages,
 * and directing users to the main application window upon successful login.
 *
 * @author Simran Kullar
 */
class LoginPage : public QMainWindow {
    Q_OBJECT
private:
    /**
 * @brief Input field for user ID.
 *
 * This QLineEdit widget allows the user to enter their unique identifier (e.g., username or email).
 * It is used during the login or signup process to identify the user.
 */
    QLineEdit* idInput;

    /**
     * @brief Button to trigger login functionality.
     *
     * This QPushButton widget is used to initiate the login process. When clicked, it validates
     * the user's credentials using the authentication system and grants access if successful.
     */
    QPushButton* loginButton;

    /**
     * @brief Button to trigger signup functionality.
     *
     * This QPushButton widget is used to initiate the signup process. When clicked, it allows
     * new users to create an account by providing necessary details and storing them in the system.
     */
    QPushButton* signupButton;

    /**
     * @brief Label to display status messages and feedback to the user.
     *
     * This QLabel widget is used to provide real-time feedback to the user, such as success messages,
     * error notifications, or instructions during the login or signup process.
     */
    QLabel* statusLabel;

    /**
     * @brief Pointer to the authentication system for user verification.
     *
     * This AuthenticateSystem object handles user authentication, including login and signup operations.
     * It verifies user credentials and manages account creation and validation.
     */
    AuthenticateSystem* authSystem;

    /**
     * @brief Pointer to the main application window to show after successful login.
     *
     * This QMainWindow widget represents the main interface of the application. It is displayed
     * to the user after they successfully log in, providing access to the application's features.
     */
    QMainWindow* mainWindow;

    void showLockDialog();

public:
    /**
     * @brief Constructor for the LoginPage class.
     *
     * Initializes the login page UI and sets up connections for login and signup buttons.
     *
     * @param auth Pointer to the AuthenticateSystem used for user authentication.
     * @param mainWin Pointer to the main window of the application.
     * @param parent The parent widget (default is nullptr).
     */
    explicit LoginPage(AuthenticateSystem* auth, QMainWindow* mainWin, QWidget* parent = nullptr);

private:
    /**
     * @brief Handles the login button click event.
     *
     * Validates user input, checks if the account exists and is not locked,
     * authenticates the user, and provides appropriate feedback.
     */
    void handleLogin();

    /**
     * @brief Handles the signup button click event.
     *
     * Creates a new user account after validating input and checking
     * if the ID is not empty, the account is not locked, and the user doesn't already exist.
     */
    void handleSignup();
};

#endif // LOGINPAGE_H