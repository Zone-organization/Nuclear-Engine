#pragma once
#define ACCESSOR(datatype, name, return_value) \
datatype Get##name() {return return_value; } \
void Set##name(datatype value) { return_value = value; )

#define ACCESSOR_DEC(datatype, name) \
datatype Get##name(); \
void Set##name(datatype value);


#define ACCESSOR_DEF(datatype, name, class_name, return_value) \
datatype class_name::Get##name() {return return_value; } \
void class_name::Set##name(datatype value) { return_value = value; )


#define ACCESSOR_DEC_REF(datatype, name) \
datatype Get##name(); \
void Set##name(const datatype& value);