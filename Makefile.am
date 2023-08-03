ACLOCAL_AMFLAGS=-I m4
AM_LDFLAGS=-no-undefined

lib_LTLIBRARIES=libtimeinterval.la
check_PROGRAMS=interval_tests
include_HEADERS=TimeInterval.h

libtimeinterval_la_SOURCES=TimeInterval.cpp

interval_tests_SOURCES=interval_tests.cpp
interval_tests_LDADD=libtimeinterval.la

TESTS=interval_tests
