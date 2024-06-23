#ifndef __UTIL_LOG_H__
#define __UTIL_LOG_H__

#define LOG_DECLARE_PATH(path)  const char * _log_path = path
#define LOG_DECLARE_UNIT(unit)  const char * _log_unit = unit
#define LOG_DECLARE_MODULE(mod) static const char * _log_mod = mod

#define LOG_INIT        _log_init
#define LOG_DEINIT      _log_deinit
#define LOG_ERR(...)    _log_err(_log_mod, __VA_ARGS__)
#define LOG_WRN(...)    _log_wrn(_log_mod, __VA_ARGS__)
#define LOG_INF(...)    _log_inf(_log_mod, __VA_ARGS__)

void _log_init (void);
void _log_deinit (void);
void _log_err (const char * mod, const char * fmt, ...);
void _log_wrn (const char * mod, const char * fmt, ...);
void _log_inf (const char * mod, const char * fmt, ...);

#endif
