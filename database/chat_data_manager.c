#include "chat_data_manager.h"
char passw[256],ip[256];
static int callback_pass(void *data, int argc, char **argv, char **azColName){
   int i;
   fprintf(stderr, "%s: ", (const char*)data);
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   snprintf(passw,sizeof(passw),"%s",argv[i-1]);
   printf("\n");
   return 0;
}
static int callback_ip(void *data, int argc, char **argv, char **azColName){
   int i;
   fprintf(stderr, "%s: ", (const char*)data);
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   snprintf(ip,sizeof(ip),"%s",argv[i-1]?argv[i-1] : "NULL");
   printf("\n");
   return 0;
}
static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   fprintf(stderr, "%s: ", (const char*)data);
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   //snprintf(passw,sizeof(passw),"%s",argv[i-1]);
   printf("\n");
   return 0;
}



char* retrieve_userpass(char* username)
{
	sqlite3 *db;
   char *zErrMsg = 0;
   char* rc2;
   int rc;
   char sql[256];
   const char* data = "Callback function called";

   /* Open database */
   rc = sqlite3_open("dat.db", &db);
   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
   }else{
      fprintf(stderr, "Opened database successfully\n");
   }
   /* Create SQL statement */
   
   //printf("%s",sql);
   /* Execute SQL statement */
   snprintf(sql,sizeof(sql),"%s%s%s","SELECT PASSWORD from USERINFO WHERE NAME IS\"",username,"\"");
  rc = sqlite3_exec(db, sql, callback_pass, (void*)data, &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Operation done successfully\n");
   }
   sqlite3_close(db);
   //printf("%s\n",passw );
   return passw;
}

int update_ipaddress(char* username,char* ipaddr)
{
	sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   char sql[256];
   const char* data = "Callback function called";

   /* Open database */
   rc = sqlite3_open("dat.db", &db);
   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
   }else{
      fprintf(stderr, "Opened database successfully\n");
   }

   /* Create merged SQL statement */
   snprintf(sql,sizeof(sql),"%s%s%s%s%s","UPDATE USER_IP set IP = \"",ipaddr,"\"WHERE NAME IS \"",username,"\"");      
   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Operation done successfully\n");
   }
   sqlite3_close(db);
   return 0;
}

char* retrieve_ipaddress(char* username)
{
	 sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   char sql[256];
   const char* data = "Callback function called";

   /* Open database */
   rc = sqlite3_open("dat.db", &db);
   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
   }else{
      fprintf(stderr, "Opened database successfully\n");
   }

   /* Create SQL statement */
   //sql = strcat("SELECT IP_ADDRESS from USER_IP WHERE NAME IS ",username);
   snprintf(sql,sizeof(sql),"%s%s%s","SELECT IP from USER_IP WHERE NAME IS\"",username,"\"");
   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback_ip, (void*)data, &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Operation done successfully\n");
   }
   sqlite3_close(db);
   return ip;
}

