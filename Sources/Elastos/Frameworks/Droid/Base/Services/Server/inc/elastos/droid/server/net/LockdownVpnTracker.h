//=========================================================================
// Copyright (C) 2012 The Elastos Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//=========================================================================

#ifndef __ELASTOS_DROID_SERVER_LOCKDOWNVPNTRACKER_H__
#define __ELASTOS_DROID_SERVER_LOCKDOWNVPNTRACKER_H__

#include "_Elastos.Droid.Server.h"
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/server/connectivity/Vpn.h"
#include "elastos/droid/content/BroadcastReceiver.h"

using Elastos::Droid::App::IPendingIntent;
using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::Content::IBroadcastReceiver;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Net::IIConnectivityManager;
using Elastos::Droid::Net::IBaseNetworkStateTracker;
using Elastos::Droid::Net::ILinkAddress;
using Elastos::Droid::Net::INetworkInfo;
using Elastos::Droid::Os::IINetworkManagementService;
using Elastos::Droid::Internal::Net::IVpnProfile;
using Elastos::Droid::Server::Connectivity::Vpn;
using Elastos::Utility::IList;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Net {

/**
 * State tracker for lockdown mode. Watches for normal {@link NetworkInfo} to be
 * connected and kicks off VPN connection, managing any required {@code netd}
 * firewall rules.
 */
class LockdownVpnTracker
    : public Object
{
private:
    class InnerSub_BroadcastReceiver
        : public BroadcastReceiver
    {
    public:
        InnerSub_BroadcastReceiver(
            /* [in] */ LockdownVpnTracker* owner)
            : mOwner(owner)
        {}

        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        TO_STRING_IMPL("LockdownVpnTracker::TrackerBroadcastReceiver: ")
    private:
        LockdownVpnTracker* mOwner;
    };

public:
    LockdownVpnTracker();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IINetworkManagementService* netService,
        /* [in] */ IIConnectivityManager* connService,
        /* [in] */ Vpn* vpn,
        /* [in] */ IVpnProfile* profile);

    static CARAPI_(Boolean) IsEnabled();

    CARAPI Init();

    CARAPI Shutdown();

    CARAPI Reset();

    CARAPI OnNetworkInfoChanged();

    CARAPI OnVpnStateChanged(
        /* [in] */ INetworkInfo* info);

    CARAPI_(AutoPtr<INetworkInfo>) AugmentNetworkInfo(
        /* [in] */ INetworkInfo* inInfo);

private:
    /**
     * Watch for state changes to both active egress network, kicking off a VPN
     * connection when ready, or setting firewall rules once VPN is connected.
     */
    CARAPI HandleStateChangedLocked();

    CARAPI InitLocked();

    CARAPI ShutdownLocked();

    CARAPI ClearSourceRulesLocked();

    CARAPI SetFirewallEgressSourceRule(
        /* [in] */ ILinkAddress* address,
        /* [in] */ Boolean allow);

    CARAPI_(void) ShowNotification(
        /* [in] */ Int32 titleRes,
        /* [in] */ Int32 iconRes);

    CARAPI_(void) HideNotification();

private:
    static const String TAG;

    /** Number of VPN attempts before waiting for user intervention. */
    static const Int32 MAX_ERROR_COUNT;

    static const String ACTION_LOCKDOWN_RESET;
    static const String ACTION_VPN_SETTINGS;
    static const String EXTRA_PICK_LOCKDOWN;

    static const Int32 ROOT_UID;

    AutoPtr<IContext> mContext;
    AutoPtr<IINetworkManagementService> mNetService;
    // AutoPtr<CConnectivityService> mConnService;
    // AutoPtr<Elastos::Droid::Server::Connectivity::Vpn> mVpn;
    AutoPtr<IVpnProfile> mProfile;

    Object mStateLock;

    AutoPtr<IPendingIntent> mConfigIntent;
    AutoPtr<IPendingIntent> mResetIntent;

    String mAcceptedEgressIface;
    String mAcceptedIface;
    AutoPtr<IList> mAcceptedSourceAddr;

    Int32 mErrorCount;

    AutoPtr<IBroadcastReceiver> mResetReceiver;
};

} // namespace Net
} // namespace Server
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_SERVER_LOCKDOWNVPNTRACKER_H__
