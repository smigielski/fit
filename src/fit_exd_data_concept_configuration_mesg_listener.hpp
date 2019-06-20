////////////////////////////////////////////////////////////////////////////////
// The following FIT Protocol software provided may be used with FIT protocol
// devices only and remains the copyrighted property of Garmin Canada Inc.
// The software is being provided on an "as-is" basis and as an accommodation,
// and therefore all warranties, representations, or guarantees of any kind
// (whether express, implied or statutory) including, without limitation,
// warranties of merchantability, non-infringement, or fitness for a particular
// purpose, are specifically disclaimed.
//
// Copyright 2019 Garmin Canada Inc.
////////////////////////////////////////////////////////////////////////////////
// ****WARNING****  This file is auto-generated!  Do NOT edit this file.
// Profile Version = 21.06Release
// Tag = production/akw/21.06.00-0-g04ae3f5
////////////////////////////////////////////////////////////////////////////////


#if !defined(FIT_EXD_DATA_CONCEPT_CONFIGURATION_MESG_LISTENER_HPP)
#define FIT_EXD_DATA_CONCEPT_CONFIGURATION_MESG_LISTENER_HPP

#include "fit_exd_data_concept_configuration_mesg.hpp"

namespace fit
{

class ExdDataConceptConfigurationMesgListener
{
public:
    virtual ~ExdDataConceptConfigurationMesgListener() {}
    virtual void OnMesg(ExdDataConceptConfigurationMesg& mesg) = 0;
};

} // namespace fit

#endif // !defined(FIT_EXD_DATA_CONCEPT_CONFIGURATION_MESG_LISTENER_HPP)
