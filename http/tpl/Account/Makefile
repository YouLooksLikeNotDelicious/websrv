parse\ tpl\ for\ account\ controller: login.hpp rechargePage.hpp accountPage.hpp changePassword.hpp forgetFirst.hpp forgetSecond.hpp forgetThird.hpp signUp.hpp

login.hpp: login.html ../Shared/navigation.html 
	@cd ../tool/;\
	php5 make_tpl.php Account:login > ../Account/login.hpp
	@echo "\033[32mTemplate Parse ok!\033[0m Parse tpl/Account/login.html to tpl/Account/login.hpp" 

rechargePage.hpp: rechargePage.html ../Shared/navigation.html
	@cd ../tool/;\
	php5 make_tpl.php Account:rechargePage > ../Account/rechargePage.hpp
	@echo "\033[32mTemplate Parse ok!\033[0m Parse tpl/Account/rechargePage.html to tpl/Account/rechargePage.hpp" 

accountPage.hpp: accountPage.html ../Shared/navigation.html
	@cd ../tool/;\
	php5 make_tpl.php Account:accountPage > ../Account/accountPage.hpp
	@echo "\033[32mTemplate Parse ok!\033[0m Parse tpl/Account/accountPage.html to tpl/Account/accountPage.hpp" 

changePassword.hpp: changePassword.html ../Shared/navigation.html
	@cd ../tool/;\
	php5 make_tpl.php Account:changePassword > ../Account/changePassword.hpp
	@echo "\033[32mTemplate Parse ok!\033[0m Parse tpl/Account/changePassword.html to tpl/Account/changePassword.hpp" 

forgetFirst.hpp: forgetFirst.html ../Shared/navigation.html
	@cd ../tool/;\
	php5 make_tpl.php Account:forgetFirst > ../Account/forgetFirst.hpp
	@echo "\033[32mTemplate Parse ok!\033[0m Parse tpl/Account/forgetFirst.html to tpl/Account/forgetFirst.hpp" 

forgetSecond.hpp: forgetSecond.html ../Shared/navigation.html
	@cd ../tool/;\
	php5 make_tpl.php Account:forgetSecond > ../Account/forgetSecond.hpp
	@echo "\033[32mTemplate Parse ok!\033[0m Parse tpl/Account/forgetSecond.html to tpl/Account/forgetSecond.hpp" 

forgetThird.hpp: forgetThird.html ../Shared/navigation.html
	@cd ../tool/;\
	php5 make_tpl.php Account:forgetThird > ../Account/forgetThird.hpp
	@echo "\033[32mTemplate Parse ok!\033[0m Parse tpl/Account/forgetThird.html to tpl/Account/forgetThird.hpp" 

signUp.hpp: signUp.html ../Shared/navigation.html
	@cd ../tool/;\
	php5 make_tpl.php Account:signUp > ../Account/signUp.hpp
	@echo "\033[32mTemplate Parse ok!\033[0m Parse tpl/Account/signUp.html to tpl/Account/signUp.hpp" 

clean:
	rm -f *.cpp *.hpp
