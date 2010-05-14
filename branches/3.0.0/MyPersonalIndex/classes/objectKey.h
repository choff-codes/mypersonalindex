#ifndef OBJECTKEY_H
#define OBJECTKEY_H

#include <QString>
#include <QMetaType>

enum objectType { objectType_Portfolio, objectType_Account, objectType_AA, objectType_Security, objectType_Symbol, objectType_None };

class objectKey
{
public:
    objectType type;
    QString description;
    int id;

    objectKey(const objectType &p_type = objectType_None, const QString &p_description = QString(), const int &p_id = -1):
        type(p_type), description(p_description), id(p_id) {}

    objectKey(const QString &p_symbol):
        type(objectType_Symbol), description(p_symbol), id(-1) {}

    objectKey key() const { return *this; }

    bool operator==(const objectKey &other) const;
    bool operator!=(const objectKey &other) const { return !(*this == other); }
    bool operator<(const objectKey &other) const;
};

Q_DECLARE_METATYPE(objectKey);
inline uint qHash(const objectKey &key) { return key.type ^ key.id; }


#endif // OBJECTKEY_H
