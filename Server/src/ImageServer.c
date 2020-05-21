/*
 *
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * ***** END GPL LICENSE BLOCK *****
 *
 * Contributor(s): Jiri Hnidek <jiri.hnidek@tul.cz>.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <signal.h>
#include <getopt.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <httpserver.h>

static int running = 0;
static int delay = 1;
static int counter = 0;
static char *conf_file_name = NULL;
static char *log_file_name = NULL;
static char *pid_file_name = NULL;
static int pid_fd = -1;
static char *app_name = NULL;
static FILE *log_stream;

/**
 * \brief Read configuration from config file
 */
int ReadConfigFile(int reload)
{
	FILE *conf_file = NULL;
	int ret = -1;

	if (conf_file_name == NULL) return 0;

	conf_file = fopen(conf_file_name, "r");

	if (conf_file == NULL) {
		syslog(LOG_ERR, "Can not open config file: %s, error: %s",
				conf_file_name, strerror(errno));
		return -1;
	}

	ret = fscanf(conf_file, "%d", &delay);

	if (ret > 0) 
	{
		if (reload == 1) 
		{
			syslog(LOG_INFO, "Reloaded configuration file %s of %s", conf_file_name, app_name);
			fprintf(log_stream, "Reloaded configuration file %s of %s \n", conf_file_name, app_name);
		}
		else
		{
			syslog(LOG_INFO, "Configuration of %s read from file %s", app_name, conf_file_name);
			fprintf(log_stream, "Configuration of %s read from file %s \n", app_name, conf_file_name);
		}
	}

	fclose(conf_file);

	return ret;
}

void OpenLogFile()
{
	if (log_file_name != NULL) 
	{
		log_stream = fopen(log_file_name, "a+");
		if (log_stream == NULL) 
		{
			syslog(LOG_ERR, "Can not open log file: %s, error: %s", log_file_name, strerror(errno));
			log_stream = stdout;
		}
	}
	else
	{
		log_stream = stdout;
	}
}

/**
 * \brief Callback function for handling signals.
 * \param	sig	identifier of signal
 */
void HandleSignal(int sig)
{
	if (sig == SIGINT) 
	{
		/* Unlock and close lockfile */
		if (pid_fd != -1) 
		{
			lockf(pid_fd, F_ULOCK, 0);
			close(pid_fd);
		}
		/* Try to delete lockfile */
		if (pid_file_name != NULL) 
		{
			unlink(pid_file_name);
		}
		running = 0;
		stopServer();
		/* Reset signal handling to default behavior */
		signal(SIGINT, SIG_DFL);
	}
	else if (sig == SIGHUP) 
	{
		fprintf(log_stream, "Reloading daemon config file ...\n");
		ReadConfigFile(1);
	}
	else if (sig == SIGCHLD)
	{
		fprintf(log_stream, "Received SIGCHLD signal\n");
	}
}

/**
 * \brief This function will Daemonize this app
 */
static void Daemonize()
{
	pid_t pid = 0;
	int fd;
	/* Fork off the parent process */
	pid = fork();
	/* An error occurred */
	if (pid < 0) 
	{
		exit(EXIT_FAILURE);
	}
	/* Success: Let the parent terminate */
	if (pid > 0) 
	{
		exit(EXIT_SUCCESS);
	}
	/* On success: The child process becomes session leader */
	if (setsid() < 0) 
	{
		exit(EXIT_FAILURE);
	}
	/* Ignore signal sent from child to parent process */
	signal(SIGCHLD, SIG_IGN);
	/* Fork off for the second time*/
	pid = fork();
	/* An error occurred */
	if (pid < 0) 
	{
		exit(EXIT_FAILURE);
	}
	/* Success: Let the parent terminate */
	if (pid > 0) 
	{
		exit(EXIT_SUCCESS);
	}
	/* Set new file permissions */
	umask(0);
	/* Change the working directory to the root directory */
	/* or another appropriated directory */
	chdir("/");
	/* Close all open file descriptors */
	for (fd = sysconf(_SC_OPEN_MAX); fd > 0; fd--) 
	{
		close(fd);
	}
	/* Reopen stdin (fd = 0), stdout (fd = 1), stderr (fd = 2) */
	stdin = fopen("/dev/null", "r");
	stdout = fopen("/dev/null", "w+");
	stderr = fopen("/dev/null", "w+");
	/* Try to write PID of daemon to lockfile */
	if (pid_file_name != NULL)
	{
		char str[256];
		pid_fd = open(pid_file_name, O_RDWR|O_CREAT, 0640);
		if (pid_fd < 0) 
		{
			/* Can't open lockfile */
			exit(EXIT_FAILURE);
		}
		if (lockf(pid_fd, F_TLOCK, 0) < 0) 
		{
			/* Can't lock file */
			exit(EXIT_FAILURE);
		}
		/* Get current PID */
		sprintf(str, "%d\n", getpid());
		/* Write PID to lockfile */
		write(pid_fd, str, strlen(str));
	}
}

/**
 * \brief Print help for this application
 */
void print_help(void)
{
	printf("\n Usage: %s [OPTIONS]\n\n", app_name);
	printf("  Options:\n");
	printf("   -h --help                 Print this help\n");
	printf("   -c --conf_file filename   Read configuration from the file\n");
	printf("   -l --log_file  filename   Write logs to the file\n");
	printf("   -d --daemon               Daemonize this application\n");
	printf("   -p --pid_file  filename   PID file used by Daemonized app\n");
	printf("\n");
}

/* Main function */
int main(int argc, char *argv[])
{
	static struct option long_options[] = {
		{"conf_file", required_argument, 0, 'c'},
		{"log_file", required_argument, 0, 'l'},
		{"help", no_argument, 0, 'h'},
		{"daemon", no_argument, 0, 'd'},
		{"pid_file", required_argument, 0, 'p'},
		{NULL, 0, 0, 0}
	};
	int value, option_index = 0, ret;	
	int start_Daemonized = 0;

	app_name = argv[0];
	/* Try to process all command line arguments */
	while ((value = getopt_long(argc, argv, "c:l:p:d:h", long_options, &option_index)) != -1) {
		switch (value) {
			case 'c':
				conf_file_name = strdup(optarg);
				break;
			case 'l':
				log_file_name = strdup(optarg);
				break;
			case 'p':
				pid_file_name = strdup(optarg);
				break;
			case 'd':				
				start_Daemonized = 1;
				break;
			case 'h':
				print_help();
				return EXIT_SUCCESS;
			default:
				break;
		}
	}
	/* First Open the Image Server Log File*/
	OpenLogFile();
	/* When daemonizing is requested at command line. */
	if (start_Daemonized == 1) 
	{
		Daemonize();
	}

	/* Open system log and write message to it */
	openlog(argv[0], LOG_PID|LOG_CONS, LOG_DAEMON);
	fprintf(log_stream, "Starting Service... %s \n", app_name);
	syslog(LOG_INFO, "Started %s", app_name);

	/* Daemon will handle two signals */
	signal(SIGINT, HandleSignal);
	signal(SIGHUP, HandleSignal);

	/* Read configuration from config file */
	ReadConfigFile(0);

	/* This global variable can be changed in function handling signal */
	running = 1;
	fprintf(log_stream, "Started Service %s \n", app_name);
	startServer(conf_file_name);
	/* Never ending loop of server */
	//while (running == 1) {
		/* Debug print */
	//	fprintf(log_stream, "Debug: %d\n", counter++);
	//	fflush(log_stream);

		/* TODO: dome something useful here */
	//	sleep(delay);
	//}

	fprintf(log_stream, "Stopping... %s \n", app_name);

	/* Free allocated memory */
	if (conf_file_name != NULL) free(conf_file_name);
	if (log_file_name != NULL) free(log_file_name);
	if (pid_file_name != NULL) free(pid_file_name);
	if (log_file_name != NULL) free(log_file_name);

	fprintf(log_stream, "Stoped Service %s \n", app_name);
		/* Close log file, when it is used. */
	if (log_stream != stdout) 
	{
		fclose(log_stream);
	}
	/* Write system log and close it. */
	syslog(LOG_INFO, "Stopped %s", app_name);
	closelog();

	return EXIT_SUCCESS;
}
