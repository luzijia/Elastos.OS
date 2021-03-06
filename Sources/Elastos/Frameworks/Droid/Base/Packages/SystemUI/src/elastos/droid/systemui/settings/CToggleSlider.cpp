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

#include "elastos/droid/systemui/settings/CToggleSlider.h"
#include "Elastos.Droid.View.h"
#include "Elastos.CoreLibrary.Core.h"
#include "elastos/droid/view/View.h"
#include "R.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Content::Res::ITypedArray;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::View;
using Elastos::Droid::View::IViewParent;
using Elastos::Droid::Widget::EIID_ICompoundButtonOnCheckedChangeListener;
using Elastos::Droid::Widget::EIID_ISeekBarOnSeekBarChangeListener;
using Elastos::Droid::Widget::ICheckable;
using Elastos::Droid::Widget::IProgressBar;
using Elastos::Core::CString;
using Elastos::Core::ICharSequence;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace SystemUI {
namespace Settings {

static const String TAG("CToggleSlider");

//==================================
//   CToggleSlider::MyCheckListener
//==================================
CAR_INTERFACE_IMPL(CToggleSlider::MyCheckListener, Object, ICompoundButtonOnCheckedChangeListener)

CToggleSlider::MyCheckListener::MyCheckListener(
    /* [in] */ CToggleSlider* host)
    : mHost(host)
{}

ECode CToggleSlider::MyCheckListener::OnCheckedChanged(
    /* [in] */ ICompoundButton* buttonView,
    /* [in] */ Boolean isChecked)
{
    if (mHost->mListener != NULL) {
        Int32 progress;
        IProgressBar::Probe(mHost->mSlider)->GetProgress(&progress);
        mHost->mListener->OnChanged(mHost, mHost->mTracking, isChecked, progress);
    }

    if (mHost->mMirror != NULL) {
        ICheckable::Probe(mHost->mMirror->mToggle)->SetChecked(isChecked);
    }
    return NOERROR;
}

//==================================
//   CToggleSlider::MySeekListener
//==================================
CAR_INTERFACE_IMPL(CToggleSlider::MySeekListener, Object, ISeekBarOnSeekBarChangeListener)

CToggleSlider::MySeekListener::MySeekListener(
    /* [in] */ CToggleSlider* host)
    : mHost(host)
{}

ECode CToggleSlider::MySeekListener::OnProgressChanged(
    /* [in] */ ISeekBar* seekBar,
    /* [in] */ Int32 progress,
    /* [in] */ Boolean fromUser)
{
    if (mHost->mListener != NULL) {
        Boolean isChecked;
        ICheckable::Probe(mHost->mToggle)->IsChecked(&isChecked);
        mHost->mListener->OnChanged(mHost, mHost->mTracking, isChecked, progress);
    }

    if (mHost->mMirror != NULL) {
        mHost->mMirror->SetValue(progress);
    }
    return NOERROR;
}

ECode CToggleSlider::MySeekListener::OnStartTrackingTouch(
    /* [in] */ ISeekBar* seekBar)
{
    return mHost->OnStartTrackingTouch(seekBar);
}

ECode CToggleSlider::MySeekListener::OnStopTrackingTouch(
     /* [in] */ ISeekBar* seekBar)
{
    return mHost->OnStopTrackingTouch(seekBar);
}

//==================================
//   CToggleSlider
//==================================
CAR_OBJECT_IMPL(CToggleSlider)

CAR_INTERFACE_IMPL(CToggleSlider, RelativeLayout, IToggleSlider)

CToggleSlider::CToggleSlider()
{
}

ECode CToggleSlider::constructor(
    /* [in] */ IContext* context)
{
    return constructor(context, NULL);
}

ECode CToggleSlider::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return constructor(context, attrs, 0);
}

ECode CToggleSlider::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    RelativeLayout::constructor(context, attrs, defStyle);

    mCheckListener = new MyCheckListener(this);
    mSeekListener = new MySeekListener(this);

    AutoPtr<IView> view;
    View::Inflate(context, R::layout::status_bar_toggle_slider, this, (IView**)&view);

    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);

    AutoPtr<ArrayOf<Int32> > attrIds = TO_ATTRS_ARRAYOF(R::styleable::ToggleSlider);
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(
        attrs, attrIds, defStyle, 0, (ITypedArray**)&a);

    AutoPtr<IView> v;
    FindViewById(R::id::toggle, (IView**)&v);
    mToggle = ICompoundButton::Probe(v);
    mToggle->SetOnCheckedChangeListener(mCheckListener);

    v = NULL;
    FindViewById(R::id::slider, (IView**)&v);
    mSlider = ISeekBar::Probe(v);
    mSlider->SetOnSeekBarChangeListener(mSeekListener);

    v = NULL;
    FindViewById(R::id::label, (IView**)&v);
    mLabel = ITextView::Probe(v);
    String str;
    a->GetString(R::styleable::ToggleSlider_text, &str);
    AutoPtr<ICharSequence> cs;
    CString::New(str, (ICharSequence**)&cs);
    mLabel->SetText(cs);

    a->Recycle();
    return NOERROR;
}

ECode CToggleSlider::SetMirror(
    /* [in] */ IToggleSlider* toggleSlider)
{
    mMirror = (CToggleSlider*)toggleSlider;
    if (mMirror != NULL) {
        Boolean isChecked;
        ICheckable::Probe(mToggle)->IsChecked(&isChecked);
        mMirror->SetChecked(isChecked);
        Int32 max;
        IProgressBar::Probe(mSlider)->GetMax(&max);
        mMirror->SetMax(max);
        Int32 progress;
        IProgressBar::Probe(mSlider)->GetProgress(&progress);
        mMirror->SetValue(progress);
    }
    return NOERROR;
}

ECode CToggleSlider::SetMirrorController(
    /* [in] */ IBrightnessMirrorController* c)
{
    mMirrorController = c;
    return NOERROR;
}

ECode CToggleSlider::OnAttachedToWindow()
{
    ViewGroup::OnAttachedToWindow();
    if (mListener != NULL) {
        mListener->OnInit(this);
    }
    return NOERROR;
}

ECode CToggleSlider::SetOnChangedListener(
    /* [in] */ IToggleSliderListener* l)
{
    mListener = l;
    return NOERROR;
}

ECode CToggleSlider::SetChecked(
    /* [in] */ Boolean checked)
{
    return ICheckable::Probe(mToggle)->SetChecked(checked);
}

ECode CToggleSlider::IsChecked(
    /* [out] */ Boolean* isChecked)
{
    ICheckable::Probe(mToggle)->IsChecked(isChecked);
    return NOERROR;
}

ECode CToggleSlider::SetMax(
    /* [in] */ Int32 max)
{
    IProgressBar::Probe(mSlider)->SetMax(max);
    if (mMirror != NULL) {
        mMirror->SetMax(max);
    }
    return NOERROR;
}

ECode CToggleSlider::SetValue(
    /* [in] */ Int32 value)
{
    IProgressBar::Probe(mSlider)->SetProgress(value);
    if (mMirror != NULL) {
        mMirror->SetValue(value);
    }
    return NOERROR;
}

ECode CToggleSlider::OnStartTrackingTouch(
    /* [in] */ ISeekBar* seekBar)
{
    mTracking = TRUE;

    if (mListener != NULL) {
        Boolean isChecked;
        ICheckable::Probe(mToggle)->IsChecked(&isChecked);
        Int32 progress;
        IProgressBar::Probe(mSlider)->GetProgress(&progress);
        mListener->OnChanged(this, mTracking, isChecked, progress);
    }

    ICheckable::Probe(mToggle)->SetChecked(FALSE);

    if (mMirror != NULL) {
        IView::Probe(mMirror->mSlider)->SetPressed(TRUE);
    }

    if (mMirrorController != NULL) {
        mMirrorController->ShowMirror();
        AutoPtr<IViewParent> parent;
        GetParent((IViewParent**)&parent);
        mMirrorController->SetLocation(IView::Probe(parent));
    }
    return NOERROR;
}

ECode CToggleSlider::OnStopTrackingTouch(
    /* [in] */ ISeekBar* seekBar)
{
    mTracking = FALSE;

    if (mListener != NULL) {
        Boolean isChecked;
        ICheckable::Probe(mToggle)->IsChecked(&isChecked);
        Int32 progress;
        IProgressBar::Probe(mSlider)->GetProgress(&progress);
        mListener->OnChanged(this, mTracking, isChecked, progress);
    }

    if (mMirror != NULL) {
        IView::Probe(mMirror->mSlider)->SetPressed(FALSE);
    }

    if (mMirrorController != NULL) {
        mMirrorController->HideMirror();
    }
    return NOERROR;
}

} // namespace Settings
} // namespace SystemUI
} // namespace Droid
} // namespace Elastos
