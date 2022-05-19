/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_BROWSER_UI_VIEWS_OMNIBOX_BRAVE_OMNIBOX_RESULT_VIEW_H_
#define BRAVE_BROWSER_UI_VIEWS_OMNIBOX_BRAVE_OMNIBOX_RESULT_VIEW_H_

#include "base/memory/raw_ptr.h"
#include "brave/components/brave_search_conversion/types.h"
#include "chrome/browser/ui/views/omnibox/omnibox_result_view.h"
#include "ui/base/metadata/metadata_header_macros.h"

class BraveSearchConversionPromotionView;

// This will render brave specific matches such as braver search conversion
// promotion.
class BraveOmniboxResultView : public OmniboxResultView {
 public:
  METADATA_HEADER(BraveOmniboxResultView);
  BraveOmniboxResultView(OmniboxPopupContentsView* popup_contents_view,
                         OmniboxEditModel* model,
                         size_t model_index);
  BraveOmniboxResultView(const BraveOmniboxResultView&) = delete;
  BraveOmniboxResultView& operator=(const BraveOmniboxResultView&) = delete;
  ~BraveOmniboxResultView() override;

  // OmniboxResultView overrides:
  void SetMatch(const AutocompleteMatch& match) override;

 private:
  bool IsBraveSearchPromotion() const;
  void UpdateForMatch(const AutocompleteMatch& match);
  void UpdateForBraveSearchConversion();

  raw_ptr<OmniboxEditModel> edit_model_ = nullptr;

  // Brave search conversion promotion
  brave_search_conversion::ConversionType brave_search_conversion_type_ =
      brave_search_conversion::ConversionType::kNone;
  raw_ptr<BraveSearchConversionPromotionView> promotion_view_ = nullptr;
};

#endif  // BRAVE_BROWSER_UI_VIEWS_OMNIBOX_BRAVE_OMNIBOX_RESULT_VIEW_H_
