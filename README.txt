steps to compile code: 

to compile code locally:
use CMakeLists.txt and VisualStudio to run code
(.cpp and .h files are in work/work/test)

Have qt installed and cmake

Qt installation: https://www.qt.io/download-qt-installer 

Note: 
- Also install all the Qt visual tool extensions in visual studio
- When you create the new project remember to choose the cmake option 

When using CMakeLists.txt 

1. right click and click on the Delete Cache and Reconfigure
2. Then click on the dropdown menu beside the "Select Startup Item" which is at the very top 
3. click NoomyPlan.exe
4. click the run icon and it will run

Steps 1 to 4 should work for both windows and mac since these steps are done within Visual Studio


===================================================================================================================================================================
THESE STEPS WERE FOR GAUL BUT SINCE WE ARE USING QCHART, IT WONT COMPILE IN GAUL SO PLEASE IGNORE THESE STEPS SINCE AND USE ABOVE STEPS TO COMPILE LOCALLY
===================================================================================================================================================================

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


step 2a: 

rm moc_budgetpage.cpp moc_budgetpageexpenses.cpp

step 2b:

/usr/lib/qt6/libexec/moc budgetpagebudget.h -o moc_budgetpagebudget.cpp
/usr/lib/qt6/libexec/moc budgetpageexpenses.h -o moc_budgetpageexpenses.cpp


step 3: 

g++ -std=c++17 -I/usr/include/x86_64-linux-gnu/qt6 \
    -I/usr/include/x86_64-linux-gnu/qt6/QtWidgets \
    -I/usr/include/x86_64-linux-gnu/qt6/QtCore \
    -I/usr/include/x86_64-linux-gnu/qt6/QtGui -fPIC \
    -c budgetpagebudget.cpp -o budgetpagebudget.o


step 4:

g++ -std=c++17 -I/usr/include/x86_64-linux-gnu/qt6 \
    -I/usr/include/x86_64-linux-gnu/qt6/QtWidgets \
    -I/usr/include/x86_64-linux-gnu/qt6/QtCore \
    -I/usr/include/x86_64-linux-gnu/qt6/QtGui -fPIC \
    -c moc_budgetpagebudget.cpp -o moc_budgetpagebudget.o


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

    g++ -std=c++17 -c budgetpageexpenses.cpp -o budgetpageexpenses.o \
    -I/usr/include/x86_64-linux-gnu/qt6 \
    -I/usr/include/x86_64-linux-gnu/qt6/QtWidgets \
    -I/usr/include/x86_64-linux-gnu/qt6/QtCore \
    -I/usr/include/x86_64-linux-gnu/qt6/QtGui \
    -fPIC

    step 5h:

    g++ -std=c++17 -c budgetpagebudget.cpp -o budgetpagebudget.o
    -I/usr/include/x86_64-linux-gnu/qt6 \
    -I/usr/include/x86_64-linux-gnu/qt6/QtWidgets \
    -I/usr/include/x86_64-linux-gnu/qt6/QtCore \
    -I/usr/include/x86_64-linux-gnu/qt6/QtGui 
    -fPIC 
    

    step 5j:

    g++ -std=c++17 -c moc_budgetpagebudget.cpp -o moc_budgetpagebudget.o \
    -I/usr/include/x86_64-linux-gnu/qt6 \
    -I/usr/include/x86_64-linux-gnu/qt6/QtWidgets \
    -I/usr/include/x86_64-linux-gnu/qt6/QtCore \
    -I/usr/include/x86_64-linux-gnu/qt6/QtGui \
    -fPIC

    step 5k:

    g++ -std=c++17 -c moc_budgetpageexpenses.cpp -o moc_budgetpageexpenses.o \
    -I/usr/include/x86_64-linux-gnu/qt6 \
    -I/usr/include/x86_64-linux-gnu/qt6/QtWidgets \
    -I/usr/include/x86_64-linux-gnu/qt6/QtCore \
    -I/usr/include/x86_64-linux-gnu/qt6/QtGui \
    -fPIC

    step 5l:

    g++ -std=c++17 -o NoomyPlan \
    main.o user.o authenticatesystem.o loginpage.o financialreport.o cashflowtracking.o budgetpagebudget.o \
    budgetpage.o budgetpageexpenses.o moc_budgetpage.o moc_budgetpageexpenses.o \
    -L/usr/lib/x86_64-linux-gnu \
    -lQt6Widgets -lQt6Core -lQt6Gui -lQt6Network -lQt6Sql -fPIC

   


step 6:

./NoomyPlan

if you get error for above open XLaunch then do step 5k again and then step 6 again and it should run

Note: moc files need to be regenerated since they can be outdated, causing errors
