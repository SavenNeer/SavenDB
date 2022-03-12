# SavenDB.inner

#预处理
cd /home/ubuntu/DBpro4/SavenDB/SavenDB.inner/design/ && ./make.sh

#处理编译
cd /home/ubuntu/DBpro4/SavenDB/SavenDB.inner/
g++ -c tbcore.cpp -o pager.o && mv pager.o ../main.module/
g++ -c test_table.cpp -o pagepool.o && mv pagepool.o ../main.module/

#尾处理
cd /home/ubuntu/DBpro4/SavenDB/SavenDB.inner/design/ && ./unmake.sh
