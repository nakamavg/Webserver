#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	char *query = getenv("QUERY_STRING");
	if (query != NULL) {
		printf("<html>\n");
		printf("<head>\n");
		printf("<title>CGI Example in C</title>\n");
		printf("</head>\n");
		printf("<body>\n");
		printf("<h1>CGI Example in C</h1>\n");
		printf("<p>Hello from CGI in C! for Dalton brothers</p>\n");
		printf("<p>Query String: %s</p>\n", query);
		printf("</body>\n");
		printf("</html>\n");
	}

	return 0;
}