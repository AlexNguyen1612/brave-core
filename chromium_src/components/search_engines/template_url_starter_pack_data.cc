/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "components/search_engines/template_url_starter_pack_data.h"

#define GetStarterPackEngines GetStarterPackEngines_ChromiumImpl

#include "src/components/search_engines/template_url_starter_pack_data.cc"
#undef GetStarterPackEngines

namespace TemplateURLStarterPackData {

std::vector<std::unique_ptr<TemplateURLData>> GetStarterPackEngines() {
  return {};  // Do nothing.
}

}  // namespace TemplateURLStarterPackData
