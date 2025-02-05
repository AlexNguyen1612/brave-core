/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_WALLET_COMMON_VALUE_CONVERSION_UTILS_H_
#define BRAVE_COMPONENTS_BRAVE_WALLET_COMMON_VALUE_CONVERSION_UTILS_H_

#include <string>
#include <vector>

#include "base/values.h"
#include "brave/components/brave_wallet/common/brave_wallet.mojom.h"
#include "third_party/abseil-cpp/absl/types/optional.h"
#include "url/origin.h"

namespace brave_wallet {

absl::optional<std::string> ExtractChainIdFromValue(
    const base::Value::Dict* dict);
base::Value::Dict EthNetworkInfoToValue(const mojom::NetworkInfo& info);
// blockExplorerUrls, iconUrls, rpcUrls will be checked if they're valid URLs
// if check_url = true.
mojom::NetworkInfoPtr ValueToEthNetworkInfo(const base::Value& value,
                                            bool check_url = false);
base::Value::List PermissionRequestResponseToValue(
    const url::Origin& origin,
    const std::vector<std::string> accounts);

mojom::BlockchainTokenPtr ValueToBlockchainToken(const base::Value::Dict& value,
                                                 const std::string& chain_id,
                                                 mojom::CoinType coin);

}  // namespace brave_wallet

#endif  // BRAVE_COMPONENTS_BRAVE_WALLET_COMMON_VALUE_CONVERSION_UTILS_H_
