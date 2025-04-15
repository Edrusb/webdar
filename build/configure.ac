########################################################################
# webdar - a web server and interface program to libdar
# Copyright (C) 2013-2025 Denis Corbin
#
# This file is part of Webdar
#
#  Webdar is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  Webdar is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with Webdar.  If not, see <http://www.gnu.org/licenses/>
#
#----
#  to contact the author: dar.linux@free.fr
########################################################################

# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.71])
AC_INIT([WEBDAR],[1.0.0.beta.1],[[https://github.com/Edrusb/webdar/issues]])
AC_CONFIG_HEADERS([config.h])
AC_LANG([C++])
AC_CONFIG_SRCDIR([src/webdar.cpp])
AC_DEFINE_UNQUOTED(WEBDAR_VERSION, "AC_PACKAGE_VERSION", [webdar version, definition in configure.ac])

AM_INIT_AUTOMAKE([subdir-objects])
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION
XGETTEXT_EXTRA_OPTIONS='--keyword=dar_gettext'
AM_ICONV

####
# configure checks what is available from the operational system:
# - it displays things on output for the user running the configure script has status information
# - it sets some shell variable that are not used outside the configuration script
# - it sets some shell variable that can be susbstitued in Makefile.in files (see AC_SUBST() and AC_CONFIG_FILES())
#   also known as "output variables"
# - it defines macros that get stored in config.h and used in source code (see AC_DEFINE())
#
# header files:
#   header files are #included in source code if they HAVE_... macro has been defined in config.h
#
# libraries:
#   necessary library flags are stored in the "output variables" LIBS substituable shell variable and passed to Makefile.in
#   that get substitued in Makefiles.in when the @LIBS@ form is met.
#

# have a specific variable for pkgconfig, setting the default value:
AC_SUBST(pkgconfigdir, [${libdir}/pkgconfig])
AC_ARG_WITH([pkgconfigdir],
            AS_HELP_STRING(--with-pkgconfigdir=DIR, [defines an alternative directory to install pkconfig files, default is '${libdir}/pkgconfig']),
            [
                if [ ! -z "$withval" ] ; then
                 AC_SUBST(pkgconfigdir, $withval)
               fi
            ],
            []
           )

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
LT_INIT
AC_PROG_MAKE_SET
AC_PROG_RANLIB

AC_MSG_CHECKING([for C++ compiler usability])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([],
                                  [ class test { public: int test; }; ])
	          ],
                  [ AC_MSG_RESULT(ok) ],
                  [ AC_MSG_ERROR([No C++ compiler found]) ]
		  )

# Defining _XOPEN_SOURCE to get extra field in struct stat
AC_DEFINE(_XOPEN_SOURCE, 700, [activates POSIX.1-2008 symbols in order to allow microsecond time setting, as well as ctime_r() call])

# Define _BSD_SOURCE in order to be able to call makedev(), minor() and major() under OpenBSD when _XOPEN_SOURCE is set
AC_DEFINE(_BSD_SOURCE, 1, [activate makedev(), major() and minor() when _XOPEN_SOURCE is set])
AC_DEFINE(_DEFAULT_SOURCE, 1, [disabling warning about _BSD_SOURCE to be deprecated])

###########
## THE FOLLOWING "DEFINE" USED TO RE-ENABLE FULL LIBC FEATURES ON DIFFERENT OPERATING SYSTEMS
## HAVE BEEN BORROWED FROM PYTHON's configure.in
##
##

# The later defininition of _XOPEN_SOURCE disables certain features
# on Linux, so we need _GNU_SOURCE to re-enable them (makedev, tm_zone).
AC_DEFINE(_GNU_SOURCE, 1, [Define on Linux to activate all library features])

# The later defininition of _XOPEN_SOURCE and _POSIX_C_SOURCE disables
# certain features on NetBSD, so we need _NETBSD_SOURCE to re-enable
# them.
AC_DEFINE(_NETBSD_SOURCE, 1, [Define on NetBSD to activate all library features])

# The later defininition of _XOPEN_SOURCE and _POSIX_C_SOURCE disables
# certain features on FreeBSD, so we need __BSD_VISIBLE to re-enable
# them.
AC_DEFINE(__BSD_VISIBLE, 1, [Define on FreeBSD to activate all library features])

# The later defininition of _XOPEN_SOURCE and _POSIX_C_SOURCE disables
# certain features on Mac OS X, so we need _DARWIN_C_SOURCE to re-enable
# them.
AC_DEFINE(_DARWIN_C_SOURCE, 1, [Define on Darwin to activate all library features])

##
##
###########

# Checks for header files.
AC_HEADER_DIRENT
AC_CHECK_INCLUDES_DEFAULT
AC_PROG_EGREP

AC_HEADER_SYS_WAIT

AC_CHECK_HEADERS([stdio.h stdlib.h syslog.h pthread.h errno.h limits.h sys/types.h sys/stat.h sys/socket.h time.h ctype.h openssl/err.h openssl/ssl.h string.h fnmatch.h netinet/ip.h netinet/in.h arpa/inet.h strings.h unistd.h fcntl.h signal.h sys/capability.h linux/capability.h dar/libdar.hpp libthreadar/libthreadar.hpp nlohmann/json.hpp])

# mandatory headers
AC_CHECK_HEADER([openssl/ssl.h],
		[],
		[
		  AC_MSG_ERROR([missing required <openssl/ssl.h> included file])
		])

AC_CHECK_HEADER([dar/libdar.hpp],
		[],
		[
		  AC_MSG_ERROR([missing required <dar/libdar.hpp> included file])
		])

AC_CHECK_HEADER([dar/tools.hpp],
		[],
		[
		  AC_MSG_ERROR([missing required <dar/tools.hpp> included file])
		])

AC_CHECK_HEADER([libthreadar/libthreadar.hpp],
		[],
		[
		  AC_MSG_ERROR([missing required <libthreadar/libthreadar.hpp> included file])
		])

AC_CHECK_HEADER([nlohmann/json.hpp],
		[],
		[
		AC_MSG_ERROR([missing required <nlohmann/json.hpp> included file])
		])

AC_SYS_LARGEFILE

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_CHECK_MEMBERS([struct stat.st_rdev])

AC_CHECK_TYPE(size_t, [AC_CHECK_SIZEOF(size_t)], [AC_MSG_ERROR([Cannot find size_t type])], [])
AC_CHECK_TYPE(time_t, [AC_CHECK_SIZEOF(time_t)], [AC_MSG_ERROR([Cannot find time_t type])], [])
AC_CHECK_TYPE(off_t, [AC_CHECK_SIZEOF(off_t)], [AC_MSG_ERROR([Cannot find off_t type])], [])

# Checks for library functions.
AC_FUNC_FNMATCH
AC_FUNC_FORK
AC_FUNC_LSTAT
AC_HEADER_MAJOR
AC_FUNC_MALLOC

AC_FUNC_STAT
AC_FUNC_UTIME_NULL

AC_CHECK_FUNCS([lchown mkdir regcomp rmdir strerror_r utime fdopendir readdir_r ctime_r getgrnam_r getpwnam_r localtime_r])


AC_MSG_CHECKING([for c++14 support])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([
				     #include <memory>
				   ],
                                   [ thread_local static int test = 0;
				     std::unique_ptr<int> x = std::make_unique<int>(0);
				   ])
                  ],
                  [ AC_MSG_RESULT(yes) ],
                  [
                     AC_MSG_RESULT([no])
                     AC_MSG_CHECKING([for c++14 support with -std=c++14 option set])
                     CXXSTDFLAGS="-std=c++14"
                     CXXFLAGS="$CXXFLAGS $CXXSTDFLAGS"
                     AC_COMPILE_IFELSE([AC_LANG_PROGRAM([
							  #include <memory>
							],
                                                        [ thread_local static int test = 0;
							  std::unique_ptr<int> x = std::make_unique<int>(0);
							])
                                        ],
                                        [ AC_MSG_RESULT(yes) ],
                                        [
                                           AC_MSG_RESULT(no)
                                           AC_MSG_ERROR([C++ compiler lack support for c++14 standard])
                                        ]
                     )
                   ])

AC_MSG_CHECKING([for sed -r/-E option])
if sed -r -e 's/(c|o)+/\1/g' > /dev/null < /dev/null ; then
    local_sed="gnu"
    AC_MSG_RESULT([GNU sed, using -r option for regex])
else
   if sed -E -e 's/(c|o)+/\1/g' > /dev/null < /dev/null ; then
     local_sed="bsd"
     AC_MSG_RESULT([BSD sed, using -E option for regex])
   else
     local_sed=unknown
     AC_MSG_ERROR([unknown switch to use with sed to support regex])
   fi
fi

AC_MSG_CHECKING([for Niels Lohmann's Json library for C++])

AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
					#include <nlohmann/json.hpp>
				   ]],
				   [[
				       using nljson = nlohmann::json;

				       nljson j, k;
				       j["one"] = 1;
				       k.push_back("a");
				       k.push_back("b");
				       k.push_back("c");
				       j["two"] = k;
				   ]]
                   )
		   ],
		   [ AC_MSG_RESULT(Found) ],
		   [ AC_MSG_RESULT([Not Found])
		     AC_MSG_ERROR([Niels Lohmann's Json library for C++ is missing])
		   ])

AC_MSG_CHECKING([for getopt() in <unistd.h>])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[extern "C" {
                                    #if HAVE_UNISTD_H
                                    #include <unistd.h>
                                    #endif
                                    }]],
                                    [ getopt(0, 0, 0); ])
                  ],
                  [
                    AC_DEFINE(HAVE_GETOPT_IN_UNISTD_H, 1, [a getopt() call is declared in <unistd.h>])
                    AC_MSG_RESULT(present)
                  ],
                   [AC_MSG_RESULT(absent)])

AC_MSG_CHECKING([for getopt_long() in <unistd.h>])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[extern "C" {
                                    #if HAVE_UNISTD_H
                                    #include <unistd.h>
                                    #endif
                                    }]],
                                    [ getopt_long(0, 0, 0, 0, 0); ])
                  ],
                  [
                    AC_DEFINE(HAVE_GETOPT_LONG_IN_UNISTD_H, 1, [a getopt_long() call is declared in <unistd.h>])
                    AC_MSG_RESULT(present)
                  ],
                   [AC_MSG_RESULT(absent)])


AC_MSG_CHECKING([for optreset presence])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[extern "C" {
                                   #if HAVE_STDIO_H
                                   #include <stdio.h>
                                   #endif
                                   #if HAVE_GETOPT_H
                                   #include <getopt.h>
                                   #else
                                   #if HAVE_UNISTD_H
                                   #include <unistd.h>
                                   #endif
                                   #endif
                                   }]], [ int x = optreset; return 0; ])
                  ],
                  [
                   AC_DEFINE(HAVE_OPTRESET, 1, [the optreset external variable exists to reset getopt standard call])
                   AC_MSG_RESULT(available)
                  ],
                  [AC_MSG_RESULT([not available])])


AC_MSG_CHECKING([for POSIX sys/capability support])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[extern "C" {
                                  #if HAVE_SYS_CAPABILITY_H
                                  #include <sys/capability.h>
                                  #else
                                  #if HAVE_LINUX_CAPABILITY_H
                                  #include <linux/capability.h>
                                  #endif
                                  #endif
                                  #if HAVE_SYS_TYPES_H
                                  #include <sys/types.h>
                                  #endif
                                  }]],
                                  [ cap_t capaset = cap_get_proc();
                                    (void)cap_free((void *)capset);
                                  ])
                   ],
                   [
                      AC_DEFINE(HAVE_CAPABILITIES, 1, [whether the system has support for POSIX.1e capabilities])
                      AC_MSG_RESULT(available)
                   ],
                   [  AC_MSG_RESULT([not available])  ])


AC_MSG_CHECKING([for strerror_r flavor])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[extern "C"
                                     {
                                     #if HAVE_STRING_H
                                     #include <string.h>
                                     #endif
                                     } // externe "C"
                                   ]],
                                   [[
                                        char *ptr = strerror_r(0, 0, 0);
                                   ]])
                  ],
                  [ AC_DEFINE(HAVE_STRERROR_R_CHAR_PTR, 1, [strerror_r() returns a char *])
                    AC_MSG_RESULT([GNU specific])
                  ],
                  [
                    AC_MSG_RESULT([XSI compliant])
                  ])




AC_ARG_ENABLE( [mode],
                AS_HELP_STRING(--enable-mode=arg, [where arg is either 32, 64 (the default) or "infinint". Makes libdar internally use 32 bits or limitless integers in place of 64 bits integers]),
		[
		    build_mode=$enableval
		    if test "$build_mode" != "32" -a "$build_mode" != "64" -a "$build_mode" != "infinint" ; then
		        AC_MSG_ERROR([Invalid argument given to --enable-mode option])
		    fi
		],
		[build_mode=64])

if test -z "$build_mode" ; then
     AC_MSG_ERROR([build_mode has not been defined in configure script])
else
     build_mode_suffix=$build_mode
     if test "$build_mode" = "infinint"  ; then
         build_mode_suffix=""
     fi
fi

AC_ARG_ENABLE( [upx],
                AS_HELP_STRING(--disable-upx, [by default configure looks for UPX and if available make executables compressed at installation time, you can disable this feature]),
                [explicit_upx=yes],
                [enable_upx=yes])

AS_IF(          [ test "x$enable_upx" != "xyes" ],
                [
                   AC_MSG_NOTICE([ignoring UPX])
                   upx="no"
                ],
                [
                   AC_CHECK_PROG(upx, upx, [yes], [no], [$PATH])
                   AS_IF( [ test "x$explicit_upx" = "xyes" -a "$upx" != "yes" ],
                          [ AC_MSG_ERROR([upx is missing]) ]
                        )
                ]
             )

AC_ARG_ENABLE( [build-html],
               AS_HELP_STRING(--disable-build-html, [don't build programming documentation (in particular libdar API documentation) and html man page]),
               [explicit_build_html=yes],
               [enable_build_html=yes])

AS_IF(         [ test "x$enable_build_html" != "xyes"],
               [
                 AC_MSG_WARN([documentation no built per user request])
                 doxygen="no"
                 groff="no"
               ],
               [ AC_CHECK_PROG(doxygen, doxygen, [yes], [no], [$PATH])
                 AC_MSG_CHECKING([for doxygen version])
                   if test "$doxygen" = "yes" ; then
                       n1=`doxygen --version | cut -d '.' -f 1`
                       n2=`doxygen --version | cut -d '.' -f 2`
                       if test $n1 -gt 1 -o $n2 -ge 3 ; then
                           AC_MSG_RESULT([ >= 1.3])
                       else
                           AC_MSG_RESULT([ too old (< 1.3) ignoring doxygen])
                           doxygen="no"
                       fi
                   fi
                 AC_CHECK_PROG(dot, dot, [YES], [NO], [$PATH])
                 # upper case value for dot variable because it goes as is into doxyfile file

                 AC_CHECK_PROG(tmp, man, [yes], [no], [$PATH])
                 if test "$tmp" = "yes" ; then
                    AC_CHECK_PROG(groff, groff, [yes], [no], [$PATH])
                 else
                    groff = "no";
                 fi
                 AS_IF( [ test "x$explicit_build_html" = "xyes" -a \( "$doxygen" != "yes" -o "$groff" != "yes" \) ],
                        [ AC_MSG_ERROR([lacking prerequisit to build documentation]) ]
                      )
               ]
     )


AC_ARG_ENABLE( [dev],
	       AS_HELP_STRING(--enable-dev, [use trivial password for development purposes]),
	       [explicit_dev=yes],
	       [enable_dev=no])

AS_IF(	       [ test "x$enable_dev" != "xno" ],
	       [ AC_MSG_WARN([WARNING! Using trivial password for development purposes])
	         AC_DEFINE(WEBDAR_DEV, 1, [trivial passwords for dev purposes])
	       ],
	       [] # else we just keep silent
	       )

AC_ARG_ENABLE( [webdar-static],
               AS_HELP_STRING(--enable-webdar-static, [building webdar_static, a webdar statically linked version]))

AS_IF(         [ test "x$enable_webdar_static" = "xyes" ],
               [build_static="yes"],
	       [build_static="no"])

PKG_CHECK_MODULES(OPENSSL,
		  openssl,
		  [],
		  [])

PKG_CHECK_MODULES(LIBDAR,
                  libdar${build_mode_suffix},
		  [],
		  [
		     AC_MSG_ERROR([libdar${build_mod_suffix} was not found or pkg-config is not installed (see also PKG_CONFIG_PATH env variable, if libdar is installed in a non-standard place)])
		  ]
		  )

PKG_CHECK_MODULES(LIBTHREADAR,
		  libthreadar,
		  [],
		  [
		    AC_MSG_ERROR([libthreadar was not found or pkg-config is not installed (see also PKG_CONFIG_PATH env variable, if libthreadar is installed in a non-standard place)])
	          ]
		  )

### specific handling of musl standard library

AC_MSG_CHECKING([for standard library])

which_test=`which test`
is_musl=`ldd "$which_test" | grep musl | wc -l`

if test $is_musl -ne 0 ; then
    # to ask musl to provide larger stacks for non threads (here 8Mio)
    LDFLAGS="$LDFLAGS -DPT_GNU_STACK=-Wl,-z,stack-size=0x800000"
    AC_MSG_RESULT([musl library detected, increasing thread stack size to 8 Mio])
else
    AC_MSG_RESULT([not musl, keeping default thread stack size])
fi

# Checks for libraries.
# AC_CHECK_LIB(ssl, [SSL_new], [], [AC_MSG_ERROR([Cannot link with libssl library]) ], [ ${OPENSSL_LIBS} ${LIBTHREADAR_LIBS} ${LIBDAR_LIBS} ])
AC_CHECK_LIB(dar${build_mode_suffix}, [for_autoconf], [], [AC_MSG_ERROR([cannot link with libdar library]) ], [ ${OPENSSL_LIBS} ${LIBTHREADAR_LIBS} ${LIBDAR_LIBS} ])
AC_CHECK_LIB(threadar, [for_autoconf], [], [AC_MSG_ERROR([Cannot link with libthreadar library]) ], [ ${OPENSSL_LIBS} ${LIBTHREADAR_LIBS} ${LIBDAR_LIBS} ])

AM_CONDITIONAL([BUILD_WEBDAR_STATIC], [ test $build_static = "yes" ])
AM_CONDITIONAL([BUILD_MODE32], [test "$build_mode" = "32"])
AM_CONDITIONAL([BUILD_MODE64], [test "$build_mode" = "64" -o -z "$build_mode"])
AM_CONDITIONAL([USE_UPX], [test "$upx" = "yes"])
AM_CONDITIONAL([USE_DOXYGEN], [test "$doxygen" = "yes"])
AM_CONDITIONAL([USE_GROFF], [test "$groff" = "yes"])
AM_CONDITIONAL([BSD_SED], [test "$local_sed" = "bsd"])
AC_SUBST([CXXSTDFLAGS], [$CXXSTDFLAGS])
AC_SUBST(UPX_PROG, [upx])
AC_SUBST(DOXYGEN_PROG, [doxygen])
AC_SUBST(HAS_DOT, [$dot])

# defaults
AC_PREFIX_DEFAULT(/usr/local)

# hack from libtool mailing-list to know from source point of view whether we are compiling for dynamic or static way

AC_CONFIG_COMMANDS([hack-libtool], [
                                    sed 's,^pic_flag=,pic_flag=" -D__DYNAMIC__ ",' libtool > libtoolT \
                                    && mv -f libtoolT libtool && chmod 755 libtool
])


AC_CONFIG_FILES([Makefile doc/Makefile src/Makefile misc/Makefile])
AC_OUTPUT
