#include <QObject>
#include <QSortFilterProxyModel>
#include <QQmlListProperty>

namespace Qommons {

class SortFilterProxyModel;

// ModelFilter

class ModelFilter: public QObject {
    Q_OBJECT
public:
    explicit ModelFilter(QObject* parent = nullptr)
        : QObject(parent)
    { }

    virtual void handleSourceModelReset(const SortFilterProxyModel& model);
    virtual void proxyModelCompleted(const SortFilterProxyModel& model);
    virtual bool acceptsRow(const QModelIndex& index, const SortFilterProxyModel& model) const = 0;
    virtual void invalidate();

Q_SIGNALS:
    void invalidated();
};

// AbstractRoleFilter

class AbstractRoleFilter: public ModelFilter {
    Q_OBJECT
    Q_PROPERTY(QString roleName READ roleName WRITE setRoleName NOTIFY roleNameChanged)
public:
    explicit AbstractRoleFilter(QObject* parent = nullptr);

    QString roleName() const { return m_roleName; }
    void setRoleName(const QString& value);

    void handleSourceModelReset(const SortFilterProxyModel& model) override;

Q_SIGNALS:
    void roleNameChanged();

protected:
    QVariant roleData(const QModelIndex& index, const SortFilterProxyModel& model) const;

private:
    QString m_roleName;
    int m_role = Qt::DisplayRole;
};

// EqualsFilter

class EqualsFilter: public AbstractRoleFilter {
    Q_OBJECT
    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY valueChanged)
public:
    explicit EqualsFilter(QObject* parent = nullptr);

    QVariant value() const { return m_value; }
    void setValue(const QVariant& value);

Q_SIGNALS:
    void valueChanged();

protected:
    bool acceptsRow(const QModelIndex& index, const SortFilterProxyModel& model) const override;

private:
    QVariant m_value;
};

// FilterContainer

class ModelFilterContainer: public ModelFilter {
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<ModelFilter> filters READ filters)
    Q_CLASSINFO("DefaultProperty", "filters")
public:
    explicit ModelFilterContainer(QObject* parent = nullptr);

    QQmlListProperty<ModelFilter> filters();

protected:
    bool acceptsRow(const QModelIndex& index, const SortFilterProxyModel& model) const override = 0;

    void proxyModelCompleted(const SortFilterProxyModel& model) override;

    const QList<ModelFilter*>& filterList() const { return m_filters; }

private:
    static void appendFilter(QQmlListProperty<ModelFilter>*, ModelFilter*);
    static int filterCount(QQmlListProperty<ModelFilter>*);
    static ModelFilter* filter(QQmlListProperty<ModelFilter>*, int);
    static void clearFilters(QQmlListProperty<ModelFilter>*);

    void appendFilter(ModelFilter* filter);
    int filterCount() const;
    ModelFilter* filter(int) const;
    void clearFilters();

    QList<ModelFilter*> m_filters;
};

// AllOfFilter

class AllOfFilter: public ModelFilterContainer {
    Q_OBJECT
public:
    explicit AllOfFilter(QObject* parent = nullptr);

protected:
    bool acceptsRow(const QModelIndex& index, const SortFilterProxyModel& model) const override;
};

// AnyOfFilter

class AnyOfFilter: public ModelFilterContainer {
    Q_OBJECT
public:
    explicit AnyOfFilter(QObject* parent = nullptr);

protected:
    bool acceptsRow(const QModelIndex& index, const SortFilterProxyModel& model) const override;
};

// OneOfFilter

class OneOfFilter: public ModelFilterContainer {
    Q_OBJECT
public:
    explicit OneOfFilter(QObject* parent = nullptr);

protected:
    bool acceptsRow(const QModelIndex& index, const SortFilterProxyModel& model) const override;
};

// NoneOfFilter

class NoneOfFilter: public ModelFilterContainer {
    Q_OBJECT
public:
    explicit NoneOfFilter(QObject* parent = nullptr);

protected:
    bool acceptsRow(const QModelIndex& index, const SortFilterProxyModel& model) const override;
};

// ModelSorter

class ModelSorter: public QObject {

};

// SortFilterProxyModel

class SortFilterProxyModel : public QSortFilterProxyModel {
    Q_OBJECT
    Q_PROPERTY(ModelFilter* filter READ filter WRITE setFilter NOTIFY filterChanged)
public:
    SortFilterProxyModel(QObject* parent = nullptr);
    ~SortFilterProxyModel();

    bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;

    int roleByName(const QString& roleName) const;

    ModelFilter* filter() const { return m_filter; }
    void setFilter(ModelFilter* value);

Q_SIGNALS:
    void filterChanged();

private:
    void onSourceModelChanged();

    ModelFilter* m_filter;
    QHash<int, QByteArray> m_roleNames;
};

}

Q_DECLARE_METATYPE(Qommons::ModelFilter*)
