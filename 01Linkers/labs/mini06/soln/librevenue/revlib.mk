# Generated on nylxdev4 at Tue Mar  7 09:31:20 2017 by mjones287 using:
# /bbsrc/bin/prod/bin/bde_metamkmk.pl '-t' 'other' '--library' 'revenue' '-f' 'revlib.mk'

#SRCS=list of source file<.c .cpp .f .gob>
#source files under current dir will be used
#if SRCS is not presented

#SRCDIRS=list of subdirectories
#source files under current dir and the
#specified sub directories will be used
#if SRCS is not presented

ANAME=librevenue.a
USER_CFLAGS   += -I.
USER_CPPFLAGS += -I.
USER_FFLAGS   += -I.


# remove this to disable checking for gcc warnings
IS_GCC_WARNINGS_CLEAN=yes

#--- do not modify below ---

IS_BDE=yes
IS_PTHREAD=yes
IS_EXCEPTION=yes
IS_DEPENDS_NATIVE=1

MKINCL?=/bbsrc/mkincludes/
include $(MKINCL)sourcelist.mk
AOBJS?=$(OBJS_AR)
include $(MKINCL)machdep.newlink

-include local.mk

include $(MKINCL)linklib.newlink
