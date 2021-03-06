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

#ifndef __ELASTOS_DROID_NET_HTTP_CERRORSTRINGS_H__
#define __ELASTOS_DROID_NET_HTTP_CERRORSTRINGS_H__

#include "_Elastos_Droid_Net_Http_CErrorStrings.h"
#include <elastos/core/Singleton.h>

using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

/**
 * Localized strings for the error codes defined in EventHandler.
 *
 * {@hide}
 */
CarClass(CErrorStrings)
    , public Singleton
    , public IErrorStrings
{
public:
    CAR_INTERFACE_DECL()

    CAR_SINGLETON_DECL()

    /**
     * Get the localized error message resource for the given error code.
     * If the code is unknown, we'll return a generic error message.
     */
    CARAPI GetString(
        /* [in] */ Int32 errorCode,
        /* [in] */ IContext* context,
        /* [out] */ String* result);

    /**
     * Get the localized error message resource for the given error code.
     * If the code is unknown, we'll return a generic error message.
     */
    CARAPI GetResource(
        /* [in] */ Int32 errorCode,
        /* [out] */ Int32* result);
};

} // namespace Http
} // namespace Net
} // namespace Droid
} // namespace Elastos

#endif //  __ELASTOS_DROID_NET_HTTP_CERRORSTRINGS_H__
