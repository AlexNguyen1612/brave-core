/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/omnibox/browser/promotion_utils.h"

#include <algorithm>

#include "brave/components/brave_search_conversion/types.h"
#include "brave/components/brave_search_conversion/utils.h"
#include "components/omnibox/browser/autocomplete_input.h"
#include "components/omnibox/browser/autocomplete_match.h"
#include "components/omnibox/browser/autocomplete_result.h"

void SortBraveSearchPromotionMatch(
    AutocompleteResult* result,
    const AutocompleteInput& input,
    brave_search_conversion::ConversionType type) {
  if (brave_search_conversion::ConversionType::kNone == type)
    return;

  ACMatches::iterator brave_search_conversion_match = std::find_if(
      result->begin(), result->end(), [&](const AutocompleteMatch& m) {
        return IsBraveSearchPromotionMatch(m, input.text());
      });
  if (brave_search_conversion_match != result->end()) {
    const size_t from_index = brave_search_conversion_match - result->begin();
    // Put as a second match for button type. Otherwise, put at last.
    const size_t to_index =
        type == brave_search_conversion::ConversionType::kButton
            ? 1
            : result->size();
    result->MoveMatch(from_index, to_index);
  }
}

bool IsBraveSearchPromotionMatch(const AutocompleteMatch& match,
                                 const std::u16string& input) {
  return match.type == AutocompleteMatchType::NAVSUGGEST &&
         match.destination_url == brave_search_conversion::GetPromoURL(input);
}
