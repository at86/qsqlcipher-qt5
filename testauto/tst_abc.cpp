#include <QtTest>

// add necessary includes here

class abc : public QObject
{
    Q_OBJECT

public:
    abc();
    ~abc();

private slots:
    void test_case1();

};

abc::abc()
{

}

abc::~abc()
{

}

void abc::test_case1()
{

}

QTEST_APPLESS_MAIN(abc)

#include "tst_abc.moc"
