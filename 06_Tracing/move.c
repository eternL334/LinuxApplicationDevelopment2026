#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

static int error(const char *message, int code)
{
    perror(message);
    return code;
}

int main(int argc, char **argv)
{
    int in, out = -1, code = 0, remove_out = 0;
    struct stat src, dst;
    char buffer[4096];
    ssize_t n;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s infile outfile\n", argv[0]);
        return 1;
    }
    in = open(argv[1], O_RDONLY);
    if (in < 0)
        return error("open infile", 2);
    if (fstat(in, &src) < 0) {
        code = error("stat infile", 3);
        goto cleanup;
    }
    out = open(argv[2], O_WRONLY | O_CREAT, 0666);
    if (out < 0) {
        code = error("open outfile", 4);
        goto cleanup;
    }
    if (fstat(out, &dst) < 0) {
        /* Пока не проверены inode, удалять outfile опасно. */
        code = error("stat outfile", 5);
        goto cleanup;
    }
    if (src.st_dev == dst.st_dev && src.st_ino == dst.st_ino) {
        fprintf(stderr, "infile and outfile are the same file\n");
        code = 6;
        goto cleanup;
    }
    remove_out = 1;
    if (ftruncate(out, 0) < 0) {
        code = error("truncate outfile", 7);
        goto cleanup;
    }
    while ((n = read(in, buffer, sizeof(buffer))) != 0) {
        if (n < 0) {
            code = error("read infile", 8);
            goto cleanup;
        }
        for (ssize_t pos = 0; pos < n;) {
            ssize_t written = write(out, buffer + pos, n - pos);
            if (written <= 0) {
                if (written == 0)
                    errno = EIO;
                code = error("write outfile", 9);
                goto cleanup;
            }
            pos += written;
        }
    }
    if (close(out) < 0)
        code = error("close outfile", 10);
    out = -1;
    if (code)
        goto cleanup;
    if (close(in) < 0)
        code = error("close infile", 11);
    in = -1;
    if (code)
        goto cleanup;
    if (unlink(argv[1]) < 0) {
        code = error("unlink infile", 12);
        goto cleanup;
    }
    return 0;

cleanup:
    if (out >= 0 && close(out) < 0)
        code = error("cleanup: close outfile", 13);
    if (in >= 0 && close(in) < 0)
        code = error("cleanup: close infile", 14);
    if (remove_out && unlink(argv[2]) < 0)
        code = error("cleanup: unlink outfile", 15);
    return code;
}
