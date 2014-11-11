/*
 *  Copyright (c) 2014, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */
#pragma once

#include <chrono>
#include <folly/Range.h>
#include <folly/SocketAddress.h>
#include <glog/logging.h>
#include <list>
#include <proxygen/lib/services/NetworkAddress.h>
#include <set>
#include <string>

namespace proxygen {

/**
 * Class that holds an LoadShed configuration for a service
 */
class LoadShedConfiguration {
 public:

  // Comparison function for SocketAddress that disregards the port
  struct AddressOnlyCompare {
    bool operator()(
     const folly::SocketAddress& addr1,
     const folly::SocketAddress& addr2) const {
      return addr1.getIPAddress() < addr2.getIPAddress();
    }
  };

  typedef std::set<folly::SocketAddress,
    AddressOnlyCompare> AddressSet;
  typedef std::set<NetworkAddress> NetworkSet;

  LoadShedConfiguration() {}

  ~LoadShedConfiguration() {}

  void addWhitelistAddr(folly::StringPiece);

  /**
   * Set/get the set of IPs that should be whitelisted through even when we're
   * trying to shed load.
   */
  void setWhitelistAddrs(const AddressSet& addrs) { whitelistAddrs_ = addrs; }
  const AddressSet& getWhitelistAddrs() const { return whitelistAddrs_; }

  /**
   * Set/get the set of networks that should be whitelisted through even
   * when we're trying to shed load.
   */
  void setWhitelistNetworks(const NetworkSet& networks) {
    whitelistNetworks_ = networks;
  }
  const NetworkSet& getWhitelistNetworks() const { return whitelistNetworks_; }

  /**
   * Set/get the maximum number of downstream connections across all VIPs.
   */
  void setMaxConnections(uint64_t maxConns) { maxConnections_ = maxConns; }
  uint64_t getMaxConnections() const { return maxConnections_; }

  /**
   * Set/get the maximum cpu usage.
   */
  void setMaxMemUsage(double max) {
    CHECK(max >= 0);
    CHECK(max <= 1);
    maxMemUsage_ = max;
  }
  double getMaxMemUsage() const { return maxMemUsage_; }

  /**
   * Set/get the maximum memory usage.
   */
  void setMaxCpuUsage(double max) {
    CHECK(max >= 0);
    CHECK(max <= 1);
    maxCpuUsage_ = max;
  }
  double getMaxCpuUsage() const { return maxCpuUsage_; }

  void setLoadUpdatePeriod(std::chrono::milliseconds period) {
    period_ = period;
  }
  std::chrono::milliseconds getLoadUpdatePeriod() const { return period_; }

  bool isWhitelisted(
      const folly::SocketAddress& addr) const;

 private:

  AddressSet whitelistAddrs_;
  NetworkSet whitelistNetworks_;
  uint64_t maxConnections_{0};
  double maxMemUsage_;
  double maxCpuUsage_;
  std::chrono::milliseconds period_;
};

}
