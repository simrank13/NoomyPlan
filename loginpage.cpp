#include "loginpage.h"
#include <iostream>
#include <QDir>
#include <QDialog>
#include <QTimer>
#include <QVBoxLayout>
#include <QLabel>
#include <QObject>
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
    // Title
    QLabel* titleLabel = new QLabel("Business Management System");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold;");

    // Subtitle
    QLabel* loginLabel = new QLabel("Login");
    loginLabel->setAlignment(Qt::AlignCenter);
    loginLabel->setStyleSheet("font-size: 16px; color: gray;");

    // Create the user icon
    QLabel* userIcon = new QLabel();
    QPixmap userPixmap("user.png");

    if (!userPixmap.isNull()) {
        userIcon->setPixmap(userPixmap.scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        userIcon->setAlignment(Qt::AlignCenter);
    }
    else {
        userIcon->setAlignment(Qt::AlignCenter);
        userIcon->setStyleSheet("font-size: 32px;");
    }

    idInput = new QLineEdit();
    idInput->setPlaceholderText("Enter your ID");
    idInput->setFixedWidth(200); // match total button width

    QHBoxLayout* idRow = new QHBoxLayout();
    idRow->addStretch();             // push content to center
    idRow->addWidget(idInput);
    idRow->addStretch();

            // add the row to the main vertical layout

    loginButton = new QPushButton("Login");
    signupButton = new QPushButton("Sign Up");
    statusLabel = new QLabel("");

    loginButton->setFixedWidth(200);
    signupButton->setFixedWidth(200);

    



    QLabel* noteLabel = new QLabel("Note: ID must be at least 5 characters long.");
    noteLabel->setStyleSheet("color: gray; font-size: 12px;");
    noteLabel->setAlignment(Qt::AlignCenter);

    // Add UI elements to the layout

    layout->addWidget(titleLabel);
    layout->addWidget(userIcon);
    layout->addWidget(loginLabel);
    //layout->addWidget(idInput);
    

    //QHBoxLayout* buttonRow = new QHBoxLayout();
    //buttonRow->addStretch();  
    layout->addLayout(idRow);
    layout->addSpacing(25);
    // pushes buttons to center
    //buttonRow->addWidget(loginButton);
    //buttonRow->addWidget(signupButton);
    //buttonRow->addStretch();             // pushes buttons to center
    //layout->addLayout(buttonRow);
    layout->addWidget(loginButton);
    layout->addSpacing(2);
    layout->addWidget(signupButton);

    layout->addWidget(statusLabel);
    layout->addWidget(noteLabel);


    layout->setAlignment(loginButton, Qt::AlignHCenter);
    layout->setAlignment(signupButton, Qt::AlignHCenter);


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
    QString id = idInput->text().trimmed();

    if (id.isEmpty()) {
        statusLabel->setText("❌ ID is required. Please try again.");
        statusLabel->setStyleSheet("color: red; font-size: 13px;");
        statusLabel->setAlignment(Qt::AlignCenter);
        return;
    }

    if (id.length() < 5) {
        statusLabel->setText("❌ ID must be at least 5 characters long.");
        statusLabel->setAlignment(Qt::AlignCenter);
        statusLabel->setStyleSheet("color: red; font-size: 13px;");
        statusLabel->setAlignment(Qt::AlignCenter);
        return;
    }

    std::cout << "Attempting login for user: " << id.toStdString() << std::endl;

    // First, check if the account is locked
    if (authSystem->isAccountLocked(id)) {
        std::cout << "[DEBUG] showLockDialog triggered!\n";
        showLockDialog();
        return;
    }

    if (!authSystem->userExists(id)) {
        statusLabel->setText("❌ You are not signed up. Please sign up first.");
        statusLabel->setStyleSheet("color: red; font-size: 13px;");
        statusLabel->setAlignment(Qt::AlignCenter);
        return;
    }

    bool success = authSystem->authenticateUser(id);

    if (success) {
        MainWindow* realMainWindow = qobject_cast<MainWindow*>(mainWindow);
        if (realMainWindow) {
            realMainWindow->setCurrentUserId(id);
            realMainWindow->show();
            this->close();
        }
        else {
            qDebug() << "Failed to cast QMainWindow* to MainWindow*";
            statusLabel->setStyleSheet("color: red; font-size: 13px;");
            statusLabel->setAlignment(Qt::AlignCenter);
        }
    }
    else {
        statusLabel->setText("❌ Incorrect login. Try again.");
        statusLabel->setStyleSheet("color: red; font-size: 13px;");
        statusLabel->setAlignment(Qt::AlignCenter);
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
 */void LoginPage::handleSignup() {
     QString id = idInput->text().trimmed();

     if (id.isEmpty()) {
         statusLabel->setText("❌ ID is required. Please try again.");
         statusLabel->setStyleSheet("color: red; font-size: 13px;");
         statusLabel->setAlignment(Qt::AlignCenter);
         return;
     }

     if (id.length() < 5) {
         statusLabel->setText("❌ ID must be at least 5 characters long.");
         statusLabel->setStyleSheet("color: red; font-size: 13px;");
         statusLabel->setAlignment(Qt::AlignCenter);
         return;
     }

     // Check again if user already exists
     if (authSystem->userExists(id)) {
         statusLabel->setText("⚠️ You already signed up. Please log in.");
         statusLabel->setStyleSheet("color: red; font-size: 13px;");
         statusLabel->setAlignment(Qt::AlignCenter);
         return;
     }

     // If they don't exist, add them and show success
     authSystem->addUser(id, "User");
     statusLabel->setText("✅ Account created successfully! Please log in.");
     statusLabel->setStyleSheet("color: green; font-size: 13px;");
     statusLabel->setAlignment(Qt::AlignCenter);
 }

 void LoginPage::showLockDialog() {
     QDialog* lockDialog = new QDialog(this);
     lockDialog->setWindowTitle("🔒 Account Locked");
     lockDialog->setModal(true);
     lockDialog->setWindowFlags(lockDialog->windowFlags() & ~Qt::WindowCloseButtonHint); // Disable close

     QVBoxLayout* dialogLayout = new QVBoxLayout(lockDialog);
     QLabel* countdownLabel = new QLabel("🔒 Your account is temporarily locked.\n\nPlease wait 60 seconds before trying again.");
     countdownLabel->setAlignment(Qt::AlignCenter);
     dialogLayout->addWidget(countdownLabel);

     QTimer* countdownTimer = new QTimer(lockDialog);
     int* remainingTime = new int(60); // heap allocated for lambda

     QObject::connect(countdownTimer, &QTimer::timeout, lockDialog, [=]() mutable {
         (*remainingTime)--;
         countdownLabel->setText(QString("🚫 ACCOUNT LOCKED\n\nTime remaining: %1 seconds").arg(*remainingTime));

         if (*remainingTime <= 0) {
             countdownTimer->stop();
             delete remainingTime;
             lockDialog->accept(); // close dialog
         }
         });

     countdownTimer->start(1000); // update every second
     lockDialog->exec();          // block interaction

   
 }
