/* simple gkamus
   (f) sgk.c
   (d) translate word (id-en)
   (lib)
      sqlite3.h -> for managing database sqlite
      db.sqlite -> word database, convert from gkamus program to sqlite
                   for information about gkamus, gkamus.sourceforge.net.
   (c) gcc -l sqlite3 $(f) -o sgk
   (l) GPL+

 */

#include <string.h>
#include <unistd.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <stdio.h>

#define DBNAME "db.sqlite3"
#define APPVERSION "0.1"

char tblNAME[10] = "en2id";

void showInfo (char* appName, int id);
int selectDb (int en2id, char *find);
int insertDB (int en2id, char *str);

int doSqlite (char *query, int show);

int main (int argc, char* argv[]) {
    int retval;

    int opt;
    int insert = 0;
    int en2id = 1;
    int info;

    /* parsing argument */
    while ((opt=getopt (argc, argv, "viahd")) != -1) {
        switch (opt) {
            /* from id to en */
        case 'i':
            en2id = 0; break;
        case 'a':
            insert = 1; break;
        case 'h':
            showInfo (argv[0], 0);
            break;
        case 'd':
            showInfo (argv[0], 1);
            break;
        case 'v':
            fprintf (stdout, "%s v%s\n", argv[0], APPVERSION);
            exit (0);
            break;
        default:
            showInfo(argv[0], -1);
        }
    }

    /* if no word to be translate */
    if (optind >= argc) {
        fprintf (stderr, "Insert words to translate or insert!\n");
        exit (1);
    }

    /* if insert to database */
    if (insert)
        retval = insertDb (en2id, argv[optind]);
    else
        retval = selectDb (en2id, argv[optind]);

    return retval;

}

int selectDb (int en2id, char *find) {
    char *tpl="select _to from %s where _from like \"%s\"";

    char query[128];

    if (!en2id)
        strcpy (tblNAME, "id2en");

    sprintf(query, tpl, tblNAME, find);

    return doSqlite (query, 1);
}

int insertDb (int en2id, char *str) {
    int i=0, retval;
    char *tpl = "insert into %s (_from, _to, ket) values(\"%s\", \"%s\", \"insert\")";

    char query[128];
    char toFrom[2][128], *token, *tmpStr;

    /* check if ':' exists */
    tmpStr = strchr (str, '|');
    /* jika
       '|' diawal text (0)
       '|' diakhir text (strlen)
       tidak ada '|'
    */
    if ( !(tmpStr-str) || (tmpStr-str+1==strlen(str)) || (tmpStr==NULL)) {
        printf ("Use '|' as separator text for row of 'from' and 'to'.\nNot allowed in begin or end of text\n");
        return 1;
    } 

    /* parsing str */
    token = strtok (str, "|");
    while (token != NULL) {
        strcpy (toFrom[i++], token);
        token = strtok (NULL, "|");
    }

    /* get table name */
    if (!en2id)
        strcpy (tblNAME, "id2en");

    sprintf (query, tpl, tblNAME, toFrom[0], toFrom[1]);

    retval = doSqlite (query, 0);
    if (retval == SQLITE_OK) {
        printf("Append success!\n");
        return retval;
    }

    printf ("Append data failed");
    return SQLITE_ERROR;
}

int doSqlite (char *query, int show) {
    sqlite3 *conn;
    sqlite3_stmt *res;

    int retval = 0;
    int rec = 0;
    char *results;

    retval = sqlite3_open (DBNAME, &conn);

    if (retval) {
        fprintf (stderr, "Can't open database (%s)\n", DBNAME);
        exit (EXIT_FAILURE);
    }

    /* if select data */
    if (show) {
        retval = sqlite3_prepare_v2 (conn, query, -1, &res, 0);

        /* no data found */
        if (retval != SQLITE_OK)
            return retval;

        retval = sqlite3_step (res);
        results = (char*)sqlite3_column_text (res, 0);

        if (!results)
            results = "";
        else
            strcat (results, "\n");

        fprintf (stdout, "%s", results);
    }

    /* just append data */
    else
        retval = sqlite3_exec (conn, query, 0, 0, 0);


    sqlite3_close (conn);

    return retval;
}

void showInfo(char* appName, int id) {
    char *help ;

    /* program help */
    if (0 == id) {
        fprintf (stdout, "%s v%s: simple kamus\n", appName, APPVERSION);
        fprintf (stdout, "simple program to translate word.\n\
from indonesia to english or otherwise.\n\
Using:\n\
\t%s [options] word.\n\
Options:\n\
\t-h:   show this page.\n\
\t-d:   show information about kamus source.\n\
\t-i:   translate from indonesia to english\n\
\t      (default from english to indonesia).\n\
\t-a:   insert into database.\n\
\tword: text to be translate.\n\
Note:\n\
\tIf insert to db use '|' as separator text.\n\
Example:\n\
\t%s home\n\
\t\tTranslate 'home' from english to indonesia.\n\
\t%s -i rumah\n\
\t\tTranslate 'rumah' from indonesia to english.\n\
\t%s -a 'one|satu'\n\
\t\tInsert to table en2id, with _from='one' and _to='satu'.\n\
\t%s -a -i 'satu|one'\n\
\t\tInsert to table id2en, with _from='satu' and _to='one'.\n\
==========\n\
Author:\temaniacs <noone.nu1@gmail.com>\n",
                 appName, appName, appName, appName, appName);
    }

    /* db help */
    else if (1 == id) {
        fprintf (stdout, "= Information about kamus source =.\n\
All source taken from 'gkamus-en.dict' and 'gkamus-id.dict'.\n\
That file from gkamus, (gkamus.sourceforge.net)\n\
File license is \"public domain\".\n\
Written by Firmansyah, Ardhan Madras.\n\
File version is v1.0\n");

    }
    else {}

    exit (0);
}
