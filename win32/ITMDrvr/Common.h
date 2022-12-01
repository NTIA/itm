

int ParseInteger(const char* str, int* value);
int ParseDouble(const char* str, double* value);
int ParsingErrorHelper(int err, const char* msg);

#ifdef _WIN32
#  define WIN32_STDCALL __stdcall
#else
#  define WIN32_STDCALL
#  define fprintf_s fprintf
#  define printf_s printf
#  define fwprintf_s fwprintf
#  define sprintf_s sprintf
#  define wprintf_s wprintf
#endif