// Copyright (c) 2020 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "brave/browser/infobars/sync_v2_migrate_infobar_delegate.h"

#include <memory>
#include <utility>

#include "base/feature_list.h"
#include "brave/browser/ui/brave_pages.h"
#include "brave/components/brave_sync/brave_sync_prefs.h"
#include "brave/components/brave_sync/features.h"
#include "brave/components/constants/pref_names.h"
#include "brave/components/constants/url_constants.h"
#include "brave/components/l10n/common/locale_util.h"
#include "brave/grit/brave_generated_resources.h"
#include "chrome/browser/infobars/confirm_infobar_creator.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/chrome_pages.h"
#include "chrome/grit/chromium_strings.h"
#include "components/infobars/content/content_infobar_manager.h"
#include "components/infobars/core/infobar.h"
#include "components/prefs/pref_service.h"
#include "components/strings/grit/components_strings.h"
#include "ui/views/vector_icons.h"

// static
void SyncV2MigrateInfoBarDelegate::Create(
    infobars::ContentInfoBarManager* infobar_manager,
    bool is_v2_user,
    Profile* profile,
    Browser* browser) {
  // Show infobar if user had enabled sync v1 (even if they hadn't
  // re-enabled it via the flag).
  // Don't show infobar if:
  // - sync flag is disabled
  // - problem with local_state
  // - user has setup sync v2
  // - dismissed notification (don't auto-dismiss)
  const bool is_flag_enabled =
      base::FeatureList::IsEnabled(brave_sync::features::kBraveSync);
  if (!is_flag_enabled) {
    return;
  }
  brave_sync::Prefs brave_sync_prefs(profile->GetPrefs());
  const bool was_v1_user = brave_sync_prefs.IsSyncV1Enabled();
  if (!was_v1_user) {
    // Not v1 user
    return;
  }
  const bool has_dismissed = brave_sync_prefs.IsSyncMigrateNoticeDismissed();
  if (has_dismissed) {
    return;
  }
  if (is_v2_user) {
    // Make sure this doesn't automatically show again if sync is turned off.
    // TODO(petemill): Might be better to change a pref in an event handler
    // rather than here.
    brave_sync_prefs.SetDismissSyncMigrateNotice(true);
    return;
  }
  // Show infobar
  infobar_manager->AddInfoBar(
      CreateConfirmInfoBar(std::unique_ptr<ConfirmInfoBarDelegate>(
          new SyncV2MigrateInfoBarDelegate(browser, profile))));
}

// Start class impl
SyncV2MigrateInfoBarDelegate::SyncV2MigrateInfoBarDelegate(Browser* browser,
    Profile* profile)
          : ConfirmInfoBarDelegate(),
            profile_(profile),
            browser_(browser) {
}

SyncV2MigrateInfoBarDelegate::~SyncV2MigrateInfoBarDelegate() {
}

infobars::InfoBarDelegate::InfoBarIdentifier
SyncV2MigrateInfoBarDelegate::GetIdentifier() const {
  return SYNC_V2_MIGRATE_INFOBAR_DELEGATE;
}

const gfx::VectorIcon& SyncV2MigrateInfoBarDelegate::GetVectorIcon() const {
  return views::kInfoIcon;
}

bool SyncV2MigrateInfoBarDelegate::ShouldExpire(
    const NavigationDetails& details) const {
  return false;
}

void SyncV2MigrateInfoBarDelegate::InfoBarDismissed() {
  brave_sync::Prefs brave_sync_prefs(profile_->GetPrefs());
  brave_sync_prefs.SetDismissSyncMigrateNotice(true);
}

std::u16string SyncV2MigrateInfoBarDelegate::GetMessageText() const {
  return brave_l10n::GetLocalizedResourceUTF16String(
      IDS_BRAVE_SYNC_V2_MIGRATE_INFOBAR_MESSAGE);
}

int SyncV2MigrateInfoBarDelegate::GetButtons() const {
  return BUTTON_OK;
}

std::u16string SyncV2MigrateInfoBarDelegate::GetButtonLabel(
    InfoBarButton button) const {
  return brave_l10n::GetLocalizedResourceUTF16String(
      IDS_BRAVE_SYNC_V2_MIGRATE_INFOBAR_COMMAND);
}

bool SyncV2MigrateInfoBarDelegate::Accept() {
  brave::ShowSync(browser_);
  return true;
}
