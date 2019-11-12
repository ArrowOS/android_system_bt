/*
 * Copyright 2019 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#include <memory>

#include "hci/hci_packets.h"
#include "module.h"

namespace bluetooth {
namespace hci {

class AdvertisingConfig {
 public:
  std::vector<GapData> advertisement;
  std::vector<GapData> scan_response;
  Address random_address;
  uint16_t interval_min;
  uint16_t interval_max;
  AdvertisingEventType event_type;
  AddressType address_type;
  PeerAddressType peer_address_type;
  Address peer_address;
  uint8_t channel_map;
  AdvertisingFilterPolicy filter_policy;
  uint8_t tx_power;  // -127 to +20 (0x7f is no preference)
};

class ExtendedAdvertisingConfig : public AdvertisingConfig {
 public:
  bool connectable;
  bool scannable;
  bool directed;
  bool high_duty_directed_connectable;
  bool legacy_pdus;
  bool anonymous;
  bool include_tx_power;
  bool use_le_coded_phy;       // Primary advertisement PHY is LE Coded
  uint8_t secondary_max_skip;  // maximum advertising events to be skipped, 0x0 send AUX_ADV_IND prior ot the next event
  uint8_t secondary_advertising_phy;  // 1 = 1M, 2 = 2M, 3 = coded
  uint8_t sid;
  bool enable_scan_request_notifications;
};

using AdvertiserId = int32_t;

class LeAdvertisingManager : public bluetooth::Module {
 public:
  static constexpr AdvertiserId kInvalidId = -1;
  LeAdvertisingManager();

  size_t GetNumberOfAdvertisingInstances() const;

  // Return -1 if the advertiser was not created, otherwise the advertiser ID.
  AdvertiserId CreateAdvertiser(const AdvertisingConfig& config,
                                const common::Callback<void(Address, AddressType)>& scan_callback,
                                const common::Callback<void(ErrorCode, uint8_t, uint8_t)>& set_terminated_callback,
                                os::Handler* handler);
  AdvertiserId CreateAdvertiser(const ExtendedAdvertisingConfig& config,
                                const common::Callback<void(Address, AddressType)>& scan_callback,
                                const common::Callback<void(ErrorCode, uint8_t, uint8_t)>& set_terminated_callback,
                                os::Handler* handler);

  void RemoveAdvertiser(AdvertiserId id);

  static const ModuleFactory Factory;

 protected:
  void ListDependencies(ModuleList* list) override;

  void Start() override;

  void Stop() override;

 private:
  struct impl;
  std::unique_ptr<impl> pimpl_;
  DISALLOW_COPY_AND_ASSIGN(LeAdvertisingManager);
};

}  // namespace hci
}  // namespace bluetooth