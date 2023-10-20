Lesson 01: Welcome
================================================================================

Lesson 02: Prerequisite GDB Knowledge
================================================================================
## compile
```bash
g++ -g demo.cc -o demo
g++ -g2 demo.cc -o demo
g++ -g3 demo.cc -o demo
g++ -ggdb demo.cc -o demo
```

## start
```bash
gdb ./demo
gdb ./demo --silent --tui
```

## basic commands
```bash
start           # pulse at start
run

next            n
continue        c
step            s
finish          fin
break           b
delete          del

list            l
backtrace       bt
info break
info locals
print i
print &i
display i       # won't need to type out print i

shell pwd | wc
```

## control
```bash
enter           # repeat last command
ctrl + L        # clear window
ctrl + p        # previous command
ctrl + n        # next command
tab             # auto complete
```

## text user interface
```bash
layout src
tui disable     # exit tui
focus src       # focus on src window
focus cmd       # focus on cmd window
```

Lesson 03: Using GDB in X Editor - Edit, Compile, Debug Cycle
================================================================================
## edit, compile, debug cycle
* steps
    1. edit source
    2. recompile
    3. run `r` again - updated
* takeaway
    * no need to restart gdb; running `r` will let gdb pick up the new binary

## edit directly in gdb
* set up
    1. export env var EDITOR
        ```bash
        EDITOR=/usr/bin/vim
        export EDITOR
        ```
    2. in gdb
        ```
        edit
        ```
* steps
    * same as before
* takeaway
    * you can edit directly in gdb

## more commands
* `shell pwd | wc` - shell command
* `file demo` - load new symbol table

Lesson 04: How to get help in GDB
================================================================================
```bash
help
help tui
help layout
apropos tui     # everything related to tui
```

Lesson 05: Saving time with conditional breakpoints
================================================================================
## breakpoints
* regex breakpoints
    ```bash
    rbreak intermediate.cc:gl*
    ```

## watchpoints
* write watch
    ```bash
    watch cycles    # locals + args, globals, and dynamics
    info b          # Type: hw watchpoint
    ```
* read watch
    ```bash
    rwatch cycles
    info b          # Type: read watchpoint
    ```

## conditional
* with breakpoints
    ```bash
    b if cycles > 100       # break at current line
    b 15 if cycles > 100    # break at line 15
    ```
* with watchpoints
    ```bash
    watch cycles if cycles > 100
    ```

## side notes
* `info locals` will
    * print out local variables that are not declared yet
    * but must be accessible in current scope

## raw questions
* [x] what can be watched
    * [x] locals & args?
        - yes, but after going out of scope, the watchpoint will be deleted
    * [x] globals?
        - yes
    * [x] dynamic?
        - yes, `watch *a`; but the behavior got really wierd after we delete a
    * [x] state in a class?
        - yes, `watch a.x`; `watch a` is different
* [x] what's the fundamental difference between breakpoints and watchpoints
    * [x] `watch cycles if cycles > 100` and `b if cycles > 100`?
        - the latter breaks on "current" line


Lesson 06: Conditional calls on the call stack
================================================================================
## conditional breakpoints
```bash
# condition on arg
condition 1 mask==0 # breakpoint number 1, argument mask

# condition on caller
condition 1 $_caller_is("SwapBuffers")

# help
help function
```

## more commands
```bash
info args
print /x mask   # hex format
print /x $rdi   # print register
```

## raw questions
* does it switch file when there're multiple tu?
    - yes
* how do i lookup all the functions like `$_caller_is`
    - `help function`
* what if the argument is of some udt, can we conditionally break on that
    - `break 12 if a == {11, 21}`

Lesson 07: Using startup scripts to save our work and create commands
================================================================================
## source
```bash
source gdb.breakpoints
```

## .gdbinit
```bash
# in .gdbinit
source gdb.breakpoints
```

## export history
```bash
set history filename gdb.history # Set the filename in which to record the command history.
set history save on              # Set saving of the history record on exit
```

## user-defined commands
```bash
define reg_cmd
    if $argc < 1
        print "needs exactly 1 argument"
    end
    if $argc == 1
        print "Setting up regular expression"
        print "arg0 is %s", $arg0
        eval "rbreak %s", $arg0
    end
end

document reg_cmd
    some helpful information about our command
end

# then in gdb you could use
reg_cmd "condition:gl*"
```

Lesson 08: Debugging in separate windows and attach to a process
================================================================================
## redirect output
```bash
# in terminal
tty     # print the file name of the terminal connected to standard input
        # /dev/pts/21

# in gdb
tty /dev/pts/21
```

## attach to process
```bash
gdb                 # without binary
attach 63360        # process has stopped!
```

Lesson 09: Pretty Printers in Python with GDB
================================================================================
## @todo

Lesson 10: Multiprocess debugging
================================================================================
## @todo

Lesson 11: Debugging multithreaded programs
================================================================================
## @todo

Lesson 12: Time Travel Debugging in GDB
================================================================================
## record & replay
```bash
record
reverse-next
reverse-step
reverse-continue
record stop
```

## rr
```bash
rr record ./prog args
rr replay # brings up gdb
rr: Saving execution to trace directory `/mnt/harmonix_vol2/soporifix_vol3/user/home/june/.local/share/rr/prog-2'
```

Lesson 13: Ah the Memories
================================================================================
## coredump
```bash
gcore
gdb -c core.87186 ./prog
```

## memory mapping
```
info proc mapping
```

## examine
```bash
x /8bc data     # examine 8 bytes of character data
x /s data
ptype data      # type = char [50]
```

## find
```bash
info proc
info proc mappings
find 0x555555554000, 0x555555558fff, "Hello, world!"    # 0x555555556004
find 0x555555554000, 0x555555558fff, 'H', 'e', 'l'
print (char*) 0x555555556004                            # "Hello, world!"

list
disas
```
