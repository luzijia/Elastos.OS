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

#include "elastos/droid/hardware/hdmi/HdmiRecordListener.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Hdmi {

CAR_INTERFACE_IMPL(HdmiRecordListener::TimerStatusData, Object, IHdmiRecordListenerTimerStatusData)

ECode HdmiRecordListener::TimerStatusData::ParseFrom(
    /* [in] */ Int32 result,
    /* [out] */ IHdmiRecordListenerTimerStatusData** _result)
{
    VALIDATE_NOT_NULL(_result);
    *_result = NULL;

    AutoPtr<TimerStatusData> data = new TimerStatusData();
    // Timer Overlap Warning - 1 bit
    data->mOverlapped = ((result >> 31) & 0x1) != 0;
    // Media Info - 2 bits;
    data->mMediaInfo = (result >> 29) & 0x3;
    // Programmed Indicator - 1 bit;
    data->mProgrammed = ((result >> 28) & 0x1) != 0;
    if (data->mProgrammed) {
        data->mProgrammedInfo = (result >> 24) & 0xF;
        data->mDurationHour = BcdByteToInt((byte) ((result >> 16) & 0xFF));
        data->mDurationMinute = BcdByteToInt((byte) ((result >> 8) & 0xFF));
    }
    else {
        // Programmed Info - 4 bits
        data->mNotProgrammedError = (result >> 24) & 0xF;
        data->mDurationHour = BcdByteToInt((byte) ((result >> 16) & 0xFF));
        data->mDurationMinute = BcdByteToInt((byte) ((result >> 8) & 0xFF));
    }

    // The last byte is used for extra error.
    data->mExtraError = result & 0xFF;
    *_result = data.Get();
    REFCOUNT_ADD(*_result);
    return NOERROR;
}

ECode HdmiRecordListener::TimerStatusData::IsOverlapped(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mOverlapped;
    return NOERROR;
}

ECode HdmiRecordListener::TimerStatusData::GetMediaInfo(
    /* [out] */ Int32* info)
{
    VALIDATE_NOT_NULL(info);

    *info = mMediaInfo;
    return NOERROR;
}

ECode HdmiRecordListener::TimerStatusData::IsProgrammed(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mProgrammed;
    return NOERROR;
}

ECode HdmiRecordListener::TimerStatusData::GetProgrammedInfo(
    /* [out] */ Int32* info)
{
    VALIDATE_NOT_NULL(info);
    *info = 0;

    Boolean result;
    IsProgrammed(&result);
    if (!result) {
        // throw new IllegalStateException(
        //         "No programmed info. Call getNotProgammedError() instead.");
        Logger::E("HdmiRecordListener::TimerStatusData", "No programmed info. Call getNotProgammedError() instead.");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    *info = mProgrammedInfo;
    return NOERROR;
}

ECode HdmiRecordListener::TimerStatusData::GetNotProgammedError(
    /* [out] */ Int32* error)
{
    VALIDATE_NOT_NULL(error);
    *error = 0;

    Boolean result;
    IsProgrammed(&result);
    if (result) {
        // throw new IllegalStateException(
        //         "Has no not-programmed error. Call getProgrammedInfo() instead.");
        Logger::E("HdmiRecordListener::TimerStatusData", "Has no not-programmed error. Call getProgrammedInfo() instead.");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    *error = mNotProgrammedError;
    return NOERROR;
}

ECode HdmiRecordListener::TimerStatusData::GetDurationHour(
    /* [out] */ Int32* hour)
{
    VALIDATE_NOT_NULL(hour);

    *hour =  mDurationHour;
    return NOERROR;
}

ECode HdmiRecordListener::TimerStatusData::GetDurationMinute(
    /* [out] */ Int32* minute)
{
    VALIDATE_NOT_NULL(minute);

    *minute = mDurationMinute;
    return NOERROR;
}

ECode HdmiRecordListener::TimerStatusData::GetExtraError(
    /* [out] */ Int32* error)
{
    VALIDATE_NOT_NULL(error);

    *error = mExtraError;
    return NOERROR;
}

Int32 HdmiRecordListener::TimerStatusData::BcdByteToInt(
    /* [in] */ Byte value)
{
    return ((value >> 4) & 0xF) * 10 + (value & 0xF);
}

CAR_INTERFACE_IMPL(HdmiRecordListener, Object, IHdmiRecordListener)

CARAPI OnOneTouchRecordResult(
    /* [in] */ Int32 result)
{
    return NOERROR;
}

CARAPI OnTimerRecordingResult(
    /* [in] */ IHdmiRecordListenerTimerStatusData* data)
{
    return NOERROR;
}

CARAPI OnClearTimerRecordingResult(
    /* [in] */ Int32 result)
{
    return NOERROR;
}

} // namespace Hdmi
} // namespace Hardware
} // namepsace Droid
} // namespace Elastos
