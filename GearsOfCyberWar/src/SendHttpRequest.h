

int SendHttpRequestViaWinsock (const char* Host, unsigned short Port,
	char* HttpRequest, unsigned int HttpRequestLength,
	char* Response, unsigned int* ResponseLength);

char* AddHttpRequestHeader(char* HttpRequest, unsigned int HttpRequestLength,
	const char* HeaderName, const char* HeaderNameLength,
	const char* HeaderValue, const char* HeaderValueLength);

char* CreateHttpGetRequestString(const char* Host, unsigned short HostLength, 
	const char* Page, unsigned short PageLength,
	const char* Method, char* QueryString, unsigned int QueryStringLength,
	unsigned int* HttpRequestLength);

int TestHttpRequest();