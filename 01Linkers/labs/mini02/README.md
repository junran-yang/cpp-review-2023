# Mini Lab - Running the linker
In this lab you will link some code that has already been compiled.

## Preparation
Log in to one of the following Linux machines:
   * `nylxdev3`,
   * `nylxdev4`,
   * `nylxdev5`,
   * `nylxdev6`,
   * `njlxdev2`,
   * `njlxdev3`,
   * `njlxdev4`,
   * `njlxdev5`

Navigate to the `labs/01_linkers/mini02` directory.

Run the `preparedir` script. Four files should have appeared: three `.o`s and
`ldcommand`

## View the files
View one of the `.o` files in an editor, e.g. vim or emacs. Don't edit it, 
just convince yourself that it's a binary file. You
might see some interesting characters in there!

## Basic linking
Execute `g++ -m32 x.o y.o z.o -o fb -L /bb/build/Linux-x86_64-32/release/robolibs/stage/opt/bb/lib -lbsl` (You can copy this).
The `-o fb` tells `g++` that the output
executable should be called `fb`. You have only supplied object files
to `g++`, therefore it knows to only perform linking.

Execute `fb`.

## Manual linking (optional)
`g++` hides a lot of link details.
As well as linking our files, it also links 'standard' files, such as the
C++ standard libraries and C runtime libraries.

View the file `ldcommand`. This is what the manual link line 
would look like. It might even work - try running it! There is a chance that it
won't be quite right for the machine you're on though.

It is highly unlikely that you will be expected to work with `ld` directly.
We do it to demonstrate that `g++` is a front end.

## Observing the linker from g++ (optional)
You can observe some of the link details from `g++` and `ld` by running:
`g++ -m32 x.o y.o z.o -o fb -L /bb/build/Linux-x86_64-32/release/robolibs/stage/opt/bb/lib -lbsl -v -Wl,--verbose`. This information guided the
creation of the `ldcommand` script.

