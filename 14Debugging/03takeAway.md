Take-away
================================================================================
## Lesson 01: Welcome
* welcome

## Lesson 02: Prerequisite GDB Knowledge
* compile
    * `g++ -g demo.cc -o demo`
    * `-g; -g2; -g3; -ggdb`
* start
    * `gdb ./demo [--silent] [--tui]`
* control
    * enter - repeat last command
    * tab - auto complete
    * `ctrl + L` - clear window
* basic commands
    * `start; run` - run program
    * `next; continue; step; finish;` - navigation
    * `break; delete;` - breakpoints
    * `info locals; info args; print i; display i; list; backtrace` - inspection
    * `info break` - meta data
* text user interface
    * `tui enable; tui disable`
    * `tui layout src; tui layout next`
    * `focus src; focus cmd`

## Lesson 03: Using GDB in X Editor - Edit, Compile, Debug Cycle
* Edit, Compile, Debug Cycle
    * no need to restart gdb; running `r` will let gdb pick up the new binary
    * you can edit directly in gdb
* more commands
    * `shell pwd | wc` - shell command
    * `file demo` - load new symbol table

## Lesson 04: How to get help in GDB
* help commands
    * `help`
    * `help tui`
    * `help tui layout`
    * `apropos tui`

## Lesson 05: Saving time with conditional breakpoints
* breakpoints
    * break
    * rbreak - regex
* watchpoints
    * watch
    * rwatch - read
* conditional
    * `break 15 if cycles > 100`
    * `watch x if cycles > 100`

## Lesson 06: Conditional calls on the call stack
* add condition on breakpoints/watchpoints
    * `condition 3 mask==0`
    * `condition 3 $_caller_is("SwapBuffers")`
* more commands
    * `help function`
    * `print /x mask`
    * `print /x $rdi`

## Lesson 07: Using startup scripts to save our work and create commands
* source file
    * `source gdb.breakpoints`
    * .gdbinit is automatically `source`d
* export history
    ```bash
    set history filename gdb.history # Set the filename in which to record the command history.
    set history save on              # Set saving of the history record on exit
    ```
* user-defined commands

## Lesson 08: Debugging in separate windows and attach to a process
* redirect output
    * `tty /dev/pts/21`
* attach to process
    ```bash
    gdb                 # without binary
    attach 63360        # process has stopped!
    ```

## Lesson 09: Pretty Printers in Python with GDB
* `set print pretty on`
* @todo

## Lesson 10: Multiprocess debugging
* @todo

## Lesson 11: Debugging multithreaded programs
* @todo

## Lesson 12: Time Travel Debugging in GDB
* record in gdb
* rr
    * `rr record`
    * `rr replay`

## Lesson 13: Ah the Memories
* examine
    * `x /8bc data`
    * `x /s data`
* type
    * `ptype data`
* memory mapping
    * `info proc mapping`
* coredump
    * `gcore`

Summary
================================================================================
* debug cycle
    * compile & start
    * no need to restart gdb after recompile
    * source script
    * attach to a running process
    * coredump
* commands
    * control keys
    * help commands
    * tui commands
    * basic commands
    * watchpoint, breakpoint, and condition
    * examine commands
* rr
    * record in gdb
    * record & replay w/ rr
* advanced
    * user-defined commands

Questions
================================================================================
* what can we do with `info`
* how do i finish until a frame?
* what if the argument is of some udt, can we conditionally break on that
