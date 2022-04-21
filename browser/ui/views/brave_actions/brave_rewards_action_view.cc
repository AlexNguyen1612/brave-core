// Copyright (c) 2022 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "brave/browser/ui/views/brave_actions/brave_rewards_action_view.h"

#include <memory>
#include <string>
#include <utility>

#include "base/strings/string_number_conversions.h"
#include "brave/app/vector_icons/vector_icons.h"
#include "brave/browser/brave_rewards/rewards_panel_service_factory.h"
#include "brave/browser/brave_rewards/rewards_service_factory.h"
#include "brave/browser/ui/brave_actions/brave_action_icon_with_badge_image_source.h"
#include "brave/common/webui_url_constants.h"
#include "brave/components/brave_rewards/common/pref_names.h"
#include "brave/components/l10n/common/locale_util.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "chrome/browser/ui/views/chrome_layout_provider.h"
#include "chrome/browser/ui/views/toolbar/toolbar_action_view.h"
#include "chrome/browser/ui/views/toolbar/toolbar_ink_drop_util.h"
#include "components/grit/brave_components_strings.h"
#include "components/prefs/pref_service.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/gfx/image/image.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/gfx/paint_vector_icon.h"
#include "ui/gfx/skia_util.h"
#include "ui/views/animation/ink_drop_impl.h"
#include "ui/views/controls/button/label_button_border.h"
#include "ui/views/controls/button/menu_button_controller.h"
#include "ui/views/controls/highlight_path_generator.h"
#include "ui/views/view_class_properties.h"

namespace {

using brave_rewards::RewardsNotificationService;
using brave_rewards::RewardsPanelServiceFactory;
using brave_rewards::RewardsServiceFactory;
using brave_rewards::RewardsTabHelper;

constexpr SkColor kIconColor = SK_ColorBLACK;
constexpr SkColor kBadgeTextColor = SK_ColorWHITE;
constexpr SkColor kBadgeNotificationBG = SkColorSetRGB(0xfb, 0x54, 0x2b);
constexpr SkColor kBadgeVerifiedBG = SkColorSetRGB(0x4c, 0x54, 0xd2);
const char kVerifiedCheck[] = "\u2713";

class ButtonHighlightPathGenerator : public views::HighlightPathGenerator {
 public:
  // views::HighlightPathGenerator:
  SkPath GetHighlightPath(const views::View* view) override {
    // Set the highlight path for the toolbar button, making it inset so that
    // the badge can show outside it in the fake margin on the right that we are
    // creating.
    DCHECK(view);
    gfx::Rect rect(view->GetPreferredSize());
    rect.Inset(gfx::Insets::TLBR(0, 0, 0, kBraveActionRightMargin));

    auto* layout_provider = ChromeLayoutProvider::Get();
    DCHECK(layout_provider);

    int radius = layout_provider->GetCornerRadiusMetric(
        views::Emphasis::kMaximum, rect.size());

    SkPath path;
    path.addRoundRect(gfx::RectToSkRect(rect), radius, radius);
    return path;
  }
};

const ui::ColorProvider* GetColorProviderForWebContents(
    base::WeakPtr<content::WebContents> web_contents) {
  if (web_contents) {
    return &web_contents->GetColorProvider();
  }

  return ui::ColorProviderManager::Get().GetColorProviderFor(
      ui::NativeTheme::GetInstanceForNativeUi()->GetColorProviderKey(nullptr));
}

}  // namespace

BraveRewardsActionView::BraveRewardsActionView(Browser* browser)
    : browser_(browser),
      bubble_manager_(this,
                      browser_->profile(),
                      GURL(kBraveRewardsPanelURL),
                      IDS_BRAVE_UI_BRAVE_REWARDS) {
  DCHECK(browser_);

  SetButtonController(std::make_unique<views::MenuButtonController>(
      this,
      base::BindRepeating(&BraveRewardsActionView::OnButtonPressed,
                          base::Unretained(this)),
      std::make_unique<views::Button::DefaultButtonControllerDelegate>(this)));

  views::HighlightPathGenerator::Install(
      this, std::make_unique<ButtonHighlightPathGenerator>());

  auto* ink_drop = views::InkDrop::Get(this);
  DCHECK(ink_drop);
  ink_drop->SetMode(views::InkDropHost::InkDropMode::ON);
  ink_drop->SetVisibleOpacity(kToolbarInkDropVisibleOpacity);
  ink_drop->SetBaseColorCallback(base::BindRepeating(
      [](views::View* host) { return GetToolbarInkDropBaseColor(host); },
      this));

  SetHasInkDropActionOnClick(true);
  SetHorizontalAlignment(gfx::ALIGN_CENTER);

  SetAccessibleName(
      brave_l10n::GetLocalizedResourceUTF16String(IDS_BRAVE_UI_BRAVE_REWARDS));

  auto* profile = browser_->profile();

  pref_change_registrar_.Init(profile->GetPrefs());
  pref_change_registrar_.Add(
      brave_rewards::prefs::kBadgeText,
      base::BindRepeating(&BraveRewardsActionView::OnPreferencesChanged,
                          base::Unretained(this)));
  pref_change_registrar_.Add(
      brave_rewards::prefs::kShowButton,
      base::BindRepeating(&BraveRewardsActionView::OnPreferencesChanged,
                          base::Unretained(this)));

  browser_->tab_strip_model()->AddObserver(this);

  if (auto* notification_service = GetNotificationService()) {
    notification_service_observation_.Observe(notification_service);
  }

  if (auto* service = RewardsPanelServiceFactory::GetForProfile(profile)) {
    panel_service_ = service;
    panel_observation_.Observe(service);
  }

  UpdateTabHelper(GetActiveWebContents());
}

BraveRewardsActionView::~BraveRewardsActionView() = default;

void BraveRewardsActionView::Update() {
  auto [text, background_color] = GetBadgeTextAndBackground();
  auto badge = std::make_unique<IconWithBadgeImageSource::Badge>(
      text, kBadgeTextColor, background_color);

  gfx::Size preferred_size = GetPreferredSize();

  auto* web_contents = GetActiveWebContents();
  auto weak_contents = web_contents ? web_contents->GetWeakPtr()
                                    : base::WeakPtr<content::WebContents>();

  auto image_source = std::make_unique<BraveActionIconWithBadgeImageSource>(
      preferred_size,
      base::BindRepeating(GetColorProviderForWebContents, weak_contents));

  image_source->SetIcon(gfx::Image(GetRewardsIcon()));
  image_source->SetBadge(std::move(badge));

  SetImage(views::Button::STATE_NORMAL,
           gfx::ImageSkia(std::move(image_source), preferred_size));

  SetVisible(ShouldShow());
}

void BraveRewardsActionView::ClosePanelForTesting() {
  if (IsPanelOpen()) {
    ToggleRewardsPanel();
  }
}

std::unique_ptr<views::LabelButtonBorder>
BraveRewardsActionView::CreateDefaultBorder() const {
  auto border = LabelButton::CreateDefaultBorder();
  border->set_insets(gfx::Insets::TLBR(0, 0, 0, 0));
  return border;
}

void BraveRewardsActionView::OnWidgetDestroying(views::Widget* widget) {
  auto* bubble_widget = bubble_manager_.GetBubbleWidget();
  DCHECK_EQ(bubble_widget, widget);
  DCHECK(bubble_observation_.IsObservingSource(bubble_widget));

  bubble_observation_.Reset();
  SetVisible(ShouldShow());

  if (panel_service_) {
    panel_service_->NotifyPanelClosed(browser_);
  }
}

void BraveRewardsActionView::OnTabStripModelChanged(
    TabStripModel* tab_strip_model,
    const TabStripModelChange& change,
    const TabStripSelectionChange& selection) {
  if (selection.active_tab_changed()) {
    UpdateTabHelper(selection.new_contents);
  }
}

void BraveRewardsActionView::OnPublisherUpdated(
    const std::string& publisher_id) {
  // TODO(zenparsing): Consider an LRUCache for this initialization.
  publisher_registered_ = {publisher_id, false};

  if (!publisher_id.empty()) {
    if (auto* rewards_service = GetRewardsService()) {
      // TODO(zenparsing): When rewards is enabled, should we automatically
      // check this again? Unfortunately we don't have a way to listen for
      // Rewards being enabled. Perhaps initialized will work?
      if (rewards_service->IsRewardsEnabled()) {
        rewards_service->IsPublisherRegistered(
            publisher_id,
            base::BindOnce(
                &BraveRewardsActionView::IsPublisherRegisteredCallback,
                weak_factory_.GetWeakPtr(), publisher_id));
      }
    }
  }

  Update();
}

void BraveRewardsActionView::OnRewardsPanelRequested(Browser* browser) {
  // If the panel is already open, then assume that the corresponding WebUI
  // handler will be listening for this event and take the panel arguments.
  if (browser == browser_ && !IsPanelOpen()) {
    ToggleRewardsPanel();
  }
}

void BraveRewardsActionView::OnNotificationAdded(
    RewardsNotificationService* service,
    const RewardsNotificationService::RewardsNotification& notification) {
  Update();
}

void BraveRewardsActionView::OnNotificationDeleted(
    RewardsNotificationService* service,
    const RewardsNotificationService::RewardsNotification& notification) {
  Update();
}

void BraveRewardsActionView::OnButtonPressed() {
  // TODO(zenparsing): We should probably DCHECK on having the panel service?
  // None of this really works without it right?
  if (IsPanelOpen() || !panel_service_) {
    ToggleRewardsPanel();
    return;
  }

  panel_service_->OpenRewardsPanel();
}

void BraveRewardsActionView::OnPreferencesChanged(const std::string& key) {
  if (key == brave_rewards::prefs::kShowButton) {
    SetVisible(ShouldShow());
  } else {
    Update();
  }
}

void BraveRewardsActionView::IsPublisherRegisteredCallback(
    const std::string& publisher_id,
    bool is_registered) {
  if (publisher_id == std::get<std::string>(publisher_registered_)) {
    publisher_registered_.second = is_registered;
    Update();
  }
}

content::WebContents* BraveRewardsActionView::GetActiveWebContents() {
  return browser_->tab_strip_model()->GetActiveWebContents();
}

brave_rewards::RewardsService* BraveRewardsActionView::GetRewardsService() {
  return RewardsServiceFactory::GetForProfile(browser_->profile());
}

brave_rewards::RewardsNotificationService*
BraveRewardsActionView::GetNotificationService() {
  if (auto* rewards_service = GetRewardsService()) {
    return rewards_service->GetNotificationService();
  }
  return nullptr;
}

bool BraveRewardsActionView::IsPanelOpen() {
  return bubble_observation_.IsObserving();
}

void BraveRewardsActionView::ToggleRewardsPanel() {
  if (IsPanelOpen()) {
    bubble_manager_.CloseBubble();
    return;
  }

  // Clear the default-on-start badge text when the user opens the panel.
  auto* prefs = browser_->profile()->GetPrefs();
  prefs->SetString(brave_rewards::prefs::kBadgeText, "");

  // TODO(zenparsing): If the button is currently hidden and the bubble manager
  // is showing cached web contents, then sometimes the positioning of the
  // bubble is off by the width of the button.
  bubble_manager_.ShowBubble();

  DCHECK(!bubble_observation_.IsObserving());
  bubble_observation_.Observe(bubble_manager_.GetBubbleWidget());
  SetVisible(ShouldShow());
}

gfx::ImageSkia BraveRewardsActionView::GetRewardsIcon() {
  // Since the BAT icon has color the actual color value here is not relevant,
  // but |CreateVectorIcon| requires one.
  return gfx::CreateVectorIcon(kBatIcon, kBraveActionGraphicSize, kIconColor);
}

std::pair<std::string, SkColor>
BraveRewardsActionView::GetBadgeTextAndBackground() {
  // 1. Display the default-on-start Rewards badge text, if specified.
  std::string text_pref = browser_->profile()->GetPrefs()->GetString(
      brave_rewards::prefs::kBadgeText);
  if (!text_pref.empty()) {
    return {text_pref, kBadgeNotificationBG};
  }

  // 2. Display the number of current notifications, if non-zero.
  size_t notifications = GetRewardsNotificationCount();
  if (notifications > 0) {
    std::string text =
        notifications > 99 ? "99+" : base::NumberToString(notifications);

    return {text, kBadgeNotificationBG};
  }

  // 3. Display a verified checkmark for verified publishers.
  if (std::get<bool>(publisher_registered_)) {
    return {kVerifiedCheck, kBadgeVerifiedBG};
  }

  return {"", kBadgeNotificationBG};
}

size_t BraveRewardsActionView::GetRewardsNotificationCount() {
  auto* service = GetNotificationService();
  return service ? service->GetAllNotifications().size() : 0;
}

bool BraveRewardsActionView::ShouldShow() {
  // If the rewards panel is open, then we should show the button regardless of
  // any other state.
  if (IsPanelOpen()) {
    return true;
  }

  // Don't show the button if this profile does not have a Rewards service.
  if (!GetRewardsService()) {
    return false;
  }

  // Don't show the button if the user has decided to hide it.
  auto* prefs = browser_->profile()->GetPrefs();
  if (!prefs->GetBoolean(brave_rewards::prefs::kShowButton)) {
    return false;
  }

  return true;
}

void BraveRewardsActionView::UpdateTabHelper(
    content::WebContents* web_contents) {
  tab_helper_ = nullptr;
  if (tab_helper_observation_.IsObserving()) {
    tab_helper_observation_.Reset();
  }

  if (web_contents) {
    if (auto* helper = RewardsTabHelper::FromWebContents(web_contents)) {
      tab_helper_ = helper;
      tab_helper_observation_.Observe(helper);
    }
  }

  OnPublisherUpdated(tab_helper_ ? tab_helper_->GetPublisherIdForTab() : "");
}
