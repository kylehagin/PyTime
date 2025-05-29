#pragma once
#include <string>
#include <host/ble_gap.h>
#include <host/ble_gatt.h>

namespace Pinetime {
  namespace Controllers {
    class NimbleController;

    class PeerCommService {
    public:
      explicit PeerCommService(NimbleController& nimble);
      void Init();
      int OnCommand(struct ble_gatt_access_ctxt* ctxt);
      void Send(const char* msg);
      bool HasMessage() const;
      std::string Receive();

    private:
      NimbleController& nimble;
      bool messageAvailable = false;
      std::string incoming;
      ble_gatt_chr_def characteristicDefinition[3];
      ble_gatt_svc_def serviceDefinition[2];
      uint16_t txHandle = 0;
      uint16_t rxHandle = 0;
    };
  }
}
