# Introduction

RunAnywhere is a tiny command line-style productivity tool on Windows.

## Primary feature

You can call out RunAnywhere by hot key, and input your pre-defined shortcut commands, then it will discover the path of current file from application which you are working on, and pass it to your command to run.

For example, you can define a shortcut command `log`, this command calls TortoiseGit to show git logs of file. After that, you call out RunAnywhere in Visual Studio, input `log` and press enter, TortoiseGit will run and open a window showing git logs of the file openning in Visual Studio.

<img src="https://user-images.githubusercontent.com/1822051/117854241-e2491180-b2bb-11eb-805b-60ded3c0e987.png" width="60%" height="60%">

In explorer window, you can also use `log` command to show git logs of selected file.

<img src="https://user-images.githubusercontent.com/1822051/117854691-51266a80-b2bc-11eb-87fd-1b90301a5867.png" width="60%" height="60%">

RunAnywhere will support more applications in the future.

## Little tools

There are also some little tools that are handy to developers integrated in RunAnywhere, sush as a calculator. As long as you input a math expression in RunAnywhere, it will show result of the expression. By pressing enter, you can copy the result into clipboard.

<img src="https://user-images.githubusercontent.com/1822051/117855720-546e2600-b2bd-11eb-9a2a-9093708f69aa.png" width="60%" height="60%">

Supported by extended syntax, calculator in RunAnywhere also has practical features like converting numbers between different bases, etc.
