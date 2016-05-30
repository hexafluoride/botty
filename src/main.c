#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include "main.h"
#include "irc_types.h"

// define these before compiling
#define IRC_SERV_ADDR "irc.rizon.io"
#define IRC_SERV_PORT 6667

#define IRC_NICK "topkek_2003"
#define IRC_DEFAULT_JOIN "#topkek_test"

int main (int argc, char *argv[])
{
	struct sockaddr_in server;
	struct hostent *server_name;

	server_name = gethostbyname(IRC_SERV_ADDR);

	memset((char *) &server, '\0', sizeof(server));

	server.sin_family = AF_INET;
	server.sin_port = htons(IRC_SERV_PORT);

	memmove((char *)&server.sin_addr.s_addr, (char *)server_name->h_addr, server_name->h_length);

	int fd = socket(AF_INET, SOCK_STREAM, 0);

	if(fd < 0)
	{
		perror("Can't open socket");
		exit(1);
	}

	if(connect(fd, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		perror("Error while connecting");
		exit(1);
	}

	write_str(fd, "NICK "IRC_NICK"\r\n");
	write_str(fd, "USER botty botty "IRC_SERV_ADDR" :botty\r\n");

	int pid = fork();

	if(pid == 0) // message listener thread
	{
		while(1)
		{
			char *msg = read_str(fd);
			printf("read: %s", msg);

			irc_message *irc_msg = parse_message(msg);
			/*printf("prefix: %s, command_id: %d, command: %s, params: %s\n",
				irc_msg->prefix,
				irc_msg->command_id,
				irc_msg->command,
				irc_msg->params);*/

			if(irc_msg->command != NULL)
			{
				if(strcmp(irc_msg->command, "PING") == 0)
				{
					send_pong(fd, irc_msg);
				}
				if(strcmp(irc_msg->command, "PRIVMSG") == 0)
				{
					char *target = strtok(irc_msg->params, " ");
					char *message = strtok(NULL, "") + 1; //skip the :

					if(strcmp(target, IRC_NICK) == 0)
					{
						target = irc_msg->prefix->nick;
					}

					printf("Received message from %s: %s\n", target, message);

					if(strcmp(message, ".bots") == 0)
					{
						send_privmsg(fd, target, "Reporting in! [C]");
					}
				}
			}

			if(irc_msg->command_id == 376) // end of MOTD
			{
				send_join(fd, IRC_DEFAULT_JOIN);
				send_privmsg(fd, IRC_DEFAULT_JOIN, "Hello!");
			}

			free(msg);
			free_irc_message(irc_msg);
		}
	}
	else
	{
		while(1)
		{
			int len = 100;
			char *buffer = (char *)calloc(len + 1, sizeof(char));
			int read = getline(&buffer, &len, stdin);

			if(read > 0)
			{
				if(strcmp(buffer, "quit") != 0)
				{
					send_quit(fd, "Bye!");
					sleep(1);
					shutdown(fd, SHUT_RDWR);
					close(fd);

					free(buffer);
					kill(pid, SIGKILL);
					break;
				}
			}

			free(buffer);
		}
	}

	return 0;
}

char* alloc_ptr(char *string)
{
	return (char *)calloc(strlen(string) + 1, sizeof(char));
}

int read_single(int fd, char *buf)
{
	return read(fd, buf, 1);
}

char* read_str(int fd)
{
	char *s_buf = (char *)calloc(1, sizeof(char));
	char *t_buf = (char *)calloc(513, sizeof(char));
	int index = 0;

	if(s_buf == NULL || t_buf == NULL)
	{
		exit(1);
	}

	int r = read_single(fd, s_buf);

	while(r > 0)
	{
		memcpy(t_buf + index++, s_buf, 1);

		if(s_buf[0] == '\n')
		{
			free(s_buf);
			return t_buf;
		}

		r = read_single(fd, s_buf);
	}

	if(r <= 0)
	{
		perror("Failed to read");

		free(s_buf);
		free(t_buf);

		exit(1);
	}

	free(s_buf);
	return t_buf;
}

int write_str(int fd, char* str)
{
	printf("wrote: %s", str);
	return write(fd, str, strlen(str));
}

