# -*- makefile -*-
# Generated on nylxdev4 at Fri May 10 14:14:39 2019 by mjones287 using:
# /bbsrc/bin/prod/bin/bde_metamkmk.pl '-t' 'other' 'revenue.mk'
# $Id$ $CSID$

TASK=revenue.tsk

# Use the SRCS macro to limit the source files that will be compiled and
# linked into your task to only those listed. If SRCS is not used then all
# source files in the current directory will be used.
#SRCS=

# Use the SRCDIRS macro to specify subdirectories that should be searched in
# addition to the current directory for source files. If SRCS has been
# specified explicitly then SRCDIRS has no effect.
#SRCDIRS=

# remove this to disable checking for gcc warnings
IS_GCC_WARNINGS_CLEAN=yes

USER_CFLAGS   += -I.
USER_CPPFLAGS += -I.
USER_FFLAGS   += -I.

# PCDEPS contains a list with your direct dependencies.
# You can find your direct dependencies by running the `genpcdeps` command
PCDEPS=

# The following section is a fallback for cases where PCDEPS is not working.
#
# If you can use PCDEPS, you should use PCDEPS. However not every application
# can be linked with PCDEPS, and the following section is necessary in case
# you use llcalc as a fallback.
#
# The LIBS macro should always contain $(INCLIBS) which is what llcalc	
# controls. You can add additional libraries in LIBS before $(INCLIBS) but not	
# after. `llcalc` will rewrite the INCLIBS macro completely each time it is run	
# so avoid making modifications to INCLIBS.
INCLIBS=
LIBS=$(INCLIBS)

# do not modify below this point
METAMKMK_VER=1.0
IS_CPPMAIN=1
IS_DEPENDS_NATIVE=1
MKINCL?=/bbsrc/mkincludes/
include $(MKINCL)sourcelist.mk
OBJS?=$(OBJS_AR)
include $(MKINCL)machdep.newlink
include $(MKINCL)linktask.newlink
# vim:ft=make
