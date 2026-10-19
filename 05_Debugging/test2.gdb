set pagination off
file ./range
set $n = 0
break range.c:30
commands
    silent
    set $n = $n + 1
    if $n >= 28 && $n <= 35
        printf "@@@ start=%d stop=%d step=%d i=%d\n", start, stop, step, i
    end
    continue
end
run -100 100 3 > /dev/null
