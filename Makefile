DIROBJ := obj/
DIREXE := exec/
DIRHEA := include/
DIRSRC := src/

ICE_HOME := /usr/include
ORACLE_HOME := /u01/app/oracle/product/11.2.0/dbhome_1/instantclient

CFLAGS := -I$(DIRHEA) -I$(ORACLE_HOME)/sdk/include -c -DICE_UTIL_API_EXPORTS -pthread -fPIC -O2 -g -std=c++11 -Wall -Werror
LDFLAGS := -lstdc++ -lIceStorm -lIceStormService -lIceBox -lGlacier2 -lIceSSL -lssl -lcrypto -lIce -lIceUtil -lbz2 -L$(ORACLE_HOME) -lclntsh -locci -lboost_thread
CC := g++

OBJS := $(subst $(DIRSRC), $(DIROBJ), \
		$(patsubst %.cpp, %.o, $(wildcard $(DIRSRC)*.cpp)))

all : dirs cardroid_server++

dirs : 
		mkdir -p $(DIROBJ) $(DIREXE)

cardroid_server++ : $(OBJS)
				$(CC) $(LDFLAGS) -o $(DIREXE)$@ $^

$(DIROBJ)%.o : $(DIRSRC)%.cpp
		$(CC) $(CFLAGS) $^ -o $@

clean : 
		rm -rf *~ core $(DIROBJ) $(DIREXE) $(DIRHEA)*~ $(DIRSRC)*~
