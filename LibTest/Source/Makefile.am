bin_PROGRAMS = libtest
INCLUDES=-I@top_srcdir@/FELL/Include -I@top_srcdir@/FeDX/Include -I@top_srcdir@/FeIO/Include -I@top_srcdir@/FeWin/Include -I@top_srcdir@/FEEL/Include -I@top_srcdir@/FeFile/Include -I@top_srcdir@/FeMath/Include -I@top_srcdir@/Include

libtest_SOURCES = libtest.cpp
LDADD = @top_srcdir@/FeWin/Source/libFeWin.a @top_srcdir@/FEEL/Source/libFEEL.a @top_srcdir@/FELL/Source/libFELL.a @top_srcdir@/FeFile/Source/libFeFile.a @top_srcdir@/FeMath/Source/libFeMath.a @top_srcdir@/FeDX/Source/libFeDX.a @top_srcdir@/FeIO/Source/libFeIO.a

#no FeIO in that list...
