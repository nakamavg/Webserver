#include "Server/ServerUp.hpp"
#include "Server/serverlist.hpp"

bool printenv(char **env)
{
	/*
		portillo me ha explicado que puedo imprimir 
		en modo debug si activo esta flag con lo cual 
		me evito comentar los prints jajaja xd Uwu
	*/
	int i = -1;
	while(env[++i])
		if (strcmp(env[i],"DEBUG=1") == false)
			return true;
	std::cout << std::endl<< std::endl;
	return false;
}
int main(int ar, char **argv, char **env)
{
	if(ar ==1)
		return 99;
	else if (ar == 2)
	{
		printenv(env);
		std::vector<serverlist> lista;
		serverlist pepe("127.0.0.1",8080);
		serverlist manolo("127.0.0.1",8081);
	lista.push_back(pepe);
	lista.push_back(manolo);
	std::cout << "Hola server"<< std::endl;
	ServerUp a(lista);
	a.start();
	}
	else
		return 98;
}