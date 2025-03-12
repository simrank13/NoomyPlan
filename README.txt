steps to compile code: 

The following steps will work on putty (and if using VcXsrv server to run code using X server)

To run on putty:
1. Install VcXsrv
2. Open putty and enable X11 by doing SSH (on left menu) > + symbol beside enable X11 forwarding 
3. host name: <uwo_id>@cs3307.gaul.csd.uwo.ca 
4. click Open
5. navigate to folder with code for example: cd 3307/stage3
6. complete following steps

step 1:

find /usr -name "moc"


step 2: 

rm moc_budgetpage.cpp moc_expenses.cpp
/usr/lib/qt6/libexec/moc budgetpage.h -o moc_budgetpage.cpp
/usr/lib/qt6/libexec/moc expenses.h -o moc_expenses.cpp


step 3: 

g++ -std=c++17 -I/usr/include/x86_64-linux-gnu/qt6 \
    -I/usr/include/x86_64-linux-gnu/qt6/QtWidgets \
    -I/usr/include/x86_64-linux-gnu/qt6/QtCore \
    -I/usr/include/x86_64-linux-gnu/qt6/QtGui -fPIC \
    -c budgetpage.cpp -o budgetpage.o


step 4:

g++ -std=c++17 -I/usr/include/x86_64-linux-gnu/qt6 \
    -I/usr/include/x86_64-linux-gnu/qt6/QtWidgets \
    -I/usr/include/x86_64-linux-gnu/qt6/QtCore \
    -I/usr/include/x86_64-linux-gnu/qt6/QtGui -fPIC \
    -c moc_budgetpage.cpp -o moc_budgetpage.o


step 5:

    step 5a:

    g++ -std=c++17 -c main.cpp -o main.o \
    -I/usr/include/x86_64-linux-gnu/qt6 \
    -I/usr/include/x86_64-linux-gnu/qt6/QtWidgets \
    -I/usr/include/x86_64-linux-gnu/qt6/QtCore \
    -I/usr/include/x86_64-linux-gnu/qt6/QtGui \
    -fPIC

    step 5b: 

    g++ -std=c++17 -c user.cpp -o user.o \
    -I/usr/include/x86_64-linux-gnu/qt6 \
    -I/usr/include/x86_64-linux-gnu/qt6/QtWidgets \
    -I/usr/include/x86_64-linux-gnu/qt6/QtCore \
    -I/usr/include/x86_64-linux-gnu/qt6/QtGui \
    -fPIC

    step 5c:

    g++ -std=c++17 -c authenticatesystem.cpp -o authenticatesystem.o \
    -I/usr/include/x86_64-linux-gnu/qt6 \
    -I/usr/include/x86_64-linux-gnu/qt6/QtWidgets \
    -I/usr/include/x86_64-linux-gnu/qt6/QtCore \
    -I/usr/include/x86_64-linux-gnu/qt6/QtGui \
    -fPIC

    step 5d:

    g++ -std=c++17 -c loginpage.cpp -o loginpage.o \
    -I/usr/include/x86_64-linux-gnu/qt6 \
    -I/usr/include/x86_64-linux-gnu/qt6/QtWidgets \
    -I/usr/include/x86_64-linux-gnu/qt6/QtCore \
    -I/usr/include/x86_64-linux-gnu/qt6/QtGui \
    -fPIC

    step 5e:

    g++ -std=c++17 -c financialreport.cpp -o financialreport.o \
    -I/usr/include/x86_64-linux-gnu/qt6 \
    -I/usr/include/x86_64-linux-gnu/qt6/QtWidgets \
    -I/usr/include/x86_64-linux-gnu/qt6/QtCore \
    -I/usr/include/x86_64-linux-gnu/qt6/QtGui \
    -fPIC

    step 5f:

    g++ -std=c++17 -c cashflowtracking.cpp -o cashflowtracking.o \
    -I/usr/include/x86_64-linux-gnu/qt6 \
    -I/usr/include/x86_64-linux-gnu/qt6/QtWidgets \
    -I/usr/include/x86_64-linux-gnu/qt6/QtCore \
    -I/usr/include/x86_64-linux-gnu/qt6/QtGui \
    -fPIC

    step 5g:

    g++ -std=c++17 -c expenses.cpp -o expenses.o \
    -I/usr/include/x86_64-linux-gnu/qt6 \
    -I/usr/include/x86_64-linux-gnu/qt6/QtWidgets \
    -I/usr/include/x86_64-linux-gnu/qt6/QtCore \
    -I/usr/include/x86_64-linux-gnu/qt6/QtGui \
    -fPIC

    step 5h:

    g++ -std=c++17 -c budgetpage.cpp -o budgetpage.o \
    -I/usr/include/x86_64-linux-gnu/qt6 \
    -I/usr/include/x86_64-linux-gnu/qt6/QtWidgets \
    -I/usr/include/x86_64-linux-gnu/qt6/QtCore \
    -I/usr/include/x86_64-linux-gnu/qt6/QtGui \
    -fPIC

    step 5i:

    g++ -std=c++17 -c moc_budgetpage.cpp -o moc_budgetpage.o \
    -I/usr/include/x86_64-linux-gnu/qt6 \
    -I/usr/include/x86_64-linux-gnu/qt6/QtWidgets \
    -I/usr/include/x86_64-linux-gnu/qt6/QtCore \
    -I/usr/include/x86_64-linux-gnu/qt6/QtGui \
    -fPIC

    step 5j:

    g++ -std=c++17 -c moc_expenses.cpp -o moc_expenses.o \
    -I/usr/include/x86_64-linux-gnu/qt6 \
    -I/usr/include/x86_64-linux-gnu/qt6/QtWidgets \
    -I/usr/include/x86_64-linux-gnu/qt6/QtCore \
    -I/usr/include/x86_64-linux-gnu/qt6/QtGui \
    -fPIC

    step 5k:

    g++ -std=c++17 -o NoomyPlan \
    main.o user.o authenticatesystem.o loginpage.o financialreport.o cashflowtracking.o \
    budgetpage.o expenses.o moc_budgetpage.o moc_expenses.o \
    -L/usr/lib/x86_64-linux-gnu \
    -lQt6Widgets -lQt6Core -lQt6Gui -lQt6Network -lQt6Sql -fPIC

   


step 6:

./NoomyPlan

if you get error for above open XLaunch then do step 5k again and then step 6 again and it should run

Note: moc files need to be regenerated since they can be outdated, causing errors
