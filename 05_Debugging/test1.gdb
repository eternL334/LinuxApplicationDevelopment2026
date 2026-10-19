set pagination off
file ./range
break range.c:30 if i % 5 == 0
commands
    silent
    printf "@@@ start=%d stop=%d step=%d i=%d\n", start, stop, step, i
    continue
end
run 1 12 > /dev/null
