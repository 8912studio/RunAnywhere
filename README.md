# Introduction

RunAnywhere is a command line-style productivity tool on Windows, mainly aims to developers.  It helps you to simplify the procedure to solve frequent and trivial works by:

- Delivering the file you currently working on to the tool you want to invoke by a simple command.
- Providing several utility tools to accomplish common tasks.

## Sample Usage Scenario

Considering that you mainly use Visual Studio and TortoiseGit in your daily development tasks. If you want to view history change logs of the the opening source file in Visual Studio, you may need to take steps like below:

1. Right click the tab of file, and select “Open Containing Folder” to open an explorer window.
2. Right click the file, and select “TortoiseGit” - “Show log”.

Not many steps though, it is still a bothering task because it depends mouse operation heavily and you need to accomplish it very often.

Things get easier with RunAnywhere. To accomplish the same task (suppose that you have imported the corresponding bundle file), you just need to:

1. Press a hot key such as CTRL + SPACE to call out command window.
2. Type “log” and press ENTER.

All is done by several simple key strokes.

<img src="https://user-images.githubusercontent.com/1822051/223924418-54d04ddd-ebc8-4892-bcd2-1f2443ed3ab0.png" width="80%" height="80%">

# Requirements
RunAnywhere requires x64 Windows 10 and above to run.

In order to discover the file from application you are currently working on, plugins or extensions (included in RunAnywhere installer) are needed to be installed to the application. For now, the number of supported applications is limited, but more will be supported in the future. For a full list of supported applications, see [here](https://github.com/Zplutor/RunAnywhere/wiki/Active-Path#supported-applications).

# Documentation

Visit the [wiki home page](https://github.com/Zplutor/RunAnywhere/wiki) to learn what feature RunAnywhere has and how to use it better.
