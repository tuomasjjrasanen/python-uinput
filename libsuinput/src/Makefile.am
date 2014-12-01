AM_CPPFLAGS = -Wall -Werror -Wextra -pedantic -std=gnu99
lib_LTLIBRARIES = libsuinput.la
libsuinput_la_SOURCES = suinput.c
libsuinput_la_LDFLAGS = -l:libudev.so -version-info 5:0:1
include_HEADERS = suinput.h
noinst_HEADERS = libudev.h
pkgconfigdir   = $(libdir)/pkgconfig
pkgconfig_DATA = libsuinput.pc
