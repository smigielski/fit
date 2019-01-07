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


#if !defined(FIT_DEVELOPER_FIELD_DESCRIPTION_HPP)
#define FIT_DEVELOPER_FIELD_DESCRIPTION_HPP

#include "fit_field_description_mesg.hpp"
#include "fit_developer_data_id_mesg.hpp"
#include <vector>

namespace fit
{
class DeveloperFieldDescription
{
public:
    DeveloperFieldDescription() = delete;
    DeveloperFieldDescription(const DeveloperFieldDescription& other);
    DeveloperFieldDescription(const FieldDescriptionMesg& desc, const DeveloperDataIdMesg& developer);
    virtual ~DeveloperFieldDescription();

    FIT_UINT32 GetApplicationVersion() const;
    FIT_UINT8 GetFieldDefinitionNumber() const;
    std::vector<FIT_UINT8> GetApplicationId() const;

private:
    FieldDescriptionMesg* description;
    DeveloperDataIdMesg* developer;
};

} // namespace fit

#endif // defined(FIT_FIELD_DEFINITION_HPP)
