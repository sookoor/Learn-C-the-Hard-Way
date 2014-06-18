#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_DATA 512
#define MAX_ROWS 100

struct Address {
  int id;
  int set;
  char name[MAX_DATA];  // MAX_DATA
  char email[MAX_DATA]; // MAX_DATA
};

struct Database {
  int max_data;
  int max_rows;
  struct Address rows[MAX_ROWS]; // MAX_ROWS
};

struct Connection {

  // FILE struct defined by the C standard library that fopen, fread,
  // fclose, and rewind work with.
  FILE *file; 
  struct Database *db;
};

void die_no_conn(const char *message)
{

  // When a function returns an error, it sents an "external" variable
  // called errno to a number to say exactly what error happend. Print
  // the error message using perror
  if(errno) {
    perror(message);
  } else {
    printf("ERROR: %s\n", message);
  }

  exit(1);
}

void Database_close(struct Connection *conn)
{
  if(conn) {
    if(conn->file) fclose(conn->file);
    if(conn->db) free(conn->db);
    free(conn);
  }
}

void die(const char *message, struct Connection *conn)
{

  Database_close(conn);

  // When a function returns an error, it sents an "external" variable
  // called errno to a number to say exactly what error happend. Print
  // the error message using perror
  if(errno) {
    perror(message);
  } else {
    printf("ERROR: %s\n", message);
  }

  exit(1);
}

void Address_print(struct Address *addr) 
{
  printf("%d %s %s\n", addr->id, addr->name, addr->email);
}

void Database_load(struct Connection *conn)
{
  int rc = fread(conn->db, sizeof(struct Database), 1, conn->file);
  if(rc != 1) die("Failed to load database.", conn);
}

struct Connection *Database_open(const char *filename, char mode, int maxRows)
{
  struct Connection *conn = malloc(sizeof(struct Connection));
  if(!conn) die("Memory error", conn);

  conn->db = malloc(sizeof(struct Database));
  if(!conn->db) die("Memory error", conn);

  if(mode == 'c') {
    conn->file = fopen(filename, "w");
  } else {
    conn->file = fopen(filename, "r+");

    if(conn->file) {
      Database_load(conn);
    }
  }
  if(!conn->file) die("Failed to open the file", conn);
  return conn;
}

void Database_write(struct Connection *conn)
{
  rewind(conn->file);
  
  int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);

  rc = fflush(conn->file);
  if(rc == -1) die("Cannot flush database.", conn);
}

void Database_create(struct Connection *conn, int maxData, int maxRows)
{
  int i = 0;

  conn->db->max_data = maxData;
  conn->db->max_rows = maxRows;

  for(i = 0; i < maxRows; i++) {
    // make a prototype to initialize it
    struct Address addr = {.id = i, .set = 0};
    // then just assign it
    conn->db->rows[i] = addr;
  }
}

void Database_set(struct Connection *conn, int id, const char *name, const char *email)
{
  int maxData = conn->db->max_data;
  struct Address *addr = &conn->db->rows[id];
  if(addr->set) die("Alread set, delete it first", conn);

  addr->set = 1;
  char *res = strncpy(addr->name, name, maxData); // (buffer, string, sizeof(buffer)) 
  // demonstrate the strncpy bug
  if(!res) 
    {
      die("Name copy failed", conn);
    } 
  else 
    {
      addr->name[maxData-1] = '\0'; // let's hope maxData > 0
    }

  res = strncpy(addr->email, email, maxData);
  if(!res) die("Email copy failed", conn);
}

void Database_get(struct Connection *conn, int id)
{
  struct Address *addr = &conn->db->rows[id];
  
  if(addr->set) {
    Address_print(addr);
  } else {
    die("ID is not set", conn);
  }
}

void Databse_delete(struct Connection *conn, int id)
{
  struct Address addr = {.id = id, .set = 0};
  conn->db->rows[id] = addr;
}

void Databse_list(struct Connection *conn)
{
  int i = 0;
  struct Database *db = conn->db;
  
  for(i = 0; i < conn->db->max_rows; i++) {
    struct Address *cur = &db->rows[i];

    if(cur->set) {
      Address_print(cur);
    }
  }
}

int main(int argc, char *argv[])
{
  if(argc < 3) die_no_conn("USAGE: ex17 <dbfile> <action> [action params]");

  char *filename = argv[1];
  char action = argv[2][0];
  int id = 0;
  int maxData = MAX_DATA;
  int maxRows = MAX_ROWS;

  if(action == 'c') {
    switch(argc) {
    case 5:
      maxRows = atoi(argv[4]);
      //fallthrough

    case 4:
      maxData = atoi(argv[3]);
      break;
    }
  }

  struct Connection *conn = Database_open(filename, action, maxRows);
    
  if(argc > 3) id = atoi(argv[3]);
  if(id >= maxRows) die("There's not that many records.", conn);

  switch(action) {
  case 'c':
    Database_create(conn, maxData, maxRows);
    Database_write(conn);
    break;

  case 'g':
    if(argc != 4) die("Need an id to get", conn);
    
    Database_get(conn, id);
    break;

  case 's':
    if(argc != 6) die("Need id, name, email to set", conn);

    Database_set(conn, id, argv[4], argv[5]);
    Database_write(conn);
    break;

  case 'd':
    if(argc != 4) die("Nee id to delete", conn);
    
    Databse_delete(conn, id);
    Database_write(conn);
    break;

  case 'l':
    Databse_list(conn);
    break;
  default:
    die("Invalid action, only: c=create, g=get, s=set, d=del, l=list", conn);
  }

  Database_close(conn);

  return 0;
}
