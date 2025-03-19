#include "loginpage.h"
#include <iostream>

/**
 * @brief Constructor for the LoginPage class.
 *
 * Initializes the login page UI and sets up connections for login and signup buttons.
 *
 * @class loginpage.cpp
 *
 * @param auth Pointer to the `AuthenticateSystem` used for user authentication.
 * @param mainWin Pointer to the main window of the application.
 * @param parent The parent widget (default is nullptr).
 *
 * @author Simran Kullar
 */
LoginPage::LoginPage(AuthenticateSystem* auth, QMainWindow* mainWin, QWidget* parent)
    : QMainWindow(parent), authSystem(auth), mainWindow(mainWin) {
    // Set window title and size
    setWindowTitle("Login - Business Management System");
    resize(800, 600);

    // Create a central widget and set it as the main widget
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Create a vertical layout for the central widget
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    // Create and configure UI elements
    QLabel* titleLabel = new QLabel("Business Management System - Login");
    idInput = new QLineEdit();
    idInput->setPlaceholderText("Enter your ID");
    loginButton = new QPushButton("Login");
    signupButton = new QPushButton("Sign Up");
    statusLabel = new QLabel("");

    // Add UI elements to the layout
    layout->addWidget(titleLabel);
    layout->addWidget(idInput);
    layout->addWidget(loginButton);
    layout->addWidget(signupButton);
    layout->addWidget(statusLabel);

    // Connect button signals to slots
    connect(loginButton, &QPushButton::clicked, this, &LoginPage::handleLogin);
    connect(signupButton, &QPushButton::clicked, this, &LoginPage::handleSignup);
}

/**
 * @brief Handles the login button click event.
 *
 * Validates user input, checks if the account exists and is not locked,
 * authenticates the user, and provides appropriate feedback.
 *
 * The function performs the following checks in sequence:
 * 1. Checks if the user account is locked
 * 2. Verifies if the user exists in the system
 * 3. Attempts to authenticate the user credentials
 *
 */
void LoginPage::handleLogin() {
    // Get the user ID from input field and remove any leading/trailing whitespace
    QString id = idInput->text().trimmed();

    // Log the login attempt to console for debugging
    std::cout << "Attempting login for user: " << id.toStdString() << std::endl;

    // Check if the account is locked due to too many failed attempts
    if (authSystem->isAccountLocked(id)) {
        // Display lock message to user
        statusLabel->setText("Account is locked! Try again in 1 minute.");
        // Log the lock status to console
        std::cout << "UI Message: Account " << id.toStdString() << " is locked! Attempts Blocked." << std::endl;
        return; // Exit function early
    }

    // Verify if the user exists in the authentication system
    if (!authSystem->userExists(id)) {
        // Inform user they need to sign up first
        statusLabel->setText("✖️ You are not signed up. Please sign up first.");
        // Log the redirect suggestion to console
        std::cout << "User " << id.toStdString() << " not found. Redirecting to sign up." << std::endl;
        return; // Exit function early
    }

    // Attempt to authenticate the user with provided credentials
    if (authSystem->authenticateUser(id)) {
        // Display success message
        statusLabel->setText("✔️ Login successful!");
        // Log successful login to console
        std::cout << "User " << id.toStdString() << " logged in successfully." << std::endl;
        // Show the main application window
        mainWindow->show();
        // Close the login window
        this->close();
    }
    else {
        // Display error message for failed authentication
        statusLabel->setText("✖️ Incorrect login. Try again.");
    }
}

/**
 * @brief Handles the signup button click event.
 *
 * Creates a new user account after validating input and checking various conditions.
 *
 * This function performs the following validation steps:
 * 1. Ensures the ID field is not empty
 * 2. Verifies the account is not currently locked
 * 3. Checks if the user already exists
 * 4. Creates a new user with default role "User" if all checks pass
 *
 */
void LoginPage::handleSignup() {
    // Get the user ID from input field and remove any leading/trailing whitespace
    QString id = idInput->text().trimmed();

    // Check if the ID field is empty
    if (id.isEmpty()) {
        // Display error message for empty ID
        statusLabel->setText("✖️ID cannot be empty.");
        return; // Exit function early
    }

    // Check if the account is currently locked (prevents creating new accounts from locked IPs)
    if (authSystem->isAccountLocked(id)) {
        // Display lock message to user
        statusLabel->setText("Account is locked! You cannot create an account right now.");
        return; // Exit function early
    }

    // Check if user already exists in the system
    if (authSystem->userExists(id)) {
        // Inform user they should log in instead of signing up again
        statusLabel->setText("⚠ You already signed up. Please log in.");
        return; // Exit function early
    }

    // Add the new user to the authentication system with default role "User"
    authSystem->addUser(id, "User");

    // Display success message and prompt user to log in
    statusLabel->setText("✔️ Account created successfully! Please log in.");
}
