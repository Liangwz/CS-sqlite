#include "sqlite_main.h"
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

// #ifdef WIN32
// #pragma comment(lib, "sqlite3.lib")
// #endif
// #ifndef boolean
// #define boolean
// /* 定义布尔型变量 */
// typedef enum boolean {false, true} bool;
// #endif
// #ifndef BOOLEAN
// #define BOOLEAN
// /* 定义布尔型变量 */
// typedef enum BOOLEAN {FALSE, TRUE} BOOL;
// #endif
// /* 定义消息变量 */
// typedef enum SQLT_MSG {SQLT_ERROR = -1, SQLT_DONE, SQLT_ROW} sqlt_msg;
// /* 定义NULL字符 */
// #define SQLT_NOTHING	("(null)")
// /* 定义NULL字符 */
// #ifndef NULL
// #define NULL	0
// #endif






/*---------------------------------------------------------------------------*/
/* 函数功能：往数据库添加数据
* 参数说明：pSqlite3 发生错误的数据库指针
* 返回说明：返回NULL表示失败。
* 注意事项：
* 使用举例：
*
* */
// struct {
//  char*name; char*occ;
//  } values[10]={
//  "Li Lei", "CEO",    "Han Meimei", "CPO",
//  "Lily", "COO",         "Lucy", "CFO",
//  "Lily", "COO",         "Lucy", "CFO",
//  "Lily", "COO",         "Lucy", "CFO",
//  "Lily", "COO",         "Lucy", "CFO",
//  };

typedef struct people{
	char*name; char*occ;
}Tpeople;
typedef Tpeople *Ppeople;
Tpeople values[10]={
	"Li Lei", "CEO",    "Han Meimei", "CPO",
	"Lily", "COO",         "Lucy", "CFO",
	"Lily", "COO",         "Lucy", "CFO",
	"Lily", "COO",         "Lucy", "CFO",
	"Lily", "COO",         "Lucy", "CFO",
 };
typedef struct machine{
	char cID[5];
	char cMacid[16];	//wol macid
	char cTime[15];		//wol time
	char cDBnum[4];		//datebase order
}Tmachine;
typedef Tmachine *Pmachine;
Tmachine macval[10]={
	"Li Lei", "CEO","1",    "Han Meimei", "CPO","2",
		"Lily", "COO","3",         "Lucy", "CFO","4",
		"Lily", "COO","5",         "Lucy", "CFO","6",
		"Lily", "COO","7",         "Lucy", "CFO","8",
		"Lily", "COO","9",         "Lucy", "CFO","10",
 };


// struct {
// 	char*name; int occ;
// } values[]={
// 	"Li Lei", 22,    "Han Meimei", 34,
// 		"Lily", 23,         "Lucy", 33,
//  };

#define LEN(x) sizeof(x)/sizeof(x[0])

// int sqlite_operation_add(void)
// {
// 		int i;
// 		sqlite3 *db;
// 		sqlite3_stmt *stmt;
// 		char name[16], occ[16];
// 		char*sql_drop="drop table if exists people;";
// 		char*sql_create="create table people (name, occupation);";
// 		char*sql_insert="insert into people values (?, ?);";
// 		char*sql_select="select * from people;";
// 		Tpeople inval = {"lwz","dev"};
// 
// 	if ( (db =open_sqlite("/mnt/hgfs/gtk_using/sqlite_mode/test_sqlite.db")) == NULL) 
// 	{
//   		printf("sqlite open err(%s)", format_sqlite_err(db));
// 	}
// 	printf("shwo the db:%d\n", db);
// // 	 execute_sql(db,sql_drop);
// 	 execute_sql(db,sql_create);
//      insert_sqlite_data(db,&inval);
// 
//      sqlite3_prepare(db, sql_select, strlen(sql_select), &stmt, NULL);
//      i=0;
//  while(SQLITE_DONE !=sqlite3_step(stmt))
// 	{
//          printf("Result[%d]: name=%s, occ=%s\n",
//              i++,
// 			sqlite3_column_text(stmt, 0),
// 			 sqlite3_column_text(stmt, 1));
// 	}
// 		
// 	sqlite3_finalize(stmt);
// 	sqlite3_close(db);
// 	return 0;
// }
int sqlite_operation_add(void)
{
	int i;
	sqlite3 *db;
	sqlite3_stmt *stmt;
	char name[16], occ[16];
	char*sql_drop="drop table if exists machine;";
	char*sql_create="create table machine (macid, time, mac_order);";//"create table people (name, occupation);";
	char*sql_insert="insert into machine values (?, ?, ?);";
 	char*sql_select="select * from machine;";
//	char*sql_select="select macid from machine limite 1;";
/*	Tpeople inval = {"lwz","dev"};*/
	
	if ( (db =open_sqlite("/mnt/hgfs/gtk_using/sqlite_mode/test_sqlite.db")) == NULL) 
	{
		printf("sqlite open err(%s)", format_sqlite_err(db));
	}
	printf("shwo the db:%d\n", db);
 // 	execute_sql(db,sql_drop);
	execute_sql(db,sql_create);
	insert_sqlite_data(db,&macval);
	
	sqlite3_prepare(db, sql_select, strlen(sql_select), &stmt, NULL);
	i=0;
	while(SQLITE_DONE !=sqlite3_step(stmt))
	{
		printf("Result[%d]: macid=%s, time=%s, mac_order=%s\n",
			i++,
			sqlite3_column_text(stmt, 0),
			sqlite3_column_text(stmt, 1),
			sqlite3_column_text(stmt, 2)
			);
// 		printf("Result[%d]: macid=%s\n",
// 			i++,
// 			sqlite3_column_text(stmt, 0)
// 			);
		sleep(1);
	}
	
	sqlite3_finalize(stmt);
	sqlite3_close(db);
	return 0;
}


/*---------------------------------------------------------------------------*/
/* 函数功能：从数据库读取符合条件的数据并返回
* 参数说明：pSqlite3 需要写入的数据库指针  table 读取数据库的table
* 返回说明：返回NULL表示失败。
* 注意事项：
* 使用举例：1．if (insert_sqlite_data(pSqlite3,data) == NULL) 
* printf("sqlite close err(%s)", format_sqlite_err());
* */
// char * read_sqlite_data(sqlite3 * pSqlite3, char *table, char *rcvbuf)
// {
// 	int i;
// 	sqlite3 *db;
// 	sqlite3_stmt *stmt;
// 	char name[16], occ[16];
// 
// 	char*sql_select="select * from people;";
// 
// 	printf("Show the sizeof rcvbuf %d\n",sizeof(rcvbuf));
// // 	memset(rcvbuf,0x00,sizeof(rcvbuf));
// 	if ( (db =open_sqlite("/mnt/hgfs/gtk_using/sqlite_mode/test_sqlite.db")) == NULL) 
// 	{
// 		printf("sqlite open err(%s)", format_sqlite_err(db));
// 	}	
// 	sqlite3_prepare(db, sql_select, strlen(sql_select), &stmt, NULL);
// 	i=0;
// 	while(SQLITE_DONE !=sqlite3_step(stmt))
// 	{
// 		printf("Result[%d]: name=%s, occ=%s\n",
// 			i++,
// 			sqlite3_column_text(stmt, 0),
// 			sqlite3_column_text(stmt, 1));
// 		memcpy(rcvbuf+strlen(rcvbuf),sqlite3_column_text(stmt, 0),strlen(sqlite3_column_text(stmt, 0)));
// 		memcpy(rcvbuf+strlen(rcvbuf),",",sizeof(","));
// 		memcpy(rcvbuf+strlen(rcvbuf),sqlite3_column_text(stmt, 1),strlen(sqlite3_column_text(stmt, 1)));
// 		memcpy(rcvbuf+strlen(rcvbuf),"  ",sizeof("  "));
// 
// 	}
// 	
// 	sqlite3_finalize(stmt);
// 	sqlite3_close(db);
// 	return 0;	
// }

char * read_sqlite_data(sqlite3 * pSqlite3, char *table, char *rcvbuf)
{
	int i;
	sqlite3 *db;
	sqlite3_stmt *stmt;
	char name[16], occ[16];
	
	char*sql_select="select * from people;";
	char*sql_select_machine="select * from machine;";
	
	//justify the input parameter 
	if ( NULL == pSqlite3 || NULL == table)
	{
 		printf("%s:%d Error input!\n",__FILE__,__LINE__);
		return 0;
	}

	printf("Show the sizeof rcvbuf %d\n",sizeof(rcvbuf));
	// 	memset(rcvbuf,0x00,sizeof(rcvbuf));
// 	if ( (db =open_sqlite("/mnt/hgfs/gtk_using/sqlite_mode/test_sqlite.db")) == NULL) 
	if ( (db =open_sqlite("/mnt/hgfs/gtk_using/tmp/server/test_sqlite.db")) == NULL) 
	{
		printf("sqlite open err(%s)", format_sqlite_err(db));
	}	
// 	sqlite3_prepare(db, sql_select, strlen(sql_select), &stmt, NULL);
	printf("hell-1\n");
	sqlite3_prepare(db, sql_select_machine, strlen(sql_select_machine), &stmt, NULL);
	i=0;
	printf("hell-2\n");
	while(SQLITE_DONE !=sqlite3_step(stmt))
	{
		printf("Result[%d]: macid=%s, time=%s, mac_order=%s\n",
			i++,
			sqlite3_column_text(stmt, 0),
			sqlite3_column_text(stmt, 1),
			sqlite3_column_text(stmt, 2)
			);
		memcpy(rcvbuf+strlen(rcvbuf),sqlite3_column_text(stmt, 0),strlen(sqlite3_column_text(stmt, 0)));
		memcpy(rcvbuf+strlen(rcvbuf),",",sizeof(","));
		memcpy(rcvbuf+strlen(rcvbuf),sqlite3_column_text(stmt, 1),strlen(sqlite3_column_text(stmt, 1)));
		memcpy(rcvbuf+strlen(rcvbuf),",",sizeof(","));
		memcpy(rcvbuf+strlen(rcvbuf),sqlite3_column_text(stmt, 2),strlen(sqlite3_column_text(stmt, 2)));
		memcpy(rcvbuf+strlen(rcvbuf),"  ",sizeof("  "));		
	}
	printf("hell-3  SQLITE_DONE:%d\n",SQLITE_DONE);	
	sqlite3_finalize(stmt);
	sqlite3_close(db);
	return 0;	
}



/*---------------------------------------------------------------------------*/
/* 函数功能：向数据表中增加一组数据
* 参数说明：pSqlite3 需要写入的数据库指针；data 传入的数据
* 返回说明：返回NULL表示失败。
* 注意事项：
* 使用举例：1．if (insert_sqlite_data(pSqlite3,data) == NULL) 
* printf("sqlite close err(%s)", format_sqlite_err());
* */
// char * insert_sqlite_data(sqlite3 * pSqlite3, void *data)
// {
// 	sqlite3_stmt *stmt;
// 	Ppeople people = (Ppeople)(data);
// 	char*sql_insert="insert into people values (?, ?);";
// 
// 	sqlite3_prepare(pSqlite3, sql_insert, strlen(sql_insert), &stmt, NULL);
// 	//printf("INSERT name=%s, occ=%s\n",values[i].name, values[i].occ);
// 	sqlite3_bind_text(stmt, 1, people->name, strlen(people->name), SQLITE_STATIC);
// 	sqlite3_bind_text(stmt, 2, people->occ, strlen(people->occ), SQLITE_STATIC);
// 	sqlite3_step(stmt);
// 	sqlite3_reset(stmt);
// 	return 0;
// }

char * insert_sqlite_data(sqlite3 * pSqlite3, void *data)
{
	sqlite3_stmt *stmt;
	Pmachine machine = (Pmachine)(data);
	char*sql_insert="insert into machine values (?, ?, ?);";
printf("########\n");
	printf("Result: macid=%s, time=%s, mac_order=%s\n",machine->cMacid,machine->cTime,machine->cDBnum);

	sqlite3_prepare(pSqlite3, sql_insert, strlen(sql_insert), &stmt, NULL);
	//printf("INSERT name=%s, occ=%s\n",values[i].name, values[i].occ);
	sqlite3_bind_text(stmt, 1, machine->cMacid, strlen(machine->cMacid), SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, machine->cTime, strlen(machine->cTime), SQLITE_STATIC);
	sqlite3_bind_text(stmt, 3, machine->cDBnum, strlen(machine->cDBnum), SQLITE_STATIC);
	sqlite3_step(stmt);
	sqlite3_reset(stmt);
	return 0;
}


/*---------------------------------------------------------------------------*/
/* 函数功能：向数据表中删除符合条件的记录
* 参数说明：pSqlite3 需要写入的数据库指针；tables 目标表单 ；cond  条件 
* 返回说明：返回NULL表示失败。
* 注意事项：
* 使用举例：1．if (insert_sqlite_data(pSqlite3,data) == NULL) 
* printf("sqlite close err(%s)", format_sqlite_err());
* */
char * del_sqlite_data(sqlite3 * pSqlite3, char *tables, char *cond)
{
	sqlite3_stmt *stmt;
	char*sql_insert="insert into people values (?, ?);";
	char *sql_del="delete from people where name=\"Lily\";";

	sqlite3_prepare(pSqlite3, sql_insert, strlen(sql_insert), &stmt, NULL);
	//printf("INSERT name=%s, occ=%s\n",values[i].name, values[i].occ);

	return 0;
}




/*---------------------------------------------------------------------------*/
/* 函数功能：格式化sqlite3数据库操作错误
* 参数说明：pSqlite3 发生错误的数据库指针
* 返回说明：返回NULL表示失败。
* 注意事项：
* 使用举例：１．if (open_sqlite(pSqlite3) == NULL) 
* printf("sqlite close err(%s)", format_sqlite_err());
* */
char * format_sqlite_err(sqlite3 * pSqlite3)
{
if (pSqlite3 == NULL) return (char *)NULL;
return (char *)sqlite3_errmsg(pSqlite3);
}
/*---------------------------------------------------------------------------*/
/* 函数功能：打开数据库
* 参数说明：pszPath 待打开的数据库文件路径,路径要求大于7个字节
* 返回说明：返回NULL表示失败。此时可使用format_err_sqlite函数得到错误文本信息
* 注意事项：如果没有对应的库文件，则产生一个新的数据库
* 使用举例：1．if (open_sqlite("test_sqlite.db") == NULL) 
* printf("sqlite open err(%s)", format_sqlite_err());
* */
sqlite3 * open_sqlite(const char * pszPath)
{
	static sqlite3 * pSqlite3;
	pSqlite3 = NULL;
	if (pszPath == NULL || strlen(pszPath) <= 7) 
	{
		printf("null or path < 7\n");
		return NULL;
	}
	if (sqlite3_open(pszPath, &pSqlite3) != SQLITE_OK) {
		/* 如果 UTF-8 格式的数据库名打开失败，则再传入UTF-16 格式的数据库名 */
		if (sqlite3_open16(pszPath, &pSqlite3) != SQLITE_OK) 
		{
			printf("bit-16 is fail\n");
			return NULL;
		}
		else 
		{
			printf("bit-16 is ok\n");
			return pSqlite3;
		}

	}
	printf("open is normal\n");	
	printf("pathway is %s\n",pszPath);	
	return pSqlite3;
}
/*---------------------------------------------------------------------------*/
/* 函数功能：关闭数据库
* 参数说明：pSqlite3 待关闭的数据库指针
* 返回说明：返回false表示失败。此时可使用format_err_sqlite函数得到错误文本信息
* 注意事项：
* 使用举例：１．if (close_sqlite(pSqlite3) == false) 
* printf("sqlite close err(%s)", format_sqlite_err());
* */
bool close_sqlite(sqlite3 * pSqlite3)
{
if (pSqlite3 == NULL) return false;
if (sqlite3_close(pSqlite3) != SQLITE_OK) return false;
pSqlite3 = NULL;
return true;
}
/*---------------------------------------------------------------------------*/
/* 函数功能：获取第一行指定列数据库内容
* 参数说明：pSqlite3 待关闭的数据库指针(IN)
* pSqlite3_Stmtsqlite3_stmt 指针(OUT)
* pszSQL标准SQL语句
* nColunmID字段序号。从0开始
* 返回说明：返回NULL表示失败或数据库中没有纪录。
* 此时可使用format_err_sqlite函数得到错误文本信息
* 否则返回指定的字符串
* 注意事项：返回值应该判断是否为NULL
* 使用举例：１．遍历获取t_db表中t_db_ab字段的所有纪录并打印
* sqlite3 * pSqlite3; 
* sqlite3_stmt * pSqlite3_Stmt;
* char *pszTxt = NULL;
* for (pszTxt = first_sqlite_column_txt(pSqlite3, &pSqlite3_Stmt, "select t_bd_ab from t_bd", 0);
* pszTxt != NULL;
* pszTxt = next_sqlite_column_txt(pSqlite3_Stmt, 0))
* {
* printf("%s\r\n", pszTxt == NULL ? NOTHING : pszTxt);
* sqlite_ret = step_sqlite_row(pSqlite3_Stmt);
* if (sqlite_ret == SQLT_ERROR) {
* 
* printf("step_sqlite_row err(%s)\r\n", format_sqlite_err(pSqlite3));
* break;
* }
* else if (sqlite_ret == SQLITE_DONE) break;
* }
* */
#ifdef MACRO_C
char * first_sqlite_column_txt(sqlite3 * pSqlite3, sqlite3_stmt ** pSqlite3_Stmt, const char * pszSQL, const unsigned int nColunmID)
#else
char * first_sqlite_column_txt(sqlite3 * pSqlite3, sqlite3_stmt ** pSqlite3_Stmt, const char * pszSQL, const unsigned int &nColunmID = 0)
#endif
{
/* 检查参数 */
if (pSqlite3 == NULL || nColunmID < 0) {
*pSqlite3_Stmt = NULL;
return (char *)NULL;
}
if (sqlite3_prepare(pSqlite3, pszSQL, -1, pSqlite3_Stmt, 0) != SQLITE_OK) {
if (sqlite3_prepare16(pSqlite3, pszSQL, -1, pSqlite3_Stmt, 0) != SQLITE_OK) {
*pSqlite3_Stmt = NULL;
return (char *)NULL;
}
}
if (sqlite3_step(*pSqlite3_Stmt) != SQLITE_ROW) {
if (*pSqlite3_Stmt != NULL) {
sqlite3_finalize(*pSqlite3_Stmt);
*pSqlite3_Stmt = NULL;
}
return (char *)NULL;
}
return (char *)sqlite3_column_text(*pSqlite3_Stmt, nColunmID);
}
/* 函数功能：获取数据库当前行指定列的数据
* 参数说明：pSqlite3_Stmtsqlite3_stmt 指针(IN/OUT)
* nColunmID对应数据表中字段的ID号，从0开始
* 返回说明：返回NULL表示失败。此时可使用format_sqlite_err函数得到错误文本信息
* 否则返回指定的字符串
* 注意事项：返回值应该判断是否为NULL
* 使用举例：参见：first_sqlite_column_txt 函数
* */
#ifdef MACRO_C
char * next_sqlite_column_txt(sqlite3_stmt * pSqlite3_Stmt, const unsigned int nColunmID)
#else
char * next_sqlite_column_txt(sqlite3_stmt * pSqlite3_Stmt, const unsigned int &nColunmID = 0)
#endif
{
if (pSqlite3_Stmt == NULL || nColunmID < 0) return (char *)NULL;
return (char *)sqlite3_column_text(pSqlite3_Stmt, nColunmID);
}
/* 函数功能：将数据库游标移动到下一行
* 参数说明：pSqlite3_Stmtsqlite3_stmt 指针(IN/OUT)
* 返回说明：返回 SQLT_ERROR表示失败。此时可使用format_sqlite_err函数得到错误文本信息
* 返回 SQLT_DONE表示当前游标及以下没有纪录了。 
* 返回 SQLT_ROW表示游标移动下一行
* 注意事项：１．遍历获取数据库内容时，当返回 SQLT_ERROR 和 SQLITE_DONE 
* 时，则该退出循环
* 使用举例：参见：first_sqlite_column_txt 函数
* */
sqlt_msg step_sqlite_row(sqlite3_stmt * pSqlite3_Stmt)
{
int sqlite_ret = 0;
if (pSqlite3_Stmt == NULL) return SQLT_ERROR;
sqlite_ret = sqlite3_step(pSqlite3_Stmt);
if (sqlite_ret == SQLITE_ROW) return SQLT_ROW;
else if (sqlite_ret == SQLITE_DONE) {
sqlite3_finalize(pSqlite3_Stmt);
pSqlite3_Stmt = NULL;
return SQLT_DONE;
}
return SQLT_ERROR;
}
/*---------------------------------------------------------------------------*/
/* 函数功能：开启事务
* 参数说明：pSqlite3sqlite3 指针
* 返回说明：返回 false表示失败。此时可使用format_sqlite_err函数得到错误文本信息
* 注意事项：１．必须和'关闭事务'成对使用
* 使用举例：
* */
bool begin_sqlite_affair(sqlite3 * pSqlite3)
{
if (pSqlite3 == NULL) return false;
if (sqlite3_exec(pSqlite3, "BEGIN;", 0, 0, 0) != SQLITE_OK) return false;
return true;
}
/* 函数功能：关闭事务
* 参数说明：pSqlite3sqlite3 指针
* 返回说明：返回 false表示失败。此时可使用format_sqlite_err函数得到错误文本信息
* 注意事项：１．必须和'开启事务'成对使用
* 使用举例：
* */
bool end_sqlite_affair(sqlite3 * pSqlite3)
{
if (pSqlite3 == NULL) return FALSE;
if (sqlite3_exec(pSqlite3, "COMMIT;", 0, 0, 0) != SQLITE_OK) return false;
return true;
}
/*---------------------------------------------------------------------------*/
/* 函数功能：执行 SQL 语句
* 参数说明：pSqlite3sqlite3 指针
* pszSQL需要执行的 SQL 字符串语句
* 返回说明：返回 false表示失败。此时可使用format_sqlite_err函数得到错误文本信息
* 注意事项：
* 使用举例：
* */
bool execute_sql(sqlite3 * pSqlite3, const char * pszSQL)
{
if (pSqlite3 == NULL) return false;
if (sqlite3_exec(pSqlite3, pszSQL, 0, 0, 0) != SQLITE_OK) return false;
return true;
}
/*---------------------------------------------------------------------------*/
/* 函数功能：获取执行 SQL 语句后所查询的数据字段数量
* 参数说明：pSqlite3sqlite3 指针
* pSqlite3_Stmtsqlite3_stmt 指针
* pszSQL需要执行的 SQL 字符串语句
* 返回说明：返回 -1表示失败。此时可使用format_sqlite_err函数得到错误文本信息
* 否则返回字段数量
* 注意事项：
* 使用举例：１．得到t_bd表中字段总合数量
* int sqlite_colunm_count = 0;
* sqlite_colunm_count = get_colunm_count(pSqlite3, &pSqlite3_Stmt, "select * from t_bd");
* if (sqlite_colunm_count < 0) {
* 
* printf("get_colunm_count err(%s)\r\n", format_sqlite_err(pSqlite3));
* }
* else printf("sqlite colunm count = %d\r\n", sqlite_colunm_count);
* */
int get_colunm_count(sqlite3 * pSqlite3, sqlite3_stmt ** pSqlite3_Stmt, const char * pszSQL)
{
static int nsqlite_colunm;
nsqlite_colunm = 0;
/* 检查参数 */
if (pSqlite3 == NULL) {
pSqlite3_Stmt = NULL;
return -1;
}
if (sqlite3_prepare(pSqlite3, pszSQL, -1, pSqlite3_Stmt, 0) != SQLITE_OK) {
if (sqlite3_prepare16(pSqlite3, pszSQL, -1, pSqlite3_Stmt, 0) != SQLITE_OK) {
pSqlite3_Stmt = NULL;
return -1;
}
}
if (sqlite3_step(*pSqlite3_Stmt) != SQLITE_ROW) {
if (pSqlite3_Stmt != NULL) {
sqlite3_finalize(*pSqlite3_Stmt);
pSqlite3_Stmt = NULL;
}
return -1;
}
nsqlite_colunm = sqlite3_column_count(*pSqlite3_Stmt);
if (*pSqlite3_Stmt != NULL) {
sqlite3_finalize(*pSqlite3_Stmt);
pSqlite3_Stmt = NULL;
}
return nsqlite_colunm;
}
/*---------------------------------------------------------------------------*/
/* 函数功能：压缩 sqlite 数据
* 参数说明：pSqlite3sqlite3 指针
* 返回说明：返回 false表示失败。此时可使用format_sqlite_err函数得到错误文本信息
* 注意事项：需要使用两倍于数据库文件大小的磁盘空间
* 使用举例：
* */
bool sqlite_compress(sqlite3 * pSqlite3)
{
if (pSqlite3 == NULL) return false;
if (sqlite3_exec(pSqlite3, "VACUUM;", 0, 0, 0) != SQLITE_OK) return false;
return true;
}
/*---------------------------------------------------------------------------*/
/* 函数功能：获取一列的列名称
* 参数说明：pSqlite3sqlite3 指针
* pSqlite3_Stmtsqlite3_stmt 指针
* pszSQL需要执行的 SQL 字符串语句
* nColunmID执行 SQL 后，对结果中的字段ＩＤ
* 返回说明：返回 NULL 表示失败。此时可使用format_sqlite_err函数得到错误文本信息
* 注意事项：
* 使用举例：
* */
#ifdef MACRO_C
char * sqlite_colunm_name(sqlite3 * pSqlite3, sqlite3_stmt ** pSqlite3_Stmt, const char * pszSQL, const unsigned int nColunmID)
#else
char * sqlite_colunm_name(sqlite3 * pSqlite3, sqlite3_stmt ** pSqlite3_Stmt, const char * pszSQL, const unsigned int &nColunmID = 0)
#endif
{
	static char szColunm[128];
	char * pszColunm_Name;
	/* 检查参数 */
	if (pSqlite3 == NULL) {
	pSqlite3_Stmt = NULL;
	return (char *)NULL;
	}
	memset(szColunm, 0, 128);
	if (sqlite3_prepare(pSqlite3, pszSQL, -1, pSqlite3_Stmt, 0) != SQLITE_OK) {
	if (sqlite3_prepare16(pSqlite3, pszSQL, -1, pSqlite3_Stmt, 0) != SQLITE_OK) {
	pSqlite3_Stmt = NULL;
	return (char *)NULL;
	}
	}
	if (sqlite3_step(*pSqlite3_Stmt) != SQLITE_ROW) {
	if (pSqlite3_Stmt != NULL) {
	sqlite3_finalize(*pSqlite3_Stmt);
	pSqlite3_Stmt = NULL;
	}
	return (char *)NULL;
	}
	pszColunm_Name = (char *)sqlite3_column_name(*pSqlite3_Stmt, nColunmID);
	sprintf(szColunm, "%s", pszColunm_Name == NULL ? "" : pszColunm_Name);
	if (*pSqlite3_Stmt != NULL) {
	sqlite3_finalize(*pSqlite3_Stmt);
	pSqlite3_Stmt = NULL;
	}
	return szColunm;
}



// int main()
// {
// //	close_sqlite(open_sqlite("./people.db"));
// 	char readbuf[200] = {0};
// 	sqlite_operation_add();
// read_sqlite_data(NULL,NULL,readbuf);
// //printf("main rcv is %s\n",readbuf);
// 	return 0;
// }
















