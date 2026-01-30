#include <cstdint>
#include <cstring>
#include <vector>
#include <emscripten/bind.h>
using namespace emscripten;

#include "../include/itm.h"

// ----- Return types -----
typedef struct {
  int     status;
  int32_t warnings;     //see Errors.h
  double  A_db;
} ITMResult;

typedef struct {
  int     status;
  int32_t warnings;     //see Errors.h
  double  A_db;
  
  double theta_hzn_0;
  double theta_hzn_1;
  double d_hzn__meter_0;
  double d_hzn__meter_1;
  double h_e__meter_0;
  double h_e__meter_1;
  double N_s;
  double delta_h__meter;
  double A_ref__db;
  double A_fs__db;
  double d__km;
  int    mode;
} ITMResultEx;

static inline void to_result(const struct IntermediateValues* s, ITMResultEx* res) {
  res->theta_hzn_0     = s->theta_hzn[0];
  res->theta_hzn_1     = s->theta_hzn[1];
  res->d_hzn__meter_0  = s->d_hzn__meter[0];
  res->d_hzn__meter_1  = s->d_hzn__meter[1];
  res->h_e__meter_0    = s->h_e__meter[0];
  res->h_e__meter_1    = s->h_e__meter[1];
  res->N_s             = s->N_s;
  res->delta_h__meter  = s->delta_h__meter;
  res->A_ref__db       = s->A_ref__db;
  res->A_fs__db        = s->A_fs__db;
  res->d__km           = s->d__km;
  res->mode            = s->mode;
}

// =================== P2P (TLS / CR) ===================
DLLEXPORT ITMResult ITM_P2P_TLS_r(
  double h_tx__meter, double h_rx__meter, val pfl,
  int climate, double N_0, double f__mhz, int pol,
  double epsilon, double sigma, int mdvar,
  double time, double location, double situation)
{
  double A_db = 0.0;
  long warnings = 0;
  auto pfl_vec = convertJSArrayToNumberVector<double>(pfl);

  int status = ITM_P2P_TLS(
    h_tx__meter, h_rx__meter, pfl_vec.data(),
    climate, N_0, f__mhz, pol,
    epsilon, sigma, mdvar, time, location, situation,
    &A_db, &warnings
  );

  ITMResult r;
  r.status = status;
  r.warnings = (int32_t)warnings;
  r.A_db = A_db;
  return r;
}

DLLEXPORT ITMResult ITM_P2P_CR_r(
  double h_tx__meter, double h_rx__meter, val pfl,
  int climate, double N_0, double f__mhz, int pol,
  double epsilon, double sigma, int mdvar,
  double confidence, double reliability)
{
  double A_db = 0.0;
  long warnings = 0;
  auto pfl_vec = convertJSArrayToNumberVector<double>(pfl);

  int status = ITM_P2P_CR(
    h_tx__meter, h_rx__meter, pfl_vec.data(),
    climate, N_0, f__mhz, pol,
    epsilon, sigma, mdvar, confidence, reliability,
    &A_db, &warnings
  );

  ITMResult r;
  r.status = status;
  r.warnings = (int32_t)warnings;
  r.A_db = A_db;
  return r;
}

// =================== AREA (TLS / CR) ===================
DLLEXPORT ITMResult ITM_AREA_TLS_r(
  double h_tx__meter, double h_rx__meter,
  int tx_site_criteria, int rx_site_criteria, double d__km,
  double delta_h__meter, int climate, double N_0, double f__mhz,
  int pol, double epsilon, double sigma, int mdvar,
  double time, double location, double situation)
{
  double A_db = 0.0;
  long warnings = 0;

  int status = ITM_AREA_TLS(
    h_tx__meter, h_rx__meter, tx_site_criteria, rx_site_criteria, d__km,
    delta_h__meter, climate, N_0, f__mhz, pol, epsilon, sigma, mdvar,
    time, location, situation, &A_db, &warnings
  );

  ITMResult r;
  r.status = status;
  r.warnings = (int32_t)warnings;
  r.A_db = A_db;
  return r;
}

DLLEXPORT ITMResult ITM_AREA_CR_r(
  double h_tx__meter, double h_rx__meter,
  int tx_site_criteria, int rx_site_criteria, double d__km,
  double delta_h__meter, int climate, double N_0, double f__mhz,
  int pol, double epsilon, double sigma, int mdvar,
  double confidence, double reliability)
{
  double A_db = 0.0;
  long warnings = 0;

  int status = ITM_AREA_CR(
    h_tx__meter, h_rx__meter, tx_site_criteria, rx_site_criteria, d__km,
    delta_h__meter, climate, N_0, f__mhz, pol, epsilon, sigma, mdvar,
    confidence, reliability, &A_db, &warnings
  );

  ITMResult r;
  r.status = status;
  r.warnings = (int32_t)warnings;
  r.A_db = A_db;
  return r;
}

// =================== *_Ex (with IntermediateValues) ===================
DLLEXPORT ITMResultEx ITM_P2P_TLS_Ex_r(
  double h_tx__meter, double h_rx__meter, val pfl,
  int climate, double N_0, double f__mhz, int pol,
  double epsilon, double sigma, int mdvar,
  double time, double location, double situation)
{
  double A_db = 0.0;
  long warnings = 0;
  auto pfl_vec = convertJSArrayToNumberVector<double>(pfl);
  
  struct IntermediateValues inter; memset(&inter, 0, sizeof(inter));
  int status = ITM_P2P_TLS_Ex(
    h_tx__meter, h_rx__meter, pfl_vec.data(), 
    climate, N_0, f__mhz, pol,
    epsilon, sigma, mdvar, time, location, situation,
    &A_db, &warnings, &inter
  );

  ITMResultEx r;
  r.status = status;
  r.warnings = (int32_t)warnings;
  r.A_db = A_db;
  to_result(&inter, &r);
  return r;
}

DLLEXPORT ITMResultEx ITM_P2P_CR_Ex_r(
  double h_tx__meter, double h_rx__meter, val pfl,
  int climate, double N_0, double f__mhz, int pol,
  double epsilon, double sigma, int mdvar,
  double confidence, double reliability)
{
  double A_db = 0.0;
  long warnings = 0;
  auto pfl_vec = convertJSArrayToNumberVector<double>(pfl);

  struct IntermediateValues inter; memset(&inter, 0, sizeof(inter));
  int status = ITM_P2P_CR_Ex(
    h_tx__meter, h_rx__meter, pfl_vec.data(),
    climate, N_0, f__mhz, pol,
    epsilon, sigma, mdvar, confidence, reliability,
    &A_db, &warnings, &inter
  );

  ITMResultEx r;
  r.status = status;
  r.warnings = (int32_t)warnings;
  r.A_db = A_db;
  to_result(&inter, &r);
  return r;
}

DLLEXPORT ITMResultEx ITM_AREA_TLS_Ex_r(
  double h_tx__meter, double h_rx__meter,
  int tx_site_criteria, int rx_site_criteria, double d__km,
  double delta_h__meter, int climate, double N_0, double f__mhz,
  int pol, double epsilon, double sigma, int mdvar,
  double time, double location, double situation)
{
  double A_db = 0.0;
  long warnings = 0;

  struct IntermediateValues inter; memset(&inter, 0, sizeof(inter));
  int status = ITM_AREA_TLS_Ex(
    h_tx__meter, h_rx__meter, tx_site_criteria, rx_site_criteria, d__km,
    delta_h__meter, climate, N_0, f__mhz, pol, epsilon, sigma, mdvar,
    time, location, situation, &A_db, &warnings, &inter
  );

  ITMResultEx r;
  r.status = status;
  r.warnings = (int32_t)warnings;
  r.A_db = A_db;
  to_result(&inter, &r);
  return r;
}

DLLEXPORT ITMResultEx ITM_AREA_CR_Ex_r(
  double h_tx__meter, double h_rx__meter,
  int tx_site_criteria, int rx_site_criteria, double d__km,
  double delta_h__meter, int climate, double N_0, double f__mhz,
  int pol, double epsilon, double sigma, int mdvar,
  double confidence, double reliability)
{
  double A_db = 0.0;
  long warnings = 0;

  struct IntermediateValues inter; memset(&inter, 0, sizeof(inter));
  int status = ITM_AREA_CR_Ex(
    h_tx__meter, h_rx__meter, tx_site_criteria, rx_site_criteria, d__km,
    delta_h__meter, climate, N_0, f__mhz, pol, epsilon, sigma, mdvar,
    confidence, reliability, &A_db, &warnings, &inter
  );

  ITMResultEx r;
  r.status = status;
  r.warnings = (int32_t)warnings;
  r.A_db = A_db;
  to_result(&inter, &r);
  return r;
}

EMSCRIPTEN_BINDINGS(itm_bindings) {
  value_object<ITMResult>("ITMResult")
    .field("status",   &ITMResult::status)
    .field("warnings", &ITMResult::warnings)
    .field("A_db",     &ITMResult::A_db);

  value_object<ITMResultEx>("ITMResultEx")
    .field("status",            &ITMResultEx::status)
    .field("warnings",          &ITMResultEx::warnings)
    .field("A_db",              &ITMResultEx::A_db)
    .field("theta_hzn_0",       &ITMResultEx::theta_hzn_0)
    .field("theta_hzn_1",       &ITMResultEx::theta_hzn_1)
    .field("d_hzn__meter_0",    &ITMResultEx::d_hzn__meter_0)
    .field("d_hzn__meter_1",    &ITMResultEx::d_hzn__meter_1)
    .field("h_e__meter_0",      &ITMResultEx::h_e__meter_0)
    .field("h_e__meter_1",      &ITMResultEx::h_e__meter_1)
    .field("N_s",               &ITMResultEx::N_s)
    .field("delta_h__meter",    &ITMResultEx::delta_h__meter)
    .field("A_ref__db",         &ITMResultEx::A_ref__db)
    .field("A_fs__db",          &ITMResultEx::A_fs__db)
    .field("d__km",             &ITMResultEx::d__km)
    .field("mode",              &ITMResultEx::mode
  );

  emscripten::function("ITM_AREA_TLS_r"   , &ITM_AREA_TLS_r   );
  emscripten::function("ITM_AREA_CR_r"    , &ITM_AREA_CR_r    );
  emscripten::function("ITM_AREA_TLS_Ex_r", &ITM_AREA_TLS_Ex_r);
  emscripten::function("ITM_AREA_CR_Ex_r" , &ITM_AREA_CR_Ex_r );

  emscripten::function("ITM_P2P_TLS"      , &ITM_P2P_TLS_r    );
  emscripten::function("ITM_P2P_CR"       , &ITM_P2P_CR_r     );
  emscripten::function("ITM_P2P_TLS_Ex"   , &ITM_P2P_TLS_Ex_r );
  emscripten::function("ITM_P2P_CR_Ex"    , &ITM_P2P_CR_Ex_r  );
}
