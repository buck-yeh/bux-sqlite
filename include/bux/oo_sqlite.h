#ifndef oo_sqlite_H_
#define oo_sqlite_H_

/*! \file
    \brief SQLite API Wrappers
*/

#include <sqlite3.h>    // sqlite3, sqlite3_stmt
#include <string>       // std::string
#include <functional>   // std::function<>

namespace bux {

//
//      Types
//
class C_SQLite
{
public:

    // Nonvirtuals
    C_SQLite(const char *path, int flags = SQLITE_OPEN_READWRITE);
    C_SQLite(const std::nothrow_t&, const char *path, int flags = SQLITE_OPEN_READWRITE);
    ~C_SQLite();
    C_SQLite(const C_SQLite &) = delete;
    C_SQLite &operator=(const C_SQLite &) = delete;
    operator sqlite3*() const { return m_sqlite; }
    void exec(const std::string &sql) const;

private:

    // Data
    sqlite3     *m_sqlite{};

    // Nonvirtuals
    int open(const char *path, int flags);
};

class C_SQLiteStmt
{
public:

    // Nonvirtuals
    constexpr C_SQLiteStmt() = default;
    C_SQLiteStmt(sqlite3 *handle, const std::string &sql);
    C_SQLiteStmt(const C_SQLiteStmt &) = delete;
    ~C_SQLiteStmt()                     { reset(); }
    void operator=(sqlite3_stmt *stmt)  { reset(stmt); }
    operator sqlite3_stmt*() const      { return m_stmt; }
    void execute() const;
    bool nextRow() const;
    void prepare(sqlite3 *handle, const std::string &sql);
    void reset(sqlite3_stmt *stmt = nullptr);

private:

    // Data
    sqlite3_stmt    *m_stmt{};
};

typedef std::function<void(const std::string&)> F_ApplyStr;

//
//      Functions
//
void getTableViewList(sqlite3 *handle, F_ApplyStr applyTable, F_ApplyStr applyView = {});
std::string openPathOfDB(std::string db_url, int open_opts);
std::string sqliteErrorSuffix(int err, char *errmsg = nullptr);

} // namespace bux

#endif // oo_sqlite_H_
