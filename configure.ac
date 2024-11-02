AC_INIT([Detect It Easy], 3.10)
AC_PROG_CC
AC_LANG(C++)
PKG_CHECK_MODULES(Qt5Core,              [Qt5Core >= 5.2.0],             [], [AC_MSG_ERROR([Qt5Core >= 5.2.0] is required.)])
PKG_CHECK_MODULES(Qt5Gui,               [Qt5Gui >= 5.2.0],              [], [AC_MSG_ERROR([Qt5Gui >= 5.2.0] is required.)])
PKG_CHECK_MODULES(Qt5Widgets,           [Qt5Widgets >= 5.2.0],          [], [AC_MSG_ERROR([Qt5Widgets >= 5.2.0] is required.)])
PKG_CHECK_MODULES(Qt5Sql,               [Qt5Sql >= 5.2.0],              [], [AC_MSG_ERROR([Qt5Sql >= 5.2.0] is required.)])
PKG_CHECK_MODULES(Qt5Script,            [Qt5Script >= 5.2.0],           [], [AC_MSG_ERROR([Qt5Script >= 5.2.0] is required.)])
PKG_CHECK_MODULES(Qt5ScriptTools,       [Qt5ScriptTools >= 5.2.0],      [], [AC_MSG_ERROR([Qt5ScriptTools >= 5.2.0] is required.)])
PKG_CHECK_MODULES(Qt5Network,           [Qt5Network >= 5.2.0],          [], [AC_MSG_ERROR([Qt5Network >= 5.2.0] is required.)])
 
PKG_CHECK_EXISTS([Qt5Core >= 5.2.0],[PKG_CHECK_VAR(QT_QMAKE,[Qt5Core >= 5.2.0],[host_bins])])

AS_IF([test -f "$QT_QMAKE/qmake"],[QT_QMAKE="$QT_QMAKE/qmake"],[AS_IF([test -f "$QT_QMAKE/qmake-qt5"],[QT_QMAKE="$QT_QMAKE/qmake-qt5"],[QT_QMAKE=""])])

X_QMAKE_SPEC=$($QT_QMAKE -query QMAKE_SPEC)
X_QT_VERSION=$($QT_QMAKE -query QT_VERSION)
X_QT_PREFIX=${prefix}
AC_MSG_NOTICE([Qt version: $X_QT_VERSION])
AC_MSG_NOTICE([Running qmake to generate the makefile...])
$QT_QMAKE "die_source.pro" -spec $X_QMAKE_SPEC

AC_MSG_NOTICE([The configure finished. Please type "make" to compile the project.])
AS_ECHO()
