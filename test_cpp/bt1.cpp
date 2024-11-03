#include <iostream>
#include <iomanip>      // Thư viện để dùng std::setw
#include <libpq-fe.h>   // Thư viện PostgreSQL
#include <string>

using namespace std;

int main() {
    const char* conninfo = "dbname=**** user=postgres password=***** hostaddr=****** port=5432";
    PGconn *conn = PQconnectdb(conninfo);

    if (PQstatus(conn) != CONNECTION_OK) {
        cerr << "Connection to database failed: " << PQerrorMessage(conn) << endl;
        PQfinish(conn);
        system("pause");
        return 1;
    }
    string query;
    string name = "Dang";
    query = "SELECT * FROM create_user('" + name + "');";
    PGresult *exct = PQexec(conn, query.c_str());
    if (PQresultStatus(exct) != PGRES_TUPLES_OK) {
        cerr << "Query failed: " << PQerrorMessage(conn) << endl;
        PQclear(exct);
        PQfinish(conn);
        system("pause");
        return 1;
    }
    cout << PQgetvalue(exct, 0, 0) << ". Mat khau sinh tu dong cua ban la: 1\n";
    PQclear(exct);
   
    PQfinish(conn);
    system("pause");
    return 0;
}
