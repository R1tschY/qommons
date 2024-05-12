#include "sortfilterproxymodel.h"

#include <utility>
#include <algorithm>
#include <QLoggingCategory>

namespace Qommons {

static Q_LOGGING_CATEGORY(logger, "Qommons.QSortFilterProxyModel");

// ModelFilter

void ModelFilter::handleSourceModelReset(const SortFilterProxyModel& model)
{
    // noop
}

void ModelFilter::invalidate()
{
    Q_EMIT invalidated();
}

void ModelFilter::proxyModelCompleted(const SortFilterProxyModel &model)
{
    // noop
}

// AbstractRoleFilter

AbstractRoleFilter::AbstractRoleFilter(QObject* parent)
    : ModelFilter(parent)
{ }

void AbstractRoleFilter::setRoleName(const QString& value)
{
    if (value != m_roleName) {
        m_roleName = value;
        Q_EMIT roleNameChanged();
    }
}

void AbstractRoleFilter::handleSourceModelReset(const SortFilterProxyModel& model)
{
    m_role = model.roleByName(m_roleName);
}

QVariant AbstractRoleFilter::roleData(const QModelIndex &index, const SortFilterProxyModel &model) const
{
    return model.sourceModel()->data(index, m_role);
}

// EqualsFilter

EqualsFilter::EqualsFilter(QObject* parent)
    : AbstractRoleFilter(parent)
{ }

void EqualsFilter::setValue(const QVariant& value)
{
    if (value != m_value) {
        m_value = value;
        Q_EMIT valueChanged();
        invalidate();
    }
}

bool EqualsFilter::acceptsRow(const QModelIndex& index, const SortFilterProxyModel& model) const
{
    return roleData(index, model) == m_value;
}

// ModelFilterContainer

QQmlListProperty<ModelFilter> ModelFilterContainer::filters()
{
    return {
        this, this,
        &ModelFilterContainer::appendFilter,
        &ModelFilterContainer::filterCount,
        &ModelFilterContainer::filter,
        &ModelFilterContainer::clearFilters
    };
}

void ModelFilterContainer::proxyModelCompleted(const SortFilterProxyModel &model)
{
    for (auto* filter : filterList()) {
        filter->proxyModelCompleted(model);
    }
}

void ModelFilterContainer::handleSourceModelReset(const SortFilterProxyModel &model)
{
    for (auto* filter : filterList()) {
        filter->handleSourceModelReset(model);
    }
}

void ModelFilterContainer::appendFilter(QQmlListProperty<ModelFilter>* list, ModelFilter* filter)
{
    reinterpret_cast<ModelFilterContainer*>(list->object)->appendFilter(filter);
}

int ModelFilterContainer::filterCount(QQmlListProperty<ModelFilter>* list)
{
    return reinterpret_cast<ModelFilterContainer*>(list->object)->filterCount();
}

ModelFilter *ModelFilterContainer::filter(QQmlListProperty<ModelFilter>* list, int index)
{
    return reinterpret_cast<ModelFilterContainer*>(list->object)->filter(index);
}

void ModelFilterContainer::clearFilters(QQmlListProperty<ModelFilter>* list)
{
    reinterpret_cast<ModelFilterContainer*>(list->object)->clearFilters();
}

void ModelFilterContainer::appendFilter(ModelFilter *filter)
{
    connect(filter, &ModelFilter::invalidated, this, &ModelFilter::invalidate);
    m_filters.append(filter);
    invalidate();
}

int ModelFilterContainer::filterCount() const
{
    return m_filters.size();
}

ModelFilter *ModelFilterContainer::filter(int index) const
{
    return m_filters.at(index);
}

void ModelFilterContainer::clearFilters()
{
    for (auto* filter : filterList()) {
        disconnect(filter);
    }
    m_filters.clear();
    invalidate();
}

// AllOfFilter

AllOfFilter::AllOfFilter(QObject *parent)
    : ModelFilterContainer(parent)
{ }

bool AllOfFilter::acceptsRow(const QModelIndex &index, const SortFilterProxyModel& model) const
{
    return std::all_of(filterList().begin(), filterList().end(), [&](ModelFilter* filter) {
        return filter->acceptsRow(index, model);
    });
}

// AnyOfFilter

AnyOfFilter::AnyOfFilter(QObject *parent)
    : ModelFilterContainer(parent)
{ }

bool AnyOfFilter::acceptsRow(const QModelIndex &index, const SortFilterProxyModel &model) const
{
    return std::any_of(filterList().begin(), filterList().end(), [&](ModelFilter* filter) {
        return filter->acceptsRow(index, model);
    });
}

// OneOfFilter

OneOfFilter::OneOfFilter(QObject *parent)
    : ModelFilterContainer(parent)
{ }

bool OneOfFilter::acceptsRow(const QModelIndex &index, const SortFilterProxyModel &model) const
{
    return std::count_if(filterList().begin(), filterList().end(), [&](ModelFilter* filter) {
        return filter->acceptsRow(index, model);
    }) == 1;
}

// NoneOfFilter

NoneOfFilter::NoneOfFilter(QObject *parent)
    : ModelFilterContainer(parent)
{ }

bool NoneOfFilter::acceptsRow(const QModelIndex &index, const SortFilterProxyModel &model) const
{
    return !std::any_of(filterList().begin(), filterList().end(), [&](ModelFilter* filter) {
        return filter->acceptsRow(index, model);
    });
}

// SortFilterProxyModel

SortFilterProxyModel::SortFilterProxyModel(QObject* parent)
    : QSortFilterProxyModel(parent)
    , m_filter(nullptr)
{
    connect(this, &QAbstractProxyModel::sourceModelChanged, this, &SortFilterProxyModel::onSourceModelChanged);
    connect(this, &QAbstractItemModel::rowsInserted, this, &SortFilterProxyModel::countChanged);
    connect(this, &QAbstractItemModel::rowsRemoved, this, &SortFilterProxyModel::countChanged);
    connect(this, &QAbstractItemModel::modelReset, this, &SortFilterProxyModel::countChanged);
    connect(this, &QAbstractItemModel::layoutChanged, this, &SortFilterProxyModel::countChanged);
}

SortFilterProxyModel::~SortFilterProxyModel() = default;

bool SortFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &sourceParent) const
{
    QModelIndex sourceIndex = sourceModel()->index(source_row, 0, sourceParent);
    if (!sourceIndex.isValid()) {
        return false;
    }

    if (!m_complete) {
        return false;
    }

    return m_filter == nullptr || m_filter->acceptsRow(sourceIndex, *this);
}

int SortFilterProxyModel::roleByName(const QString &roleName) const
{
    return m_roleNames.key(roleName.toUtf8(), Qt::DisplayRole);
}

void SortFilterProxyModel::setFilter(ModelFilter *value)
{
    if (m_filter != value) {
        if (m_filter != nullptr) {
            disconnect(m_filter);
        }

        m_filter = value;

        if (value != nullptr) {
            connect(
                        m_filter, &ModelFilter::invalidated,
                        this, &SortFilterProxyModel::invalidateFilter);
            if (m_complete) {
                m_filter->proxyModelCompleted(*this);
            }
        }

        Q_EMIT filterChanged();
    }
}

int SortFilterProxyModel::count() const
{
    return rowCount();
}

QVariantMap SortFilterProxyModel::get(int row) const
{
    QModelIndex modelIndex = index(row, 0);
    const QHash<int, QByteArray> roles = roleNames();

    QVariantMap result;
    for (auto iter = roles.begin(); iter != roles.end(); ++iter) {
        result.insert(iter.value(), data(modelIndex, iter.key()));
    }
    return result;
}

QVariant SortFilterProxyModel::get(int row, const QString& roleName) const
{
    return data(index(row, 0), roleByName(roleName));
}

void SortFilterProxyModel::onSourceModelChanged()
{
    if (m_sourceModel) {
        disconnect(m_sourceModel);
    }
    m_sourceModel = sourceModel();
    if (m_sourceModel) {
        connect(m_sourceModel, &QAbstractItemModel::modelReset, this, &SortFilterProxyModel::onSourceModelReset);
    }

    onSourceModelReset();
}

void SortFilterProxyModel::onSourceModelReset()
{
    if (!m_complete) {
        return;
    }

    if (m_sourceModel) {
        m_roleNames = m_sourceModel->roleNames();
    } else {
        m_roleNames.clear();
    }

    if (m_filter != nullptr) {
        m_filter->handleSourceModelReset(*this);
    }
}

void SortFilterProxyModel::classBegin() { };

void SortFilterProxyModel::componentComplete()
{
    m_complete = true;

    if (m_filter != nullptr) {
        m_filter->proxyModelCompleted(*this);
    }

    if (m_sourceModel) {
        onSourceModelReset();
    }

    invalidate();
}

}

#include "sortfilterproxymodel.moc"

