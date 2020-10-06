// This is a modified test suite from
// https://github.com/sijk/qt5-sqlcipher/blob/master/test-shared/main.cpp
#include <QtTest>
#include <QTemporaryDir>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

class TestQSqlCipher: public QObject
{
    Q_OBJECT
private slots:
    void initTestCase() // will run once before the first test
    {
        // Check that the driver exists
        QVERIFY2(QSqlDatabase::isDriverAvailable("QSQLCIPHER"), "QSQLCIPHER driver not found.");
        // Set the database file
        QString dbname = QDir(tmpDir.path()).absoluteFilePath("test.db3");
//        QString dbname = QDir("/Users/antaowang/Downloads/").absoluteFilePath("test.db3");
        qDebug() << dbname;
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLCIPHER", "db");
        db.setDatabaseName(dbname);
    }
    void cleanup()
    {
        QSqlDatabase db = QSqlDatabase::database("db", false);
        db.close();
    }

    void checkVersion();
    void checkCompileOptions();
    void createDbWithPassphrase();
    void refuseToReadWithoutPassphrase();
    void allowToReadWithPassphrase();

    void cleanupTestCase()
    {
        QSqlDatabase::removeDatabase("db");
    }
private:
    QTemporaryDir tmpDir;
};

void TestQSqlCipher::checkVersion()
{
    QSqlQuery q(QSqlDatabase::database("db"));
    QVERIFY2(q.exec("PRAGMA cipher_version;"), q.lastError().text().toLatin1().constData());
    QVERIFY(q.next());
    // The next should have been optional
    // maybe if there is a need to test for sqlcipher's version
    // QCOMPARE(q.value(0).toString(), QString("3.4.2"));
}

void TestQSqlCipher::checkCompileOptions()
{
    QSqlQuery q(QSqlDatabase::database("db"));
    QVERIFY2(q.exec("PRAGMA compile_options"), q.lastError().text().toLatin1().constData());
    bool hasCodec = false;
    while(q.next())
    {
        if(q.value(0).toString() == QString("HAS_CODEC"))
        {
            hasCodec = true;
            break;
        }
    }
    QVERIFY2(hasCodec, "'HAS_CODEC' should be in sqlcipher's compile_options.");
}

void TestQSqlCipher::createDbWithPassphrase()
{
    QSqlQuery q(QSqlDatabase::database("db"));
    QStringList queries;
    queries << "PRAGMA key='foobar'"
            << "create table foo(bar integer, bar2 text)"
            << "insert into foo values (42, 'a')"
            << "insert into foo values (43, 'b')";
    for(const QString& qs : queries)
    {
        QVERIFY2(q.exec(qs), q.lastError().text().toLatin1().constData());
    }
}

void TestQSqlCipher::refuseToReadWithoutPassphrase()
{
    QSqlQuery q(QSqlDatabase::database("db"));
    QVERIFY(!q.exec("select bar from foo"));
}

void TestQSqlCipher::allowToReadWithPassphrase()
{
    QSqlQuery q(QSqlDatabase::database("db"));
//    QStringList queries;
//    queries << "PRAGMA key='foobar'"
//            << "select bar from foo"
//            << "select bar from foo where bar2='b'";
//    for(const QString& qs : queries)
//    {
//        QVERIFY2(q.exec(qs), q.lastError().text().toLatin1().constData());
//    }
//    QVERIFY(q.next());
//    QVERIFY(q.value(0).toInt() == 42);
//    QVERIFY(q.value(0).toInt() == 42);


    QVERIFY2(q.exec("PRAGMA key='foobar'"), q.lastError().text().toLatin1().constData());

    QVERIFY2(q.exec("select bar from foo where bar2='a'"), q.lastError().text().toLatin1().constData());
    QVERIFY(q.next());
    QVERIFY(q.value(0).toInt() == 42);

    QVERIFY2(q.exec("select bar from foo order by bar asc"), q.lastError().text().toLatin1().constData());
    QVERIFY(q.next());
    QVERIFY(q.value(0).toInt() == 42);

    QVERIFY2(q.exec("select bar from foo where bar2='b'"), q.lastError().text().toLatin1().constData());
    QVERIFY(q.next());
    QVERIFY(q.value(0).toInt() == 43);
}

QTEST_GUILESS_MAIN(TestQSqlCipher)
//QTEST_APPLESS_MAIN(TestQSqlCipher)
#include "main.moc"
