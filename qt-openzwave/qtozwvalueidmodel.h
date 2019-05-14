#ifndef QTOZWVALUEIDMODEL_H
#define QTOZWVALUEIDMODEL_H

#include <QObject>
#include <QAbstractItemModel>

class QTOZW_ValueIds : public QAbstractTableModel {
    Q_OBJECT
public:
    enum ValueIdColumns {
        Label,
        Value,
        Type,
        Instance,
        CommandClass,
        Index,
        Node,
        Genre,
        Help,
        ValueIDKey,
        ValueFlags,
        ValueIdCount
    };
    Q_ENUM(ValueIdColumns)
    enum ValueIdGenres {
        Basic,
        User,
        Config,
        System,
        GenreCount
    };
    Q_ENUM(ValueIdGenres)
    enum ValueIdTypes {
        Bool,
        Byte,
        Decimal,
        Int,
        List,
        Schedule,
        Short,
        String,
        Button,
        Raw,
        BitSet,
        TypeCount
    };
    Q_ENUM(ValueIdTypes)
    enum ValueIDFlags {
        FlagCount
    };
    Q_ENUM(ValueIDFlags)
    QTOZW_ValueIds(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole);
protected:
    QVariant getValueData(uint64_t, ValueIdColumns);
    int32_t getValueRow(uint64_t _node);

    QHash<int32_t, QHash<ValueIdColumns, QVariant> > m_valueData;
};

class QTOZW_ValueIds_internal : public QTOZW_ValueIds {
    Q_OBJECT
public:
    QTOZW_ValueIds_internal(QObject *parent=nullptr);
public Q_SLOTS:
    void addValue(uint64_t _vidKey);
    void setValueData(uint64_t _vidKey, QTOZW_ValueIds::ValueIdColumns column, QVariant data);
    void setValueFlags(uint64_t _vidKey, QTOZW_ValueIds::ValueIDFlags _flags, bool _value);

};



#endif // QTOZWVALUEIDMODEL_H
