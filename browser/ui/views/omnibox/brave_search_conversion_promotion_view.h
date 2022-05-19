/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_BROWSER_UI_VIEWS_OMNIBOX_BRAVE_SEARCH_CONVERSION_PROMOTION_VIEW_H
#define BRAVE_BROWSER_UI_VIEWS_OMNIBOX_BRAVE_SEARCH_CONVERSION_PROMOTION_VIEW_H

#include "base/memory/raw_ptr.h"
#include "brave/components/brave_search_conversion/types.h"
#include "ui/base/metadata/metadata_header_macros.h"
#include "ui/views/view.h"

class BraveOmniboxResultSelectionIndicator;

namespace views {
class ImageView;
}  // namespace views

class BraveSearchConversionPromotionView : public views::View {
 public:
  METADATA_HEADER(BraveSearchConversionPromotionView);

  BraveSearchConversionPromotionView();
  BraveSearchConversionPromotionView(
      const BraveSearchConversionPromotionView&) = delete;
  BraveSearchConversionPromotionView& operator=(
      const BraveSearchConversionPromotionView&) = delete;
  ~BraveSearchConversionPromotionView() override;

  void SetType(brave_search_conversion::ConversionType type);

  // views::View overrides:
  gfx::Size CalculatePreferredSize() const override;

 private:
  raw_ptr<BraveOmniboxResultSelectionIndicator> selection_indicator_ = nullptr;
  raw_ptr<views::ImageView> icon_view_ = nullptr;
  brave_search_conversion::ConversionType type_ =
      brave_search_conversion::ConversionType::kNone;
};

#endif  // BRAVE_BROWSER_UI_VIEWS_OMNIBOX_BRAVE_SEARCH_CONVERSION_PROMOTION_VIEW_H
