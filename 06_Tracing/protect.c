#define _GNU_SOURCE
#include <dlfcn.h>
#include <errno.h>
#include <string.h>

int unlink(const char *path)
{
    int (*original_unlink)(const char *);
    const char *name = strrchr(path, '/');

    name = name ? name + 1 : path;
    if (strstr(name, "PROTECT")) {
        errno = EACCES;
        return -1;
    }
    original_unlink = (int (*)(const char *))dlsym(RTLD_NEXT, "unlink");
    if (!original_unlink) {
        errno = ENOSYS;
        return -1;
    }
    return original_unlink(path);
}
