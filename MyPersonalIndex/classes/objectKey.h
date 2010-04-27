#ifndef OBJECTKEY_H
#define OBJECTKEY_H

#include <QtCore>

enum objectType { objectType_Portfolio, objectType_Account, objectType_AA, objectType_Security, objectType_Symbol };

class objectKey
{
public:
    objectType type;
    int id;
    QString description;

    objectKey(): type(objectType_Portfolio), id(-1) {}
    objectKey(const objectType &p_type, const int &p_id, const QString &p_description): type(p_type), id(p_id), description(p_description) {}

    bool operator==(const objectKey &other) const;
    bool operator!=(const objectKey &other) const { return !(*this == other); }
    bool operator<(const objectKey &other) const;
};

inline uint qHash(const objectKey &key) { return key.type ^ key.id; }

#endif // OBJECTKEY_H
