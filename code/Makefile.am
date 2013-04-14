## Place generated object files (.o) into the same directory as their source
## files, in order to avoid collisions when non-recursive make is used.
AUTOMAKE_OPTIONS = subdir-objects

## Additional flags to pass to aclocal when it is invoked automatically at
## make time. The ${ACLOCAL_FLAGS} variable is picked up from the environment
## to provide a way for the user to supply additional arguments.
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}
# -Waggregate-return

SRCDIR = src

bin_PROGRAMS = slp

slp_SOURCES = src/Main.cpp src/slp/Tree.cpp src/slp/Algorithm.cpp\
              src/slp/Good.cpp
#-Werror 
slp_CPPFLAGS = -Iinclude\
               -std=c++0x\
               -pedantic\
               -O0 -g3\
               -Wall -Wextra\
               -Wdouble-promotion -Wformat=2 -Winit-self\
               -Wswitch-default -Wswitch-enum -Wuninitialized\
               -Wstrict-overflow=5 -Wno-div-by-zero -Wtrampolines\
               -Wundef -Wshadow -Wcast-align -Wmissing-include-dirs\
               -Wwrite-strings -Wconversion -Wlogical-op\
               -Wmissing-declarations -Winline\
               -Wmissing-field-initializers -Wpacked\
               -Wvla -Wno-missing-braces -Wstack-protector

#slp_LDFLAGS = -Llib -Wl,--rpath -Wl,lib
slp_LDFLAGS = -Llib -lz -lbz2 -llapack -Wl,--rpath -Wl,lib

slp_LDADD = ${slp_LIBS} -lClp -lCoinUtils
