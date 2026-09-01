// Copyright 2026 The Lynx Authors. All rights reserved.
// Licensed under the Apache License Version 2.0 that can be found in the
// LICENSE file in the root directory of this source tree.

#import "clay/shell/platform/darwin/macos/framework/Source/overlay_platform_plugin_mac.h"

#include <utility>

namespace clay {

OverlayPlatformPluginMac::OverlayPlatformPluginMac(
    ClayOverlayView* overlay_view)
    : overlay_view_(overlay_view) {}

void OverlayPlatformPluginMac::SetHitTestTransparent(bool transparent) {
  hit_test_transparent_ = transparent;
  if (node_id_ != -1) {
    [overlay_view_ setHitTestTransparent:transparent forViewId:node_id_];
  }
}

void OverlayPlatformPluginMac::OnDetachFromTree() {
  if (node_id_ != -1) {
    [overlay_view_ removeOpaqueRectForViewId:node_id_];
  }
}

void OverlayPlatformPluginMac::OnViewDestroy() {
  if (node_id_ != -1) {
    [overlay_view_ removeHitTestStateForViewId:node_id_];
    node_id_ = -1;
  }
}

void OverlayPlatformPluginMac::InitPlatformOverlay(
    std::shared_ptr<Actor<fml::WeakPtr<OverlayListener>>> overlay_listener,
    int id, std::string tag, ExternalViewPlugin* recording_plugin) {
  OverlayPlatformPlugin::InitPlatformOverlay(
      std::move(overlay_listener), id, std::move(tag), recording_plugin);
  node_id_ = id;
  [overlay_view_ setHitTestTransparent:hit_test_transparent_
                             forViewId:node_id_];
}

std::unique_ptr<OverlayPlatformPlugin>
OverlayPlatformServiceMac::CreateOverlayPlatformPlugin() {
  if (!overlay_view_controller_service_) {
    return nullptr;
  }
  return std::make_unique<OverlayPlatformPluginMac>(
      overlay_view_controller_service_->GetOverlayView());
}

void OverlayPlatformServiceMac::OnInit(
    ServiceManager& service_manager, const PlatformServiceContext& ctx) {
  overlay_view_controller_service_ =
      service_manager.GetService<OverlayViewControllerService>();
}

void OverlayPlatformServiceMac::OnDestroy() {
  overlay_view_controller_service_ = nullptr;
}

}  // namespace clay
