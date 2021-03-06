# rest
An app designed to remind you of rest and protect your eyesight when you are working.  
It's my first Qt Project.  

![](README/rest.png)  

![](README/rest.gif)

***

## Update Log:

## V0.2.1  
+ Change menu text when the timer is paused  
+ Move the forcelock logic into class MyTimer  
+ Start another round after the screen is unlocked  
+ After force quit, user doesn't need to unlock again  

## V0.2   
+ Use LockWorkStation() rather than CloseScreen()  
(I change the general logic because the app can receive a notification when user unlock the computer. The previous version will call CloseScreen() frequently, which might do harm to the screen)  
+ Redesign UI  
(Use layouts and change the structure of widgets.)  
+ Refactor the class MySettings  
+ Fix some bugs  

## V0.1
+ Add Settings  
(support customized rest time and work time)  
+ Add Simple Mode  
(take less area on the screen)  

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
