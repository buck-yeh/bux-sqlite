
## Table of Contents

   * [Rationale](#rationale)
   * [API Summary](#api-summary)
      * [Recurring Types and Their Safe Counterparts](#recurring-types-and-their-safe-counterparts)
   * [Installation](#installation)
      * [in <a href="https://archlinux.org/" rel="nofollow">ArchLinux</a>](#in-archlinux)
      * [from github in any of <a href="https://distrowatch.com/" rel="nofollow">Linux distros</a>](#from-github-in-any-of-linux-distros)

*(Created by [gh-md-toc](https://github.com/ekalinin/github-markdown-toc))*

## Rationale

The idea is to minimize boilerplate code when doing the same things using native [SQLite API](https://www.sqlite.org/capi3ref.html). Only recurring usages are wrapped into helper classes or plain functions. Mixed uses of this module and the native API are expected. Always prefer `bux::C_SQLite`, `bux::C_SQLiteStmt` over `sqlite3*`, `sqlite3_stmt*` and you will be fine.  

## API Summary

### Recurring Types and Their Safe Counterparts

| Original Type | Wrapper Class | 
|:-------------:|:--------------|
| `sqlite3*`      |  `bux::C_SQLite` |
| `sqlite3_stmt*` |  `bux::C_SQLiteStmt` |

1. The right colum (_class type_) of each row above can be cast to the left column (_native SQLite pointer type_) implicitly & _safely_.
2. `bux::C_SQLite` can only be constructed by a valid SQLite databse file name.
3. `bux::C_SQLiteStmt` is contructed by passing a `bux::C_SQLite` instance and a SQL statement. And it is the recommended way to bind SQL arguments by calling native `sqlite3_bind_\w+()` functions before its own `execute()` method.

## Installation

### in [ArchLinux](https://archlinux.org/)

1. Make sure you have installed [`yay`](https://aur.archlinux.org/packages/yay/) or any other [pacman wrapper](https://wiki.archlinux.org/index.php/AUR_helpers).
2. `yay -S bux-sqlite` to install. `bux` is also installed with it.
3. `yay -Ql bux-sqlite` to see the installed files:

   ~~~bash
   bux-sqlite /usr/
   bux-sqlite /usr/include/
   bux-sqlite /usr/include/bux/
   bux-sqlite /usr/include/bux/oo_sqlite.h
   bux-sqlite /usr/lib/
   bux-sqlite /usr/lib/libbux-sqlite.a
   bux-sqlite /usr/share/
   bux-sqlite /usr/share/licenses/
   bux-sqlite /usr/share/licenses/bux-sqlite/
   bux-sqlite /usr/share/licenses/bux-sqlite/LICENSE
   ~~~

4. Include the sole header file by prefixing the header name with `bux/`:

   ~~~c++
   #include <bux/oo_sqlite.h>
   ~~~

   *p.s.* Compiler is expected to search `/usr/include` by default.
5. If directly using `gcc` or `clang` is intended, the required compiler flags are `-lbux-sqlite -lbux`

### from github in any of [Linux distros](https://distrowatch.com/)

1. Make sure you have installed `cmake` `make` `gcc` `git` `fmt` `sqlite`, or the likes. Known package names in different distros/package-managers:
   | Distro/PkgMngr | Package Name |
   |:----------------:|:------------:|
   | ArchLinux/yay | `fmt`, `sqlite` |
   | Fedora/dnf | `fmt-devel`, `sqlite-devel` |

2. ~~~bash
   git clone https://github.com/buck-yeh/bux-sqlite.git
   cd bux-sqlite
   cmake -D FETCH_DEPENDEES=1 -D DEPENDEE_ROOT=_deps .
   make -j
   ~~~

3. Include `include/bux/oo_sqlite.h` and link with `src/libbux-sqlite.a`
