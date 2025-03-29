QT       += core widgets gui charts

SOURCES += main.cpp \
    BudgetPage.cpp \
    budgetpageexpenses.cpp \
    budgetpagebudget.cpp \
    financialreport.cpp \
    loginpage.cpp \
    cashflowtracking.cpp \
    authenticatesystem.cpp \
    dashboard.cpp \
    user.cpp

    

HEADERS += financialreport.h \
    loginpage.h \
    user.h \
    cashflowtracking.h \
    authenticatesystem.h \
    budgetpage.h \
    budgetpagebudget.h \
    budgetpageexpenses.h \
    dashboard.h \
    inventory.h


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
