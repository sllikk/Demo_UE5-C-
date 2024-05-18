// Fill out your copyright notice in the Description page of Project Settings.

#include "Property/CustomDamage.h"

DEFINE_LOG_CATEGORY(LogCustomDamage)

UCustomDamage::UCustomDamage(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), DamageType()
{
	InitialDamageType();

}

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------

void UCustomDamage::InitialDamageType()
{
	DataType.Add(EDamageType::DMG_BURN,			FDamageTypeData{1, 1});
	DataType.Add(EDamageType::DMG_FALL,			FDamageTypeData{0, 0});
	DataType.Add(EDamageType::DMG_CRUSH,			FDamageTypeData{0, 0});
	DataType.Add(EDamageType::DMG_DROWN,			FDamageTypeData{0, 0});
	DataType.Add(EDamageType::DMG_BULLET,			FDamageTypeData{0, 0});
	DataType.Add(EDamageType::DMG_SLASH,			FDamageTypeData{0, 0});
	DataType.Add(EDamageType::DMG_POISON,			FDamageTypeData{0, 0});
	DataType.Add(EDamageType::DMG_ELECTRO,			FDamageTypeData{0, 0});
	DataType.Add(EDamageType::DMG_EXPLODE,			FDamageTypeData{1, 0});
	DataType.Add(EDamageType::DMG_GENERIC,			FDamageTypeData{0, 0});
	DataType.Add(EDamageType::DMG_CHEMICAL,		FDamageTypeData{0, 0});
	DataType.Add(EDamageType::DMG_VEHICLE,			FDamageTypeData{0, 0});
	DataType.Add(EDamageType::DMG_NERVEGAS,		FDamageTypeData{0, 0});
	DataType.Add(EDamageType::DMG_PARALYZE,		FDamageTypeData{0, 0});
	DataType.Add(EDamageType::DMG_SLOWBURN,		FDamageTypeData{0, 0});
	DataType.Add(EDamageType::DMG_RADIATION,		FDamageTypeData{0, 0});
	DataType.Add(EDamageType::DMG_SKIP_ARMOR,		FDamageTypeData{0, 0});
	DataType.Add(EDamageType::DMG_SKIP_RAGDOLL,    FDamageTypeData{0, 0});
	DataType.Add(EDamageType::DMG_ENERGYBEAM,      FDamageTypeData{0, 0});
	DataType.Add(EDamageType::DMG_DROWNRECOVER,    FDamageTypeData{0, 0});
}


FDamageTypeData UCustomDamage::GetDamageTypeData() const
{
	const FDamageTypeData* Data = DataType.Find(DamageType);
	if (Data != nullptr)
	{
		UE_LOG(LogCustomDamage, Log, TEXT("Damage type data found for %s"), *UEnum::GetValueAsString(DamageType));
		return *Data;
	}
	else
	{
		UE_LOG(LogCustomDamage, Warning, TEXT("No damage type data found for %s"), *UEnum::GetValueAsString(DamageType));
		return FDamageTypeData();
	}
	
}

EDamageType UCustomDamage::GetCurrentDamageType() const
{
	return DamageType;
}

void UCustomDamage::SetCurrentDamageType(EDamageType TypeDamage)
{
	DamageType = TypeDamage;

	UE_LOG(LogCustomDamage, Warning, TEXT("Damage type set to %s"), *UEnum::GetValueAsString(TypeDamage));

}

