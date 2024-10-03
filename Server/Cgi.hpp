#include "../Incl/Libs.hpp"
#include <unistd.h>
class Cgi
{
  public:
	// Constructor
	Cgi();

	// Destructor
	~Cgi();

  private:
	void executeCGIScript(const char *scriptPath, char *const argv[],
		char *const envp[])
	{
	};
};