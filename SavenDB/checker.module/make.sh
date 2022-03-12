# main.cpp
cd /home/ubuntu/DBpro4/SavenDB/main.module/ && g++ main.cpp cmd.o dbuser.o sql_view.o pager.o pagepool.o \
sql_grant.o sql_index.o utils.o -o test1 /usr/lib/x86_64-linux-gnu/libboost_regex.so.1.65.1 -lsqlite3
