#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <rhash.h>

#ifdef HAVE_READLINE
#include <readline/readline.h>
#endif

static char *read_command(void)
{
#ifdef HAVE_READLINE
    if (isatty(STDIN_FILENO))
        return readline("rhasher> ");
#endif
    char *line = NULL;
    size_t size = 0;
    if (getline(&line, &size, stdin) < 0) {
        free(line);
        return NULL;
    }
    return line;
}

static void process_command(char *line)
{
    char *command = strtok(line, " \t\r\n");
    char *argument = strtok(NULL, " \t\r\n");
    unsigned algorithm;
    unsigned char digest[64];
    char output[129];

    if (!command)
        return;
    if (!argument) {
        fprintf(stderr, "Missing argument\n");
        return;
    }

    if (!strcasecmp(command, "md5"))
        algorithm = RHASH_MD5;
    else if (!strcasecmp(command, "sha1"))
        algorithm = RHASH_SHA1;
    else if (!strcasecmp(command, "tth"))
        algorithm = RHASH_TTH;
    else {
        fprintf(stderr, "Unknown algorithm: %s\n", command);
        return;
    }

    int result;
    if (argument[0] == '"')
        result = rhash_msg(algorithm, argument + 1, strlen(argument + 1), digest);
    else
        result = rhash_file(algorithm, argument, digest);

    if (result < 0) {
        fprintf(stderr, "Cannot hash: %s\n", argument);
        return;
    }

    int format = isupper((unsigned char)command[0]) ? RHPR_HEX : RHPR_BASE64;
    size_t length = rhash_print_bytes(output, digest,
                                     rhash_get_digest_size(algorithm), format);
    output[length] = '\0';
    puts(output);
}

int main(void)
{
    rhash_library_init();
    char *line;
    while ((line = read_command()) != NULL) {
        process_command(line);
        free(line);
    }
    if (ferror(stdin)) {
        fprintf(stderr, "Cannot read input\n");
        return 1;
    }
    return 0;
}
