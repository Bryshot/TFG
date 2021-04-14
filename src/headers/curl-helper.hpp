#pragma once
#include <curl/curl.h>
#include <QLibrary>

#if defined(WIN32)
constexpr auto curl_library_name = "libcurl.dll";
#elif __APPLE__
#define curl_library_name "libcurl.4.dylib"
#else
#define curl_library_name "libcurl.so.4"
#endif

typedef CURL *(*initFunction)(void);
typedef CURLcode (*setOptFunction)(CURL *, CURLoption, ...);
typedef CURLcode (*performFunction)(CURL *);
typedef void (*cleanupFunction)(CURL *);
typedef void (*resetFunction)(CURL *);

extern initFunction f_curl_init;     //Función encargada de inicializar un CURL
extern setOptFunction f_curl_setopt; //Función encargada de establecer un opción
extern performFunction
	f_curl_perform; //Función encargada realizar la lógica del CURL
extern cleanupFunction f_curl_cleanup; //Función encargada de eliminar un CURL
extern resetFunction
	f_curl_reset; //Función encargada de reiniciar un CURL (Eliminar todas sus opciones establecidas)

extern QLibrary *loaded_curl_lib;

bool resolveCurl();
bool loadCurl();
