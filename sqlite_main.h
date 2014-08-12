#ifndef _SQLITE_MAIN_H
#define _SQLITE_MAIN_H


#include <stdio.h>
#include <string.h>
#include "sqlite3.h"/* sqlite 数据库操作第三方库头文件 */

#ifdef WIN32
#pragma comment(lib, "sqlite3.lib")
#endif
#ifndef boolean
#define boolean
/* 定义布尔型变量 */
typedef enum boolean {false, true} bool;
#endif
#ifndef BOOLEAN
#define BOOLEAN
/* 定义布尔型变量 */
typedef enum BOOLEAN {FALSE, TRUE} BOOL;
#endif
/* 定义消息变量 */
typedef enum SQLT_MSG {SQLT_ERROR = -1, SQLT_DONE, SQLT_ROW} sqlt_msg;
/* 定义NULL字符 */
#define SQLT_NOTHING	("(null)")
/* 定义NULL字符 */
#ifndef NULL
#define NULL	0
#endif

#define  MACRO_C   //使用的是c语言



/*---------------------------------------------------------------------------*/
/* 函数功能：从数据库读取符合条件的数据并返回
* 参数说明：pSqlite3 需要写入的数据库指针  table 读取数据库的table
* 返回说明：返回NULL表示失败。
* 注意事项：
* 使用举例：1．if (insert_sqlite_data(pSqlite3,data) == NULL) 
* printf("sqlite close err(%s)", format_sqlite_err());
* */
char * read_sqlite_data(sqlite3 * pSqlite3, char *table, char *rcvbuf);

/*---------------------------------------------------------------------------*/
/* 函数功能：向数据表中增加一组数据
* 参数说明：pSqlite3 需要写入的数据库指针；data 传入的数据
* 返回说明：返回NULL表示失败。
* 注意事项：
* 使用举例：1．if (insert_sqlite_data(pSqlite3,data) == NULL) 
* printf("sqlite close err(%s)", format_sqlite_err());
* */
char * insert_sqlite_data(sqlite3 * pSqlite3, void *data);


/*---------------------------------------------------------------------------*/
/* 函数功能：格式化sqlite3数据库操作错误
* 参数说明：pSqlite3 发生错误的数据库指针
* 返回说明：返回NULL表示失败。
* 注意事项：
* 使用举例：１．if (open_sqlite(pSqlite3) == NULL) 
* printf("sqlite close err(%s)", format_sqlite_err());
* */
char * format_sqlite_err(sqlite3 * pSqlite3);


/*---------------------------------------------------------------------------*/
/* 函数功能：打开数据库
* 参数说明：pszPath 待打开的数据库文件路径
* 返回说明：返回NULL表示失败。此时可使用format_err_sqlite函数得到错误文本信息
* 注意事项：如果没有对应的库文件，则产生一个新的数据库
* 使用举例：１．if (open_sqlite("test_sqlite.db") == NULL) 
* printf("sqlite open err(%s)", format_sqlite_err());
* */
sqlite3 * open_sqlite(const char * pszPath);

/*---------------------------------------------------------------------------*/
/* 函数功能：关闭数据库
* 参数说明：pSqlite3 待关闭的数据库指针
* 返回说明：返回false表示失败。此时可使用format_err_sqlite函数得到错误文本信息
* 注意事项：
* 使用举例：１．if (close_sqlite(pSqlite3) == false) 
* printf("sqlite close err(%s)", format_sqlite_err());
* */
bool close_sqlite(sqlite3 * pSqlite3);

/*---------------------------------------------------------------------------*/
/* 函数功能：执行 SQL 语句
* 参数说明：pSqlite3sqlite3 指针
* pszSQL需要执行的 SQL 字符串语句
* 返回说明：返回 false表示失败。此时可使用format_sqlite_err函数得到错误文本信息
* 注意事项：
* 使用举例：
* */
bool execute_sql(sqlite3 * pSqlite3, const char * pszSQL);


#ifdef MACRO_C
char * sqlite_colunm_name(sqlite3 * pSqlite3, sqlite3_stmt ** pSqlite3_Stmt, const char * pszSQL, const unsigned int nColunmID);
#else
char * sqlite_colunm_name(sqlite3 * pSqlite3, sqlite3_stmt ** pSqlite3_Stmt, const char * pszSQL, const unsigned int &nColunmID = 0);
#endif


#endif
