// Copyright 2026 The Lynx Authors. All rights reserved.
// Licensed under the Apache License Version 2.0 that can be found in the
// LICENSE file in the root directory of this source tree.

#ifndef CLAY_SHELL_PLATFORM_DARWIN_MACOS_OVERLAY_PLATFORM_PLUGIN_MAC_H_
#define CLAY_SHELL_PLATFORM_DARWIN_MACOS_OVERLAY_PLATFORM_PLUGIN_MAC_H_

#include <memory>
#include <string>

#include "clay/common/service/service_manager.h"
#include "clay/shell/platform/darwin/macos/framework/Source/overlay_view_controller_service.h"
#include "clay/ui/platform/overlay_service.h"

namespace clay {

class OverlayPlatformPluginMac final : public OverlayPlatformPlugin {
 public:
  explicit OverlayPlatformPluginMac(ClayOverlayView* overlay_view);
  ~OverlayPlatformPluginMac() override = default;

  void ChangeVisibility(bool visible) override {}
  void SetLevel(int level) override {}
  void SetCutOutMode(bool is_cut_out) override {}
  void SetAndroidSoftInputMode(std::string mode) override {}
  void SetAndroidNativeEventPass(bool is_pass) override {}
  void SetStatusBarTranslucent(bool is_translucent) override {}
  void SetStatusBarTranslucentStyle(std::string style) override {}
  void SetAndroidFullScreen(bool is_full_screen) override {}
  void SetHitTestTransparent(bool transparent) override;
  void SetPreferredSize(int width, int height) override {}

  void OnAttachToTree() override {}
  void OnDetachFromTree() override;
  void OnViewDestroy() override;

  void InitPlatformOverlay(
      std::shared_ptr<Actor<fml::WeakPtr<OverlayListener>>> overlay_listener,
      int id, std::string tag,
      ExternalViewPlugin* recording_plugin) override;

 private:
  int64_t node_id_ = -1;
  bool hit_test_transparent_ = false;
  __weak ClayOverlayView* overlay_view_ = nil;
};

class OverlayPlatformServiceMac final : public OverlayService {
 public:
  std::unique_ptr<OverlayPlatformPlugin> CreateOverlayPlatformPlugin()
      override;

  void OnInit(ServiceManager& service_manager,
              const PlatformServiceContext& ctx) override;
  void OnDestroy() override;

 private:
  Puppet<Owner::kPlatform, OverlayViewControllerService>
      overlay_view_controller_service_;
};

}  // namespace clay

#endif  // CLAY_SHELL_PLATFORM_DARWIN_MACOS_OVERLAY_PLATFORM_PLUGIN_MAC_H_
