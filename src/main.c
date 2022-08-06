#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "utils.h"

#define LOADAVGMONITOR_DBFILE "/var/lib/loadavgmonitord/main.sqlite"

int prepare_new_db_file(char *path)
{
  sqlite3 *db;
  sqlite3_open(path, &db);

  sqlite3_exec(db,
               "CREATE TABLE loadavg (id INTEGER PRIMARY KEY, "
               "measured_at DATETIME, avg1m INTEGER, avg5m INTEGER, "
               "avg15m INTEGER);",
               NULL, NULL, NULL);
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

int main(int argc, char **argv)
{
  for (;;) {
    if (check_db_file(LOADAVGMONITOR_DBFILE) == 0) {
      sqlite3 *db;
      sqlite3_stmt *stmt;
      sqlite3_open(LOADAVGMONITOR_DBFILE, &db);
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
      sqlite3_close(db);
    }
    else {
      prepare_new_db_file(LOADAVGMONITOR_DBFILE);
    }
    msleep(5000);
  }
}
