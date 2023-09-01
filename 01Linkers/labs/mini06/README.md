# Mini Lab - Creating a library
In this lab you will practise creating and linking to a library.

## Structure
We have organized the code into two subdirectories. One
contains the library (`librevenue`), and one contains the code
that will create an executable (`main`).

This is a very simple program, so splitting into a library and a driver
is not essential. However, it is very important for larger projects where
code must be shared. It also helps with testing.

## Top level `CMakeLists.txt`
Make a 'build' directory at the top level (at the same level
as the `librevenue` and `main` directories). `cd` into
it and run `bbcmake ..` as usual. Now do a `make`. 

That was suspiciously fast! It didn't actually build anything!

The top level `CMakeLists.txt` doesn't know about the subdirectories,
so let's add the following to the top level `CMakeLists.txt`:
```
add_subdirectory(librevenue)
add_subdirectory(main)
```

Run `bbcmake ..` from the build directory again. Changes to
the structure of the project will require us to run `bbcmake` again.

Harrumph! Someone hasn't set up the subdirectory cmake files correctly.

## Subdirectory `CMakeLists.txt` files
The `CMakeLists.txt` files aren't quite set up correctly. Both of them
have an incorrect list of source files.

Edit both of them so that instead of `x.cpp` they have a list of the 
`.cpp` files from that directory. Order does not matter, and 
you should not add `.h` files.

`cd` into the `build` directory, run `bbcmake ..` and then build. Both
the library and executable should be successfully.

## Running the executable
To run the executable, you need to find where `bbcmake` put it. You can
find it in a subdirectory of the `build` directory. You have to
find it though!
