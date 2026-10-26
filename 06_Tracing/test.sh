#!/bin/sh
set -eu
root=$(pwd)
tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT HUP INT TERM
cd "$tmp"
printf 'hello\n' > expected
dd if=/dev/zero bs=4096 count=3 >> expected 2>/dev/null

status() {
    want=$1
    shift
    got=0
    "$@" >stdout 2>stderr || got=$?
    if [ "$got" -ne "$want" ]; then
        cat stderr >&2
        echo "Expected exit $want, got $got: $*" >&2
        exit 1
    fi
}

reset_files() {
    cp expected in
    printf 'old output\n' > out
}

inject() {
    call=$1 path=$2 want=$3 remains=$4
    reset_files
    status "$want" strace -qq -o trace -P "$tmp/$path" \
        -e "inject=$call:error=EIO:when=1" "$root/move" "$tmp/in" "$tmp/out"
    grep -q INJECTED trace
    cmp expected in
    if [ "$remains" = yes ]; then
        test -f out
    else
        test ! -e out
    fi
    echo "OK: $call ($path), exit $want"
}

status 1 "$root/move"
status 2 "$root/move" missing out
reset_files
status 0 "$root/move" in out
test ! -e in
cmp expected out
touch in
status 0 "$root/move" in out
test ! -s out
test ! -e in
reset_files
status 6 "$root/move" in in
cmp expected in
ln in alias
status 6 "$root/move" in alias
cmp expected in
rm alias
echo 'OK: normal move, empty file, arguments, same inode'

# libc может реализовывать fstat через fstat или newfstatat.
reset_files
strace -qq -o trace -P "$tmp/in" -e trace=fstat,newfstatat \
    "$root/move" "$tmp/in" "$tmp/out"
stat_call=$(sed -n 's/^\([a-z0-9_]*\)(.*/\1/p' trace | head -n 1)
test -n "$stat_call"

inject openat in 2 yes
inject "$stat_call" in 3 yes
inject openat out 4 yes
inject "$stat_call" out 5 yes
inject ftruncate out 7 no
inject read in 8 no
inject write out 9 no
inject close out 10 no
inject close in 11 no
inject unlink in 12 no

# Ошибки при уборке после неудачного копирования.
for extra in 'close out 13' 'close in 14' 'unlink out 15'; do
    set -- $extra
    reset_files
    status "$3" strace -qq -o trace -P "$tmp/in" -P "$tmp/out" \
        -e inject=read:error=EIO:when=1 \
        -e "inject=$1:error=EIO:when=$(if [ "$2" = in ]; then echo 2; else echo 1; fi)" \
        "$root/move" "$tmp/in" "$tmp/out"
    cmp expected in
    if [ "$3" = 15 ]; then test -f out; else test ! -e out; fi
    echo "OK: cleanup $extra"
done

cp expected file_PROTECT
status 12 env LD_PRELOAD="$root/libprotect.so" "$root/move" file_PROTECT out
cmp expected file_PROTECT
test ! -e out
reset_files
status 0 env LD_PRELOAD="$root/libprotect.so" "$root/move" in out
test ! -e in
cmp expected out
echo 'OK: LD_PRELOAD'

# Проверяем также сам shell-сценарий с локальными путями.
sed -e "s|@BINDIR@|$root|g" -e "s|@LIBDIR@|$root|g" "$root/safemove.in" > safemove
status 12 sh ./safemove file_PROTECT out
cmp expected file_PROTECT
test ! -e out
echo 'All tests passed'
