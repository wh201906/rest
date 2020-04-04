# rest
An app designed to have a rest and protect eyesight when working.  
Developed by Qt. Now available on Windows.

***

Update Log:

## V0.0.3
+ Add single instance support(experimental)
+ Refactor the code  
(Replace EventHandler with MyTimer, which is a customized subclass of QTimer)
+ Add signal interval  
(The app used to send close screen signal per seconds in rest mode, now the frequency is adjustable)  
+ Add the Context Menu  
(Right click to get more actions)
+ Optimize the logic  

## V0.0.2
+ Fix the crash issue  
(Caused by SendMessageA since the system doesn't response the message sometimes. It get fixed after replacing SendMessageA with PostMessageA, which return immediately)  
+ Add some comments  
+ Add the update log to README  

## V0.0.1
+ Initial Version