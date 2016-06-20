CFLAGS = $(shell pkg-config --cflags openssl)
LDLIBS = $(shell pkg-config --libs openssl)

botty/botty: botty/main.c botty/irc_types.c
	$(CC) $(CFLAGS) $(LDLIBS) $^ -o $@
