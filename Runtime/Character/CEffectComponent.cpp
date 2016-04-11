#include "CEffectComponent.hpp"

namespace urde
{

SObjectTag CEffectComponent::GetSObjectTagFromStream(CInputStream& in)
{
    char tpChars[4];
    in.readBytesToBuf(tpChars, 4);
    return {tpChars, in.readUint32Big()};
}

CEffectComponent::CEffectComponent(CInputStream& in)
{
    x0_name = in.readString();
    x10_tag = GetSObjectTagFromStream(in);
    x18_boneName = in.readString();
    x28_ = in.readFloatBig();
    x2c_ = in.readUint32Big();
    x30_ = in.readUint32Big();
}

}
