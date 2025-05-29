#pragma once
#include "displayapp/screens/Screen.h"
#include "displayapp/apps/Apps.h"
#include "displayapp/Controllers.h"
#include "Symbols.h"
#include <lvgl/lvgl.h>

namespace Pinetime {
  namespace Applications {
    namespace Screens {
      class PeerComm : public Screen {
      public:
        explicit PeerComm(Controllers::PeerCommService& service);
        ~PeerComm() override;
        bool OnTouchEvent(TouchEvents event) override;
        void Refresh() override;

      private:
        Controllers::PeerCommService& service;
        lv_obj_t* label{};
        lv_task_t* taskRefresh{};
      };
    }

    template <>
    struct AppTraits<Apps::PeerComm> {
      static constexpr Apps app = Apps::PeerComm;
      static constexpr const char* icon = Screens::Symbols::bluetooth;
      static Screens::Screen* Create(AppControllers& controllers) {
        return new Screens::PeerComm(*controllers.peerCommService);
      };
    };
  }
}
