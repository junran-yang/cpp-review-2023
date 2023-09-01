# Mini Lab - Declarations & Header Files 
In this lab you will create a header file.

## Preparation
Log in to a Linux machine.

`cd` to the lab directory. There are several C++ files. `main`
calls to the functions `licensingRevenue` (in `licensing.cpp`) and
`salesRevenue` (in `sales.cpp`). Within their implementations, they
attempt to call `sumOf`, which is defined in `sumof.cpp`.

They call the `sumOf` function correctly, but the compiler is not happy because
it cannot see the shape of `sumOf`.

## Lab Part 1
To make life a little easier for you, we have supplied a
`buildcommand` file. This contains (almost) the command that is
needed to compile this program.

Try running `buildcommand`. You will likely see that at least
one of the errors is that it cannot find `licensing.h`. There
is a `licensing.h` file in the current directory though!

Although the file is there, the compiler doesn't know that it
is supposed to look in the current directory for files that
have been `#include`d. Modify the `buildcommand` so that
it can look in the current directory by adding `-I.`. Before the
`*.cpp` is a good place.

Run the command again, and the errors should now be complaining
about `sumOf`. Looks like we forgot to write a `.h` file!

Create a `sumof.h` file, and make other changes as necessary to build and
run this program.

Hints:
  * The `sumOf` function looks scary. We don't ask you to understand it. Just
    create the declaration from it.
  * Don't forget that any .cpp file defining components that have been declared in a corresponding .h file should itself `#include` that `.h` file. According to the BDE standard, the `#include` of the corresponding `.h` file must be the first non-comment line in the `.cpp` file.
  * `g++ *.cpp` compiles all of the files separately and then links them. It's
    convenient. However, don't think that the compiler gets to see more than one
    file at a time!
  * By default the compiler produces an executable called `a.out`. You
    can provide a name with the `-o` option, e.g. `-o myprog`.

## Lab Part 2
Most programmers think of source files. The compiler sees translation units.
You can see what the compiler sees using the `-E` option. This shows
the result of preprocessing.
  * Run `g++ -m32 --std=gnu++14 -isystem /bb/build/Linux-x86_64-32/release/robolibs/stage/opt/bb/include -I. -E main.cpp` 
  * How many lines are there?
  * What did your `#include`'s add to the output?


## Lab Part 3 (optional)
We don't recommend global variables. However, for this lab, you're going
to create one.

  * Add a global variable of type `bool` called `roundDown` that is defined in
`sumof.cpp`, and declared in `sumof.h`. It should start with a value of 
`false`. 
  * Change the implementation of `sumOf` so that if `roundDown` is true it
  rounds down (we're not going to show you the code for this - 
  you can work it out!). However, you should know that C++ is happy converting
  a `double` to an `int`, and in that process will discard the fractional part.
  * From `main` set `roundDown` to true before any other functions are called.
