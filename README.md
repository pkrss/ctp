# ctp


== about us ==

see blogs: https://blogs.pkstate.com

=== reference ===

Visual Studio Code如何编写运行C、C++？
https://www.zhihu.com/question/30315894
  must Install：
    C/C++（就是有些教程里的cpptools）
    C/C++ Clang Command Adapter：提供静态检测（Lint）
    Code RunnerInclude Autocomplete：提供头文件补全
    C/C++ Snippets：Snippets即重用代码块
  Option instal：
    Bracket Pair Colorizer：彩虹花括号
    -- One Dark Pro：大概是VS Code安装量最高的主题
    GBKtoUTF8：把GBK编码的文档转换成UTF8编码的

=== windows download package ===

https://code.visualstudio.com/docs/languages/cpp

clang: http://releases.llvm.org/5.0.1/LLVM-5.0.1-win64.exe
download mingw copy into clang path https://sourceforge.net/projects/mingw-w64/files/
https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win64/Personal%20Builds/mingw-builds/7.2.0/threads-posix/seh/x86_64-7.2.0-release-posix-seh-rt_v5-rev0.7z

option Install：
  -- C++ Intellisense
    install gnu global from: https://www.gnu.org/software/global/download.html
    In the workspace folder of your C/C++ project, run 'gtags'. This will generate the GTAGS, GRTAGS, and GPATH files. If you made changes to your code, run 'gtags' again to renew the tag files.
