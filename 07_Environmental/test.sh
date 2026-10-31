#!/bin/sh
set -eu
program=$1
md5sum=$2
sha1sum=$3
tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' 0
cd "$tmp"

check() {
    printf '%s\n' "$1" | "$program" > actual 2> errors
    printf '%s\n' "$2" > expected
    diff -u expected actual
    test ! -s errors
}

printf 'abc' > data
: > empty
printf 'a\000b\n' > binary

for algorithm in MD5 SHA1; do
    case "$algorithm" in
        MD5) reference=$md5sum ;;
        SHA1) reference=$sha1sum ;;
    esac
    for file in data empty binary; do
        sum=$("$reference" "$file")
        check "$algorithm $file ignored" "${sum%% *}"
    done
    sum=$("$reference" data)
    check "$algorithm \"abc ignored" "${sum%% *}"
    sum=$("$reference" empty)
    check "$algorithm \"" "${sum%% *}"
done

check 'mD5 "abc' 'kAFQmDzST7DWlj99KOF/cg=='
check 'Md5 "abc' '900150983cd24fb0d6963f7d28e17f72'
check 'md5 data' 'kAFQmDzST7DWlj99KOF/cg=='
check 'md5 "' '1B2M2Y8AsgTpgAmY7PhCfg=='
check 'sha1 "abc' 'qZk+NkcGgWq6PiVxeFDCbJzQ2J0='
check 'sha1 data' 'qZk+NkcGgWq6PiVxeFDCbJzQ2J0='
check 'sha1 "' '2jmj7l5rSw0yVb/vlWAYkK/YBwk='

# TTH: эталоны из утилиты rhash (--tth --hex / --base64).
check 'TTH "abc' '0487ca26443f59cfbc780f3ca0ce509c8c352c27c5dcca20'
check 'TTH data' '0487ca26443f59cfbc780f3ca0ce509c8c352c27c5dcca20'
check 'TTH "' '5d9ed00a030e638bdb753a6a24fb900e5a63b8e73e6c25b6'
check 'tth "abc' 'BIfKJkQ/Wc+8eA88oM5QnIw1LCfF3Mog'
check 'tth data' 'BIfKJkQ/Wc+8eA88oM5QnIw1LCfF3Mog'
check 'tth empty' 'XZ7QCgMOY4vbdTpqJPuQDlpjuOc+bCW2'

# После каждой ошибки следующая команда должна выполняться.
for command in 'unknown data' 'MD5' 'SHA1 missing'; do
    printf '%s\n' "$command" 'MD5 "abc' | "$program" > actual 2> errors
    printf '%s\n' '900150983cd24fb0d6963f7d28e17f72' > expected
    diff -u expected actual
    test -s errors
done

# Несколько команд, пустые строки и EOF без завершающего перевода строки.
printf '\n \t\nMD5 "abc\nSHA1 "abc' | "$program" > actual 2> errors
printf '%s\n' '900150983cd24fb0d6963f7d28e17f72' \
    'a9993e364706816aba3e25717850c26c9cd0d89d' > expected
diff -u expected actual
test ! -s errors

"$program" < /dev/null > actual 2> errors
test ! -s actual
test ! -s errors
echo 'All tests passed'
