#pragma once

#include <QObject>
#include <QProcess>
#include <QAbstractListModel>
#include <deque>

namespace Qommons {

class Process : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString program READ program WRITE setProgram NOTIFY programChanged)
    Q_PROPERTY(QStringList arguments READ arguments WRITE setArguments NOTIFY argumentsChanged)
    Q_PROPERTY(int exitCode READ exitCode NOTIFY finished)
    Q_PROPERTY(ExitStatus exitStatus READ exitStatus NOTIFY finished)
    Q_PROPERTY(ProcessError error READ error NOTIFY errorOccurred)
    Q_PROPERTY(ProcessState state READ state NOTIFY stateChanged)
    Q_PROPERTY(ProcessChannelMode processChannelMode READ processChannelMode WRITE setProcessChannelMode NOTIFY processChannelModeChanged)
public:
    enum ProcessError {
        FailedToStart,
        Crashed,
        Timedout,
        ReadError,
        WriteError,
        UnknownError
    };
    Q_ENUM(ProcessError)

    enum ProcessState {
        NotRunning,
        Starting,
        Running
    };
    Q_ENUM(ProcessState)

    enum ExitStatus {
        NormalExit,
        CrashExit
    };
    Q_ENUM(ExitStatus)

    enum ProcessChannelMode {
        SeparateChannels,
        MergedChannels,
        ForwardedChannels,
        ForwardedOutputChannel,
        ForwardedErrorChannel
    };
    Q_ENUM(ProcessChannelMode)

    Process(QObject* parent = nullptr);
    ~Process();

    QString program() const;
    void setProgram(const QString& value);

    QStringList arguments() const;
    void setArguments(const QStringList& value);

    ProcessChannelMode processChannelMode() const;
    void setProcessChannelMode(ProcessChannelMode value);

    int exitCode() const;
    ExitStatus exitStatus() const;
    ProcessError error() const;
    ProcessState state() const;

public slots:
    void start();
    void start(const QString& command, const QStringList& arguments);

signals:
    void started();
    void errorOccurred();
    void finished();
    void stateChanged();

    void programChanged();
    void argumentsChanged();
    void processChannelModeChanged();

private:
    QProcess process;

    friend class ProcessOutputModel;
};


class ProcessOutputModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(Process* process READ process WRITE setProcess)
public:
    ProcessOutputModel(QObject* parent = nullptr);
    ~ProcessOutputModel();

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    Process* process() const;
    void setProcess(Process* process);

private:
    std::deque<QString> lines;
    int size = 0;
    Process* process_;

    void onReadyRead();
    void started();
};

}
