#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include "main.h"
#include "irc_types.h"

irc_message* parse_message(char *line)
{
	char *part = strtok(line, " ");
	irc_message *msg = (irc_message *)calloc(1, sizeof(irc_message));

	if(part[0] == ':')
	{
		msg->prefix = alloc_ptr(part);
		strcpy(msg->prefix, part);
		part = strtok(NULL, " ");
	}

	uintmax_t num = strtoumax(part, NULL, 10);
	if(num != 0)
	{
		msg->command_id = num;
	}
	else
	{
		msg->command = alloc_ptr(part);
		strcpy(msg->command, part);
	}

	char* remainder = strtok(NULL, "\r");

	msg->params = alloc_ptr(remainder);
	strcpy(msg->params, remainder);

	return msg;
}

irc_message* create_message(char *prefix, char *command, int command_id, char *params)
{
	irc_message *msg = (irc_message *)calloc(1, sizeof(irc_message));

	if(prefix != NULL)
	{
		msg->prefix = alloc_ptr(prefix);
		strcpy(msg->prefix, prefix);
	}

	if(command != NULL)
	{
		msg->command = alloc_ptr(command);
		strcpy(msg->command, command);
	}

	if(command_id > 0)
	{
		msg->command_id = command_id;
	}

	if(params != NULL)
	{
		msg->params = alloc_ptr(params);
		strcpy(msg->params, params);
	}

	return msg;
}

void send_quit(int fd, char *reason)
{
	char *buf = (char *)calloc(513, sizeof(char));

	strcat(buf, ":");
	strcat(buf, reason);

	send_message(fd, create_message(NULL, "QUIT", 0, buf));

	free(buf);
}

void send_join(int fd, char *channel)
{
	send_message(fd, create_message(NULL, "JOIN", 0, channel));
}

void send_privmsg(int fd, char *target, char *message)
{
	char *buf = (char *)calloc(513, sizeof(char));

	strcat(buf, target);
	strcat(buf, " :");
	strcat(buf, message);

	send_message(fd, create_message(NULL, "PRIVMSG", 0, buf));

	free(buf);
}

void send_pong(int fd, irc_message *ping)
{
	send_message(fd, create_message(NULL, "PONG", 0, ping->params));
}

void send_message(int fd, irc_message *msg)
{
	char *buf = (char *)calloc(513, sizeof(char));

	if(msg->prefix != NULL)
	{
		strcat(buf, msg->prefix);
		strcat(buf, " ");
	}

	if(msg->command != NULL)
	{
		strcat(buf, msg->command);
		strcat(buf, " ");
	}
	else
	{
		char *command = (char *)calloc(4, sizeof(char));
		snprintf(command, 3, "%03d", msg->command_id);

		strcat(buf, command);
		strcat(buf, " ");
		free(command);
	}

	if(msg->params != NULL)
	{
		strcat(buf, msg->params);
	}

	strcat(buf, "\r\n");

	write_str(fd, buf);

	free(buf);
}