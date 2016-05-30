botty: src/main.c src/irc_types.c
	$(CC) $(CFLAGS) $(LDLIBS) $^ -o $@
