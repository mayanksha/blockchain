all:    readchain cryptotest

ifeq ($(DEBUG),1)
    CPPFLAGS:=-Wall -Werror -g -std=c++14
else
    CPPFLAGS:=-Wall -Werror -O3 -std=c++14
endif

all: readchain cryptotest

crypto.o: crypto.cpp crypto.h
	g++ -c ${CPPFLAGS} crypto.cpp

transaction.o: transaction.cpp transaction.h crypto.h
	g++ -c ${CPPFLAGS} transaction.cpp

blockchain.o: blockchain.cpp blockchain.h transaction.h crypto.h
	g++ -c ${CPPFLAGS} blockchain.cpp

cryptotest.o: cryptotest.cpp crypto.h
	g++ -c ${CPPFLAGS} cryptotest.cpp

readchain.o: readchain.cpp crypto.h blockchain.h transaction.h
	g++ -c ${CPPFLAGS} readchain.cpp

readchain: readchain.o transaction.o crypto.o blockchain.o helper.o
	g++ -L/usr/local/lib ${CPPFLAGS} -o readchain readchain.o transaction.o crypto.o helper.o blockchain.o -lssl -lcrypto

cryptotest: cryptotest.o crypto.o
	g++ -L/usr/local/lib ${CPPFLAGS} -o cryptotest cryptotest.o crypto.o -lssl -lcrypto

clean:
	rm -f a.out crypto.o transaction.o blockchain.o cryptotest.o readchain.o cryptotest readchain 

.PHONY: clean
