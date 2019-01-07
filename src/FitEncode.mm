////////////////////////////////////////////////////////////////////////////////
// The following FIT Protocol software provided may be used with FIT protocol
// devices only and remains the copyrighted property of Garmin Canada Inc.
// The software is being provided on an "as-is" basis and as an accommodation,
// and therefore all warranties, representations, or guarantees of any kind
// (whether express, implied or statutory) including, without limitation,
// warranties of merchantability, non-infringement, or fitness for a particular
// purpose, are specifically disclaimed.
//
// Copyright 2018 Garmin Canada Inc.
////////////////////////////////////////////////////////////////////////////////
// ****WARNING****  This file is auto-generated!  Do NOT edit this file.
// Profile Version = 20.80Release
// Tag = production/akw/20.80.00-0-g64ad259
////////////////////////////////////////////////////////////////////////////////


#import <Foundation/Foundation.h>
#import "FitEncode.h"
#import "FitMesgDefinition.h"
#import "FitMesg.h"

#include "fit.hpp"
#include "fit_crc.hpp"
#include "fit_protocol_validator.hpp"

// Declare all private variables

@interface FitEncode ()

- (void)commonInitWithVersion:(fit::ProtocolVersion)ver;

@property fit::MesgDefinition *lastMesgDefinition;
@property FIT_UINT32 dataSize;
@property FILE *encodeFile;
@property fit::ProtocolVersion version;
@property fit::ProtocolValidator *validator;

- (void) WriteFileHeader;

@end

@implementation FitEncode

@synthesize lastMesgDefinition;
@synthesize dataSize;
@synthesize encodeFile;
@synthesize version;
@synthesize validator;

- (id)init
{
    self = [super init];
    if (self)
    {
        [self commonInitWithVersion:fit::ProtocolVersion::V10];
    }
    return self;
}

- (void)commonInitWithVersion:(fit::ProtocolVersion)ver
{
    dataSize = 0;
    version = ver;
    validator = new fit::ProtocolValidator(ver);
    lastMesgDefinition = new fit::MesgDefinition[FIT_MAX_LOCAL_MESGS];
}

- (id)initWithVersion:(fit::ProtocolVersion)ver
{
    self = [super init];
    if (self)
    {
        [self commonInitWithVersion:ver];
    }
    return self;
}

-(void)finalize
{
    //Clean-up managed resources if not already done
    if(lastMesgDefinition!= NULL) { delete[] lastMesgDefinition; lastMesgDefinition = NULL;}
    [super finalize];
}

-(void)dealloc
{
    //Clean-up managed resources if not already done
    if(lastMesgDefinition!= NULL) { delete[] lastMesgDefinition; lastMesgDefinition = NULL;}
}

- (void) Open:(FILE *)file
{
    encodeFile = file;
    [self WriteFileHeader];
}

- (void) WriteMesgDef:(const fit::MesgDefinition &) mesgDef
{
    if (!encodeFile)
        return;

    if ( !validator->ValidateMesgDefn( mesgDef ) )
        return;

    FitMesgDefinition *fmd = [[FitMesgDefinition alloc] init];
    dataSize += [fmd Write:encodeFile forMesgDefn:&mesgDef];
    lastMesgDefinition[mesgDef.GetLocalNum()] = mesgDef;
}

- (void) WriteMesg:(const fit::Mesg &) mesg
{
    fit::MesgDefinition mesgDefinition(mesg);

    if (!encodeFile)
        return;

    if ( !validator->ValidateMesg( mesg ) )
        return;

    if (!lastMesgDefinition[mesg.GetLocalNum()].Supports(mesgDefinition))
        [self WriteMesgDef:mesgDefinition];

    FitMesg *fm = [[FitMesg alloc] init];
    dataSize += [fm Write:encodeFile forMesg:&mesg withDef:&(lastMesgDefinition[mesg.GetLocalNum()])];
}

- (void) WriteMesgs:(const std::vector<fit::Mesg> &) mesgs
{
    for (std::vector<fit::Mesg>::size_type i = 0; i < mesgs.size(); i++)
        [self WriteMesg:mesgs[i]];
}

- (FIT_BOOL) Close
{
    FIT_UINT32 fileBytesLeft = FIT_FILE_HDR_SIZE + dataSize;
    FIT_UINT16 crc = 0;
    FIT_UINT8 byte;

    if (!encodeFile)
        return FIT_FALSE;

    [self WriteFileHeader];

    fseek(encodeFile, 0, SEEK_SET);

    while (fileBytesLeft > 0)
    {
        if(!fread(&byte, 1, 1, encodeFile))
            return FIT_FALSE; // File smaller than expected.

        crc = fit::CRC::Get16(crc, byte);
        fileBytesLeft--;
    }

    fseek(encodeFile, 0, SEEK_END);
    byte = crc & 0xff;
    fwrite(&byte, 1, 1, encodeFile);
    byte = (FIT_UINT8)(crc >> 8);
    fwrite(&byte, 1, 1, encodeFile);

    return FIT_TRUE;
}

- (void) OnMesg:(fit::Mesg&) mesg
{
    [self WriteMesg:mesg];
}

- (void) OnMesgDefinition:(fit::MesgDefinition&) mesgDef
{
    [self WriteMesgDef:mesgDef];
}

- (void) WriteFileHeader
{
    FIT_FILE_HDR file_header;

    if (!encodeFile)
        return;

    file_header.header_size = FIT_FILE_HDR_SIZE;
    file_header.profile_version = FIT_PROFILE_VERSION;
    file_header.protocol_version = fit::versionMap.at( version ).GetVersionByte();
    memcpy((FIT_UINT8 *)&file_header.data_type, ".FIT", 4);
    file_header.data_size = dataSize;
    file_header.crc = fit::CRC::Calc16(&file_header, FIT_STRUCT_OFFSET(crc, FIT_FILE_HDR));

    fseek(encodeFile, 0, SEEK_SET);
    fwrite((const char *)&file_header, FIT_FILE_HDR_SIZE, 1, encodeFile);
}

@end
