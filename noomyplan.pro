QT       += core widgets gui
TARGET = noomyplan
SOURCES += main.cpp \
    BudgetPage.cpp \
    expense.cpp\
    expense.cpp\
    financialreport.cpp\
 	loginpage.cpp\
	user.cpp\
	cashflowtracking.cpp\
	authenticatesystem.cpp\
HEADERS += BudgetPage.h \
    expense.h\
	financialreport.h\
	loginpage.h\
	user.h\
	cashflowtracking.h\
	authenticatesystem.h\


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
