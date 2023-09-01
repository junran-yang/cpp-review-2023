# Mini Lab - Hello, World!
In this lab you will write and compile a small program. What else could it
be other than 'Hello, World!'. It's a little more complex than you might
be expecting though...

This lab is not intended to teach you any C++. It is intended to be
completed 'inverted', i.e. you will do the work before we have
taught what anything means!

## Your development environment
We don't ask much from your environment. You need:

   * to be logged into a Linux machine,
   * to have a unix text editor (vi, emacs, nedit, nano) that you can use to
     edit files in your home directory
If you have those, then you're good to go!

But wait.... what about an IDE? We won't be using an IDE on this course. None
of our 'projects' are large enough to require it. We're going
to focus on learning the language, not an IDE.

## Where to work
For this course you will be working in a clone of this repository. If
you haven't made a clone, then go ahead and do so now. There should be no
need to fork.

`cd` into the `labs/01_linkers/mini01` directory.

## The code
Create and edit `hello.cpp` and type in the following 
code. We want you to actually type it in so you start getting a feel
for some of the C++.
**DO NOT COPY PASTE**
```
#include <bsl_iostream.h>

int main(int argc, char** argv)
{
    bsl::cout << "Hello, World!" << bsl::endl;

    return 0;
}
```
**DO NOT COPY PASTE**

## Make the executable
Compile the code with the following command (**do** copy paste!):
```
g++ hello.cpp -m32 --std=gnu++14 \
  -isystem /bb/build/Linux-x86_64-32/release/robolibs/stage/opt/bb/include \
  -L /bb/build/Linux-x86_64-32/release/robolibs/stage/opt/bb/lib \
  -lbsl
```

You can copy-paste into your Unix session by first copying the text
in windows (Ctrl-C), and the clicking either the middle button in
an X-Window window, or right button if in a PuTTY window.

Why did we suggest copy-pasting this command, but not the program? A C++
programmer writes code (the `.cpp`), but not compile commands. The commands
are normally managed by a build system of some kind: `cmake`, or the
legacy `plink` in Bloomberg. For this lab, we're going to be
doing it ourselves with `g++`.

On success, you should be left with a file called `a.out` in this directory.
This is an executable, and can be run with `./a.out`.

Errors from the compiler are not always as helpful as they could be - C++
is a complex language. If you have errors, and
can't see an error on the line that the compiler is complaining about,
then try looking at the preceding lines. Always look at the first error
first. Fix the error, and try compiling again.

## What was that command?
Or, you're probably more likely to be thinking what the $%@# was that!.

There's a lot happening in the command. And, whilst you don't need to
learn to write it, it's not a bad idea that we pull it apart.

#### `g++`
This is the compiler from the GNU compiler suite. Invoking it as `g++`
says that we want to use C++ not C (you'd use `gcc` for C).

#### `hello.cpp`
The source file - the only bit of code supplied by you.

#### `-m32`
Compile for 32 bits, even though this is a 64 bit machine. Outside of
Bloomberg you probably wouldn't use it, but the libraries
that we're going to link against are built for 32 bit
executables. If we don't compile for 32 bit, it will give us an error.

*Link*? *Library*? A *library* is an already written, compiled piece of
code. The compiler will glue (*link*) it with our code to produce the
executable.

#### `--std=gnu++14`
This is the C++ standard we're compiling for. Even though we're sticking
to C++03 on this course, the libraries we use were compiled for C++14
and we should do the same.

#### `-isystem /bb/build/Linux-x86_64-32/release/robolibs/stage/opt/bb/include`
In `hello.cpp` there is a `#include <bsl_iostream.h>`. This is a request
to use the contents of that file. Where is that file? `g++` knows for
'standard' files, but not Bloomberg files. This option tells the compiler
that it should also look in
`/bb/build/Linux-x86_64-32/release/robolibs/stage/opt/bb/include` when it
tries to read the `bsl_iostream.h` file.

Why does it need the contents of that file? C++ is a statically typed
language, it needs to be told that `bsl::cout` exists before
it is used, and what we can do with that 'thing'. This `.h` file
describes `bsl::cout`.

There are some similarities between this and Python's `import` but there
are also some fundamental differences which we will return to later.

#### `-lbsl`
We've skipped one option, and will explain it after this one. `-lbsl` should
be read as two parts:
   * `-l` link against the library called...
   * `bsl` (actually it transforms the name into `libbsl.a`)

`-lbsl` says that we want to link against the code that is in the
`bsl` library, but how do we know where to look for it...

#### `-L /bb/build/Linux-x86_64-32/release/robolibs/stage/opt/bb/lib`
Adds the directory to the list of places the compiler will look
for libraries.

## Change the name of the executable
Use the `-o` option to change the name of the executable created, 
e.g. `-o hiya.tsk`. Remember to use the new name to run your program.

The `.tsk` filename extension is a Bloomberg standard for executables.

## Treasure hunt - optional 
Who is listed as the author in the file `bsl_iostream.h`?

Who is the owner (Unix filesystem) of the `bsl` library. Remember
that the name is not directly `bsl`!
