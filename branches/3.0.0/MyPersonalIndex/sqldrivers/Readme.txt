To Rebuid:

1. Remove SQLITE_OMIT_EXTENSION from .pro file in qt/src/plugins/sqldrivers/sqlite
2. Add to sqlite.c in qt/src/3rdparty/sqlite

  #ifndef SQLITE_ENABLE_LOAD_EXTENSION
  # define SQLITE_ENABLE_LOAD_EXTENSION 1
  #endif

