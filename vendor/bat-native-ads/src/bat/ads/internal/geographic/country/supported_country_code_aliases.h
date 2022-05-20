/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_GEOGRAPHIC_COUNTRY_SUPPORTED_COUNTRY_CODE_ALIASES_H_
#define BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_GEOGRAPHIC_COUNTRY_SUPPORTED_COUNTRY_CODE_ALIASES_H_

#include <string>

#include "base/containers/flat_map.h"
#include "base/containers/flat_set.h"

namespace ads {
namespace geographic {

using SupportedCountryCodeSet = base::flat_set<std::string>;
using SupportedCountryCodeMap = base::flat_map<int, SupportedCountryCodeSet>;

}  // namespace geographic
}  // namespace ads

#endif  // BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_GEOGRAPHIC_COUNTRY_SUPPORTED_COUNTRY_CODE_ALIASES_H_
