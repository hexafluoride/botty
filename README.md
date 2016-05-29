# botty
Tiny IRC bot written in C using Unix sockets.

# Building
Needs `xbuild` to compile.
Just run

    xbuild 
    botty/bin/Debug/botty

or

    xbuild /p:Configuration=Release
    botty/bin/Release/botty

at the project root.

# Configuration
At this moment, configuration is done via `#define` preprocessor directives. I might implement an actual configuration file in the future(but not the foreseeable future).

# Features
Right now, all it does is connect to an IRC server, join a channel and say "Hello!". It will also respond to ".bots" with "Reporting in! [C]". This project is mainly a learning exercise so I don't see myself implementing a lot of features.
