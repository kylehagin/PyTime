#include "displayapp/screens/PeerComm.h"
#include <lvgl/lvgl.h>

using namespace Pinetime::Applications::Screens;

PeerComm::PeerComm(Controllers::PeerCommService& svc) : service{svc} {
  label = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_color(label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  lv_obj_set_style_local_text_font(label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &jetbrains_mono_bold_20);
  lv_label_set_text(label, "PeerComm ready");
  lv_obj_align(label, nullptr, LV_ALIGN_CENTER, 0, 0);
  taskRefresh = lv_task_create(RefreshTaskCallback, 500, LV_TASK_PRIO_MID, this);
}

PeerComm::~PeerComm() {
  lv_task_del(taskRefresh);
  lv_obj_clean(lv_scr_act());
}

bool PeerComm::OnTouchEvent(TouchEvents event) {
  if (event == TouchEvents::Tap) {
    service.Send("ping");
    lv_label_set_text(label, "Sent: ping");
    return true;
  }
  return false;
}

void PeerComm::Refresh() {
  if (service.HasMessage()) {
    std::string msg = service.Receive();
    lv_label_set_text_fmt(label, "Recv: %s", msg.c_str());
  }
}

