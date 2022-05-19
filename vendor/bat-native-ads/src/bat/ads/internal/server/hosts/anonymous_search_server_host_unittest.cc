/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/server/hosts/anonymous_search_server_host.h"

#include "bat/ads/ad_type.h"
#include "bat/ads/internal/base/unittest_util.h"
#include "bat/ads/internal/server/hosts/server_host_types.h"
#include "bat/ads/internal/server/hosts/server_host_util.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- brave_unit_tests --filter=BatAds*

namespace ads {

TEST(BatAdsAnonymousSearchServerHostTest, GetProductionHost) {
  // Arrange
  SetEnvironment(mojom::Environment::kProduction);

  // Act
  const std::string host = server::GetAnonymousHost(AdType::kSearchResultAd);

  // Assert
  const std::string expected_host = "https://search.anonymous.ads.brave.com";
  EXPECT_EQ(expected_host, host);
}

TEST(BatAdsAnonymousSearchServerHostTest, GetStagingHost) {
  // Arrange
  SetEnvironment(mojom::Environment::kStaging);

  // Act
  const std::string host = server::GetAnonymousHost(AdType::kSearchResultAd);

  // Assert
  const std::string expected_host =
      "https://search.anonymous.ads.bravesoftware.com";
  EXPECT_EQ(expected_host, host);
}

}  // namespace ads
