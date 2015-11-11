ifneq (mips, $(findstring mips, $(CC)))
CXX = g++ -Wall -g -std=c++11 
else
CXX := $(CXX)
endif

build: Request.o SessionInfo.o AccountBusiness.o 

AccountBusiness.o: ../base.hpp ../../libs/web.hpp ../../libs/session.hpp Request.hpp SessionBusiness.hpp SessionBusiness.cpp ../../libs/url_encode.hpp

SessionBusiness.o: ../../libs/session.hpp Request.hpp SessionInfo.hpp

Request.hpp Request.cpp: Request.struct
	python ../../../generator/json_serializor_generator2.py Request.struct

SessionInfo.hpp SessionInfo.cpp: SessionInfo.struct
	python ../../../generator/json_serializor_generator2.py SessionInfo.struct

Request.o: Request.hpp Request.cpp

SessionInfo.o: SessionInfo.hpp SessionInfo.cpp

clean:
	@rm -rf *.o Request.hpp Request.cpp SessionInfo.hpp SessionInfo.cpp
