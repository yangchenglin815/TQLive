;可以检测和结束32位程序进程和64位程序进程

;插件作者：pigger
;NSIS中文论坛首发
;www.nsisfans.com

OutFile test_ansi.exe

Section test
    ;插件调用示例
    killer::IsProcessRunning "QQ.exe"
    Pop $R0
    MessageBox MB_OK "是否在运行：$R0"
    
    killer::KillProcess "QQ.exe"
SectionEnd
