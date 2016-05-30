botty/botty: botty/main.c botty/irc_types.c
	$(CC) $(CFLAGS) $(LDLIBS) $^ -o $@
