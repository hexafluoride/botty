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
	char *save_pointer;
	char *part = strtok_r(line, " ", &save_pointer);
	irc_message *msg = (irc_message *)calloc(1, sizeof(irc_message));

	if(part[0] == ':')
	{
		msg->prefix = parse_prefix(part);
		part = strtok_r(NULL, " ", &save_pointer);
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

	char* remainder = strtok_r(NULL, "\r", &save_pointer);

	msg->params = alloc_ptr(remainder);
	strcpy(msg->params, remainder);

	return msg;
}

irc_message* create_message(char *prefix, char *command, int command_id, char *params)
{
	irc_message *msg = (irc_message *)calloc(1, sizeof(irc_message));

	if(prefix != NULL)
	{
		msg->prefix = parse_prefix(prefix);
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

irc_prefix* create_prefix(char *nick, char *user, char *host)
{
	irc_prefix *ret = (irc_prefix *)calloc(1, sizeof(irc_prefix));
	char *buffer = (char *)calloc(513, sizeof(char));

	if(nick != NULL)
	{
		ret->nick = alloc_ptr(nick);
		strcpy(ret->nick, nick);

		strcat(buffer, nick);
		strcat(buffer, "!");
	}
	if(user != NULL)
	{
		ret->user = alloc_ptr(user);
		strcpy(ret->user, user);

		strcat(buffer, user);
		strcat(buffer, "@");
	}

	ret->host = alloc_ptr(host);
	strcpy(ret->host, host);

	strcat(buffer, host);

	ret->raw_prefix = alloc_ptr(buffer);
	strcpy(ret->raw_prefix, buffer);
	free(buffer);

	return ret;
}

irc_prefix* parse_prefix(char *prefix)
{
	irc_prefix *ret;
	char *save_pointer;

	if(strstr(prefix, "!") == NULL)
	{
		ret = create_prefix(NULL, NULL, prefix);
		return ret;
	}

	char *nick = strtok_r(prefix, "!", &save_pointer);
	char *user = strtok_r(NULL, "@", &save_pointer);
	char *host = strtok_r(NULL, "", &save_pointer);

	ret = create_prefix(nick, user, host);

	return ret;
}

void free_irc_prefix(irc_prefix *prefix)
{
	if(prefix == NULL)
		return;

	free(prefix->raw_prefix);
	free(prefix->nick);
	free(prefix->user);
	free(prefix->host);
	free(prefix);
}

void free_irc_message(irc_message *msg)
{
	if(msg == NULL)
		return;

	free_irc_prefix(msg->prefix);
	free(msg->command);
	free(msg->params);
	free(msg);
}

void send_quit(int fd, char *reason)
{
	char *buf = (char *)calloc(513, sizeof(char));

	strcat(buf, ":");
	strcat(buf, reason);

	send_message_and_free(fd, create_message(NULL, "QUIT", 0, buf));

	free(buf);
}

void send_join(int fd, char *channel)
{
	send_message_and_free(fd, create_message(NULL, "JOIN", 0, channel));
}

void send_privmsg(int fd, char *target, char *message)
{
	char *buf = (char *)calloc(513, sizeof(char));

	strcat(buf, target);
	strcat(buf, " :");
	strcat(buf, message);

	send_message_and_free(fd, create_message(NULL, "PRIVMSG", 0, buf));

	free(buf);
}

void send_pong(int fd, irc_message *ping)
{
	send_message_and_free(fd, create_message(NULL, "PONG", 0, ping->params));
}

void send_message(int fd, irc_message *msg)
{
	char *buf = (char *)calloc(513, sizeof(char));

	if(msg->prefix != NULL)
	{
		strcat(buf, msg->prefix->raw_prefix);
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

void send_message_and_free(int fd, irc_message *msg)
{
	send_message(fd, msg);
	free_irc_message(msg);
}