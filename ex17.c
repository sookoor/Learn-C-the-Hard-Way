#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_DATA 512
#define MAX_ROWS 100

typedef struct {
  int id;
  int set;
  char *name;  // MAX_DATA
  char *email; // MAX_DATA
} Address;

typedef struct {
  int max_data;
  int max_rows;
  Address **rows; // USE ARRAY OF POINTERS
} Database;

typedef struct {
  // FILE struct defined by the C standard library that fopen, fread,
  // fclose, and rewind work with.
  FILE *file; 
  Database *db;
} Connection;

// Global variable
Connection *conn;

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

void Database_close()
{
  size_t i;
  if(conn) {
    if(conn->db && conn->db->rows) {
      for(i = 0; i < conn->db->max_rows; i++) {
        Address *cur = conn->db->rows[i];
        free(cur->name);
        free(cur->email);
        free(cur);
      }
      free(conn->db->rows);
    }
    if(conn->file) fclose(conn->file);
    if(conn->db) free(conn->db);
    free(conn);
  }
}

void die(const char *message)
{

  Database_close();

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

void Address_print(Address *addr) 
{
  printf("%d %s %s\n", addr->id, addr->name, addr->email);
}

void Database_load(Connection *conn)
{
  // Each database will have two `int` values. Read those first
  if(!(conn->db && conn->file)) die("Database_load: Invalid Connection info");

  // fread(ptr, size, nmemb, stream): read data from given `stream` into
  // the array pointed to by `ptr`
  // ptr: pointer to a block of memory with minimum size of `size` * `nmemb`
  // size: size in bytes of each element to be read
  // nmemb: number of elements, each one with a size of `size` bytes
  // stream: pointer to a file object that specifies an input stream
  if(fread(&conn->db->max_data, sizeof(conn->db->max_data), 1, conn->file) != 1) die("Database_load: Could not read max_data");

  if(fread(&conn->db->max_rows, sizeof(conn->db->max_rows), 1, conn->file) != 1) die("Database_load: Could not read max_rows");

  conn->db->rows = (Address**)malloc(sizeof(Address *) * conn->db->max_rows);
  if(!(conn->db->rows)) die("Database_load: Could not create max_rows Address structures");
    
  // Now load each address structure
  size_t i = 0;
  for(i = 0; i < conn->db->max_rows; i++) {

    // read two int values first
    conn->db->rows[i] = (Address*)malloc(sizeof(Address));
    Address *row = conn->db->rows[i];
    if(fread(&row->id, sizeof(row->id), 1, conn->file) != 1) die("Database_load: Could not read Address->id");
      
    if(fread(&row->set, sizeof(row->set), 1, conn->file) != 1) die("Database_load: Could not read Address->set");

    // Allocate two buffers large enough for our data
    row->name = malloc(conn->db->max_data * sizeof(*row->name));
    row->email = malloc(conn->db->max_data * sizeof(*row->email));
    if (!(row->name && row->email)) die("Database_load: Failed to allocate Address strings");
      
    // Now just read both strings
    if(fread(row->name, conn->db->max_data * sizeof(*row->name), 1, conn->file) != 1) die("Database load: Failed to read Address->name");
     
    if(fread(row->email, conn->db->max_data * sizeof(*row->email), 1, conn->file) != 1) die("Database_load: Failed to read Address->email");
      
  }
}

void Database_open(const char *filename, char mode)
{
  conn = malloc(sizeof(Connection));
  if(!conn) die("Memory error");

  conn->db = malloc(sizeof(Database));
  if(!conn->db) die("Memory error");

  if(mode == 'c') {
    conn->file = fopen(filename, "w");
  } else {
    conn->file = fopen(filename, "r+");

    if(conn->file) {
      Database_load(conn);
    }
  }
  if(!conn->file) die("Failed to open the file");
}

void Database_write()
{
  rewind(conn->file);

  // Write max_data and max_rows to the file first
  if(fwrite(&conn->db->max_data, sizeof(conn->db->max_data), 1, conn->file) != 1) die("Database_write: Failed to write max_data");
  if(fwrite(&conn->db->max_rows, sizeof(conn->db->max_rows), 1, conn->file) != 1) die("Database write: Failed to write max_rows");

  // Write address list
  int i = 0;
  for(i = 0; i < conn->db->max_rows; i++) {
    Address *row = conn->db->rows[i];
    if(fwrite(&row->id, sizeof(row->id), 1, conn->file) != 1) die("Database_write: Failed to write Address->id");
    if(fwrite(&row->set, sizeof(row->set), 1, conn->file) != 1) die("Database_write: Failed to write Address->set");
    if(fwrite(row->name, sizeof(char)*conn->db->max_data, 1, conn->file) != 1) die("Database_write: Failed to write Address->name");
    if(fwrite(row->email, sizeof(char)*conn->db->max_data, 1, conn->file) != 1) die("Database write: Failed to write Address->email");
  }

  int rc = fflush(conn->file);
  if(rc == -1) die("Cannot flush database.");
}

void Database_create(int maxData, int maxRows)
{
  int i = 0;
  conn->db->max_data = maxData;
  conn->db->max_rows = maxRows;
  conn->db->rows = (Address**)malloc(sizeof(Address*) * maxRows);

  for(i = 0; i < maxRows; i++) {
    conn->db->rows[i] = (Address*)malloc(sizeof(Address));
    conn->db->rows[i]->id = i;
    conn->db->rows[i]->set = 0;
    conn->db->rows[i]->name = (char *)malloc(conn->db->max_data);

    // memset(str, c, n) copies the character `c` to the first `n`
    // characters of the string pointed to by the argument `str`. The
    // function returns a pointer to the memory area `str`
    conn->db->rows[i]->name = (char *)memset(conn->db->rows[i]->name, ' ', conn->db->max_data);

    conn->db->rows[i]->email = (char *)malloc(conn->db->max_data);
    conn->db->rows[i]->email = (char *)memset(conn->db->rows[i]->email, ' ', conn->db->max_data);
  }
}

void Database_set(int id, const char *name, const char *email)
{

  if(!(conn && conn->db && conn->db->rows && conn->db->rows[id])) return;

  Address *addr = conn->db->rows[id];
  int maxData = conn->db->max_data;
  if(addr->set == 1) die("Alread set, delete it first");

  addr->set = 1;
  addr->name = malloc(sizeof(char) * maxData);
  addr->email = malloc(sizeof(char) * maxData);

  char *res = strncpy(addr->name, name, maxData); // (buffer, string, sizeof(buffer)) 
  // demonstrate the strncpy bug
  if(!res) 
    {
      die("Name copy failed");
    } 
  else 
    {
      addr->name[maxData-1] = '\0'; // let's hope maxData > 0
    }

  res = strncpy(addr->email, email, maxData);
  if(!res) die("Email copy failed");
}

void Database_get(int id)
{
  Address *addr = conn->db->rows[id];
  
  if(addr->set) {
    Address_print(addr);
  } else {
    die("ID is not set");
  }
}

void Database_delete(int id)
{
  conn->db->rows[id]->set = 0;
}

void Database_list()
{
  int i = 0;
  Database *db = conn->db;
  
  for(i = 0; i < conn->db->max_rows; i++) {
    Address *cur = db->rows[i];
    
    if(cur->set) {
      Address_print(cur);
    }
  }
}

void Database_find(char *field, char *key)
{
  int i = 0;
  Database *db = conn->db;

  for(i = 0; i < conn->db->max_rows; i++) {
    Address *cur = db->rows[i];
    
    if(cur->set) {
      if(strcmp(field, "id") == 0) { 
        if(cur->id == atoi(key)) {
          Address_print(cur);
          return;
        }
      }
      else if(strcmp(field, "name") == 0) {  
        if(strcmp(cur->name, key) == 0) {
          Address_print(cur);
          return;
        }
      }
      else if(strcmp(field, "email") == 0) { 
        if(strcmp(cur->email, key) == 0) {
          Address_print(cur);
          return;
        }
      }
      else {
        die("Invalid field, only: id, name, email");
      }
    }
  }
  printf("%s %s not in database\n", field, key);
}

int main(int argc, char *argv[])
{
  if(argc < 3) die_no_conn("USAGE: ex17 <dbfile> <action> [action params]");

  char *filename = argv[1];
  char action = argv[2][0];
  Database_open(filename, action);
  int id = 0;

  int max_data = MAX_DATA;
  int max_rows = MAX_ROWS;

  switch(action) {
  case 'c':
    switch(argc) {
    case 5:
      max_rows = atoi(argv[4]);
      //fallthrough
    case 4:
      max_data = atoi(argv[3]);
      break;
    }
    Database_create(max_data, max_rows);
    Database_write();
    break;

  case 'g':
    if(argc != 4) die("Need an id to get");
    
    id = atoi(argv[3]); 
    if(id >= conn->db->max_rows) die("There's not that many records.");
    
    Database_get(id);
    break;

  case 's':
    if(argc != 6) die("Need id, name, email to set");

    id = atoi(argv[3]); 
    if(id >= conn->db->max_rows) die("There's not that many records.");

    Database_set(id, argv[4], argv[5]);
    Database_write();
    break;

  case 'd':
    if(argc != 4) die("Need id to delete");

    id = atoi(argv[3]); 
    if(id >= conn->db->max_rows) die("There's not that many records.");
    
    Database_delete(id);
    Database_write();
    break;

  case 'l':
    Database_list();
    break;

  case 'f':
    if(argc != 5) die("Need field and key to find");

    Database_find(argv[3], argv[4]);
    break;
  default:
    die("Invalid action, only: c=create, g=get, s=set, d=del, l=list, f=find");
  }

  Database_close();

  return 0;
}
