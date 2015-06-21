# Autoconf file for llama-shell

AC_PREREQ([2.69])
AC_INIT([llama-shell], [0.4.0beta], [andrewrmack@gmail.com])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])

# Host info
AC_CANONICAL_HOST

# Makefiles
AM_INIT_AUTOMAKE([foreign -Wall -Werror])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([libintl.h fcntl.h stdlib.h string.h termios.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_PID_T

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_CHECK_FUNCS([dup2 getcwd strerror])

AC_CONFIG_FILES([Makefile src/Makefile])


AC_DEFINE_UNQUOTED([MACHTYPE], ["$host"],
    [Define host information for version])

AC_OUTPUT
