#include "CFBStreamedCompression.hpp"

namespace urde
{

CFBStreamedCompression::CFBStreamedCompression(CInputStream& in, IObjectStore& objStore, bool pc)
: m_pc(pc)
{
    x0_scratchSize = in.readUint32Big();
    x4_evnt = in.readUint32Big();

    xc_rotsAndOffs = GetRotationsAndOffsets(x0_scratchSize / 4 + 1, in);

    if (x4_evnt)
        x8_evntToken = objStore.GetObj(SObjectTag{FOURCC('EVNT'), x4_evnt});
}

std::unique_ptr<u32[]> CFBStreamedCompression::GetRotationsAndOffsets(u32 words, CInputStream& in)
{
    std::unique_ptr<u32[]> ret(new u32[words]);

    Header head;
    head.read(in);
    *reinterpret_cast<Header*>(ret.get()) = head;

    u32* bitmapOut = &ret[9];
    u32 bitmapBitCount = in.readUint32Big();
    bitmapOut[0] = bitmapBitCount;
    u32 bitmapWordCount = (bitmapBitCount + 31) / 32;
    for (u32 i=0 ; i<bitmapWordCount ; ++i)
        bitmapOut[i+1] = in.readUint32Big();

    in.readUint32Big();
    u8* chans = reinterpret_cast<u8*>(bitmapOut + bitmapWordCount + 1);
    u8* bs = ReadBoneChannelDescriptors(chans, in);
    u32 bsWords = ComputeBitstreamWords(chans);

    u32* bsPtr = reinterpret_cast<u32*>(bs);
    for (u32 w=0 ; w<bsWords ; ++w)
        bsPtr[w] = in.readUint32Big();

    return ret;
}

u8* CFBStreamedCompression::ReadBoneChannelDescriptors(u8* out, CInputStream& in)
{
    u32 boneChanCount = in.readUint32Big();
    *reinterpret_cast<u32*>(out) = boneChanCount;
    out += 4;

    if (m_pc)
    {
        for (int b=0 ; b<boneChanCount ; ++b)
        {
            *reinterpret_cast<u32*>(out) = in.readUint32Big();
            out += 4;

            *reinterpret_cast<u32*>(out) = in.readUint32Big();
            out += 4;

            for (int i=0 ; i<3 ; ++i)
            {
                *reinterpret_cast<u32*>(out) = in.readUint32Big();
                out += 4;
            }

            u32 tCount = in.readUint32Big();
            *reinterpret_cast<u32*>(out) = tCount;
            out += 4;

            if (tCount)
            {
                for (int i=0 ; i<3 ; ++i)
                {
                    *reinterpret_cast<u32*>(out) = in.readUint32Big();
                    out += 4;
                }
            }
        }
    }
    else
    {
        for (int b=0 ; b<boneChanCount ; ++b)
        {
            *reinterpret_cast<u32*>(out) = in.readUint32Big();
            out += 4;

            *reinterpret_cast<u16*>(out) = in.readUint16Big();
            out += 2;

            for (int i=0 ; i<3 ; ++i)
            {
                *reinterpret_cast<s16*>(out) = in.readInt16Big();
                out += 2;
                *reinterpret_cast<u8*>(out) = in.readUByte();
                out += 1;
            }

            u16 tCount = in.readUint16Big();
            *reinterpret_cast<u16*>(out) = tCount;
            out += 2;

            if (tCount)
            {
                for (int i=0 ; i<3 ; ++i)
                {
                    *reinterpret_cast<s16*>(out) = in.readInt16Big();
                    out += 2;
                    *reinterpret_cast<u8*>(out) = in.readUByte();
                    out += 1;
                }
            }
        }
    }

    return out;
}

u32 CFBStreamedCompression::ComputeBitstreamWords(const u8* chans)
{
    u32 boneChanCount = *reinterpret_cast<const u32*>(chans);
    chans += 4;

    u32 keyCount;

    u32 totalBits = 0;
    if (m_pc)
    {
        keyCount = *reinterpret_cast<const u32*>(chans + 0x4);
        for (u32 c=0 ; c<boneChanCount ; ++c)
        {
            chans += 0x8;
            totalBits += *reinterpret_cast<const u32*>(chans) & 0xff;
            totalBits += *reinterpret_cast<const u32*>(chans + 0x4) & 0xff;
            totalBits += *reinterpret_cast<const u32*>(chans + 0x8) & 0xff;
            u32 tKeyCount = *reinterpret_cast<const u32*>(chans + 0xc);
            chans += 0x10;
            if (tKeyCount)
            {
                totalBits += *reinterpret_cast<const u32*>(chans) & 0xff;
                totalBits += *reinterpret_cast<const u32*>(chans + 0x4) & 0xff;
                totalBits += *reinterpret_cast<const u32*>(chans + 0x8) & 0xff;
                chans += 0xc;
            }
        }
    }
    else
    {
        keyCount = *reinterpret_cast<const u16*>(chans + 0x4);
        for (u32 c=0 ; c<boneChanCount ; ++c)
        {
            chans += 0x6;
            totalBits += *reinterpret_cast<const u8*>(chans + 0x2);
            totalBits += *reinterpret_cast<const u8*>(chans + 0x5);
            totalBits += *reinterpret_cast<const u8*>(chans + 0x8);
            u16 tKeyCount = *reinterpret_cast<const u16*>(chans + 0x9);
            chans += 0xb;
            if (tKeyCount)
            {
                totalBits += *reinterpret_cast<const u8*>(chans + 0x2);
                totalBits += *reinterpret_cast<const u8*>(chans + 0x5);
                totalBits += *reinterpret_cast<const u8*>(chans + 0x8);
                chans += 0x9;
            }
        }
    }

    return (totalBits * keyCount + 31) / 32;
}

}
