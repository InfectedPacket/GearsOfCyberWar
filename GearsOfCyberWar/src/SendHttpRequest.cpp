#pragma once

#include <winsock2.h>
#include <windows.h>
#include <stdio.h>

#pragma comment(lib,"ws2_32.lib")

/**
<summary>

<para>If query string is provided...</para>
</summary>
<param name="Host">The target host, either an IP address or URL.</param>
<param name="HostLength">The length of the buffer containing the host.</param>
<param name="Page"></param>
<param name="PageLength"></param>
<param name="QueryString"></param>
<param name="QueryStringLength"></param>
<param name="HttpRequestLength"></param>
<returns></returns>
*/
char* CreateHttpGetRequestString(const char* Host, unsigned short HostLength, 
	const char* Page, unsigned short PageLength,
	char* QueryString, unsigned int QueryStringLength,
	unsigned int* HttpRequestLength) {

	if (Host == NULL || HostLength <= 0) {
		return NULL;
	}

	if (Page == NULL || PageLength <= 0) {
		return NULL;
	}

	int result = 0;
	char* BasicHttpRequestFmt;
	unsigned int HttpRequestSize = 65535;
	char* BasicHttpRequest = new char[HttpRequestSize];
	SecureZeroMemory(BasicHttpRequest, HttpRequestSize);

	if (QueryString == NULL || QueryStringLength <= 0) {
		BasicHttpRequestFmt = "GET %s HTTP/1.1\r\nHost: %s\r\n";
		result = sprintf(BasicHttpRequest, BasicHttpRequestFmt, Page, Host);
	} else {
		BasicHttpRequestFmt = "GET %s?%s HTTP/1.1\r\nHost: %s\r\n";
		result = sprintf(BasicHttpRequest, BasicHttpRequestFmt, Page, QueryString, Host);
	}

	if (result < 0) { return NULL; }
	*HttpRequestLength = (unsigned int)result;
	return BasicHttpRequest;
}

/**
<summary></summary>
<param name="HeaderName"></param>
<param name="HeaderNameLength"></param>
<param name="HeaderValue"></param>
<param name="HeaderValueLength"></param>
<returns></returns>
*/
char* AddHttpRequestHeader(char* HttpRequest, unsigned int HttpRequestLength,
	const char* HeaderName, unsigned int HeaderNameLength,
	const char* HeaderValue, unsigned int HeaderValueLength) {

		if (HttpRequest == NULL || HttpRequestLength <= 0) {
			return NULL;
		}

		if (HeaderName == NULL || HeaderNameLength <= 0) {
			return HttpRequest;
		}

		strcat(HttpRequest, HeaderName);
		strcat(HttpRequest, ": ");
		strcat(HttpRequest, HeaderValue);
		strcat(HttpRequest, "\r\n");

		return HttpRequest;
}

/**
<summary></summary>
<param name="Host"></param>
<param name="Port"></param>
<param name="HttpRequest"></param>
*/
int SendHttpRequestViaWinsock (const char* Host, unsigned short Port,
	char* HttpRequest, unsigned int HttpRequestLength,
	char* Response, unsigned int* ResponseSize){

	//Argument validation checks
	if (Host == NULL) { return -1;}
	if (Port < 0 || Port > 65535) { return -1;}
	if (HttpRequest == NULL || HttpRequestLength <= 0) { return -1;}

	//Initiate Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        return -1;
    }

	//Create a local TCP socket on the given port
    SOCKET Socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    struct hostent *host;
    host = gethostbyname(Host);

	//Error: could not resolve host, check the connection
	if (host == NULL) { return -1; }

    SOCKADDR_IN SockAddr;
    SockAddr.sin_port=htons(Port);
    SockAddr.sin_family=AF_INET;
    SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);
 
    if(connect(Socket,(SOCKADDR*)(&SockAddr),sizeof(SockAddr)) != 0){
        return 1; //Error: could not connect to the host
    }

	//Send the request to the host
	send(Socket, HttpRequest, HttpRequestLength, 0);

	//Prepare to read and store the response
    //unsigned short BufferSize = 10000;
	//char* HttpResponseBuffer = new char[BufferSize];
    int nDataLength;
	unsigned int ResponseLength = 0;
    while ((nDataLength = recv(Socket,Response,*ResponseSize,0)) > 0){        
		ResponseLength = ResponseLength + nDataLength;
    }
    closesocket(Socket);
    WSACleanup();
    return ResponseLength;
}

int TestHttpRequest() {
	int result = 0;
	const char* szUrl = "http://127.0.0.1";
	const char* szUA = "IE8 (compatible; mozilla 5.0)";
	const char* szPage = "index.php";
	char* szQueryString = "cmd=903849&il=he2934";
	unsigned short Port = 80;
	unsigned int HttpRequestLen = 0;
	char* HttpRequest = CreateHttpGetRequestString(szUrl, strlen(szUrl)+1,
		szPage, strlen(szPage), szQueryString, strlen(szQueryString), &HttpRequestLen);
	HttpRequest = AddHttpRequestHeader(HttpRequest, HttpRequestLen, 
		"User-Agent", strlen("User-Agent"), szUA, strlen(szUA));
	HttpRequest = AddHttpRequestHeader(HttpRequest, HttpRequestLen,
		"DNT", strlen("DNT"), "1", 1);
	printf("[*] Created Http Request:\n");
	printf(HttpRequest);
	printf("[*] Sending Http Request to %s.\n", szUrl);
	char* HttpResponse = new  char[65535];
	unsigned int HttpResponseLen = 0;
	HttpResponseLen = SendHttpRequestViaWinsock(szUrl, Port, HttpRequest, HttpRequestLen, HttpResponse, &HttpResponseLen);
	if (result < 0) {
		printf("[-] Could not send http request to %s:%d.\n", szUrl, Port);
	} else {
		printf("[+] Request sent. Received %d byte(s) from %s:%d.\n", HttpResponseLen, szUrl, Port);
	}
	return 0;
}