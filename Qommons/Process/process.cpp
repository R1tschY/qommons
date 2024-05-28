#include "process.h"

#include <QProcess>
#include <QtQml>
#include <QLoggingCategory>

namespace Qommons {

static Q_LOGGING_CATEGORY(logger, "Qommons.Process");

Process::Process(QObject* parent)
    : QObject(parent)
{
    connect(&process, &QProcess::started, this, &Process::started);
    connect(
                &process, static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
                this, &Process::finished);
    connect(&process, &QProcess::errorOccurred, this, &Process::errorOccurred);
    connect(&process, &QProcess::stateChanged, this, &Process::stateChanged);
}

Process::~Process() = default;

QString Process::program() const
{
    return process.program();
}

void Process::setProgram(const QString &value)
{
    if (process.program() != value) {
        process.setProgram(value);
        emit programChanged();
    }
}

QStringList Process::arguments() const
{
    return process.arguments();
}

void Process::setArguments(const QStringList &value)
{
    if (process.arguments() != value) {
        process.setArguments(value);
        emit argumentsChanged();
    }
}

Process::ProcessChannelMode Process::processChannelMode() const
{
    return (Process::ProcessChannelMode)(int)process.processChannelMode();
}

void Process::setProcessChannelMode(ProcessChannelMode value)
{
    auto newValue = (QProcess::ProcessChannelMode)(int)value;
    if (newValue != process.processChannelMode()) {
        process.setProcessChannelMode(newValue);
        emit processChannelModeChanged();
    }
}

int Process::exitCode() const
{
    return process.exitCode();
}

Process::ExitStatus Process::exitStatus() const
{
    return (Process::ExitStatus)(int)process.exitStatus();
}

Process::ProcessError Process::error() const
{
    return (Process::ProcessError)(int)process.error();
}

Process::ProcessState Process::state() const
{
    return (Process::ProcessState)(int)process.state();
}

void Process::start()
{
    process.start();
}

void Process::start(const QString &command, const QStringList &arguments)
{
    process.start(command, arguments);
}

ProcessOutputModel::ProcessOutputModel(QObject *parent)
    : QAbstractListModel(parent)
{ }

ProcessOutputModel::~ProcessOutputModel() = default;

int ProcessOutputModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    } else {
        return size;
    }
}

QVariant ProcessOutputModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if (row < 0 || row >= size) {
        return { };
    }

    if (role == Qt::DisplayRole) {
        return lines[row];
    }

    return { };
}

QHash<int, QByteArray> ProcessOutputModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(Qt::DisplayRole, "line");
    return roles;
}

Process *ProcessOutputModel::process() const
{
    return process_;
}

void ProcessOutputModel::setProcess(Process *process)
{
    if (process_ == process) {
        return;
    }

    beginResetModel();

    if (process_ != nullptr) {
        disconnect(process_);
    }

    lines.clear();
    size = 0;
    process_ = process;

    if (process != nullptr) {
        QProcess* qprocess = &process->process;
        connect(qprocess, &QProcess::readyRead, this, &ProcessOutputModel::onReadyRead);
        connect(qprocess, &QProcess::started, this, &ProcessOutputModel::started);
    }

    endResetModel();
}

void ProcessOutputModel::onReadyRead()
{
    QProcess* qprocess = &process_->process;

    while (qprocess->canReadLine()) {
        char buf[1024];
        qint64 bytes = qprocess->readLine(buf, sizeof(buf));
        if (bytes > 0) {
            if (buf[bytes - 1] == '\n') {
                bytes -= 1;
            }

            lines.push_back(QString::fromUtf8(buf, bytes));
        }
    }

    if (size != (int)lines.size()) {
        beginInsertRows({}, size, lines.size() - 1);
        size = (int)lines.size();
        endInsertRows();
    }
}

void ProcessOutputModel::started()
{
    beginResetModel();
    lines.clear();
    size = 0;
    endResetModel();
}

}

#include "moc_process.cpp"
