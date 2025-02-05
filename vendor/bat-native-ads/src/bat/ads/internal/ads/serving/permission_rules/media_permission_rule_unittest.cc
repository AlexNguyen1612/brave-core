/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/ads/serving/permission_rules/media_permission_rule.h"

#include <vector>

#include "base/test/scoped_feature_list.h"
#include "bat/ads/internal/ads/serving/permission_rules/permission_rule_features.h"
#include "bat/ads/internal/base/unittest/unittest_base.h"
#include "url/gurl.h"

// npm run test -- brave_unit_tests --filter=BatAds*

namespace ads {

class BatAdsMediaPermissionRuleTest : public UnitTestBase {
 protected:
  BatAdsMediaPermissionRuleTest() = default;

  ~BatAdsMediaPermissionRuleTest() override = default;
};

TEST_F(BatAdsMediaPermissionRuleTest, AllowAdIfMediaIsNotPlaying) {
  // Arrange

  // Act
  MediaPermissionRule permission_rule;
  const bool is_allowed = permission_rule.ShouldAllow();

  // Assert
  EXPECT_TRUE(is_allowed);
}

TEST_F(BatAdsMediaPermissionRuleTest, AllowAdIfMediaIsStoppedForSingleTab) {
  // Arrange
  TabManager::GetInstance()->OnTabUpdated(1, GURL("https://brave.com"), true,
                                          false);

  TabManager::GetInstance()->OnMediaPlaying(1);
  TabManager::GetInstance()->OnMediaStopped(1);

  // Act
  MediaPermissionRule permission_rule;
  const bool is_allowed = permission_rule.ShouldAllow();

  // Assert
  EXPECT_TRUE(is_allowed);
}

TEST_F(BatAdsMediaPermissionRuleTest, AllowAdIfMediaIsStoppedOnMultipleTabs) {
  // Arrange
  TabManager::GetInstance()->OnTabUpdated(1, GURL("https://brave.com"), true,
                                          false);

  TabManager::GetInstance()->OnMediaPlaying(1);
  TabManager::GetInstance()->OnMediaPlaying(2);
  TabManager::GetInstance()->OnMediaStopped(1);
  TabManager::GetInstance()->OnMediaStopped(2);

  // Act
  MediaPermissionRule permission_rule;
  const bool is_allowed = permission_rule.ShouldAllow();

  // Assert
  EXPECT_TRUE(is_allowed);
}

TEST_F(BatAdsMediaPermissionRuleTest,
       AllowAdIfMediaIsPlayingOnMultipleTabsButStoppedForVisibleTab) {
  // Arrange
  TabManager::GetInstance()->OnTabUpdated(1, GURL("https://brave.com"), true,
                                          false);

  TabManager::GetInstance()->OnMediaPlaying(1);
  TabManager::GetInstance()->OnMediaPlaying(2);
  TabManager::GetInstance()->OnMediaStopped(1);

  // Act
  MediaPermissionRule permission_rule;
  const bool is_allowed = permission_rule.ShouldAllow();

  // Assert
  EXPECT_TRUE(is_allowed);
}

TEST_F(BatAdsMediaPermissionRuleTest,
       DoNotAllowAdIfMediaIsPlayingOnVisibleTab) {
  // Arrange
  TabManager::GetInstance()->OnTabUpdated(1, GURL("https://brave.com"), true,
                                          false);

  TabManager::GetInstance()->OnMediaPlaying(1);

  // Act
  MediaPermissionRule permission_rule;
  const bool is_allowed = permission_rule.ShouldAllow();

  // Assert
  EXPECT_FALSE(is_allowed);
}

TEST_F(BatAdsMediaPermissionRuleTest,
       AlwaysAllowAdIfMediaIsPlayingOnVisibleTabIfPermissionRuleIsDisabled) {
  // Arrange
  base::FieldTrialParams kParameters;
  kParameters["should_only_serve_ads_if_media_is_not_playing"] = "false";
  std::vector<base::test::ScopedFeatureList::FeatureAndParams> enabled_features;
  enabled_features.push_back(
      {permission_rules::features::kFeature, kParameters});

  const std::vector<base::Feature> disabled_features;

  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeaturesAndParameters(enabled_features,
                                                    disabled_features);

  TabManager::GetInstance()->OnTabUpdated(1, GURL("https://brave.com"), true,
                                          false);

  TabManager::GetInstance()->OnMediaPlaying(1);

  // Act
  MediaPermissionRule permission_rule;
  const bool is_allowed = permission_rule.ShouldAllow();

  // Assert
  EXPECT_TRUE(is_allowed);
}

TEST_F(BatAdsMediaPermissionRuleTest,
       DoNotAllowAdIfMediaIsPlayingOnMultipleTabs) {
  // Arrange
  TabManager::GetInstance()->OnTabUpdated(1, GURL("https://brave.com"), true,
                                          false);

  TabManager::GetInstance()->OnMediaPlaying(1);
  TabManager::GetInstance()->OnMediaPlaying(2);

  // Act
  MediaPermissionRule permission_rule;
  const bool is_allowed = permission_rule.ShouldAllow();

  // Assert
  EXPECT_FALSE(is_allowed);
}

TEST_F(BatAdsMediaPermissionRuleTest,
       DoNotAllowAdIfMediaIsPlayingOnMultipleTabsButStoppedForOccludedTab) {
  // Arrange
  TabManager::GetInstance()->OnTabUpdated(1, GURL("https://brave.com"), true,
                                          false);

  TabManager::GetInstance()->OnMediaPlaying(1);
  TabManager::GetInstance()->OnMediaPlaying(2);
  TabManager::GetInstance()->OnMediaStopped(2);

  // Act
  MediaPermissionRule permission_rule;
  const bool is_allowed = permission_rule.ShouldAllow();

  // Assert
  EXPECT_FALSE(is_allowed);
}

}  // namespace ads
