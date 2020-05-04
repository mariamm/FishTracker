//! [0]
#include <QtTest/QtTest>
#include <QString>

class MyFirstTest: public QObject
{
    Q_OBJECT
private slots:
    void toUpper();
};
//! [0]

//! [1]
void MyFirstTest::toUpper()
{
    QString str = "Hello";
    QCOMPARE(str.toUpper(), QString("HELLO"));
}
//! [1]

//! [2]
QTEST_MAIN(MyFirstTest)
#include "MyFirstTest.moc"
//! [2]
