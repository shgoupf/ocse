#ifndef __afu_h__
#define __afu_h__

#include "Descriptor.h"
#include "TagManager.h"
#include "MachineController.h"

extern "C" {
#include "tlx_interface.h"
#include "utils.h"
}

#include <string>
#include <vector>

class AFU
{
private:
    enum AFU_State
    { IDLE, RESET, READY, RUNNING, WAITING_FOR_LAST_RESPONSES };

    AFU_EVENT afu_event;
    Descriptor descriptor;

    std::map < uint16_t, MachineController * >context_to_mc;
    std::map < uint16_t,
        MachineController * >::iterator highest_priority_mc;

    MachineController *machine_controller;

    AFU_State state;
    AFU_State config_state;

    uint64_t global_configs[3];	// stores MMIO registers for global configurations
    uint8_t  tlx_afu_cmd_max_credit;
    uint8_t  tlx_afu_data_max_credit;

    int reset_delay;

    void resolve_tlx_afu_cmd();
    void resolve_tlx_afu_resp();
    void tlx_afu_config_read();
    void tlx_afu_config_write();
    void resolve_control_event ();
    void resolve_response_event (uint32_t cycle);
    void set_seed ();
    void set_seed (uint32_t);

    void reset ();
    void reset_machine_controllers ();

    bool get_mmio_read_parity ();
    bool set_jerror_not_run;

public:
    /* constructor sets up descriptor from config file, establishes server socket connection
       and waits for client to connect */
    AFU (int port, std::string filename, bool parity, bool jerror);

    /* starts the main loop of the afu test platform */
    void start ();

    /* destrutor close the socket connection */
    ~AFU ();

};


#endif
