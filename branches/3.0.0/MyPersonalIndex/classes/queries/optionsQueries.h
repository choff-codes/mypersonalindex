#ifndef OPTIONSQUERIES_H
#define OPTIONSQUERIES_H

#include "queries.h"
#include "globals.h"

class optionsQueries : public queries
{
public:
    queryInfo* updateSettings(globals::settings* const s);
};

#endif // OPTIONSQUERIES_H
