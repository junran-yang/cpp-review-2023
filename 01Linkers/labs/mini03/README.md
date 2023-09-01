# Mini Lab - What's in a .o file? 
In this lab you will learn to use the tool `nm`.

## Preparation
Log in to one of the Linux machines used for the previous lab.

Run the `preparedir` script. Six `.o` files should have appeared.

## Your Mission
You must create a link line that creates a working program.

Your link line will be of the form `g++ -m32 -L /bb/build/Linux-x86_64-32/release/robolibs/stage/opt/bb/lib -o prog main.o ... -lbsl`. You have
to decide which `.o` files to add to the command (where the ... are). Note: don't move
or remove `main.o`.

## Your Tools
You only need two tools to complete this lab. You already know
how to use `/usr/bin/g++`, although remember that you need the
full path while doing this lab.

The other tool you need is `nm`. `nm` lists the symbols in a `.o` file. 

Note, you will be seeing the 'mangled names'. You can used the `--demangle`
option if you prefer.

Don't waste time trying to understand all of the undefined symbols in `main.o`.
Ignore references to `gxx_personality` in any of the `.o` files.
Your starting point here is that it needs *_Z10gobblefunkv* (*gobblefunk()*
when demangled), which is 
provided by at least one of the `.o` files. All of the other symbols
needed by main will be found automatically.

The man page for `nm` will explain what the different characters mean, however
you will be most interested in:

| Character | Meaning |
| --------- | ------- |
| U         | Undefined - this .o file needs it |
| T         | Defined (in text) - this .o provides a definition |
| D         | Defined (in data) - this .o provides a definition, non-zero initialized |
| B         | Defined (in data) - this .o provides a definition, zero initialized |

## Further Work (optional)

Once you have a working executable, then try to find the following:
  * A link line that will cause a multiple definition error.
  * A link line that will cause an undefined reference error.

You probably already discovered those while trying to get a working link
line!

## Further Considerations
The linker we have just used is 'kind' to us. It was willing to look
'backwards' for symbols. Some of the linkers used in Bloomberg 
do not do this. Our slides show the forward-only situation - all of
the arrows went left to right. If you drew a diagram for your link
line, would all of the arrows go left to right?




