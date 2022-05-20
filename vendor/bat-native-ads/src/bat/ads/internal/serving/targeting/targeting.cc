/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/serving/targeting/targeting.h"

#include "bat/ads/internal/serving/targeting/targeting_constants.h"
#include "bat/ads/internal/serving/targeting/targeting_util.h"
#include "bat/ads/internal/serving/targeting/user_model_info.h"

namespace ads {
namespace targeting {

SegmentList GetTopChildSegments(const UserModelInfo& user_model) {
  return GetTopSegments(user_model, /* parent_only */ false);
}

SegmentList GetTopParentSegments(const UserModelInfo& user_model) {
  return GetTopSegments(user_model, /* parent_only */ true);
}

SegmentList GetTopChildInterestSegments(const UserModelInfo& user_model) {
  return GetTopSegments(user_model.interest_segments, kTopInterestSegmentsCount,
                        /* parent_only */ false);
}

SegmentList GetTopParentInterestSegments(const UserModelInfo& user_model) {
  return GetTopSegments(user_model.interest_segments, kTopInterestSegmentsCount,
                        /* parent_only */ true);
}

SegmentList GetTopChildLatentInterestSegments(const UserModelInfo& user_model) {
  return GetTopSegments(user_model.latent_interest_segments,
                        kTopLatentInterestSegmentsCount,
                        /* parent_only */ false);
}

SegmentList GetTopParentLatentInterestSegments(
    const UserModelInfo& user_model) {
  return GetTopSegments(user_model.latent_interest_segments,
                        kTopLatentInterestSegmentsCount,
                        /* parent_only */ true);
}

SegmentList GetTopChildPurchaseIntentSegments(const UserModelInfo& user_model) {
  return GetTopSegments(user_model.purchase_intent_segments,
                        kTopPurchaseIntentSegmentsCount,
                        /* parent_only */ false);
}

SegmentList GetTopParentPurchaseIntentSegments(
    const UserModelInfo& user_model) {
  return GetTopSegments(user_model.purchase_intent_segments,
                        kTopPurchaseIntentSegmentsCount,
                        /* parent_only */ true);
}

}  // namespace targeting
}  // namespace ads
