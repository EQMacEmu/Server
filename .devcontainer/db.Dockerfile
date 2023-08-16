FROM mariadb:10.3.32-focal

# additional mysql conf properties
COPY db-context/mariadb.cnf /etc/mysql/mariadb.conf.d

# base database
ADD db-context/alkabor_2023-08-01-14_55.tar.gz /docker-entrypoint-initdb.d
ADD db-context/tblloginserversettings.sql /docker-entrypoint-initdb.d
# takp .tar contains this, dont want it to run
RUN rm /docker-entrypoint-initdb.d/drop_system.sql

# extra sql scripts
ADD db-context/2023_07_27_tblLoginServerAccounts.sql /docker-entrypoint-initdb.d/z_auto_create.sql
ADD db-context/launcher_boats.sql /docker-entrypoint-initdb.d/z_launcher_boats.sql