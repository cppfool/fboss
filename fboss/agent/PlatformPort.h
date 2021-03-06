/*
 *  Copyright (c) 2004-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */
#pragma once

#include "fboss/agent/types.h"

namespace facebook { namespace fboss {

class PlatformPort {
 public:
  PlatformPort() {}
  PlatformPort(PlatformPort&&) = default;
  PlatformPort& operator=(PlatformPort&&) = default;
  virtual ~PlatformPort() {}

  /*
   * Get the PortID for this port
   */
  virtual PortID getPortID() const = 0;

  /*
   * preDisable() will be called by the hardware code just before disabling
   * a port.
   *
   * The "temporary" parameter will be true if the port is configured to be
   * enabled, but is only being disabled temporarily to make some settings
   * changes.  (For instance, to change the port speed or interface type.)
   */
  virtual void preDisable(bool temporary) = 0;
  /*
   * postDisable() will be called by the hardware code just after disabling a
   * port.
   */
  virtual void postDisable(bool temporary) = 0;

  /*
   * preEnable() will be called by the hardware code just before enabling
   * a port.
   */
  virtual void preEnable() = 0;
  /*
   * postEnable() will be called by the hardware code just after enabling
   * a port.
   */
  virtual void postEnable() = 0;

  /*
   * isMediaPresent() must be implemented by the PlatformPort subclass,
   * and should return if media is present in the port.
   *
   * For instance, for a port connected to an SFP+ module, this should indicate
   * if a module is plugged in.  For an internal backplane port, this should
   * indicate if the remote fabric or line card is present.
   *
   * This is primarily used to indicate if link is expected to be up on the
   * port or not.  If no media is present, it is expected that the link will be
   * down.  If media is present but the link is down, the hardware code may try
   * periodically tuning the port parameters to see if it can achieve link.
   */
  virtual bool isMediaPresent() = 0;

  /*
   * Status indication functions.  These are mostly intended to allow
   * controlling LED indicators about the port.
   */

  /*
   * linkStatusChanged() will be called by the hardware code whenever
   * the link status changed.
   *
   * up indicates if link is present.
   * adminUp indicates if the port is enabled or disabled.
   */
  virtual void linkStatusChanged(bool up, bool adminUp) = 0;

  /*
   * statusIndication() will be called by the hardware code once a second.
   *
   * enabled  - If the port is enabled or not.
   * link     - If link is up on the port.
   * ingress  - If any packets have been received on the port since the last
   *            statusIndication() call.
   * egress   - If any packets have been sent on the port since the last
   *            statusIndication() call.
   * discards - If any packets have been discarded on the port since the last
   *            statusIndication() call.
   * errors   - If any packet errors have occurred on the port since the last
   *            statusIndication() call.
   */
  virtual void statusIndication(bool enabled, bool link,
                                bool ingress, bool egress,
                                bool discards, bool errors) = 0;

 private:
  // Forbidden copy constructor and assignment operator
  PlatformPort(PlatformPort const &) = delete;
  PlatformPort& operator=(PlatformPort const &) = delete;
};

}} // facebook::fboss
