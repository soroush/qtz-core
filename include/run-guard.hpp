#ifndef QTZ_RUNGUARD_HPP
#define QTZ_RUNGUARD_HPP

#include <QObject>
#include <QString>
#include <QSharedMemory>
#include <QSystemSemaphore>

class RunGuard
{

public:
    RunGuard( const QString& key );
    ~RunGuard();

    bool isAnotherRunning();
    bool tryToRun();
    void release();

private:

private:
    QString generateKeyHash(const QString& key, const QString& salt );

    const QString key;
    const QString memLockKey;
    const QString sharedmemKey;

    QSharedMemory sharedMem;
    QSystemSemaphore memLock;

    Q_DISABLE_COPY( RunGuard )
};


#endif // QTZ_RUNGUARD_HPP