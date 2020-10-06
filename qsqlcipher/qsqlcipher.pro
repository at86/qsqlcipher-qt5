TARGET = qsqlcipher

QT_FOR_CONFIG += sqldrivers-private

HEADERS += $$PWD/qsql_sqlite_p.h
SOURCES += $$PWD/qsql_sqlite.cpp $$PWD/smain.cpp

CONFIG += link_pkgconfig

#PKGCONFIG += sqlcipher
# or if pkg-config is not available
# INCLUDEPATH += /path/to/include/sqlcipher
# LIBS += -L/path/to/lib/ -lsqlcipher -lcrypto
# ===> brew install sqlcipher
INCLUDEPATH += /usr/local/Cellar/sqlcipher/4.4.0/include/sqlcipher
LIBS += -L/usr/local/Cellar/sqlcipher/4.4.0/lib -lsqlcipher

OTHER_FILES += sqlcipher.json

PLUGIN_CLASS_NAME = QSQLCipherDriverPlugin

# follows contents of include(../qsqldriverbase.pri)
QT  = core core-private sql-private

PLUGIN_TYPE = sqldrivers
load(qt_plugin)

DEFINES += QT_NO_CAST_TO_ASCII QT_NO_CAST_FROM_ASCII
