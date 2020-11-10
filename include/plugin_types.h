#if !defined (_PLUGIN_TYPES_H)
#define _PLUGIN_TYPES_H

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <thread>
#include <tuple>
#include <vector>
#include <sstream>
#include <list>


#include <scorep/SCOREP_MetricTypes.h>
#include <scorep/SCOREP_PublicTypes.h>

/* Counter IDs range (as provided to Score-P) */
#define SCOREP_COUNTER_ID_START_UCX   0

#define UCX_SAMPLING_METRIC_NAME "UCX@1"

/* Temporary counter name which will be renamed later if used */
#define TEMPORARY_COUNTER_NAME_TEMPLATE "temporary_counter_name_______________________________________"

/* TODO: Check if we can calculate this by the numbe of nodes */
#define UCX_NUM_TEMPORARY_COUNTERS 50

#define TEMPORARY_UCX_COUNTER_NAME_GEN(output_str, metric_name, counter_name, idx) {\
        char temp_str[10];\
        snprintf(temp_str, sizeof(temp_str), "%u", idx);\
        output_str = metric_name + "_" + TEMPORARY_COUNTER_NAME_TEMPLATE + temp_str;\
    }

/* The maximum size of the prefix of a counter name */
#define UCX_SAMPLING_COUNTER_NAME_PREFIX_MAX_SIZE 512

#define SCOREP_DEFINITIONS_NEWMETRIC_FUNC_NAME   "SCOREP_Definitions_NewMetric"
#define SCOREP_PROFILE_TRIGGER_INTEGER_FUNC_NAME "SCOREP_Profile_TriggerInteger"
#define SCOREP_LOCATION_GET_CURRENTCPULOCATION_FUNC_NAME "SCOREP_Location_GetCurrentCPULocation"
#define SCOREP_LOCATION_CREATENONCPULOCATION_FUNC_NAME "SCOREP_Location_CreateNonCPULocation"
#define SCOREP_ACQUIREPERPROCESSMETRICS_LOCATION_FUNC_NAME "SCOREP_Location_AcquirePerProcessMetricsLocation"
#define SCOREP_RELEASEPERPROCESSMETRICS_LOCATION_FUNC_NAME "SCOREP_Location_ReleasePerProcessMetricsLocation"
#define SCOREP_STRICTLY_SYNCHRONOUS_METRIC_NAME_UPDATE_FUNC_NAME "SCOREP_Strictly_Synchronous_metric_name_update"

typedef SCOREP_MetricHandle (* SCOREP_Definitions_NewMetric_t)(
        const char*                name,
        const char*                description,
        SCOREP_MetricSourceType    sourceType,
        SCOREP_MetricMode          mode,
        SCOREP_MetricValueType     valueType,
        SCOREP_MetricBase          base,
        int64_t                    exponent,
        const char*                unit,
        SCOREP_MetricProfilingType profilingType,
        SCOREP_MetricHandle        parentHandle );

/* We won't use the location in the plugin */
typedef uint64_t SCOREP_Location;

/**
   Called when a user metric / atomic / context event for integer values was triggered.
   @param thread A pointer to the thread location data of the thread that executed
                 the event.
   @param metric Handle of the triggered metric.
   @param value  Sample for the metric.
 */
typedef void (* SCOREP_Profile_TriggerInteger_t)( SCOREP_Location*    thread,
        SCOREP_MetricHandle metric,
        uint64_t            value );

/**
 * Returns a metric location. Into this location all asynchronous
 * metrics of a process will be recorded. As this location is shared among
 * adapters, this function acquires a mutex that need to be released by a call
 * to SCOREP_Location_ReleasePerProcessMetricsLocation(). The location is
 * created during the first call to this function.
 * If @a timestamp is non NULL, than also takes a timestamp for this location
 * and return it in @a *timestamp.
 */
typedef SCOREP_Location *
( *SCOREP_Location_AcquirePerProcessMetricsLocation_t)( uint64_t* timestamp );


typedef void
( *SCOREP_Location_ReleasePerProcessMetricsLocation_t)( void );

typedef void
( *SCOREP_Strictly_Synchronous_metric_name_update_t)(const char *current_name,
        const char *new_name, const char *metric_name, size_t num_metrics_set);

/**
 * This function can be used by subsystems to create new locations.
 *
 * @param parent            Handle of parent location.
 * @param type              Type of new location.
 * @param name              Name of new location.
 *
 * @return Returns handle for new location.
 */
typedef SCOREP_Location*
(* SCOREP_Location_CreateNonCPULocation_t)( SCOREP_Location*    parent,
                                      SCOREP_LocationType type,
                                      const char*         name );

typedef SCOREP_Location * ( *SCOREP_Location_GetCurrentCPULocation_t)();

typedef struct tag_scorep_counter_data {
    /* name of the counter */
    char name[UCX_SAMPLING_COUNTER_NAME_PREFIX_MAX_SIZE];

    /* Counter ID */
    uint32_t scorep_counter_id;

    /* Score-P metric handle */
    SCOREP_MetricHandle metric_handle;

    /* Score-P metric already renamed */
    uint32_t scorep_metric_already_renamed;

    /* Recorded metric value */
    uint64_t value;

} scorep_counter_data_t;

typedef std::vector<scorep_counter_data_t *> scorep_counters_list_t;

#endif /* _PLUGIN_TYPES_H */
