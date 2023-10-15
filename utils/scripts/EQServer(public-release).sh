#!/bin/bash

# using xterm as terminal so that tput does not produce error messages
export TERM=xterm

##### SYSTEM USER, DATABASE AND DIRS #####
serverdir=eqserver
dbuser=changeme
dbpass=changeme
lsdb=peqmac_ls
gamedb=peqmac
#Typically LAN IP
lsIP=127.0.0.1
#Typically LAN IP
gameIP=127.0.0.1
#Typically localhost or loopback IP
dbIP=127.0.0.1
#valid public mode is monthly
dbMode=monthly
## GIT BRANCH ##
branch=Dev
## LAUNCHERS TRUE/FALSE ##
boats=TRUE
zone1=TRUE
zone2=TRUE
zone3=TRUE
dynamic1=FALSE
dynamic2=FALSE
##### TELNET #####
telnetuser="changeme"
pass="changeme"
server="127.0.0.1"
port="9000"

#############################NOTHING BELOW THIS NEEDS TO BE EDITED!#############################
user=$USER
path=/home/$user/$serverdir
NOW=$(date +"%Y-%m-%d-%H:%M")

ulimit -c unlimited

##### CALCULATE TIMER AND ZONE COUNTS #####
if  [ $boats = "TRUE" ]; then
	zonecount_boats=$(mysql -u $dbuser --password=$dbpass -h $dbIP -D $gamedb -se "SELECT COUNT(enabled) FROM launcher_zones WHERE enabled = 1 AND launcher = 'boats';")
	let zonecount_temp=$((zonecount_temp+zonecount_boats))
fi

if  [ $zone1 = "TRUE" ]; then
	zonecount_zone1=$(mysql -u $dbuser --password=$dbpass -h $dbIP -D $gamedb -se "SELECT COUNT(enabled) FROM launcher_zones WHERE enabled = 1 AND launcher = 'zone1';")
	let zonecount_temp=$((zonecount_temp+zonecount_zone1))
fi

if  [ $zone2 = "TRUE" ]; then
	zonecount_zone2=$(mysql -u $dbuser --password=$dbpass -h $dbIP -D $gamedb -se "SELECT COUNT(enabled) FROM launcher_zones WHERE enabled = 1 AND launcher = 'zone2';")
	let zonecount_temp=$((zonecount_temp+zonecount_zone2))
fi

if  [ $zone3 = "TRUE" ]; then
	zonecount_zone3=$(mysql -u $dbuser --password=$dbpass -h $dbIP -D $gamedb -se "SELECT COUNT(enabled) FROM launcher_zones WHERE enabled = 1 AND launcher = 'zone3';")
	let zonecount_temp=$((zonecount_temp+zonecount_zone3))
fi

if  [ $dynamic1 = "TRUE" ]; then
	dynamiccount1=$(mysql -u $dbuser --password=$dbpass -h $dbIP -D $gamedb -se "SELECT launcher.dynamics FROM launcher WHERE launcher.name='dynzone1';")
	let zonecount_temp=$((zonecount_temp+dynamiccount1))
fi

if  [ $dynamic2 = "TRUE" ]; then
	dynamiccount2=$(mysql -u $dbuser --password=$dbpass -h $dbIP -D $gamedb -se "SELECT launcher.dynamics FROM launcher WHERE launcher.name='dynzone2';")
	let zonecount_temp=$((zonecount_temp+dynamiccount2))
fi

if [ $zonecount_temp > 0 ]; then
	zonecount=$zonecount_temp
fi

##### CHECK IF SERVER RUNNING #####
isrunning()
{
if ([ -f loginserver.pid ] && ps -p $(cat loginserver.pid) > /dev/null;) || ([ -f world.pid ] && ps -p $(cat world.pid) > /dev/null;) || ([ -f queryserv.pid ] && ps -p $(cat queryserv.pid) > /dev/null;) || 
	([ -f ucs.pid ] && ps -p $(cat ucs.pid) > /dev/null;) || ([ -f eqlauncha.pid ] && ps -p $(cat eqlauncha.pid) > /dev/null;) || ([ -f eqlaunchb.pid ] && ps -p $(cat eqlaunchb.pid) > /dev/null;) || 
	([ -f eqlaunchc.pid ] && ps -p $(cat eqlaunchc.pid) > /dev/null;) || ([ -f eqlaunchd.pid ] && ps -p $(cat eqlaunchd.pid) > /dev/null;) || ([ -f eqlaunche.pid ] && ps -p $(cat eqlaunche.pid) > /dev/null;) || 
	([ -f eqlaunchf.pid ] && ps -p $(cat eqlaunchf.pid) > /dev/null;) then
	echo "$(tput bold)$(tput setaf 1)You need to shut down the server before issuing this command.$(tput sgr0)";
	return 1
else
	return 0
fi
}

##### 10 MIN WARNING #####
ten-min-warn()
{
	if [ telnetuser = "changeme" || pass = "changeme" ]; then
		echo "You must set a telnet user and/or password to run this command!"
		exit 1
	fi
	VAR=$(expect -c "
	spawn telnet $server $port
	expect \"Username:\"
	send \"$telnetuser\n\"
	expect \"Password:\"
	send \"$pass\n\"
	expect \"$telnetuser>\"
	expect \"$telnetuser>\"
	send \"lock\n\"
	expect \"$telnetuser>\"
	expect \"$telnetuser>\"
	send \"broadcast The server will be rebooting in 10 minutes. Please plan accordingly. The server is locked until reboot.\n\"
	sleep 55
	expect \"$telnetuser>\"
	send \"broadcast The server will be rebooting in 9 minutes. Please plan accordingly. The server is locked until reboot.\n\"
	sleep 59
	expect \"$telnetuser>\"
	send \"broadcast The server will be rebooting in 8 minutes. Please plan accordingly. The server is locked until reboot.\n\"
	sleep 59
	expect \"$telnetuser>\"
	send \"broadcast The server will be rebooting in 7 minutes. Please plan accordingly. The server is locked until reboot.\n\"
	sleep 59
	expect \"$telnetuser>\"
	send \"broadcast The server will be rebooting in 6 minutes. Please plan accordingly. The server is locked until reboot.\n\"
	sleep 59
	expect \"$telnetuser>\"
	send \"broadcast The server will be rebooting in 5 minutes. Please plan accordingly. The server is locked until reboot.\n\"
	sleep 59
	expect \"$telnetuser>\"
	send \"broadcast The server will be rebooting in 4 minutes. Please plan accordingly. The server is locked until reboot.\n\"
	sleep 59
	expect \"$telnetuser>\"
	send \"broadcast The server will be rebooting in 3 minutes. Please plan accordingly. The server is locked until reboot.\n\"
	sleep 59
	expect \"$telnetuser>\"
	send \"broadcast The server will be rebooting in 2 minutes. Please plan accordingly. The server is locked until reboot.\n\"
	sleep 59
	expect \"$telnetuser>\"
	send \"broadcast The server will be rebooting in 1 minutes. Please plan accordingly. The server is locked until reboot.\n\"
	sleep 59
	expect \"$telnetuser>\"
	send \"exit\n\"
	")
	echo "$VAR"
}

##### 30 MIN WARNING #####
thirty-min-warn()
{
	if [ telnetuser = "changeme" || pass = "changeme" ]; then
		echo "You must set a telnet user and/or password to run this command!"
		exit 1
	fi
	VAR=$(expect -c "
	spawn telnet $server $port
	expect \"Username:\"
	send \"$telnetuser\n\"
	expect \"Password:\"
	send \"$pass\n\"
	expect \"$telnetuser>\"
	expect \"$telnetuser>\"
	send \"lock\n\"
	expect \"$telnetuser>\"
	expect \"$telnetuser>\"
	send \"broadcast The server will be rebooting in 30 minutes. Please plan accordingly.\n\"
	expect \"$telnetuser>\"
	send \"exit\n\"
	")
	echo "$VAR"
}

##### RELOAD QUESTS/RULES #####
## I can't get reload to work right.
reload()
{
	if [ telnetuser = "changeme" || pass = "changeme" ]; then
		echo "You must set a telnet user and/or password to run this command!"
		exit 1
	fi
	VAR=$(expect -c "
	spawn telnet $server $port
	expect \"Username:\"
	send \"$telnetuser\n\"
	expect \"Password:\"
	send \"$pass\n\"
	expect \"$telnetuser>\"
	expect \"$telnetuser>\"
	send \"reloadworld\n\"
	expect \"$telnetuser>\"
	expect \"$telnetuser>\"
	send \"exit\n\"
	")
	echo "$VAR"
}

##### START SERVER SECTION #####
case "$1" in
start)
if ( isrunning == 0 ) then
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
	
	rm -rf logs/*.log
	chmod --recursive ugo+rwx quests
	
	sleep 5
	echo -en "\r\t $(tput bold)$(tput setaf 2)Loading Shared Mem...$(tput sgr0)";
	./shared_memory > /dev/null 2>&1 &
	
	sleep 5
	echo -en "\r\t $(tput bold)$(tput setaf 2)Starting World Server...$(tput sgr0)";
	./world > /dev/null 2>&1 &
	echo $! > world.pid
	
	sleep 5
	echo -en "\r\t $(tput bold)$(tput setaf 2)Starting Query Server...$(tput sgr0)";
	./queryserv > /dev/null 2>&1 &
	echo $! > queryserv.pid
	
	sleep 5
	echo -en "\r\t $(tput bold)$(tput setaf 2)Starting Chat Server...$(tput sgr0)";
	./ucs > /dev/null 2>&1 &
	echo $! > ucs.pid
	
	sleep 5
	echo -en "\r\t $(tput bold)$(tput setaf 2)Starting the Zone Launchers...$(tput sgr0)";
	echo -e "\r";
	
	boats()
	{
	if [ $boats = "TRUE" ]; then
		./eqlaunch boats > /dev/null 2>&1 &
		echo $! > eqlauncha.pid
	fi
	}
	zone1()
	{
	if [ $zone1 = "TRUE" ]; then
		./eqlaunch zone1 > /dev/null 2>&1 &
		echo $! > eqlaunchb.pid
	fi
	}
	zone2()
	{
	if [ $zone2 = "TRUE" ]; then
		./eqlaunch zone2 > /dev/null 2>&1 &
		echo $! > eqlaunchc.pid
	fi
	}
	zone3()
	{
	if [ $zone3 = "TRUE" ]; then
		./eqlaunch zone3 > /dev/null 2>&1 &
		echo $! > eqlaunchd.pid
	fi
	}
	dynzone1()
	{
	if [ $dynamic1 = "TRUE" ]; then
		./eqlaunch dynzone1 > /dev/null 2>&1 &
		echo $! > eqlaunche.pid
	fi
	}
	dynzone2()
	{
	if [ $dynamic2 = "TRUE" ]; then
		./eqlaunch dynzone2 > /dev/null 2>&1 &
		echo $! > eqlaunchf.pid
	fi
	}
	boats &
	zone1 &
	zone2 &
	zone3 &
	dynzone1 &
	dynzone2
	
	let timet=$zonecount;
	for i in $(seq 1 $zonecount);do
		echo -en "\r\t [$(tput bold)$(tput setaf 2)$timet$(tput sgr0)] seconds remain until all zones are up.";
		let timet=timet-1;
		sleep 1;
	done;
	
	echo -en "\r\t\033[K $(tput bold)$(tput setaf 2)Starting Login Server...$(tput sgr0)";
	echo -e "\r";
	./loginserver > /dev/null 2>&1 &
	echo $! > loginserver.pid
fi
$0 status
;;

##### STOP THE SERVER #####
stop|shutdownNOW)
	kill $(cat world.pid)
	kill $(cat queryserv.pid)
	kill $(cat ucs.pid)
	kill $(cat eqlauncha.pid)
	kill $(cat eqlaunchb.pid)
	kill $(cat eqlaunchc.pid)
	kill $(cat eqlaunchd.pid)
	kill $(cat eqlaunche.pid)
	kill $(cat eqlaunchf.pid)
	kill $(cat loginserver.pid)
	killall -s SIGKILL world
	killall -s SIGKILL queryserv
	killall -s SIGKILL ucs
	killall -s SIGKILL eqlauncha
	killall -s SIGKILL eqlaunchb
	killall -s SIGKILL eqlaunchc
	killall -s SIGKILL eqlaunchd
	killall -s SIGKILL eqlaunche
	killall -s SIGKILL eqlaunchf
	killall -s SIGKILL eqlaunch
	killall -s SIGKILL zone
	killall -s SIGKILL loginserver
	rm -f *.pid
	echo All server components have been exited.
;;

##### RESTART #####
restart|reload)
	$0 stop
	$0 start
;;

##### STATUS SECTION #####
status)
	if [ -f loginserver.pid ] && ps -p $(cat loginserver.pid) > /dev/null; then
		echo -e Login Server '\t\t\t' [$(tput bold)$(tput setaf 2)UP$(tput sgr0)]
	else
		echo -e Login Server '\t\t\t' [$(tput bold)$(tput setaf 1)DOWN$(tput sgr0)]
	fi
	if [ -f world.pid ] && ps -p $(cat world.pid) > /dev/null; then
		echo -e World Server '\t\t\t' [$(tput bold)$(tput setaf 2)UP$(tput sgr0)]
	else
		echo -e World Server '\t\t\t' [$(tput bold)$(tput setaf 1)DOWN$(tput sgr0)]
	fi
	if [ -f queryserv.pid ] && ps -p $(cat queryserv.pid) > /dev/null; then
		echo -e Query Server '\t\t\t' [$(tput bold)$(tput setaf 2)UP$(tput sgr0)]
	else
		echo -e Query Server '\t\t\t' [$(tput bold)$(tput setaf 1)DOWN$(tput sgr0)]
	fi
	if [ -f ucs.pid ] && ps -p $(cat ucs.pid) > /dev/null; then
		echo -e Chat Server '\t\t\t' [$(tput bold)$(tput setaf 2)UP$(tput sgr0)]
	else
		echo -e Chat Server '\t\t\t' [$(tput bold)$(tput setaf 1)DOWN$(tput sgr0)]
	fi
	if [ -f eqlauncha.pid ] && ps -p $(cat eqlauncha.pid) > /dev/null; then
		echo -e Boat Launcher '\t\t\t' [$(tput bold)$(tput setaf 2)UP$(tput sgr0)]
	elif [ $boats = "FALSE" ]; then
		echo -e Boat Launcher '\t\t\t' [$(tput bold)$(tput setaf 3)DISABLED$(tput sgr0)]
	else
		echo -e Boat Launcher '\t\t\t' [$(tput bold)$(tput setaf 1)DOWN$(tput sgr0)]
	fi
	if [ -f eqlaunchb.pid ] && ps -p $(cat eqlaunchb.pid) > /dev/null; then
		echo -e Zone1 Launcher '\t\t\t' [$(tput bold)$(tput setaf 2)UP$(tput sgr0)]
	elif [ $zone1 = "FALSE" ]; then
		echo -e Zone1 Launcher '\t\t\t' [$(tput bold)$(tput setaf 3)DISABLED$(tput sgr0)]
	else
		echo -e Zone1 Launcher '\t\t\t' [$(tput bold)$(tput setaf 1)DOWN$(tput sgr0)]
	fi
	if [ -f eqlaunchc.pid ] && ps -p $(cat eqlaunchc.pid) > /dev/null; then
		echo -e Zone2 Launcher '\t\t\t' [$(tput bold)$(tput setaf 2)UP$(tput sgr0)]
	elif [ $zone2 = "FALSE" ]; then
		echo -e Zone2 Launcher '\t\t\t' [$(tput bold)$(tput setaf 3)DISABLED$(tput sgr0)]
	else
		echo -e Zone2 Launcher '\t\t\t' [$(tput bold)$(tput setaf 1)DOWN$(tput sgr0)]
	fi
	if [ -f eqlaunchd.pid ] && ps -p $(cat eqlaunchd.pid) > /dev/null; then
		echo -e Zone3 Launcher '\t\t\t' [$(tput bold)$(tput setaf 2)UP$(tput sgr0)]
	elif [ $zone3 = "FALSE" ]; then
		echo -e Zone3 Launcher '\t\t\t' [$(tput bold)$(tput setaf 3)DISABLED$(tput sgr0)]
	else
		echo -e Zone3 Launcher '\t\t\t' [$(tput bold)$(tput setaf 1)DOWN$(tput sgr0)]
	fi
	if [ -f eqlaunche.pid ] && ps -p $(cat eqlaunche.pid) > /dev/null; then
		echo -e Dynamic1 Zone Launcher '\t\t' [$(tput bold)$(tput setaf 2)UP$(tput sgr0)]
	elif [ $dynamic1 = "FALSE" ]; then
		echo -e Dynamic1 Zone Launcher '\t\t' [$(tput bold)$(tput setaf 3)DISABLED$(tput sgr0)]
	else
		echo -e Dynamic1 Zone Launcher '\t\t' [$(tput bold)$(tput setaf 1)DOWN$(tput sgr0)]
	fi
	if [ -f eqlaunchf.pid ] && ps -p $(cat eqlaunchf.pid) > /dev/null; then
		echo -e Dynamic2 Zone Launcher '\t\t' [$(tput bold)$(tput setaf 2)UP$(tput sgr0)]
	elif [ $dynamic2 = "FALSE" ]; then
		echo -e Dynamic2 Zone Launcher '\t\t' [$(tput bold)$(tput setaf 3)DISABLED$(tput sgr0)]
	else
		echo -e Dynamic2 Zone Launcher '\t\t' [$(tput bold)$(tput setaf 1)DOWN$(tput sgr0)]
	fi
	if (($(pgrep -c zone) == $zonecount)); then
		echo -e '\t' $(tput bold)$(tput setaf 2)\($(pgrep -c zone)\)$(tput sgr0) $(tput bold)of $zonecount zones launched
	elif (($(pgrep -c zone) < $zonecount)); then
		echo -e '\t' $(tput bold)$(tput setaf 1)\($(pgrep -c zone)\)$(tput sgr0) $(tput bold)of $zonecount zones launched
		echo $'\r'
		echo "Will refresh in 10 seconds. Hit CTRL C if you want to break this refresh loop."
		echo "Use this only if there is a hang or something bad going on."
		echo "This will kill the server if you CTRL C."
		echo $'\r'
		sleep 10
		$0 status
	elif (($(pgrep -c zone) > $zonecount)); then
		echo -e '\t' $(tput bold)$(tput setaf 3)\($(pgrep -c zone)\)$(tput sgr0) $(tput bold)of $zonecount zones launched
	fi
;;

##### REBOOT #####
reboot)
	ten-min-warn
	sleep 5
	$0 restart
;;

rebootNOW)
	$0 restart
;;

##### SHUTDOWN WITH WARNING #####
shutdownwarn)
	ten-min-warn
	$0 stop
;;

##### CRON #####
cron)
	thirty-min-warn
	sleep 1200
	ten-min-warn
	$0 stop
	sleep 5
	$0 update
;;

##### BACKUP #####
backup)
	mkdir bin-backups_$NOW
	backups=$path/bin-backups_$NOW
	
	cp $path/source/build/bin/* $backups
	cp $path/source/utils/defaults/commands.pl $backups
	cp $path/source/utils/defaults/plugin.pl $backups
	cp $path/source/utils/defaults/worldui.pl $backups
	cp $path/source/utils/defaults/mime.types $backups
	cp $path/source/loginserver/login_util/login_opcodes.conf $backups
	cp $path/source/loginserver/login_util/login_opcodes_oldver.conf $backups
	cp $path/source/loginserver/login_util/login_opcodes_sod.conf $backups
	cp $path/source/utils/patches/patch_Mac.conf $backups
	cp $path/source/utils/patches/opcodes.conf $backups
	cp $path/source/utils/patches/patch_Evolution.conf $backups

	mysqldump -u $dbuser -h $dbIP --compact --allow-keywords --extended-insert --tables --password=$dbpass $gamedb > $backups/gamedatabase.sql
	mysqldump -u $dbuser -h $dbIP --compact --allow-keywords --extended-insert --tables --password=$dbpass $lsdb > $backups/loginserver.sql
	
	tar -zcvf $backups/gamedatabase.tar.gz $backups/gamedatabase.sql
	tar -zcvf $backups/loginserver.tar.gz $backups/loginserver.sql

	rm $backups/*.sql
;;

##### BUILD ONLY #####
## This dual build function is to not have 2 backups fire in update
## but still allow for correct function elsewhere.
build)
$0 backup
sleep 1
$0 build2
;;

build2)
if ( isrunning == 0 ) then
	cd $path/logs
	sleep 1
	rm *.log
	
	#Tak_Build
	mkdir $path/source/build
	cd $path/source/build/
	sleep 1
	cmake -G "Unix Makefiles" .. #-i
	make clean
	make -j1
	cd $path
fi
;;

##### UPDATE ALL #####
update)
if ( isrunning == 1 ) then
	ten-min-warn
	sleep 1
	$0 updateNOW
else
	$0 updateNOW
fi
;;

updateNOW)
	$0 backup
	sleep 1
	$0 updatefinalstage
	sleep 1
	$0 start
;;

downloadMaps)
	echo Maps..
	if [ -d "$path/Maps" ]; then
		echo dir exists, pulling update...
		cd $path/Maps/
		sleep 1
		git pull
	else
		echo dir does not exist, grabbing clone from git...
		git clone https://github.com/EQMacEmu/eqmacemu-maps $path/Maps
	fi
;;

test)
;;

downloadQuests)
	echo Quests..
	if [ -d "$path/quests" ]; then
		echo dir exists, pulling update...
		cd $path/quests/
		sleep 1
		git stash
		sleep 1
		git pull
	else
		echo dir does not exist, grabbing clone from git...
		git clone https://github.com/EQMacEmu/peqmacquests $path/quests
  		cp -r $path/source/utils/defaults/plugins $path/quests/plugins
		ln -s $path/quests/plugins $path/plugins
		ln -s $path/quests/lua_modules $path/lua_modules
	fi
;;

downloadSource)
if ( isrunning == 0 ) then
	echo Source..
	cd $path/
	
	# For a clean build and if there are cmake changes.	
	rm -r -f $path/source/
	mkdir $path/source
	sleep 1
	git clone https://github.com/EQMacEmu/Server.git $path/source/
	cd $path/source
	sleep 1
	git checkout $branch
fi
;;

downloadAll)
	$0 backup
	sleep 1
	$0 updatefinalstage
;;

updatefinalstage)
	$0 stop
	sleep 1
	
	#Quests
	$0 downloadQuests

	#Maps
	$0 downloadMaps
	
	#Database
	$0 dbupdate

	#Source
	$0 downloadSource

	# Start building
	mkdir $path/source/build
	sleep 1
	cd $path/
	$0 build2
;;

preinstall)
if [ $user = "root" ]; then 
	apt update
	apt -y upgrade
	apt -y install bash
	apt -y install openssh-server
	apt -y install git
	apt -y install gcc
	apt -y install g++
	apt -y install cpp
	apt -y install unzip
	apt -y install make
	apt -y install cmake
	apt -y install subversion
	apt -y install libio-stringy-perl
	apt -y install zlib-bin
	apt -y install zlibc
	apt -y install libperl-dev
	apt -y install uuid-dev
	
	#use this block for mysql otherwise comment it for mariadb
	###########################################################	
	#apt -y install default-mysql-server
	#apt -y install default-mysql-client
	#apt -y install default-libmysqlclient-dev
	#apt -y install libdbd-mysql-perl
	###########################################################
	
	#use this block for mariadb otherwise comment it for mysql
	###########################################################
	apt -y install mariadb-server
	apt -y install default-libmysqlclient-dev
	apt -y install libmariadbd-dev
	apt -y install libwtdbomysql-dev
	apt -y install libwtdbomysql35
	###########################################################
	
	apt -y install libmysql++
	apt -y install libperl5i-perl
	apt -y install libboost-all-dev
	apt -y install liblua5.1-0
	apt -y install liblua5.1-0-dev
	apt -y install libluabind-dev
	apt -y install lua5.1
	apt -y install build-essential
	apt -y install libssl1.0.0
	apt -y install libssl-dev
	apt -y install openssl
	apt -y install libcurl4-openssl-dev
	apt -y install ruby1.9.1
	apt -y install ruby1.9.1-dev
	apt -y install ctags
	apt -y install apache2
	apt -y install libapache2-mod-php5
	apt -y install php5-mysql
	apt -y install php5-gd
	apt -y install expect
	apt -y install xinetd 
	apt -y install valgrind
	apt -y install proftpd
	apt -y install
	exit 1
else
	echo This needs you to run the script as sudo...
	exit 1
fi
;;

dbdaily)
	echo "Daily database dump access is restricted. You have the wrong script for that."
	echo "Access is by invite only, requests are auto ignored."
	exit 1
;;

dbmonthly)
	cd $path/db/
	rm *.txt
	rm *.gz
	rm *.sql
	rm *.zip
	wget http://www.takproject.org/monthlydrop/alkabor_monthly.zip
	unzip alkabor_monthly.zip
;;

dbinstall)
	if [ dbuser = "changeme" || dbpass = "changeme" ]; then
		echo "You must set a database user and/or password to run this step!"
		exit 1
	fi
	mysql -u $dbuser --password=$dbpass -h $dbIP -e "CREATE USER '"$user"'@'"$dbIP"' IDENTIFIED BY '"$dbpass"';"
	mysql -u $dbuser --password=$dbpass -h $dbIP -e "GRANT ALL PRIVILEGES ON "$gamedb" . * TO '"$user"'@'"$dbIP"';"
	mysql -u $dbuser --password=$dbpass -h $dbIP -e "GRANT ALL PRIVILEGES ON "$lsdb" . * TO '"$user"'@'"$dbIP"';"
	mysql -u $dbuser --password=$dbpass -h $dbIP -e "CREATE USER '"$dbuser"'@'"$dbIP"' IDENTIFIED BY '"$dbpass"';"
	mysql -u $dbuser --password=$dbpass -h $dbIP -e "GRANT ALL PRIVILEGES ON "$gamedb" . * TO '"$dbuser"'@'"$dbIP"';"
	mysql -u $dbuser --password=$dbpass -h $dbIP -e "GRANT ALL PRIVILEGES ON "$lsdb" . * TO '"$dbuser"'@'"$dbIP"';"
	mysql -u $dbuser --password=$dbpass -h $dbIP -e "flush privileges;"
	
	if [ $dbMode = "daily" ]; then
	$0 dbdaily
	elif [ $dbMode = "monthly" ]; then
	$0 dbmonthly
	fi
	
	cd $path/db/
	mysql -u $dbuser --password=$dbpass -h $dbIP -e "create database "$gamedb";"
	mysql -u $dbuser --password=$dbpass -h $dbIP -e "create database "$lsdb";"
	echo alkabor*
	mysql -u $dbuser --password=$dbpass -h $dbIP -D $gamedb < alkabor*.sql
	echo player_tables*
	mysql -u $dbuser --password=$dbpass -h $dbIP -D $gamedb < player_tables*.sql
	echo data_tables*
	mysql -u $dbuser --password=$dbpass -h $dbIP -D $gamedb < data_tables*sql
	echo login_tables*
	mysql -u $dbuser --password=$dbpass -h $dbIP -D $lsdb < login_tables*.sql
;;

dbupdate)
	if [ $dbMode = "daily" ]; then
	$0 dbdaily
	elif [ $dbMode = "monthly" ]; then
	$0 dbmonthly
	fi
	
	cd $path/db/
	mysql -u $dbuser --password=$dbpass -h $dbIP -D $gamedb < drop_system.sql
	mysql -u $dbuser --password=$dbpass -h $dbIP -D $gamedb < alkabor*.sql
;;

install)
if [ $user = "root" ]; then 
	echo Not safe to run as root, exiting...
	exit 1
else
	echo user check passed
	mkdir /home/$user/$serverdir
	sleep 1
	cd $path
	mkdir $path/logs
	sleep 1
	mkdir $path/shared
	sleep 1
	mkdir $path/db
	sleep 1
	$0 updatefinalstage
	sleep 1
	echo Symlinking files..
	ln -s $path/source/utils/defaults/commands.pl $path/commands.pl
	ln -s $path/source/utils/defaults/plugin.pl $path/plugin.pl
	ln -s $path/source/utils/defaults/worldui.pl $path/worldui.pl
	ln -s $path/source/utils/defaults/mime.types $path/mime.types
	ln -s $path/source/utils/defaults/templates $path/templates
	ln -s $path/source/utils/defaults/updated_templates $path/updated_templates
	echo Opcodes..
	ln -s $path/source/loginserver/login_util/login_opcodes.conf $path/login_opcodes.conf
	ln -s $path/source/loginserver/login_util/login_opcodes_oldver.conf $path/login_opcodes_oldver.conf
	ln -s $path/source/loginserver/login_util/login_opcodes_sod.conf $path/login_opcodes_sod.conf
	ln -s $path/source/utils/patches/patch_Mac.conf $path/patch_Mac.conf
 	ln -s $path/source/utils/patches/chat_opcodes.conf $path/chat_opcodes.conf
	ln -s $path/source/utils/patches/opcodes.conf $path/opcodes.conf
	ln -s $path/source/utils/patches/patch_Evolution.conf $path/patch_Evolution.conf
	echo Server Binaries..
	ln -s $path/source/build/bin/world $path/world
	ln -s $path/source/build/bin/zone $path/zone
	ln -s $path/source/build/bin/ucs $path/ucs
	ln -s $path/source/build/bin/libcommon.a $path/libcommon.a
	ln -s $path/source/build/bin/libluabind.a $path/libluabind.a
	ln -s $path/source/build/bin/shared_memory $path/shared_memory
	ln -s $path/source/build/bin/queryserv $path/queryserv
	ln -s $path/source/build/bin/loginserver $path/loginserver
	ln -s $path/source/build/bin/eqlaunch $path/eqlaunch
	ln -s $path/source/build/bin/web_interface $path/web_interface
	cd /home/$user
	sleep 1
	cp /home/$user/EQServer.sh $path/EQServer.sh
	echo Setup complete, if you have not done the database setup do this now before using any other commands.
fi
;;

setupini)
	cd $path
	echo '[database]' > login.ini
	echo 'host = '$dbIP >> login.ini
	echo 'port = 3306' >> login.ini
	echo 'db = '$lsdb >> login.ini
	echo 'user = '$dbuser >> login.ini
	echo 'password = '$dbpass >> login.ini
	echo 'subsystem = MySQL' >> login.ini
	echo $'\r' >> login.ini
	echo '[options]' >> login.ini
	echo 'auto_account_create = TRUE' >> login.ini
	echo 'auto_account_activate = TRUE' >> login.ini
	echo 'failed_login_log = TRUE' >> login.ini
	echo 'good_loginIP_log = TRUE' >> login.ini
	echo 'unregistered_allowed = TRUE' >> login.ini
	echo 'reject_duplicate_servers = TRUE' >> login.ini
	echo 'trace = FALSE' >> login.ini
	echo 'world_trace = FALSE' >> login.ini
	echo 'dump_packets_in = FALSE' >> login.ini
	echo 'dump_packets_out = FALSE' >> login.ini
	echo 'listen_port = 5998' >> login.ini
	echo 'local_network = '$gameIP >> login.ini
	echo 'network_ip = '$gameIP >> login.ini
	echo 'salt = 12345678' >> login.ini
	echo $'\r' >> login.ini
	echo '[security]' >> login.ini
	echo 'plugin = EQEmuAuthCrypto' >> login.ini
	echo 'mode = 5' >> login.ini
	echo $'\r' >> login.ini
	echo '[Old]' >> login.ini
	echo 'port = 6000' >> login.ini
	echo 'opcodes = login_opcodes_oldver.conf' >> login.ini
	echo $'\r' >> login.ini
	echo '[schema]' >> login.ini
	echo 'access_log_table = tblaccountaccesslog' >> login.ini
	echo 'account_table = tblLoginServerAccounts' >> login.ini
	echo 'world_registration_table = tblWorldServerRegistration' >> login.ini
	echo 'world_admin_registration_table = tblServerAdminRegistration' >> login.ini
	echo 'world_server_type_table = tblServerListType' >> login.ini
 	echo 'loginserver_setting_table = tblloginserversettings' >> login.ini
	
	echo '[LoginServerDatabase]' > db.ini
	echo 'host = '$dbIP >> db.ini
	echo 'port = 3306' >> db.ini
	echo 'db = '$lsdb >> db.ini
	echo 'user = '$dbuser >> db.ini
	echo 'password = '$dbpass >> db.ini
	echo $'\r' >> db.ini
	echo '[GameServerDatabase]' >> db.ini
	echo '#Game Server section not used yet.' >> db.ini
	echo 'host = '$gameIP >> db.ini
	echo 'port = 3306' >> db.ini
	echo 'db = '$gamedb >> db.ini
	echo 'user = '$dbuser >> db.ini
	echo 'password = '$dbpass >> db.ini
;;

setupxml)
	echo '<?xml version="1.0"?>' > eqemu_config.xml
	echo '<server>' >> eqemu_config.xml
	echo '	<world>' >> eqemu_config.xml
	echo '		<shortname>PMD</shortname>' >> eqemu_config.xml
	echo '		<longname>PEQMac Dev</longname>' >> eqemu_config.xml
	echo $'\r' >> eqemu_config.xml
	echo '		<address>'$gameIP'</address>' >> eqemu_config.xml
	echo '		<localaddress>'$gameIP'</localaddress>' >> eqemu_config.xml
	echo $'\r' >> eqemu_config.xml
	echo '		<loginserver>' >> eqemu_config.xml
	echo '			<host>'$lsIP'</host>' >> eqemu_config.xml
	echo '			<port>5998</port>' >> eqemu_config.xml
	echo '			<account></account>' >> eqemu_config.xml
	echo '			<password></password>' >> eqemu_config.xml
	echo '		</loginserver>' >> eqemu_config.xml
	echo $'\r' >> eqemu_config.xml
	echo '		<unlocked/>' >> eqemu_config.xml
	echo $'\r' >> eqemu_config.xml
	echo '		<tcp ip="'$gameIP'" port="9000" telnet="enable"/>' >> eqemu_config.xml
	echo $'\r' >> eqemu_config.xml
	echo '		<key>12345</key>' >> eqemu_config.xml
	echo $'\r' >> eqemu_config.xml
	echo '		<http port="9080" enabled="TRUE" mimefile="mime.types"/>' >> eqemu_config.xml
	echo '	</world>' >> eqemu_config.xml
	echo $'\r' >> eqemu_config.xml
	echo '	<chatserver>' >> eqemu_config.xml
	echo '		<host>'$gameIP'</host>' >> eqemu_config.xml
	echo '		<port>7778</port>' >> eqemu_config.xml
	echo '	</chatserver>' >> eqemu_config.xml
	echo $'\r' >> eqemu_config.xml
	echo '	<mailserver>' >> eqemu_config.xml
	echo '		<host>'$gameIP'</host>' >> eqemu_config.xml
	echo '		<port>7778</port>' >> eqemu_config.xml
	echo '	</mailserver>' >> eqemu_config.xml
	echo $'\r' >> eqemu_config.xml
	echo '	<zones>' >> eqemu_config.xml
	echo '		<defaultstatus>0</defaultstatus>' >> eqemu_config.xml
	echo '		<ports low="7100" high="7105"/>' >> eqemu_config.xml
	echo '	</zones>' >> eqemu_config.xml
	echo $'\r' >> eqemu_config.xml
	echo '	<database>' >> eqemu_config.xml
	echo '		<host>'$dbIP'</host>' >> eqemu_config.xml
	echo '		<port>3306</port>' >> eqemu_config.xml
	echo '		<username>'$dbuser'</username>' >> eqemu_config.xml
	echo '		<password>'$dbpass'</password>' >> eqemu_config.xml
	echo '		<db>'$gamedb'</db>' >> eqemu_config.xml
	echo '	</database>' >> eqemu_config.xml
	echo $'\r' >> eqemu_config.xml
	echo '	<qsdatabase>' >> eqemu_config.xml
	echo '		<host>'$dbIP'</host>' >> eqemu_config.xml
	echo '		<port>3306</port>' >> eqemu_config.xml
	echo '		<username>'$dbuser'</username>' >> eqemu_config.xml
	echo '		<password>'$dbpass'</password>' >> eqemu_config.xml
	echo '		<db>'$gamedb'</db>' >> eqemu_config.xml
	echo '	</qsdatabase>' >> eqemu_config.xml
	echo $'\r' >> eqemu_config.xml
	echo '	<files>' >> eqemu_config.xml
	echo '		<spells>spells_us.txt</spells>' >> eqemu_config.xml
	echo '		<opcodes>opcodes.conf</opcodes>' >> eqemu_config.xml
	echo '		<logsettings>log.ini</logsettings>' >> eqemu_config.xml
	echo '	</files>' >> eqemu_config.xml
	echo $'\r' >> eqemu_config.xml
	echo '	<directories>' >> eqemu_config.xml
	echo '		<maps>Maps</maps>' >> eqemu_config.xml
	echo '		<quests>quests/</quests>' >> eqemu_config.xml
	echo '		<plugins>quests/plugins</plugins>' >> eqemu_config.xml
	echo '	</directories>' >> eqemu_config.xml
	echo '</server>' >> eqemu_config.xml
;;

git-list)
	cd $path/source
	git branch -a
;;

##### HELP SECTION #####
help|*)
	printf "\n"
	printf " $(tput bold)$(tput setaf 3)There is redundant duplicates to keep legacy commands usable for those used to them.$(tput sgr0)\n"
	printf "\n"
	printf " Usage: \t$0 [$(tput bold)$(tput setaf 1)start$(tput sgr0)|$(tput bold)$(tput setaf 1)stop$(tput sgr0)|$(tput bold)$(tput setaf 1)reload$(tput sgr0)|$(tput bold)$(tput setaf 1)restart$(tput sgr0)|$(tput bold)$(tput setaf 1)status$(tput sgr0)|$(tput bold)$(tput setaf 1)help$(tput sgr0)] or\r\n"
	printf " \t\t[$(tput bold)$(tput setaf 1)backup$(tput sgr0)|$(tput bold)$(tput setaf 1)build$(tput sgr0)|$(tput bold)$(tput setaf 1)downloadAll$(tput sgr0)|$(tput bold)$(tput setaf 1)downloadMaps$(tput sgr0)|$(tput bold)$(tput setaf 1)downloadQuests$(tput sgr0)|$(tput bold)$(tput setaf 1)downloadSource$(tput sgr0)|\r\n\t\t$(tput bold)$(tput setaf 1)reboot$(tput sgr0)|$(tput bold)$(tput setaf 1)rebootNOW$(tput sgr0)|$(tput bold)$(tput setaf 1)shutdownwarn$(tput sgr0)|$(tput bold)$(tput setaf 1)shutdownNOW$(tput sgr0)|$(tput bold)$(tput setaf 1)update$(tput sgr0)|$(tput bold)$(tput setaf 1)updateNOW$(tput sgr0)|\r\n\t\t$(tput bold)$(tput setaf 3)preinstall$(tput sgr0)|$(tput bold)$(tput setaf 1)install$(tput sgr0)|$(tput bold)$(tput setaf 1)dbinstall$(tput sgr0)|$(tput bold)$(tput setaf 1)dbupdate$(tput sgr0)]|$(tput bold)$(tput setaf 1)setupini$(tput sgr0)]|$(tput bold)$(tput setaf 1)setupxml$(tput sgr0)]\n"
	printf "\n"
	printf " $(tput bold)$(tput setaf 2)start$(tput sgr0)\t\tStarts the server components if not already started.\n"
	printf " $(tput bold)$(tput setaf 2)stop$(tput sgr0)\t\tStops all the server components started by this script with no warning.\n"
	printf " $(tput bold)$(tput setaf 2)restart/reload$(tput sgr0)\tRestarts the server with no warning.\n"
	printf " $(tput bold)$(tput setaf 2)status$(tput sgr0)\t\tLists the status of the server components.\n"
	printf "\n"
	printf " $(tput bold)$(tput setaf 2)cron$(tput sgr0)\t\tDon't manually fire this, it is only for cron. It will lock your ssh.\r\n\t\t\t(It fires a 30 min warning, waits 20 mins, then 10 min warning, updates and restarts.)\n"
	printf " $(tput bold)$(tput setaf 2)backup$(tput sgr0)\t\tDoes full time-stamp backup of bins and database. Can be done live while server is running.\r\n\t\t\t(You need 600mb to 1gb free depending on population and expansions enabled for this.)\r\n\t\t\t(It generates the full db sql of LS and game, then archives it and deletes the sql.)\n"
	printf " $(tput bold)$(tput setaf 2)build$(tput sgr0)\t\tOnly builds the server, no updates. Only for ssh use, not ingame command.\n"
	printf " $(tput bold)$(tput setaf 2)downloadAll$(tput sgr0)\tNo warning, shuts down, updates everything and builds. Does not start server.\n"
	printf " $(tput bold)$(tput setaf 2)downloadMaps$(tput sgr0)\tOnly downloads the Maps. No stop/start/restart.\n"
	printf " $(tput bold)$(tput setaf 2)downloadQuests$(tput sgr0)\tOnly downloads the Quests. No stop/start/restart. Only download uneffected by server online status.\n"
	printf " $(tput bold)$(tput setaf 2)downloadSource$(tput sgr0)\tOnly downloads the Source if server is stopped. No stop/start/restart.\n"
	printf " $(tput bold)$(tput setaf 2)reboot$(tput sgr0)\t\tRestarts the server with a 10 min warning.\n"
	printf " $(tput bold)$(tput setaf 2)rebootNOW$(tput sgr0)\tRestarts the server with no warning.\n"
	printf " $(tput bold)$(tput setaf 2)shutdownwarn$(tput sgr0)\tFires 10 min warning then shuts off server. No build or update.\n"
	printf " $(tput bold)$(tput setaf 2)shutdownNOW$(tput sgr0)\tNO Warning, just shuts off server. No build or update.\n"
	printf " $(tput bold)$(tput setaf 2)update$(tput sgr0)\t\tFires 10 min warning, shuts down, updates everything and builds, then starts server.\n"
	printf " $(tput bold)$(tput setaf 2)updateNOW$(tput sgr0)\tNo warning, shuts down, updates everything and builds, then starts server.\n"
	printf "\n"
	printf " $(tput bold)$(tput setaf 3)OS and database installs/update. Use these in order listed. After server is setup you only need to use dbupdate.$(tput sgr0)\n"
	printf " $(tput bold)$(tput setaf 1)RED$(tput sgr0)$(tput bold)$(tput setaf 3) indicates a permissions difference from normal execution. This means, run the script in root for that command only.$(tput sgr0)\n"
	printf " $(tput bold)$(tput setaf 1)\t\tWarning, ini and xml setups WILL overwrite existing!$(tput sgr0)\n"
	printf "\n"
	printf " $(tput bold)$(tput setaf 1)preinstall$(tput sgr0)\tInstalls OS prerequisites for the server (Requires you to run this script as $(tput bold)$(tput setaf 1)sudo or root$(tput sgr0).) Does not start server.\n"
	printf " $(tput bold)$(tput setaf 2)install$(tput sgr0)\tInstalls everything server related except database and builds. Does not start server.\n"
	printf " $(tput bold)$(tput setaf 2)dbinstall$(tput sgr0)\tInstalls clean database. Will not overwrite, only use as initial install. Does not start server.\n"
	printf " $(tput bold)$(tput setaf 2)dbupdate$(tput sgr0)\tUpdates database to the current released dump. Does not start server.\n"
	printf " $(tput bold)$(tput setaf 2)setupini$(tput sgr0)\tWrites defaults to login.ini and db.ini, you will need to edit to your liking. Does not start server.\n"
	printf " $(tput bold)$(tput setaf 2)setupxml$(tput sgr0)\tWrites defaults to eqemu_config.xml, you will need to edit to your liking. Does not start server.\n"
	printf "\n"
	printf " $(tput bold)$(tput setaf 2)git-list$(tput sgr0)\tDisplays all git branches. You can specify a branch in this file to checkout.\n"
	printf " $(tput bold)$(tput setaf 2)help$(tput sgr0)\t\tDisplays this message\n\n"
;;
 
esac
exit 0
