# scorep_plugin_mpi
Score-P MPI data acquisition plugin

git clone --recurse-submodules  https://github.com/shuki-zanyovka/scorep_plugin_ucx
cd scorep_plugin_ucx

To build:

mkdir BUILD
cd BUILD

cmake ../ -DCMAKE_C_STANDARD_COMPUTED_DEFAULT=GNU -DCMAKE_CXX_STANDARD_COMPUTED_DEFAULT=GNU -DCMAKE_CXX_COMPILER=/usr/local/bin/g++ -DCMAKE_C_COMPILER=/usr/local/bin/gcc
make


Then, to use - Type the following commandline:


# Set path of plugin:
export SCOREP_PLUGIN_UCX_PATH=<PATH>/score-p-plugins/scorep_plugin_ucx/BUILD
export LD_LIBRARY_PATH=$SCOREP_PLUGIN_UCX_PATH:$LD_LIBRARY_PATH

export SCOREP_METRIC_PLUGINS="scorep_plugin_ucx"

# When using UCX@50, the number of UCX counters stored is limited to max = 50.
export SCOREP_METRIC_SCOREP_PLUGIN_UCX=UCX@50

# Enable tracing
export SCOREP_ENABLE_TRACING=true

# Note, that profiling must be disabled when using the UCX data acquisition plugin
export SCOREP_ENABLE_PROFILING=false

export SCOREP_TOTAL_MEMORY=1000M

export UCX_STATS_DEST="udp:localhost:37873"

# Apply filter:
export UCX_STATS_FILTER="rx_am*,bytes_short,bytes_bcopy,bytes_zcopy,rx*,tx*"

mpirun -n 2 <mpi_application>

