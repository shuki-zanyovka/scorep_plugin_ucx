#if !defined(_SCOREP_PLUGIN_UCX_H_)
#define _SCOREP_PLUGIN_UCX_H_

#include <scorep/plugin/plugin.hpp>

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <thread>
#include <tuple>
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif
#include <mpi.h>
#ifdef __cplusplus
}
#endif

#include <ucx_sampling.h>
#include <plugin_types.h>

using namespace scorep::plugin::policy;
using ThreadId = std::thread::id;
using TimeValuePair = std::pair<scorep::chrono::ticks, double>;
using MetricProperty = scorep::plugin::metric_property;
using ThreadEventPair = std::tuple<ThreadId, std::string>;


class scorep_plugin_ucx : public scorep::plugin::base<scorep_plugin_ucx,
    sync, once, scorep_clock>
{
    public:
        scorep_plugin_ucx();

        ~scorep_plugin_ucx();

        std::vector<MetricProperty>
        get_metric_properties(const std::string& metric_name);

        int32_t
        add_metric(const std::string& metric);

        void
        start();

        void
        stop();

        template <typename Proxy>
        void get_current_value(int32_t id, Proxy& proxy);

        template <typename Proxy>
        void get_optional_value(int32_t id, Proxy& proxy);

    private:
        /* Indicates whether or not MPI is initialized */
        int m_mpi_t_initialized;

        /* MPI rank of this process */
        int m_mpi_rank;

        /* UCX sampling object */
        ucx_sampling m_ucx_sampling;
        std::string m_ucx_metric_name;
        size_t m_n_ucx_counters;

        /* UCX counters list + Score-P handles */
        scorep_counters_list_t m_ucx_counters_list;

        /* Pointer to Score-P function SCOREP_Definitions_NewMetric() */
        SCOREP_Definitions_NewMetric_t m_pSCOREP_Definitions_NewMetric_func;

        /* Pointer to Score-P function SCOREP_Profile_TriggerInteger() */
        SCOREP_Profile_TriggerInteger_t m_pSCOREP_Profile_TriggerInteger_func;

        SCOREP_Location_GetCurrentCPULocation_t m_pSCOREP_Location_GetCurrentCPULocation_func;

        SCOREP_Location_CreateNonCPULocation_t m_pSCOREP_Location_CreateNonCPULocation_func;

        SCOREP_Location_AcquirePerProcessMetricsLocation_t m_pSCOREP_Location_AcquirePerProcessMetricsLocation_func;

        SCOREP_Location_ReleasePerProcessMetricsLocation_t m_pSCOREP_Location_ReleasePerProcessMetricsLocation_func;

        SCOREP_Strictly_Synchronous_metric_name_update_t m_pSCOREP_Strictly_Synchronous_metric_name_update_func;

        SCOREP_Location *m_thread_location = NULL;

        void
        ucx_counters_scorep_update(void);

        /* Renames a Score-P metric (for dynamic allocation of metrics) */
        int
        scorep_metric_rename(uint32_t counter_id, const char *counter_new_name, size_t num_metrics_set);
};


template <typename Proxy>
void
scorep_plugin_ucx::get_current_value(int32_t id, Proxy& proxy)
{
    uint64_t val = 0;
    int ret;
    int flag;

    /* UCX counter */
    /* Enumerate PVARs */
    if (0 == m_mpi_t_initialized) {

       ret = MPI_Initialized(&flag);
       if (flag) {
         m_mpi_t_initialized = 1;
         /* get global rank */
         PMPI_Comm_rank(MPI_COMM_WORLD, &m_mpi_rank);
       }
    }

#if 0
    int mpi_rank = mpi_t_sampling_object.mpi_rank_get();
#endif

    /*
       Need MPI_T initialization here since the UDP port number of the
       UCX statistics server is derived from the process_id.
       Also, all statistics are aggregated by MPI_rank 0 (root).
    */
    if (m_mpi_t_initialized) {// && (mpi_rank == 0)) {
        /* Get UCX statistics */
        ret = m_ucx_sampling.ucx_statistics_current_value_get(m_mpi_rank, id,
                &m_ucx_counters_list, &val);
        /* Rename UCX counters in Score-P log? only if ret != 0 */
        if (ret) {
           ucx_counters_scorep_update();
        }
    }

    proxy.write(val);
}


template <typename Proxy>
void
scorep_plugin_ucx::get_optional_value(int32_t id, Proxy& proxy)
{
    get_current_value(id, proxy);
}

#endif /* _SCOREP_PLUGIN_UCX_H_ */
