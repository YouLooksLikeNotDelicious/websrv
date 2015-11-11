parse\ tpls:
	v=`date +"%m%d%H%M%S"`;\
    sed -re "s/common\.js(\?v=[0-9]*)*/common.js?v=$$v/g" Shared/layout.html > /tmp/layout.html;\
    mv /tmp/layout.html Shared/layout.html;
	
	@make -C Account

clean:
	make -C Account clean
