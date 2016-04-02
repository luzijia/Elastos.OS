#ifndef __ELASTOS_DROID_TELECOMM_TELECOM_CVIDEOPROFILE_H__
#define __ELASTOS_DROID_TELECOMM_TELECOM_CVIDEOPROFILE_H__

#include "_Elastos_Droid_Telecomm_Telecom_CVideoProfile.h"
#include "elastos/droid/telecomm/telecom/VideoProfile.h"

namespace Elastos {
namespace Droid {
namespace Telecomm {
namespace Telecom {

CarClass(CVideoProfile)
    , public VideoProfile
{
public:
    CAR_OBJECT_DECL()
};

} // namespace Telecom
} // namespace Telecomm
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_TELECOMM_TELECOM_CVIDEOPROFILE_H__