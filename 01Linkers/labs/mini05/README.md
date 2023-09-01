# Mini Lab - Building
In this lab you will practise using `cmake` and, on the 
way, see some underlying `make`.

## `make`
### Building with `make`
`cd` into the `make` directory. In here you will find a solution to the
previous lab.

There is also a file called `Makefile`. This controls building
our project.

Run `make` (with no arguments - it automatically looks for
the `Makefile` file for instructions). It should perform four
compilations, and perform a link.

`revenue.tsk` has been created, go ahead and run it. This was
created by the link stage.

There are also four `.o` files. These were created for each compilation.

### `Makefile` contents
Open `Makefile` and view its contents. We're not going to teach
you `make`, but it is helpful have a basic understanding.

The lines at the start:
```
CC=/opt/bb/bin/g++
COMPILE_OPTS=-Wall -Werror -m32 --std=gnu++14 -isystem /bb/build/Linux-x86_64-32/release/robolibs/stage/opt/bb/include -I.
LINK_OPTS=-m32 -L /bb/build/Linux-x86_64-32/release/robolibs/stage/opt/bb/lib -lbsl -lbde
```
set up some variables for later use. You should recognize the options
from earlier.

Following that, there is this *rule*:
```
revenue.tsk: licensing.o sales.o sumof.o main.o
	${CC} -o revenue.tsk main.o licensing.o sales.o sumof.o ${LINK_OPTS}
```
The first line says that `revenue.tsk` will need to be (re)created if:
   * it doesn't exist
   * or one of the dependencies (the `.o` files) is more recent (has a later timestamp).

Its position is important because the first rule provides the default
*target* - what will `make` try to build if you provide no arguments.
However, the position might be misleading, because before `revenue.tsk`
can be created we need to find out if any of the `.o`s need to
be created.

There is a rule for each `.o` file. Apart from deciding which is
the default target the order of the rules is not important. `make`
builds a graph of dependencies.

Each target has its own list of dependencies. This is a simple
project, but in a more complex project, the dependencies may contain
many `.h` files.

### `make` in operation
Run `make` again. You should see:
```
make: 'revenue.tsk' is up to date.
```
`make` avoided repeating work.

Run `touch main.o`. This updates the time stamp for `main.o`. It will
appear to be more recent than `revenue.tsk`.

Run `make`. This time, `make` only needed to relink.

Run `touch sales.h`. There are two targets that have `sales.h`
as a dependency: `main.o` and `sales.o`. Before running make, how
many things do you think it will do?

Run `make`. It performs two compilations and, because at least one of
the `.o` files was updated, `revenue.tsk` has to be linked again.

### Writing your own `make` files
You don't. Maintaining a make file is tedious and error-prone. It also
becomes much more complex if you need it to work for different
architectures.

Why did we show it to you? So that you can know enough to
forget about it!

## `cmake`
`cd` into the `cmake` directory. This has the same starting code
as the last exercise, but the directory structure is different.

The source code has moved into a directory marked `src`. There
is nothing special about this name. It could be anything, but
this is the convention that we will be following for this course. In
larger projects, it's likely to be named after the part of the
project that is within that directory.

We will be creating two `CMakeLists.txt`. Create the first one
at the 'top' level, i.e. the same level as `src` (not within it).
Copy the following text into the file:
```
cmake_minimum_required(VERSION 3.12)
project(revenue)

find_package(ConfigureBbTarget REQUIRED)
find_package(CompilerHelpers REQUIRED)
configure_compile_options( 
  COMPILER_ID GNU
  LANGUAGES CXX
  OPTIONS -Wall -Werror
)

add_subdirectory(src)
```

Now create `src/CMakeLists.txt` and add the following contents:
```
add_executable(revenue.tsk main.cpp sales.cpp licensing.cpp sumof.cpp)
target_link_libraries(revenue.tsk PRIVATE bsl)
configure_bb_target(revenue.tsk V2 HEADER_DIRS .)
```

Create a directory called 'build' at the same level as `src`, and the
first `CMakeLists.txt` file. `cd` into the build directory and
run:
```
bbcmake ..
```
This should create many files in the `build` directory.
One of them you will recognize: `Makefile`.

You should now be able to build the program using `make` once more.

So it's built. How do you run it? From within the 'build' directory, run
`src/revenue.tsk`.

## `cmake` explanation
Have a look at the `Makefile` that `bbcmake` created. This is substantially
more complex than the one we had earlier. It's `cmake`'s job to
create and maintain this. `bbcmake` is a wrapper around `cmake` that
knows how we build software at Bloomberg.

`bbcmake` is run by developers to generate the Makefile, e.g. when they clone a 
repo or when structural changes are made to a project. To build the code,
they use `make` as before. `make` is run much more frequently than `bbcmake`.

We will explain the contents of the `CMakeLists.txt` files here, but
it is not our intention to teach `cmake` properly, just to give you
enough information so that you are comfortable with it.

### Top level `CMakeLists.txt`
The top level `CMakeLists.txt` file controls settings for the whole project.
```
cmake_minimum_required(VERSION 3.12)
project(revenue)
```
The above configuration provides meta information for the whole project.

```
find_package(ConfigureBbTarget REQUIRED)
find_package(CompilerHelpers REQUIRED)
configure_compile_options( 
  COMPILER_ID GNU
  LANGUAGES CXX
  OPTIONS -Wall -Werror
)
```
The above lines enable Bloomberg specific helpers, and configure default 
builds to use `-Wall -Werror`. In 'real life' we would need
to consider 'build types' which are different build configurations.
To keep life simple for ourselves, we're just going to work with
the default build type.

```
add_subdirectory(src)
```
This tells `cmake` to also look for a `CMakeLists.txt` in the 
`src` directory. Again, there is nothing special about the
name `src`. If your project creates multiple libraries, then they
might each have their own directory, named for that part of the 
project.

When we run `make` from the build directory, it placed the executable
under `src`. That may seem a little weird. Why would the executable
appear in a directory called `src`? Because `cmake` creates
directories that match the structure of the project.

### Subdirectory `CMakeLists.txt`
```
add_executable(revenue.tsk main.cpp sales.cpp licensing.cpp sumof.cpp)
```
Sets up a *target* to build - `revenue.tsk`. The `.cpp` files are
listed and `cmake` will automatically deduce their dependencies.

```
target_link_libraries(revenue.tsk PRIVATE bsl)
```
States that `revenue.tsk` will need to be linked against the `bsl`
library. Here we link against a single library, `bsl`. If you
need to link against multiple libraries, then use a
**space separated** list (not comma separated).

```
configure_bb_target(revenue.tsk V2 HEADER_DIRS .)
```
States that `revenue.tsk` is a Bloomberg project, and adds
'.' as a place to look for header files. More details,
such as what the `V2` means can be found in the 
[Bloomberg cmake documentation](http://tutti.prod.bloomberg.com/cmake-docs/reference/cmake-configure-bb-target/README). 
You don't need to know at the moment though!

In a larger project, one with multiple subdirectories, you would
create a separate `CMakeLists.txt` file for each subdirectory.

## metamkmk (Legacy) - Optional
`metamkmk` and `plink` are now considered legacy approaches to
building within Bloomberg.

Within the `plink`  directory, run `metamkmk -t other revenue.mk`. This
creates a new makefile, but it is called `revenue.mk`. 

`metamkmk` performs a similar job to `bbcmake`. It creates a makefile
that is suitable for `plink` rather than one suited for the general `make`.


## plink (Legacy) - Optional
To build, run `plink revenue.mk`.

There will be some very lengthy messages that show how the compiler is
being run. You can normally ignore these.

What is the name of the executable? Does it run?

`plink` sends the build job to a build machine. Currently it is building
for Linux, but it can build for another architecture.

The supported architectures are:
  * sundev1
  * ibm
  * linux

Try compiling for a different architecture by using the `-a` flag, e.g.
`plink -a sundev1 revenue.mk`.

An important difference between `bbcmake` and `plink` is that `plink`
expects to do an in-source build, i.e. in the same directory as the 
source files. That can leave you with a 'messy' source directory. `bbcmake`
expects to be out-of-source, hence you need to create a build directory. If
you try to run `bbcmake` in the source directory, it will refuse to run.

Another important difference is that `cmake` is much better at dealing
with projects that are split into multiple parts. `plink` expects 
to only produce one target, e.g. just a library or an executable. This is
a major limitation of `plink` when writing testable code.
