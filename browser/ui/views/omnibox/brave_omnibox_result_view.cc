/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/ui/views/omnibox/brave_omnibox_result_view.h"

#include <memory>

#include "brave/browser/ui/views/omnibox/brave_search_conversion_promotion_view.h"
#include "brave/components/brave_search_conversion/utils.h"
#include "brave/components/omnibox/browser/promotion_utils.h"
#include "chrome/browser/ui/views/omnibox/omnibox_suggestion_button_row_view.h"
#include "components/omnibox/browser/autocomplete_controller.h"
#include "components/omnibox/browser/autocomplete_match.h"
#include "components/omnibox/browser/autocomplete_result.h"
#include "components/omnibox/browser/omnibox_edit_model.h"
#include "ui/base/metadata/metadata_impl_macros.h"
#include "ui/views/controls/button/image_button.h"

using brave_search_conversion::ConversionType;
using brave_search_conversion::GetConversionType;

BraveOmniboxResultView::BraveOmniboxResultView(
    OmniboxPopupContentsView* popup_contents_view,
    OmniboxEditModel* model,
    size_t model_index)
    : OmniboxResultView(popup_contents_view, model, model_index),
      edit_model_(model) {}

BraveOmniboxResultView::~BraveOmniboxResultView() = default;

void BraveOmniboxResultView::SetMatch(const AutocompleteMatch& match) {
  UpdateForMatch(match);

  OmniboxResultView::SetMatch(match);
}

void BraveOmniboxResultView::UpdateForMatch(const AutocompleteMatch& match) {
  const auto prev_brave_search_conversion_type = brave_search_conversion_type_;
  const auto input = edit_model_->autocomplete_controller()->input();
  if (IsBraveSearchPromotionMatch(match, input.text())) {
    brave_search_conversion_type_ = GetConversionType();
  } else {
    brave_search_conversion_type_ = ConversionType::kNone;
  }

  if (prev_brave_search_conversion_type != brave_search_conversion_type_) {
    UpdateForBraveSearchConversion();
  }
}

void BraveOmniboxResultView::UpdateForBraveSearchConversion() {
  const bool is_brave_search_promotion = IsBraveSearchPromotion();
  suggestion_container_->SetVisible(!is_brave_search_promotion);
  button_row_->SetVisible(!is_brave_search_promotion);
  if (promotion_view_) {
    promotion_view_->SetVisible(is_brave_search_promotion);
  }

  if (!is_brave_search_promotion)
    return;

  if (!promotion_view_) {
    promotion_view_ =
        AddChildView(std::make_unique<BraveSearchConversionPromotionView>());
    promotion_view_->SetVisible(true);
  }
  promotion_view_->SetType(brave_search_conversion_type_);
}

bool BraveOmniboxResultView::IsBraveSearchPromotion() const {
  return brave_search_conversion_type_ != ConversionType::kNone;
}

BEGIN_METADATA(BraveOmniboxResultView, OmniboxResultView)
END_METADATA
