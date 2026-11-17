# FireStation Manager

Qt/C++ management application with Oracle 11g XE storage.

## Oracle setup

Create an Oracle 11g schema and run [`sql/oracle11g_schema.sql`](sql/oracle11g_schema.sql) while connected as that schema owner. The script creates the shared tables, sequences, ID triggers and read-only compatibility views. It does not drop tables or seed fabricated records.

Configure these process environment variables before launching:

- `FIRESTATION_ORACLE_DSN`: an Oracle ODBC DSN (or a connection string accepted by the selected Qt SQL driver)
- `FIRESTATION_ORACLE_USER`: the application schema user
- `FIRESTATION_ORACLE_PASSWORD`: the schema password
- `FIRESTATION_QT_SQL_DRIVER`: optional Qt driver name; defaults to `QODBC`

The Qt SQL driver and Oracle ODBC client must be installed on the machine. All modules use the same default Qt SQL connection.

## Build

Open `FireStationManager.pro` in Qt Creator with Qt Widgets, SQL, Charts and PrintSupport modules installed, or run `qmake` followed by the platform build tool.
