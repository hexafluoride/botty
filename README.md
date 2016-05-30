# botty
Tiny IRC bot written in C using Unix sockets.

## Building
Needs `mdtool` to compile.
Just run

    mdtool build 
    botty/bin/Debug/botty

or

    mdtool build -c:Release
    botty/bin/Release/botty

at the project root.

## Configuration
At this moment, configuration is done via `#define` preprocessor directives. I might implement an actual configuration file in the future(but not the foreseeable future).

## Features
Right now, all it does is connect to an IRC server, join a channel and say "Hello!". It will also respond to ".bots" with "Reporting in! [C]". This project is mainly a learning exercise so I don't see myself implementing a lot of features.

Oh, it also has no memory leaks, _at all_. (as of commit [85708a](https://github.com/hexafluoride/botty/commit/85708a06999efa7427870106546da2a2e94ff251))

    ==30443== HEAP SUMMARY:
    ==30443==     in use at exit: 0 bytes in 0 blocks
    ==30443==   total heap usage: 613 allocs, 613 frees, 110,733 bytes allocated
    ==30443== 
    ==30443== All heap blocks were freed -- no leaks are possible
    ==30443== 
    ==30443== For counts of detected and suppressed errors, rerun with: -v
    ==30443== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
    ==30441== 
    ==30441== HEAP SUMMARY:
    ==30441==     in use at exit: 0 bytes in 0 blocks
    ==30441==   total heap usage: 80 allocs, 80 frees, 35,174 bytes allocated
    ==30441== 
    ==30441== All heap blocks were freed -- no leaks are possible
    ==30441== 
    ==30441== For counts of detected and suppressed errors, rerun with: -v
    ==30441== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)


## Known issues
* Quit message doesn't work for some reason (apparently, this is server behavior. unregistered clients or clients that have only recently joined have their quit message overriden "to prevent spam" on Rizon. I can't see how that prevents spam, but eh. NickServ support coming soon. _/rant_)
