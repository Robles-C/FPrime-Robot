// ======================================================================
// \title  Tank1Topology.cpp
// \brief cpp file containing the topology instantiation code
//
// ======================================================================
// Provides access to autocoded functions
#include <Tank1/Top/Tank1TopologyAc.hpp>
// Note: Uncomment when using Svc:TlmPacketizer
//#include <Tank1/Top/Tank1PacketsAc.hpp>

// Necessary project-specified types
#include <Fw/Types/MallocAllocator.hpp>
#include <Svc/FramingProtocol/FprimeProtocol.hpp>

// Used for 1Hz synthetic cycling
#include <Os/Mutex.hpp>
#include <Fw/Logger/Logger.hpp>

// Allows easy reference to objects in FPP/autocoder required namespaces
using namespace Tank1;

// The reference topology uses a malloc-based allocator for components that need to allocate memory during the
// initialization phase.
Fw::MallocAllocator mallocator;

// The reference topology uses the F´ packet protocol when communicating with the ground and therefore uses the F´
// framing and deframing implementations.
Svc::FprimeFraming framing;
Svc::FprimeDeframing deframing;

Svc::ComQueue::QueueConfigurationTable configurationTable;

// The reference topology divides the incoming clock signal (1Hz) into sub-signals: 1Hz, 1/2Hz, and 1/4Hz with 0 offset
Svc::RateGroupDriver::DividerSet rateGroupDivisorsSet{{{1, 0}, {5, 0}, {10, 0}}};

// Rate groups may supply a context token to each of the attached children whose purpose is set by the project. The
// reference topology sets each token to zero as these contexts are unused in this project.
NATIVE_INT_TYPE rateGroup1Context[Svc::ActiveRateGroup::CONNECTION_COUNT_MAX] = {};
NATIVE_INT_TYPE rateGroup2Context[Svc::ActiveRateGroup::CONNECTION_COUNT_MAX] = {};
NATIVE_INT_TYPE rateGroup3Context[Svc::ActiveRateGroup::CONNECTION_COUNT_MAX] = {};

// A number of constants are needed for construction of the topology. These are specified here.
enum TopologyConstants {
    CMD_SEQ_BUFFER_SIZE = 5 * 1024,
    FILE_DOWNLINK_TIMEOUT = 1000,
    FILE_DOWNLINK_COOLDOWN = 1000,
    FILE_DOWNLINK_CYCLE_TIME = 1000,
    FILE_DOWNLINK_FILE_QUEUE_DEPTH = 10,
    HEALTH_WATCHDOG_CODE = 0x123,
    COMM_PRIORITY = 100,
    // bufferManager constants
    FRAMER_BUFFER_SIZE = FW_MAX(FW_COM_BUFFER_MAX_SIZE, FW_FILE_BUFFER_MAX_SIZE + sizeof(U32)) + HASH_DIGEST_LENGTH + Svc::FpFrameHeader::SIZE,
    FRAMER_BUFFER_COUNT = 30,
    DEFRAMER_BUFFER_SIZE = FW_MAX(FW_COM_BUFFER_MAX_SIZE, FW_FILE_BUFFER_MAX_SIZE + sizeof(U32)),
    DEFRAMER_BUFFER_COUNT = 30,
    COM_DRIVER_BUFFER_SIZE = 3000,
    COM_DRIVER_BUFFER_COUNT = 30,
    BUFFER_MANAGER_ID = 200
};

// Ping entries are autocoded, however; this code is not properly exported. Thus, it is copied here.
Svc::Health::PingEntry pingEntries[] = {
    {PingEntries::Tank1_blockDrv::WARN, PingEntries::Tank1_blockDrv::FATAL, "blockDrv"},
    {PingEntries::Tank1_tlmSend::WARN, PingEntries::Tank1_tlmSend::FATAL, "chanTlm"},
    {PingEntries::Tank1_cmdDisp::WARN, PingEntries::Tank1_cmdDisp::FATAL, "cmdDisp"},
    {PingEntries::Tank1_cmdSeq::WARN, PingEntries::Tank1_cmdSeq::FATAL, "cmdSeq"},
    {PingEntries::Tank1_eventLogger::WARN, PingEntries::Tank1_eventLogger::FATAL, "eventLogger"},
    {PingEntries::Tank1_fileDownlink::WARN, PingEntries::Tank1_fileDownlink::FATAL, "fileDownlink"},
    {PingEntries::Tank1_fileManager::WARN, PingEntries::Tank1_fileManager::FATAL, "fileManager"},
    {PingEntries::Tank1_fileUplink::WARN, PingEntries::Tank1_fileUplink::FATAL, "fileUplink"},
    {PingEntries::Tank1_prmDb::WARN, PingEntries::Tank1_prmDb::FATAL, "prmDb"},
    {PingEntries::Tank1_rateGroup1::WARN, PingEntries::Tank1_rateGroup1::FATAL, "rateGroup1"},
    {PingEntries::Tank1_rateGroup2::WARN, PingEntries::Tank1_rateGroup2::FATAL, "rateGroup2"},
    {PingEntries::Tank1_rateGroup3::WARN, PingEntries::Tank1_rateGroup3::FATAL, "rateGroup3"},
};

/**
 * \brief configure/setup components in project-specific way
 *
 * This is a *helper* function which configures/sets up each component requiring project specific input. This includes
 * allocating resources, passing-in arguments, etc. This function may be inlined into the topology setup function if
 * desired, but is extracted here for clarity.
 */
void configureTopology(const TopologyState& state) {
    // Buffer managers need a configured set of buckets and an allocator used to allocate memory for those buckets.
    Svc::BufferManager::BufferBins upBuffMgrBins;
    memset(&upBuffMgrBins, 0, sizeof(upBuffMgrBins));
    upBuffMgrBins.bins[0].bufferSize = FRAMER_BUFFER_SIZE;
    upBuffMgrBins.bins[0].numBuffers = FRAMER_BUFFER_COUNT;
    upBuffMgrBins.bins[1].bufferSize = DEFRAMER_BUFFER_SIZE;
    upBuffMgrBins.bins[1].numBuffers = DEFRAMER_BUFFER_COUNT;
    upBuffMgrBins.bins[2].bufferSize = COM_DRIVER_BUFFER_SIZE;
    upBuffMgrBins.bins[2].numBuffers = COM_DRIVER_BUFFER_COUNT;
    bufferManager.setup(BUFFER_MANAGER_ID, 0, mallocator, upBuffMgrBins);

    // Framer and Deframer components need to be passed a protocol handler
    framer.setup(framing);
    deframer.setup(deframing);

    // Command sequencer needs to allocate memory to hold contents of command sequences
    cmdSeq.allocateBuffer(0, mallocator, CMD_SEQ_BUFFER_SIZE);

    // Rate group driver needs a divisor list
    rateGroupDriver.configure(rateGroupDivisorsSet);

    // Rate groups require context arrays.
    rateGroup1.configure(rateGroup1Context, FW_NUM_ARRAY_ELEMENTS(rateGroup1Context));
    rateGroup2.configure(rateGroup2Context, FW_NUM_ARRAY_ELEMENTS(rateGroup2Context));
    rateGroup3.configure(rateGroup3Context, FW_NUM_ARRAY_ELEMENTS(rateGroup3Context));

    // File downlink requires some project-derived properties.
    fileDownlink.configure(FILE_DOWNLINK_TIMEOUT, FILE_DOWNLINK_COOLDOWN, FILE_DOWNLINK_CYCLE_TIME,
                           FILE_DOWNLINK_FILE_QUEUE_DEPTH);

    // Parameter database is configured with a database file name, and that file must be initially read.
    prmDb.configure("PrmDb.dat");
    prmDb.readParamFile();

    // Health is supplied a set of ping entires.
    health.setPingEntries(pingEntries, FW_NUM_ARRAY_ELEMENTS(pingEntries), HEALTH_WATCHDOG_CODE);

    // Note: Uncomment when using Svc:TlmPacketizer
    // tlmSend.setPacketList(Tank1PacketsPkts, Tank1PacketsIgnore, 1);

    // Events (highest-priority)
    configurationTable.entries[0] = {.depth = 100, .priority = 0};
    // Telemetry
    configurationTable.entries[1] = {.depth = 500, .priority = 2};
    // File Downlink
    configurationTable.entries[2] = {.depth = 100, .priority = 1};
    // Allocation identifier is 0 as the MallocAllocator discards it
    comQueue.configure(configurationTable, 0, mallocator);
    if (state.hostname != nullptr && state.port != 0) {
        comDriver.configure(state.hostname, state.port);
    }

    bool openSuccess = i2cDriver.open("/dev/i2c-1");

    Os::File::Status status = openSuccess ? Os::File::Status::OP_OK : Os::File::Status::DOESNT_EXIST;
    if (status != Os::File::Status::OP_OK) {
        Fw::Logger::log("[ERROR] Failed to open I2C device: /dev/i2c-1\n");
    }

    Os::File::Status status1 = gpioDriverA.open("/dev/gpiochip0", 17, Drv::LinuxGpioDriver::GpioConfiguration::GPIO_INPUT);
    if (status1 != Os::File::Status::OP_OK) {
        Fw::Logger::log("[ERROR] Failed to open GPIO pin\n");
    }

    Os::File::Status status2 = gpioDriverB.open("/dev/gpiochip0", 21, Drv::LinuxGpioDriver::GpioConfiguration::GPIO_INPUT);
    if (status2 != Os::File::Status::OP_OK) {
        Fw::Logger::log("[ERROR] Failed to open GPIO pin\n");
    }

    Os::File::Status status3 = gpioTrig.open("/dev/gpiochip0", 23, Drv::LinuxGpioDriver::GpioConfiguration::GPIO_OUTPUT);
    if (status3 != Os::File::Status::OP_OK) {
        Fw::Logger::log("[ERROR] Failed to open GPIO pin\n");
    }

    Os::File::Status status4 = gpioEcho.open("/dev/gpiochip0", 24, Drv::LinuxGpioDriver::GpioConfiguration::GPIO_INPUT);
    if (status4 != Os::File::Status::OP_OK) {
        Fw::Logger::log("[ERROR] Failed to open GPIO pin\n");
    }

    Os::File::Status status5 = gpioLed.open("/dev/gpiochip0", 13, Drv::LinuxGpioDriver::GpioConfiguration::GPIO_OUTPUT);
    if (status5 != Os::File::Status::OP_OK) {
        Fw::Logger::log("[ERROR] Failed to open GPIO pin\n");
    }

}

// Public functions for use in main program are namespaced with deployment name Tank1
namespace Tank1 {
void setupTopology(const TopologyState& state) {
    // Autocoded initialization. Function provided by autocoder.
    initComponents(state);
    // Autocoded id setup. Function provided by autocoder.
    setBaseIds();
    // Autocoded connection wiring. Function provided by autocoder.
    connectComponents();
    // Autocoded configuration. Function provided by autocoder.
    configComponents(state);
    // Deployment-specific component configuration. Function provided above. May be inlined, if desired.
    configureTopology(state);
    // Autocoded command registration. Function provided by autocoder.
    regCommands();
    // Autocoded parameter loading. Function provided by autocoder.
    loadParameters();
    // Autocoded task kick-off (active components). Function provided by autocoder.
    startTasks(state);
    // Initialize socket communication if and only if there is a valid specification
    if (state.hostname != nullptr && state.port != 0) {
        Os::TaskString name("ReceiveTask");
        // Uplink is configured for receive so a socket task is started
        comDriver.start(name, COMM_PRIORITY, Default::STACK_SIZE);
    }
}

// Variables used for cycle simulation
Os::Mutex cycleLock;
volatile bool cycleFlag = true;

void startSimulatedCycle(Fw::TimeInterval interval) {
    cycleLock.lock();
    bool cycling = cycleFlag;
    cycleLock.unLock();

    // Main loop
    while (cycling) {
        Tank1::blockDrv.callIsr();
        Os::Task::delay(interval);

        cycleLock.lock();
        cycling = cycleFlag;
        cycleLock.unLock();
    }
}

void stopSimulatedCycle() {
    cycleLock.lock();
    cycleFlag = false;
    cycleLock.unLock();
}

void teardownTopology(const TopologyState& state) {
    // Autocoded (active component) task clean-up. Functions provided by topology autocoder.
    stopTasks(state);
    freeThreads(state);

    // Other task clean-up.
    comDriver.stop();
    (void)comDriver.join();

    // Resource deallocation
    cmdSeq.deallocateBuffer(mallocator);
    bufferManager.cleanup();
}
};  // namespace Tank1
