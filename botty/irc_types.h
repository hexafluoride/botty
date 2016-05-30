#ifndef IRC_TYPES_H_GUARD
#define IRC_TYPES_H_GUARD

typedef struct irc_prefix
{
	char *raw_prefix;
	char *nick;
	char *user;
	char *host;
} irc_prefix;

typedef struct irc_message
{
	irc_prefix *prefix;
	int command_id;
	char *command;
	char *params;
} irc_message;

irc_message* parse_message(char *line);
irc_message* create_message(char *prefix, char *command, int command_id, char *params);

irc_prefix* parse_prefix(char *prefix);
irc_prefix* create_prefix(char *nick, char *user, char *host);

void free_irc_message(irc_message *message);
void free_irc_prefix(irc_prefix *prefix);

void send_pong(int fd, irc_message *ping);
void send_message(int fd, irc_message *msg);
void send_message_and_free(int fd, irc_message *msg);
void send_privmsg(int fd, char *target, char *message);
void send_join(int fd, char *channel);
void send_quit(int fd, char *reason);

#endif
