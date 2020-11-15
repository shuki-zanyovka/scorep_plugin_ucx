/**
* Copyright (C) Mellanox Technologies Ltd. 2001-2013.  ALL RIGHTS RESERVED.
* Copyright (C) Huawei Technologies Co., Ltd. 2020.  ALL RIGHTS RESERVED.
*/
#if !defined(_UCX_SAMPLING_H_)
#define _UCX_SAMPLING_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
#include <ucs/stats/libstats.h>
#include <ucs/stats/stats.h>
#ifdef __cplusplus
}
#endif

#include <plugin_types.h>

/* Enable Debug prints */
//#define UCX_DEBUG_ENABLE

#if defined(UCX_DEBUG_ENABLE)
#define DEBUG_PRINT printf
#else
#define DEBUG_PRINT
#endif

#define THRESHOLD 						0
#define NOT_FOUND 						-1
#define FALSE 							0
#define TRUE 							1
#define STR_SZ 							256
#define NUM_PERF_VAR_SUPPORTED			50

#define NEG_INF							-10000000
#define POS_INF							10000000

/*********************************/
/* Main class for ucx Sampling */
/*********************************/
class ucx_sampling {
public:
   /* Constructor */
   ucx_sampling();

   /* Destructor */
   ~ucx_sampling();

   /* Read current value of a PVAR */
   int
   ucx_statistics_current_value_get(int mpi_rank, uint32_t index,
       scorep_counters_list_t *ucx_counters_list, uint64_t *value, uint64_t *prev_value);

   int
   ucx_statistics_server_start(int port);

   /* Update & initialize counters from UCX statistics */
   void
   ucx_statistics_all_counters_update(scorep_counters_list_t *new_counters_list,
       int initialize_counters_enable);

private:

   uint64_t
   recursive_scan_counters_list(ucs_stats_node_t *root,
       ucs_list_link_t *stats,
       const char *counters_root_name,
       scorep_counters_list_t *ucx_counters_list,
       uint64_t num_objects,
       uint64_t& num_counters,
       int initialize_counters_enable);

   /* Scan the counters list to see if there are any new counters */
   void
   scan_counters_list(ucs_list_link_t *stats,
       const char *counters_root_name,
       scorep_counters_list_t *ucx_counters_list,
       int initialize_counters_enable);

private:

   /* Handle for UCX statistics server */
   ucs_stats_server_h m_ucx_stats_server;

   /*
      Enable statistics server on process,
      if port bound successfully
   */
   int m_statistics_server_process_enable;

   /* Score-P counters initialized (updated dynamically) */
   int m_counters_initialized_on_scorep;
};


#endif /* _UCX_SAMPLING_H_ */
