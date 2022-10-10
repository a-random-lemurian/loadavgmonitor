#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "utils.h"
#include "argparse.h"

#define LOADAVGMONITOR_DBFILE "/var/lib/loadavgmonitord/main.sqlite"

int table_exists_in_sqlite_file(char *table, sqlite3 *db)
{
  sqlite3_stmt *stmt;

  const char *sql = "SELECT 1 FROM sqlite_master"
                    "WHERE type='table' AND name=?";

  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
  sqlite3_bind_text(stmt, 1, table, -1, SQLITE_TRANSIENT);
  rc = sqlite3_step(stmt);

  int table_exists;
  if (rc == SQLITE_ROW) {
      table_exists = 1;
  }
  else if (rc == SQLITE_DONE) {
      table_exists = 0;
  }

  sqlite3_finalize(stmt);

  return table_exists;
}

void prepare_new_db_file_from_dbconn(sqlite3 *db)
{
  sqlite3_exec(db,
               "CREATE TABLE IF NOT EXISTS "
               "loadavg (id INTEGER PRIMARY KEY, "
               "measured_at DATETIME, avg1m INTEGER, avg5m INTEGER, "
               "avg15m INTEGER);",
               NULL, NULL, NULL);

  sqlite3_exec(db,
               "CREATE TABLE IF NOT EXISTS "
               "memory (id INTEGER PRIMARY KEY, "
               "measured_at DATETIME, free INTEGER,"
               "shared INTEGER, used INTEGER, buffer INTEGER);",
               NULL, NULL, NULL);
}

void prepare_new_db_file(char *path)
{
  sqlite3 *db;
  sqlite3_open(path, &db);
  prepare_new_db_file_from_dbconn(db);
}

/*
 * Check for the existence of a database file
 *
 * Check for this file:
 * /var/lib/loadavgmonitord/main.sqlite
 */
int check_db_file(char* file)
{
  if (access(file, F_OK) == 0) {
    return 0;
  }
  else {
    prepare_new_db_file(file);
    return 1;
  }
}

void monitor(char* file, int delay_ms)
{
  sqlite3 *db;
  sqlite3_stmt *stmt;
  sqlite3_open(file, &db);

  for (;;) {
    monitor_loadavg(db);
    msleep(delay_ms);
  }

  msleep(delay_ms);
}

void monitor_loadavg(sqlite3 *db)
{
  sqlite3_stmt *stmt;
  sqlite3_prepare(db,
                  "INSERT INTO loadavg (measured_at, "
                  "avg1m, avg5m, avg15m)"
                  "VALUES (datetime('now'), ?, ?, ?)",
                  -1, &stmt, NULL);

  double loadavg[3];
  int rc = getloadavg(loadavg, 3);

  sqlite3_bind_double(stmt, 1, loadavg[0]);
  sqlite3_bind_double(stmt, 2, loadavg[1]);
  sqlite3_bind_double(stmt, 3, loadavg[2]);
  sqlite3_step(stmt);
  sqlite3_finalize(stmt);
}

int main(int argc, char **argv)
{
  int delay_ms = 5000;
  char* file = LOADAVGMONITOR_DBFILE;

  struct argparse_option opts[] = {
    OPT_HELP(),
    OPT_GROUP("Basic options"),
    OPT_STRING('f', "file", &file, "SQLite database file to write load average readings to, "LOADAVGMONITOR_DBFILE" by default"),
    OPT_INTEGER('d', "delay", &delay_ms, "Delay between each measurement in milliseconds, 5 seconds by default"),
    OPT_END()
  };

  struct argparse ap;
  argparse_init(&ap, opts, NULL, 0);
  argparse_describe(&ap, "Load average monitor",
                    "-------------\n"
                    "https://github.com/a-random-lemurian/loadavgmonitord\n"
                    "Compiled "__DATE__" "__TIME__"");

  argparse_parse(&ap, argc, (const char**)argv);
  monitor(file, delay_ms);
}
