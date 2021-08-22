#include <bux/oo_sqlite.h>
#include <bux/XException.h> // RUNTIME_ERROR()
#include <cstring>          // strcmp()

/* Currently not used
#include <vector>           // std::vector<>
#ifdef __unix__
#include <unistd.h>         // access()
#else
#include <io.h>             // access()
#define R_OK 0
#endif
*/

namespace bux {

//
//      Functions
//
std::string sqliteErrorSuffix(int err, char *errmsg)
{
    std::string ret = " with sqlite error("+std::to_string(err)+"): ";
    if (errmsg)
    {
        ret += errmsg;
        sqlite3_free(errmsg);
    }
    else
        ret += sqlite3_errstr(err);

    return ret;
}

void getTableViewList(sqlite3 *handle, F_ApplyStr applyTable, F_ApplyStr applyView)
{
    const C_SQLiteStmt stmt{handle, "select type,name from sqlite_master where name NOT GLOB 'sqlite_*'"};
        // Know-How from sqlite source shell.c parsing command ".tables"
    while (stmt.nextRow())
    {
        const auto type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        const auto name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        if (applyTable && !strcmp(type, "table"))
            applyTable(name);
        else if (applyView && !strcmp(type, "view"))
            applyView(name);
    }
}

/* Currently not used
std::string openPathOfDB(std::string db_url, int open_opts)
{
    std::vector<std::string> qp;
    if (open_opts & SQLITE_OPEN_READONLY)
        qp.emplace_back("mode=ro");
    if (open_opts & SQLITE_OPEN_READWRITE)
        qp.emplace_back(access(db_url.c_str(), R_OK)? "mode=rwc": "mode=rw");
    if (open_opts & SQLITE_OPEN_MEMORY)
        qp.emplace_back("mode=memory");
    if (open_opts & SQLITE_OPEN_NOMUTEX)
        qp.emplace_back("nolock=1");
    if (open_opts & SQLITE_OPEN_FULLMUTEX)
        qp.emplace_back("nolock=0");
    if (open_opts & SQLITE_OPEN_SHAREDCACHE)
        qp.emplace_back("cache=shared");
    if (open_opts & SQLITE_OPEN_PRIVATECACHE)
        qp.emplace_back("cache=private");

    if (!qp.empty())
    {
#if _WIN32
        if (db_url[1] == ':' && isalpha(db_url[0]))
            // Started by drive letter
            db_url.insert(0, "file:///");
#endif
        char delim = db_url.find('?') == std::string::npos? '?': '&';
        for (auto &i: qp)
        {
            db_url += delim;
            db_url += i;
            delim = '&';
        }
    }
    return db_url;
}*/

//
//      Implemen Classes
//
C_SQLite::C_SQLite(const char *path, int flags)
{
    auto err = open(path, flags);
    if (SQLITE_OK != err)
        RUNTIME_ERROR("Fail to open sqlite {} with error({}): {}", path, err, sqlite3_errstr(err));
}

C_SQLite::C_SQLite(const std::nothrow_t&, const char *path, int flags)
{
    auto err = open(path, flags);
    if (SQLITE_OK != err)
    {
        sqlite3_close_v2(m_sqlite);
        m_sqlite = nullptr;
    }
}

C_SQLite::~C_SQLite()
{
    sqlite3_close_v2(m_sqlite);
}

void C_SQLite::exec(const std::string &sql) const
{
    char *errmsg;
    const int err = sqlite3_exec(m_sqlite, sql.c_str(), nullptr, nullptr, &errmsg);
    if (SQLITE_OK != err)
        RUNTIME_ERROR("Exec \"{}\"{}", sql, sqliteErrorSuffix(err,errmsg));
}

int C_SQLite::open(const char *path, int flags)
{
    sqlite3_config(SQLITE_CONFIG_URI, 1);
    return sqlite3_open_v2(path, &m_sqlite, flags, nullptr);
}

C_SQLiteStmt::C_SQLiteStmt(sqlite3 *handle, const std::string &sql)
{
    prepare(handle, sql);
}

void C_SQLiteStmt::execute() const
{
    while (nextRow());
    sqlite3_reset(m_stmt);
}

bool C_SQLiteStmt::nextRow() const
{
    switch (int err = sqlite3_step(m_stmt))
    {
    case SQLITE_ROW:
        return true;
    case SQLITE_DONE:
        return false;
    default:
        RUNTIME_ERROR("Exec statement{}", sqliteErrorSuffix(err));
    }
}

void C_SQLiteStmt::prepare(sqlite3 *handle, const std::string &sql)
{
    reset();
    const int err = sqlite3_prepare_v2(handle, sql.c_str(), int(sql.size()+1), &m_stmt, nullptr);
    /* As of 2014/5/20, the official sqlite3_prepare() document mentions:
     * If the caller knows that the supplied string is nul-terminated, then there is a small
     * performance advantage to be gained by passing an nByte parameter that is equal to the number
     * of bytes in the input string including the nul-terminator bytes as this saves SQLite from
     * having to make a copy of the input string.
     */
    if (SQLITE_OK != err)
        RUNTIME_ERROR("Prepare '{}'{}", sql, sqliteErrorSuffix(err));
}

void C_SQLiteStmt::reset(sqlite3_stmt *stmt)
{
   sqlite3_finalize(m_stmt);
   m_stmt = stmt;
}

} // namespace bux
