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


#include <ostream>
#include <algorithm>
#include "fit_mesg.hpp"
#include "fit_mesg_definition.hpp"
#include "fit_factory.hpp"

namespace fit
{

Mesg::Mesg()
    : profile(FIT_NULL)
    , localNum(0)
    , fields()
    , devFields()
{
}

Mesg::Mesg(const Mesg &mesg)
    : profile(mesg.profile)
    , localNum(mesg.localNum)
    , fields(mesg.fields)
    , devFields(mesg.devFields)
{
}

Mesg::Mesg(const Profile::MESG_INDEX index)
    : profile(&Profile::mesgs[index])
    , localNum(0)
    , fields()
    , devFields()
{
}

Mesg::Mesg(const std::string& name)
    : profile(Profile::GetMesg(name))
    , localNum(0)
    , fields()
    , devFields()
{
}

Mesg::Mesg(const FIT_UINT16 num)
    : profile(Profile::GetMesg(num))
    , localNum(0)
    , fields()
    , devFields()
{
}

FIT_BOOL Mesg::IsValid(void) const
{
    return (profile != FIT_NULL);
}

FIT_BOOL Mesg::GetIsFieldAccumulated(const FIT_UINT8 num) const
{
    const Field *field = this->GetField(num);
    if (field != FIT_NULL)
    {
        return field->GetIsAccumulated();
    }
    return FIT_FALSE;
}

const DeveloperField* Mesg::GetDeveloperField(FIT_UINT8 developerDataIndex, FIT_UINT8 num) const
{
    for (int i = 0; i < (int)devFields.size(); i++)
    {
        const DeveloperFieldDefinition& def = devFields[i].GetDefinition();
        if ((def.GetNum() == num) &&
            (def.GetDeveloperDataIndex() == developerDataIndex))
        {
            return &devFields[i];
        }
    }

    return FIT_NULL;
}

int Mesg::Write(std::ostream& file, const MesgDefinition* mesgDef) const
{
    MesgDefinition mesgDefOnNull;
    int mesgSize = 1;
    int fieldSize = 0;

    file.put((localNum & FIT_HDR_TYPE_MASK)); // Message record header with local message number.

    if (mesgDef == FIT_NULL)
    {
        mesgDefOnNull = MesgDefinition(*this);
        mesgDef = &mesgDefOnNull;
    }

    for (FIT_UINT16 fieldDefIndex = 0; fieldDefIndex < (mesgDef->GetFields().size()); fieldDefIndex++)
    {
        const Field* field = GetField(mesgDef->GetFieldByIndex(fieldDefIndex)->GetNum());
        const FieldDefinition* definition = mesgDef->GetFieldByIndex(fieldDefIndex);

        fieldSize = WriteField(file, field, definition->GetSize(), definition->GetType());
        if (fieldSize == 0)
        {
            // Something went wrong during our write
            return 0;
        }

        mesgSize += fieldSize;
    }

    for (DeveloperFieldDefinition def : mesgDef->GetDevFields())
    {
        const DeveloperField* field = GetDeveloperField(def.GetDeveloperDataIndex(), def.GetNum());

        fieldSize = WriteField(file, field, def.GetSize(), def.GetType());
        if (fieldSize == 0)
        {
            // Something went wrong during our write
            return 0;
        }

        mesgSize += fieldSize;
    }

    return mesgSize;
}

std::string Mesg::GetName() const
{
    if (profile == FIT_NULL)
        return "unknown";

    return profile->name;
}

FIT_UINT16 Mesg::GetNum() const
{
    if (profile == FIT_NULL)
        return FIT_MESG_NUM_INVALID;

    return profile->num;
}

FIT_UINT8 Mesg::GetLocalNum() const
{
    return localNum;
}

void Mesg::SetLocalNum(const FIT_UINT8 newLocalNum)
{
    if (newLocalNum < FIT_MAX_LOCAL_MESGS)
        localNum = newLocalNum;
}

FIT_BOOL Mesg::HasField(const int fieldNum) const
{
    for (int i = 0; i < (int)fields.size(); i++)
    {
        if (fields[i].GetNum() == fieldNum)
            return FIT_TRUE;
    }

    return FIT_FALSE;
}

void Mesg::AddField(const Field& field)
{
    Field *existingField = GetField(field.GetNum());

    if (existingField == FIT_NULL)
        fields.push_back(field);
}

Field* Mesg::AddField(const FIT_UINT8 fieldNum)
{
    Field *field = GetField(fieldNum);

    if (field == FIT_NULL)
    {
        fields.push_back(Field(profile->num, fieldNum));
        field = &fields[fields.size() - 1];
    }

    return field;
}

void Mesg::AddDeveloperField(const DeveloperField& field)
{
    for (FIT_UINT16 i = 0; i < devFields.size(); ++i)
    {
        if (devFields[i].GetDefinition() == field.GetDefinition())
        {
            devFields[i] = field;
            return;
        }
    }

    devFields.push_back(field);
}

void Mesg::SetField(const Field& field)
{
    for (int i = 0; i < (int)fields.size(); i++)
    {
        if (fields[i].GetNum() == field.GetNum())
        {
            fields[i] = field;
            return;
        }
    }

    fields.push_back(field);
}

void Mesg::SetFields(const Mesg& mesg)
{
    if (mesg.GetNum() != GetNum())
        return;

    for (int i=0; i < (int)mesg.fields.size(); i++)
        SetField(mesg.fields[i]);
}

int Mesg::GetNumFields() const
{
    return (int)fields.size();
}

int Mesg::GetNumDevFields() const
{
    return (int)devFields.size();
}

Field* Mesg::GetFieldByIndex(const FIT_UINT16 index)
{
    if (index < fields.size())
        return &fields[index];

    return FIT_NULL;
}

Field* Mesg::GetField(const FIT_UINT8 fieldNum)
{
    for (int i = 0; i < (int)fields.size(); i++)
    {
        if (fields[i].GetNum() == fieldNum)
            return &fields[i];
    }

    return FIT_NULL;
}

Field* Mesg::GetField(const std::string& name)
{
    for (FIT_UINT16 i = 0; i < (FIT_UINT16)fields.size(); i++)
    {
        if (fields[i].GetName().compare(name) == 0)
            return &fields[i];

        for (FIT_UINT16 j = 0; j < fields[i].GetNumSubFields(); j++)
        {
            if ((fields[i].GetName(j).compare(name) == 0) && CanSupportSubField(&(fields[i]),j))
                return &fields[i];
        }
    }

    return FIT_NULL;
}

const Field* Mesg::GetFieldByIndex(const FIT_UINT16 index) const
{
    if (index < fields.size())
        return &fields[index];

    return FIT_NULL;
}

const Field* Mesg::GetField(const FIT_UINT8 fieldNum) const
{
    for (int i = 0; i < (int)fields.size(); i++)
    {
        if (fields[i].GetNum() == fieldNum)
            return &fields[i];
    }

    return FIT_NULL;
}

const Field* Mesg::GetField(const std::string& name) const
{
    for (FIT_UINT16 i = 0; i < (FIT_UINT16)fields.size(); i++)
    {
        if (fields[i].GetName().compare(name) == 0)
            return &fields[i];

        for (FIT_UINT16 j = 0; j < fields[i].GetNumSubFields(); j++)
        {
            if ((fields[i].GetName(j).compare(name) == 0) && CanSupportSubField(&(fields[i]),j))
                return &fields[i];
        }
    }

    return FIT_NULL;
}

const std::vector<DeveloperField>& Mesg::GetDeveloperFields() const
{
    return devFields;
}

FIT_BOOL Mesg::CanSupportSubField(const FIT_UINT8 fieldNum, const FIT_UINT16 subFieldIndex) const
{
    if ((subFieldIndex == FIT_SUBFIELD_INDEX_MAIN_FIELD) || (subFieldIndex == FIT_SUBFIELD_INDEX_ACTIVE_SUBFIELD))
        return FIT_TRUE;

    const Profile::SUBFIELD* subField = Profile::GetSubField(GetNum(), fieldNum, subFieldIndex);

    if (subField == FIT_NULL)
        return FIT_FALSE;

    for (int i = 0; i < (int) subField->numMaps; i++)
    {
        const Field* refField = GetField(subField->maps[i].refFieldNum);

        if (refField != FIT_NULL)
        {
            FIT_FLOAT64 refValue = refField->GetFLOAT64Value(0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
            refValue += ((refValue >= 0.0) ? (0.5) : (-0.5));
            if ((FIT_SINT32)refValue == subField->maps[i].refFieldValue)
                return FIT_TRUE;
        }
    }

    return FIT_FALSE;
}

FIT_BOOL Mesg::CanSupportSubField(const Field* field, const FIT_UINT16 subFieldIndex) const
{
    if ((subFieldIndex == FIT_SUBFIELD_INDEX_MAIN_FIELD) || (subFieldIndex == FIT_SUBFIELD_INDEX_ACTIVE_SUBFIELD))
        return FIT_TRUE;

    if (field == FIT_NULL)
        return FIT_FALSE;

    const Profile::SUBFIELD* subField = field->GetSubField(subFieldIndex);

    if (subField == FIT_NULL)
        return FIT_FALSE;

    for (int i = 0; i < (int) subField->numMaps; i++)
    {
        const Field* refField = GetField(subField->maps[i].refFieldNum);

        if (refField != FIT_NULL)
        {
            FIT_FLOAT64 refValue = refField->GetFLOAT64Value(0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
            refValue += ((refValue >= 0.0) ? (0.5) : (-0.5));
            if ((FIT_SINT32)refValue == subField->maps[i].refFieldValue)
                return FIT_TRUE;
        }
    }

    return FIT_FALSE;
}

FIT_UINT16 Mesg::GetActiveSubFieldIndexByFieldIndex(const FIT_UINT16 fieldIndex) const
{
    if ((int) fieldIndex >= GetNumFields())
        return FIT_SUBFIELD_INDEX_MAIN_FIELD;

    for (FIT_UINT16 i = 0; i < fields[fieldIndex].GetNumSubFields(); i++)
    {
        const Profile::SUBFIELD* subField = fields[fieldIndex].GetSubField(i);

        if (subField != FIT_NULL)
        {
            for (int j = 0; j < (int) subField->numMaps; j++)
            {
                const Field* refField = GetField(subField->maps[j].refFieldNum);

                if (refField != FIT_NULL)
                {
                    FIT_FLOAT64 refValue = refField->GetFLOAT64Value(0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
                    refValue += ((refValue >= 0.0) ? (0.5) : (-0.5));
                    if ((FIT_SINT32)refValue == subField->maps[j].refFieldValue)
                        return ((FIT_UINT16) i);
                }
            }
        }
    }

    return FIT_SUBFIELD_INDEX_MAIN_FIELD;
}

FIT_UINT16 Mesg::GetActiveSubFieldIndex(const FIT_UINT8 fieldNum) const
{
    const Profile::FIELD* field = Profile::GetField(GetNum(), fieldNum);

    if (field == FIT_NULL)
        return FIT_SUBFIELD_INDEX_MAIN_FIELD;

    for (int i = 0; i < (int) field->numSubFields; i++)
    {
        const Profile::SUBFIELD& subField = field->subFields[i];

        for (int j = 0; j < (int) subField.numMaps; j++)
        {
            const Field* refField = GetField(subField.maps[j].refFieldNum);

            if (refField != FIT_NULL)
            {
                FIT_FLOAT64 refValue = refField->GetFLOAT64Value(0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
                refValue += ((refValue >= 0.0) ? (0.5) : (-0.5));
                if ((FIT_SINT32)refValue == subField.maps[j].refFieldValue)
                    return ((FIT_UINT16) i);
            }
        }
    }

    return FIT_SUBFIELD_INDEX_MAIN_FIELD;
}

FIT_UINT8 Mesg::GetFieldNumValues(const FIT_UINT8 fieldNum, const FIT_UINT16 subFieldIndex) const
{
    const Field *field = GetField(fieldNum);

    if ((field == FIT_NULL) || (!CanSupportSubField(fieldNum, subFieldIndex)))
        return 0;

    return field->GetNumValues();
}

FIT_ENUM Mesg::GetFieldENUMValue(const FIT_UINT8 fieldNum, const FIT_UINT8 fieldArrayIndex, const FIT_UINT16 subFieldIndex) const
{
    const Field *field = GetField(fieldNum);

    if ((field == FIT_NULL) || (!CanSupportSubField(fieldNum, subFieldIndex)))
        return FIT_ENUM_INVALID;

    return field->GetENUMValue(fieldArrayIndex);
}

FIT_BYTE Mesg::GetFieldBYTEValue(const FIT_UINT8 fieldNum, const FIT_UINT8 fieldArrayIndex, const FIT_UINT16 subFieldIndex) const
{
    const Field *field = GetField(fieldNum);

    if ((field == FIT_NULL) || (!CanSupportSubField(fieldNum, subFieldIndex)))
        return FIT_BYTE_INVALID;

    return field->GetBYTEValue(fieldArrayIndex);
}

FIT_SINT8 Mesg::GetFieldSINT8Value(const FIT_UINT8 fieldNum, const FIT_UINT8 fieldArrayIndex, const FIT_UINT16 subFieldIndex) const
{
    const Field *field = GetField(fieldNum);

    if ((field == FIT_NULL) || (!CanSupportSubField(fieldNum, subFieldIndex)))
        return FIT_SINT8_INVALID;

    return field->GetSINT8Value(fieldArrayIndex);
}

FIT_UINT8 Mesg::GetFieldUINT8Value(const FIT_UINT8 fieldNum, const FIT_UINT8 fieldArrayIndex, const FIT_UINT16 subFieldIndex) const
{
    const Field *field = GetField(fieldNum);

    if ((field == FIT_NULL) || (!CanSupportSubField(fieldNum, subFieldIndex)))
        return FIT_UINT8_INVALID;

    return field->GetUINT8Value(fieldArrayIndex);
}

FIT_UINT8Z Mesg::GetFieldUINT8ZValue(const FIT_UINT8 fieldNum, const FIT_UINT8 fieldArrayIndex, const FIT_UINT16 subFieldIndex) const
{
    const Field *field = GetField(fieldNum);

    if ((field == FIT_NULL) || (!CanSupportSubField(fieldNum, subFieldIndex)))
        return FIT_UINT8Z_INVALID;

    return field->GetUINT8ZValue(fieldArrayIndex);
}

FIT_SINT16 Mesg::GetFieldSINT16Value(const FIT_UINT8 fieldNum, const FIT_UINT8 fieldArrayIndex, const FIT_UINT16 subFieldIndex) const
{
    const Field *field = GetField(fieldNum);

    if ((field == FIT_NULL) || (!CanSupportSubField(fieldNum, subFieldIndex)))
        return FIT_SINT16_INVALID;

    return field->GetSINT16Value(fieldArrayIndex);
}

FIT_UINT16 Mesg::GetFieldUINT16Value(const FIT_UINT8 fieldNum, const FIT_UINT8 fieldArrayIndex, const FIT_UINT16 subFieldIndex) const
{
    const Field *field = GetField(fieldNum);

    if ((field == FIT_NULL) || (!CanSupportSubField(fieldNum, subFieldIndex)))
        return FIT_UINT16_INVALID;

    return field->GetUINT16Value(fieldArrayIndex);
}

FIT_UINT16Z Mesg::GetFieldUINT16ZValue(const FIT_UINT8 fieldNum, const FIT_UINT8 fieldArrayIndex, const FIT_UINT16 subFieldIndex) const
{
    const Field *field = GetField(fieldNum);

    if ((field == FIT_NULL) || (!CanSupportSubField(fieldNum, subFieldIndex)))
        return FIT_UINT16Z_INVALID;

    return field->GetUINT16ZValue(fieldArrayIndex);
}

FIT_SINT32 Mesg::GetFieldSINT32Value(const FIT_UINT8 fieldNum, const FIT_UINT8 fieldArrayIndex, const FIT_UINT16 subFieldIndex) const
{
    const Field *field = GetField(fieldNum);

    if ((field == FIT_NULL) || (!CanSupportSubField(fieldNum, subFieldIndex)))
        return FIT_SINT32_INVALID;

    return field->GetSINT32Value(fieldArrayIndex);
}

FIT_UINT32 Mesg::GetFieldUINT32Value(const FIT_UINT8 fieldNum, const FIT_UINT8 fieldArrayIndex, const FIT_UINT16 subFieldIndex) const
{
    const Field *field = GetField(fieldNum);

    if ((field == FIT_NULL) || (!CanSupportSubField(fieldNum, subFieldIndex)))
        return FIT_UINT32_INVALID;

    return field->GetUINT32Value(fieldArrayIndex);
}

FIT_UINT32Z Mesg::GetFieldUINT32ZValue(const FIT_UINT8 fieldNum, const FIT_UINT8 fieldArrayIndex, const FIT_UINT16 subFieldIndex) const
{
    const Field *field = GetField(fieldNum);

    if ((field == FIT_NULL) || (!CanSupportSubField(fieldNum, subFieldIndex)))
        return FIT_UINT32Z_INVALID;

    return field->GetUINT32ZValue(fieldArrayIndex);
}

FIT_SINT64 Mesg::GetFieldSINT64Value(const FIT_UINT8 fieldNum, const FIT_UINT8 fieldArrayIndex, const FIT_UINT16 subFieldIndex) const
{
    const Field *field = GetField(fieldNum);

    if ((field == FIT_NULL) || (!CanSupportSubField(fieldNum, subFieldIndex)))
        return FIT_SINT64_INVALID;

    return field->GetSINT64Value(fieldArrayIndex);
}

FIT_UINT64 Mesg::GetFieldUINT64Value(const FIT_UINT8 fieldNum, const FIT_UINT8 fieldArrayIndex, const FIT_UINT16 subFieldIndex) const
{
    const Field *field = GetField(fieldNum);

    if ((field == FIT_NULL) || (!CanSupportSubField(fieldNum, subFieldIndex)))
        return FIT_UINT64_INVALID;

    return field->GetUINT64Value(fieldArrayIndex);
}

FIT_UINT64Z Mesg::GetFieldUINT64ZValue(const FIT_UINT8 fieldNum, const FIT_UINT8 fieldArrayIndex, const FIT_UINT16 subFieldIndex) const
{
    const Field *field = GetField(fieldNum);

    if ((field == FIT_NULL) || (!CanSupportSubField(fieldNum, subFieldIndex)))
        return FIT_UINT64Z_INVALID;

    return field->GetUINT64ZValue(fieldArrayIndex);
}

FIT_FLOAT32 Mesg::GetFieldFLOAT32Value(const FIT_UINT8 fieldNum, const FIT_UINT8 fieldArrayIndex, const FIT_UINT16 subFieldIndex) const
{
    const Field *field = GetField(fieldNum);

    if ((field == FIT_NULL) || (!CanSupportSubField(fieldNum, subFieldIndex)))
        return FIT_FLOAT32_INVALID;

    if (subFieldIndex == FIT_SUBFIELD_INDEX_ACTIVE_SUBFIELD)
        return field->GetFLOAT32Value(fieldArrayIndex, GetActiveSubFieldIndex(fieldNum));
    else
        return field->GetFLOAT32Value(fieldArrayIndex, subFieldIndex);
}

FIT_FLOAT64 Mesg::GetFieldFLOAT64Value(const FIT_UINT8 fieldNum, const FIT_UINT8 fieldArrayIndex, const FIT_UINT16 subFieldIndex) const
{
    const Field *field = GetField(fieldNum);

    if ((field == FIT_NULL) || (!CanSupportSubField(fieldNum, subFieldIndex)))
        return FIT_FLOAT64_INVALID;

    if (subFieldIndex == FIT_SUBFIELD_INDEX_ACTIVE_SUBFIELD)
        return field->GetFLOAT64Value(fieldArrayIndex, GetActiveSubFieldIndex(fieldNum));
    else
        return field->GetFLOAT64Value(fieldArrayIndex, subFieldIndex);
}

FIT_WSTRING Mesg::GetFieldSTRINGValue(const FIT_UINT8 fieldNum, const FIT_UINT8 fieldArrayIndex, const FIT_UINT16 subFieldIndex) const
{
    const Field *field = GetField(fieldNum);

    if ((field == FIT_NULL) || (!CanSupportSubField(fieldNum, subFieldIndex)))
        return FIT_WSTRING_INVALID;

    if (subFieldIndex == FIT_SUBFIELD_INDEX_ACTIVE_SUBFIELD)
        return field->GetSTRINGValue(fieldArrayIndex, GetActiveSubFieldIndex(fieldNum));
    else
        return field->GetSTRINGValue(fieldArrayIndex, subFieldIndex);
}

void Mesg::SetFieldENUMValue(const FIT_UINT8 fieldNum, const FIT_ENUM value, const FIT_UINT8 fieldArrayIndex, const FIT_UINT16 subFieldIndex)
{
    if (!CanSupportSubField(fieldNum, subFieldIndex))
        return;

    Field *field = AddField(fieldNum);

    if (field == FIT_NULL)
        return;

    return field->SetENUMValue(value, fieldArrayIndex);
}

void Mesg::SetFieldBYTEValue(const FIT_UINT8 fieldNum, const FIT_BYTE value, const FIT_UINT8 fieldArrayIndex, const FIT_UINT16 subFieldIndex)
{
    if (!CanSupportSubField(fieldNum, subFieldIndex))
        return;

    Field *field = AddField(fieldNum);

    if (field == FIT_NULL)
        return;

    return field->SetBYTEValue(value, fieldArrayIndex);
}

void Mesg::SetFieldSINT8Value(const FIT_UINT8 fieldNum, const FIT_SINT8 value, const FIT_UINT8 fieldArrayIndex, const FIT_UINT16 subFieldIndex)
{
    if (!CanSupportSubField(fieldNum, subFieldIndex))
        return;

    Field *field = AddField(fieldNum);

    if (field == FIT_NULL)
        return;

    return field->SetSINT8Value(value, fieldArrayIndex);
}

void Mesg::SetFieldUINT8Value(const FIT_UINT8 fieldNum, const FIT_UINT8 value, const FIT_UINT8 fieldArrayIndex, const FIT_UINT16 subFieldIndex)
{
    if (!CanSupportSubField(fieldNum, subFieldIndex))
        return;

    Field *field = AddField(fieldNum);

    if (field == FIT_NULL)
        return;

    return field->SetUINT8Value(value, fieldArrayIndex);
}

void Mesg::SetFieldUINT8ZValue(const FIT_UINT8 fieldNum, const FIT_UINT8 value, const FIT_UINT8 fieldArrayIndex, const FIT_UINT16 subFieldIndex)
{
    if (!CanSupportSubField(fieldNum, subFieldIndex))
        return;

    Field *field = AddField(fieldNum);

    if (field == FIT_NULL)
        return;

    return field->SetUINT8ZValue(value, fieldArrayIndex);
}

void Mesg::SetFieldSINT16Value(const FIT_UINT8 fieldNum, const FIT_SINT16 value, const FIT_UINT8 fieldArrayIndex, const FIT_UINT16 subFieldIndex)
{
    if (!CanSupportSubField(fieldNum, subFieldIndex))
        return;

    Field *field = AddField(fieldNum);

    if (field == FIT_NULL)
        return;

    return field->SetSINT16Value(value, fieldArrayIndex);
}

void Mesg::SetFieldUINT16Value(const FIT_UINT8 fieldNum, const FIT_UINT16 value, const FIT_UINT8 fieldArrayIndex, const FIT_UINT16 subFieldIndex)
{
    if (!CanSupportSubField(fieldNum, subFieldIndex))
        return;

    Field *field = AddField(fieldNum);

    if (field == FIT_NULL)
        return;

    return field->SetUINT16Value(value, fieldArrayIndex);
}

void Mesg::SetFieldUINT16ZValue(const FIT_UINT8 fieldNum, const FIT_UINT16Z value, const FIT_UINT8 fieldArrayIndex, const FIT_UINT16 subFieldIndex)
{
    if (!CanSupportSubField(fieldNum, subFieldIndex))
        return;

    Field *field = AddField(fieldNum);

    if (field == FIT_NULL)
        return;

    return field->SetUINT16ZValue(value, fieldArrayIndex);
}

void Mesg::SetFieldSINT32Value(const FIT_UINT8 fieldNum, const FIT_SINT32 value, const FIT_UINT8 fieldArrayIndex, const FIT_UINT16 subFieldIndex)
{
    if (!CanSupportSubField(fieldNum, subFieldIndex))
        return;

    Field *field = AddField(fieldNum);

    if (field == FIT_NULL)
        return;

    return field->SetSINT32Value(value, fieldArrayIndex);
}

void Mesg::SetFieldUINT32Value(const FIT_UINT8 fieldNum, const FIT_UINT32 value, const FIT_UINT8 fieldArrayIndex, const FIT_UINT16 subFieldIndex)
{
    if (!CanSupportSubField(fieldNum, subFieldIndex))
        return;

    Field *field = AddField(fieldNum);

    if (field == FIT_NULL)
        return;

    return field->SetUINT32Value(value, fieldArrayIndex);
}

void Mesg::SetFieldUINT32ZValue(const FIT_UINT8 fieldNum, const FIT_UINT32Z value, const FIT_UINT8 fieldArrayIndex, const FIT_UINT16 subFieldIndex)
{
    if (!CanSupportSubField(fieldNum, subFieldIndex))
        return;

    Field *field = AddField(fieldNum);

    if (field == FIT_NULL)
        return;

    return field->SetUINT32ZValue(value, fieldArrayIndex);
}

void Mesg::SetFieldSINT64Value(const FIT_UINT8 fieldNum, const FIT_SINT64 value, const FIT_UINT8 fieldArrayIndex, const FIT_UINT16 subFieldIndex)
{
    if (!CanSupportSubField(fieldNum, subFieldIndex))
        return;

    Field *field = AddField(fieldNum);

    if (field == FIT_NULL)
        return;

    return field->SetSINT64Value(value, fieldArrayIndex);
}

void Mesg::SetFieldUINT64Value(const FIT_UINT8 fieldNum, const FIT_UINT64 value, const FIT_UINT8 fieldArrayIndex, const FIT_UINT16 subFieldIndex)
{
    if (!CanSupportSubField(fieldNum, subFieldIndex))
        return;

    Field *field = AddField(fieldNum);

    if (field == FIT_NULL)
        return;

    return field->SetUINT64Value(value, fieldArrayIndex);
}

void Mesg::SetFieldUINT64ZValue(const FIT_UINT8 fieldNum, const FIT_UINT64Z value, const FIT_UINT8 fieldArrayIndex, const FIT_UINT16 subFieldIndex)
{
    if (!CanSupportSubField(fieldNum, subFieldIndex))
        return;

    Field *field = AddField(fieldNum);

    if (field == FIT_NULL)
        return;

    return field->SetUINT64ZValue(value, fieldArrayIndex);
}

void Mesg::SetFieldFLOAT32Value(const FIT_UINT8 fieldNum, const FIT_FLOAT32 value, const FIT_UINT8 fieldArrayIndex, const FIT_UINT16 subFieldIndex)
{
    if (!CanSupportSubField(fieldNum, subFieldIndex))
        return;

    Field *field = AddField(fieldNum);

    if (field == FIT_NULL)
        return;

    if (subFieldIndex == FIT_SUBFIELD_INDEX_ACTIVE_SUBFIELD)
        return field->SetFLOAT32Value(value, fieldArrayIndex, GetActiveSubFieldIndex(fieldNum));
    else
        return field->SetFLOAT32Value(value, fieldArrayIndex, subFieldIndex);
}

void Mesg::SetFieldFLOAT64Value(const FIT_UINT8 fieldNum, const FIT_FLOAT64 value, const FIT_UINT8 fieldArrayIndex, const FIT_UINT16 subFieldIndex)
{
    if (!CanSupportSubField(fieldNum, subFieldIndex))
        return;

    Field *field = AddField(fieldNum);

    if (field == FIT_NULL)
        return;

    if (subFieldIndex == FIT_SUBFIELD_INDEX_ACTIVE_SUBFIELD)
        return field->SetFLOAT64Value(value, fieldArrayIndex, GetActiveSubFieldIndex(fieldNum));
    else
        return field->SetFLOAT64Value(value, fieldArrayIndex, subFieldIndex);
}

void Mesg::SetFieldSTRINGValue(const FIT_UINT8 fieldNum, const FIT_WSTRING& value, const FIT_UINT8 fieldArrayIndex)
{
    Field *field = AddField(fieldNum);

    if (field == FIT_NULL)
        return;

    return field->SetSTRINGValue(value, fieldArrayIndex);
}

std::vector<const FieldBase*> Mesg::GetOverrideFields(const FIT_UINT8 fieldNum) const
{
    std::vector<const FieldBase*> vector;

    const FieldBase* nativeField = GetField(fieldNum);

    if (FIT_NULL != nativeField)
    {
        vector.emplace_back(nativeField);
    }

    for (FIT_UINT32 i = 0; i < devFields.size(); i++)
    {
        const DeveloperField& devField = devFields[i];
        if (devField.GetNativeOverride() == fieldNum)
        {
            vector.emplace_back(&devField);
        }
    }

    return vector;
}

void Mesg::RemoveExpandedFields(void)
{
    fields.erase(std::remove_if(fields.begin(),
                                fields.end(),
                                [](Field& field){return field.GetIsExpanded();}),
                 fields.end());
}

int Mesg::WriteField(std::ostream& file, const FieldBase* field, FIT_UINT8 defSize, FIT_UINT8 defType)
{
    FIT_UINT8 fieldSize = 0;

    if (field != FIT_NULL)
    {
        fieldSize = field->Write(file);

        if (fieldSize == 0)
            return 0;
    }

    if (fieldSize < defSize)
    {
        FIT_UINT8 baseTypeNum = (defType & FIT_BASE_TYPE_NUM_MASK);

        if (baseTypeNum < FIT_BASE_TYPES)
        {
            FIT_UINT8 baseTypeByteIndex = (fieldSize % (baseTypeSizes[baseTypeNum]));
            FIT_UINT8 numBytesRemaining = (defSize) - fieldSize;

            while (numBytesRemaining--)
            {
                file.put(*(baseTypeInvalids[baseTypeNum] + baseTypeByteIndex));

                if ((++baseTypeByteIndex) >= baseTypeSizes[baseTypeNum])
                    baseTypeByteIndex = 0;

                fieldSize++;
            }
        }
        else
        {
            return 0;
            // Do not continue if base type not supported.
        }
    }

    return fieldSize;
}
} // namespace fit
