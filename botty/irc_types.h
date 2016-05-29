#ifndef IRC_TYPES_H_GUARD
#define IRC_TYPES_H_GUARD

typedef struct irc_message
{
	char* prefix;
	int command_id;
	char* command;
	char* params;
} irc_message;

irc_message* parse_message(char* line);
irc_message* create_message(char *prefix, char *command, int command_id, char *params);
void send_pong(int fd, irc_message *ping);
void send_message(int fd, irc_message *msg);
void send_privmsg(int fd, char *target, char *message);
void send_join(int fd, char *channel);
void send_quit(int fd, char *reason);

#endif
