#pragma once
#ifndef __SAS__
#define __SAS__ extern
#endif

__SAS__ TextLayer *sas_layer;
__SAS__ void init_sas();
__SAS__ void deinit_sas();
__SAS__ void update_sas(bool on);