Jni2DDE Library
===============
Access DDE applications in Java, using a native calls (JNI) to call DDE functions

1) This is a quick example of how to access programs through DDE in Java using JNI

2) DDE is primarily a windows technology, which makes this non-portable (I know it goes against Java thinking)

3) However you may be able to use it on i386 Linux through WINE or through the Linux DDE Library or maybe through
   Remote DDE with a windows server (C:\Windows\System32\Ddeshare.exe)
   
4) The project is a Dev-C++ project which uses gcc / gnu tools, so you can build without it
   Along with C standard include dirs, you will have to include the java headers {java sdk}\include and {java sdk}\include\win32
   
5) The .dll and .class binaries are included as getting this working is dependent on the function signature being
   exactly right so that java can look it up, so you can still use it incase you can't build it
   
6) There is a debug dll if you need it, just temporarily rename it to the same name as the original

7) There isn't a server implementation, but it is possible. I also did not implement much in the DDE
   client call back function, but you can change this to do what you require.
   
8) The example gets data from row 1 column 1 to row 2 column 2 in excel, outputs it in char and integer format 
   and lets you know the difference in time between doing 5 queries in 1 connection and 5 seperate connections