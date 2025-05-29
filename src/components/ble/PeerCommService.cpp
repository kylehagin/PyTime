#include "components/ble/PeerCommService.h"
#include "components/ble/NimbleController.h"
#include <cstring>
#include <nrf_log.h>

using namespace Pinetime::Controllers;

namespace {
  constexpr ble_uuid128_t CharUuid(uint8_t x) {
    return ble_uuid128_t{.u = {.type = BLE_UUID_TYPE_128},
                         .value = {0xd0,0x42,0x19,0x3a,0x3b,0x43,0x23,0x8e,0xfe,0x48,0xfc,0x78,x,0xa1,0x00,0x00}};
  }

  constexpr ble_uuid128_t serviceUuid{CharUuid(0x00)};
  constexpr ble_uuid128_t txUuid{CharUuid(0x01)};
  constexpr ble_uuid128_t rxUuid{CharUuid(0x02)};

  int Callback(uint16_t, uint16_t, ble_gatt_access_ctxt* ctxt, void* arg) {
    return static_cast<PeerCommService*>(arg)->OnCommand(ctxt);
  }
}

PeerCommService::PeerCommService(NimbleController& nimble)
    : nimble{nimble} {
  characteristicDefinition[0] = {.uuid = &txUuid.u,
                                 .access_cb = Callback,
                                 .arg = this,
                                 .flags = BLE_GATT_CHR_F_NOTIFY,
                                 .val_handle = &txHandle};
  characteristicDefinition[1] = {.uuid = &rxUuid.u,
                                 .access_cb = Callback,
                                 .arg = this,
                                 .flags = BLE_GATT_CHR_F_WRITE};
  characteristicDefinition[2] = {0};

  serviceDefinition[0] = {.type = BLE_GATT_SVC_TYPE_PRIMARY, .uuid = &serviceUuid.u, .characteristics = characteristicDefinition};
  serviceDefinition[1] = {0};
}

void PeerCommService::Init() {
  int res = ble_gatts_count_cfg(serviceDefinition);
  ASSERT(res == 0);
  res = ble_gatts_add_svcs(serviceDefinition);
  ASSERT(res == 0);
}

int PeerCommService::OnCommand(ble_gatt_access_ctxt* ctxt) {
  if (ble_uuid_cmp(ctxt->chr->uuid, &rxUuid.u) == 0 && ctxt->op == BLE_GATT_ACCESS_OP_WRITE_CHR) {
    size_t len = OS_MBUF_PKTLEN(ctxt->om);
    char data[len + 1];
    os_mbuf_copydata(ctxt->om, 0, len, data);
    data[len] = '\0';
    incoming.assign(data, len);
    messageAvailable = true;
  }
  return 0;
}

void PeerCommService::Send(const char* msg) {
  os_mbuf* om = ble_hs_mbuf_from_flat(msg, strlen(msg));
  uint16_t conn = nimble.connHandle();
  if (conn != 0 && conn != BLE_HS_CONN_HANDLE_NONE) {
    ble_gattc_notify_custom(conn, txHandle, om);
  }
}

bool PeerCommService::HasMessage() const {
  return messageAvailable;
}

std::string PeerCommService::Receive() {
  messageAvailable = false;
  return incoming;
}

