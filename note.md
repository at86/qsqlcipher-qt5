# install QSQLCIPHER lib
Use Qt Creator compile project,
cd `build-qsqlcipher-Desktop_Qt_5_14_2_clang_64bit-Release/qsqlcipher`,
run `make install` will install QSQLCIPHER lib

# run tests
Modify `/qsqlcipher.pro`
```
# SUBDIRS += qsqlcipher
# ===> up line change to below
SUBDIRS += qsqlcipher \
    tests
```
Change view to `Tests`,
run tests.
