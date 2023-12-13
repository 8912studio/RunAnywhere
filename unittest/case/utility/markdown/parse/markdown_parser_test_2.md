Here's how you can do it:

1. Include the necessary header file at the top of your source file:
```cpp
#include <QDebug>
```

2. Use the qDebug() function to print the debug information:
```cpp
qDebug() << "Debug information";
```

3. You can also include variables or values in the debug output:
```cpp
int value = 42;
qDebug() << "The value is:" << value;
```

4. To build and run the application with debug output.

5. When you run the application,

Note: qDebug() statements are only displayed in the console when the application is running in debug mode.

Here's an example of using qInfo() for release mode:
```cpp
qInfo() << "Information message";
```

Additionally, you can also redirect the debug output to a file