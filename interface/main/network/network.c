/**
 * @brief network layer
 *
 * CLOSED SOURCE, NOT FOR PUBLIC RELEASE
 * (c) Copyright 2015 - 2020, Coert Vonk
 * All rights reserved. Use of copyright notice does not imply publication.
 * All text above must be included in any redistribution
 **/

#include <string.h>
#include <esp_system.h>
#include <esp_log.h>

#include "../datalink/datalink.h"
#include "network.h"

uint8_t
network_ic_len(uint8_t const ic_typ)
{
    switch (ic_typ) {
        case DATALINK_CHLOR_TYP_PING_REQ:
            return sizeof(mChlorPingReq_ic_t);
        case DATALINK_CHLOR_TYP_PING:
            return sizeof(mChlorPing_ic_t);
        case DATALINK_CHLOR_TYP_NAME:
            return sizeof(mChlorName_ic_t);
        case DATALINK_CHLOR_TYP_LEVEL_SET:
            return sizeof(mChlorLevelSet_ic_t);
        case DATALINK_CHLOR_TYP_LEVEL_RESP:
            return sizeof(mChlorPing_ic_t);
        case DATALINK_CHLOR_TYP_X14:
            return sizeof(mChlor0X14_ic_t);
        default:
            return 0;
    };
}

network_addrgroup_t
network_groupaddr(uint16_t const addr)
{
	return (network_addrgroup_t)(addr >> 4);
}

uint8_t
network_devaddr(uint8_t group, uint8_t const id)
{
	return (group << 4) | id;
}
