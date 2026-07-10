// Copyright (c) 2026 Krileon. All Rights Reserved.

#include "GameUserSettingsEnhancedBlueprint.h"

#include "Misc/ConfigCacheIni.h"
#include "UObject/UnrealType.h"
#include "HAL/IConsoleManager.h"

void UGameUserSettingsEnhancedBlueprint::LoadSettings( bool bForceReload )
{
	Super::LoadSettings( bForceReload );

	LoadBlueprintSettings();

	ApplyBlueprintSettings();

	OnLoadBlueprintSettings();
}

void UGameUserSettingsEnhancedBlueprint::SaveSettings()
{
	OnSaveBlueprintSettings();

	SaveBlueprintSettings();

	Super::SaveSettings();
}

void UGameUserSettingsEnhancedBlueprint::ApplyNonResolutionSettings()
{
	Super::ApplyNonResolutionSettings();

	ApplyBlueprintSettings();

	OnApplyBlueprintSettings();
}

void UGameUserSettingsEnhancedBlueprint::SetToDefaults()
{
 	Super::SetToDefaults();

	ResetBlueprintSettings();
}

void UGameUserSettingsEnhancedBlueprint::ResetToCurrentSettings()
{
 	Super::ResetToCurrentSettings();

	LoadBlueprintSettings();
}

bool UGameUserSettingsEnhancedBlueprint::IsDirty() const
{
	if ( Super::IsDirty() ) {
		return true;
	}

	return IsBlueprintDirty();
}

void UGameUserSettingsEnhancedBlueprint::ValidateSettings()
{
 	Super::ValidateSettings();

	ValidateBlueprintSettings();
}

void UGameUserSettingsEnhancedBlueprint::SaveBlueprintSettings()
{
	if ( ! GConfig ) {
		return;
	}

	UClass* MyClass = GetClass();

	if ( ! MyClass->HasAnyClassFlags( CLASS_CompiledFromBlueprint ) ) {
		return;
	}

	for ( TFieldIterator<FProperty> PropIt( MyClass ); PropIt; ++PropIt ) {
		FProperty* Property = *PropIt;

		if ( ! Property->GetOwnerClass()->HasAnyClassFlags( CLASS_CompiledFromBlueprint ) ) {
			continue;
		}

		if ( ! Property->HasAnyPropertyFlags( CPF_Edit ) ) {
			continue;
		}

		FString Section = TEXT( "BlueprintSettings" );
		FString ConfigKey = Property->GetName().Replace( TEXT( " " ), TEXT( "" ) );
		const void* ValuePtr = Property->ContainerPtrToValuePtr<void>( this );

		if ( FBoolProperty* BoolProp = CastField<FBoolProperty>( Property ) ) {
			bool CurrentValue = BoolProp->GetPropertyValue( ValuePtr );

			GConfig->SetBool( *Section, *ConfigKey, CurrentValue, GGameUserSettingsIni );
		} else if ( FIntProperty* IntProp = CastField<FIntProperty>( Property ) ) {
			int32 CurrentValue = IntProp->GetPropertyValue( ValuePtr );

			GConfig->SetInt( *Section, *ConfigKey, CurrentValue, GGameUserSettingsIni );
		} else if ( FFloatProperty* FloatProp = CastField<FFloatProperty>( Property ) ) {
			float CurrentValue = FloatProp->GetPropertyValue( ValuePtr );

			GConfig->SetFloat( *Section, *ConfigKey, CurrentValue, GGameUserSettingsIni );
		} else if ( FDoubleProperty* DoubleProp = CastField<FDoubleProperty>( Property ) ) {
			double CurrentValue = DoubleProp->GetPropertyValue( ValuePtr );

			GConfig->SetFloat( *Section, *ConfigKey, static_cast<float>( CurrentValue ), GGameUserSettingsIni );
		} else if ( FNameProperty* NameProp = CastField<FNameProperty>( Property ) ) {
			FName CurrentValue = NameProp->GetPropertyValue( ValuePtr );

			GConfig->SetString( *Section, *ConfigKey, *CurrentValue.ToString(), GGameUserSettingsIni );
		} else if ( FEnumProperty* EnumProp = CastField<FEnumProperty>( Property ) ) {
			int64 CurrentValue = EnumProp->GetUnderlyingProperty()->GetSignedIntPropertyValue( ValuePtr );

			GConfig->SetInt( *Section, *ConfigKey, static_cast<int32>( CurrentValue ), GGameUserSettingsIni );
		} else if ( FStrProperty* StrProp = CastField<FStrProperty>( Property ) ) {
			FString CurrentValue = StrProp->GetPropertyValue( ValuePtr );

			GConfig->SetString( *Section, *ConfigKey, *CurrentValue, GGameUserSettingsIni );
		} else if ( FByteProperty* ByteProp = CastField<FByteProperty>( Property ) ) {
			uint8 CurrentValue = ByteProp->GetPropertyValue( ValuePtr );

			GConfig->SetInt( *Section, *ConfigKey, static_cast<int32>( CurrentValue ), GGameUserSettingsIni );
		} else if ( FInt64Property* Int64Prop = CastField<FInt64Property>( Property ) ) {
			int64 CurrentValue = Int64Prop->GetPropertyValue( ValuePtr );

			GConfig->SetString( *Section, *ConfigKey, *FString::Printf( TEXT( "%lld" ), CurrentValue ), GGameUserSettingsIni );
		} else if ( FTextProperty* TextProp = CastField<FTextProperty>( Property ) ) {
			FText CurrentValue = TextProp->GetPropertyValue( ValuePtr );

			GConfig->SetString( *Section, *ConfigKey, *CurrentValue.ToString(), GGameUserSettingsIni );
		} else if ( FStructProperty* StructProp = CastField<FStructProperty>( Property ) ) {
			FString CurrentValue;

			StructProp->ExportTextItem_Direct( CurrentValue, ValuePtr, nullptr, this, PPF_None, nullptr );

			GConfig->SetString( *Section, *ConfigKey, *CurrentValue, GGameUserSettingsIni );
		} else if ( FObjectProperty* ObjProp = CastField<FObjectProperty>( Property ) ) {
			FString CurrentValue;

			ObjProp->ExportTextItem_Direct( CurrentValue, ValuePtr, nullptr, this, PPF_None, nullptr );

			GConfig->SetString( *Section, *ConfigKey, *CurrentValue, GGameUserSettingsIni );
		} else if ( FSoftObjectProperty* SoftObjProp = CastField<FSoftObjectProperty>( Property ) ) {
			FString CurrentValue;

			SoftObjProp->ExportTextItem_Direct( CurrentValue, ValuePtr, nullptr, this, PPF_None, nullptr );

			GConfig->SetString( *Section, *ConfigKey, *CurrentValue, GGameUserSettingsIni );
		} else if ( FArrayProperty* ArrayProp = CastField<FArrayProperty>( Property ) ) {
			FString CurrentValue;

			ArrayProp->ExportTextItem_Direct( CurrentValue, ValuePtr, nullptr, this, PPF_None, nullptr );

			GConfig->SetString( *Section, *ConfigKey, *CurrentValue, GGameUserSettingsIni );
		} else if ( FMapProperty* MapProp = CastField<FMapProperty>( Property ) ) {
			FString CurrentValue;

			MapProp->ExportTextItem_Direct( CurrentValue, ValuePtr, nullptr, this, PPF_None, nullptr );

			GConfig->SetString( *Section, *ConfigKey, *CurrentValue, GGameUserSettingsIni );
		} else if ( FSetProperty* SetProp = CastField<FSetProperty>( Property ) ) {
			FString CurrentValue;

			SetProp->ExportTextItem_Direct( CurrentValue, ValuePtr, nullptr, this, PPF_None, nullptr );

			GConfig->SetString( *Section, *ConfigKey, *CurrentValue, GGameUserSettingsIni );
		}
	}
}

void UGameUserSettingsEnhancedBlueprint::LoadBlueprintSettings()
{
	if ( ! GConfig ) {
		return;
	}

	UClass* MyClass = GetClass();

	if ( ! MyClass->HasAnyClassFlags( CLASS_CompiledFromBlueprint ) ) {
		return;
	}

	for ( TFieldIterator<FProperty> PropIt( MyClass ); PropIt; ++PropIt ) {
		FProperty* Property = *PropIt;

		if ( ! Property->GetOwnerClass()->HasAnyClassFlags( CLASS_CompiledFromBlueprint ) ) {
			continue;
		}

		if ( ! Property->HasAnyPropertyFlags( CPF_Edit ) ) {
			continue;
		}

		FString Section = TEXT( "BlueprintSettings" );
		FString ConfigKey = Property->GetName().Replace( TEXT( " " ), TEXT( "" ) );
		void* ValuePtr = Property->ContainerPtrToValuePtr<void>( this );

		if ( FBoolProperty* BoolProp = CastField<FBoolProperty>( Property ) ) {
			bool ConfigValue = false;

			if ( ! GConfig->GetBool( *Section, *ConfigKey, ConfigValue, GGameUserSettingsIni ) ) {
				continue;
			}

			BoolProp->SetPropertyValue( ValuePtr, ConfigValue );
		} else if ( FIntProperty* IntProp = CastField<FIntProperty>( Property ) ) {
			int32 ConfigValue = 0;

			if ( ! GConfig->GetInt( *Section, *ConfigKey, ConfigValue, GGameUserSettingsIni ) ) {
				continue;
			}

			IntProp->SetPropertyValue( ValuePtr, ConfigValue );
		} else if ( FFloatProperty* FloatProp = CastField<FFloatProperty>( Property ) ) {
			float ConfigValue = 0.0f;

			if ( ! GConfig->GetFloat( *Section, *ConfigKey, ConfigValue, GGameUserSettingsIni ) ) {
				continue;
			}

			FloatProp->SetPropertyValue( ValuePtr, ConfigValue );
		} else if ( FDoubleProperty* DoubleProp = CastField<FDoubleProperty>( Property ) ) {
			float ConfigValue = 0.0f; 

			if ( ! GConfig->GetFloat( *Section, *ConfigKey, ConfigValue, GGameUserSettingsIni ) ) {
				continue;
			}

			DoubleProp->SetPropertyValue( ValuePtr, static_cast<double>( ConfigValue ) );
		} else if ( FNameProperty* NameProp = CastField<FNameProperty>( Property ) ) {
			FString ConfigValue;

			if ( ! GConfig->GetString( *Section, *ConfigKey, ConfigValue, GGameUserSettingsIni ) ) {
				continue;
			}

			NameProp->SetPropertyValue( ValuePtr, FName( *ConfigValue ) );
		} else if ( FEnumProperty* EnumProp = CastField<FEnumProperty>( Property ) ) {
			int32 ConfigValue = 0;

			if ( ! GConfig->GetInt( *Section, *ConfigKey, ConfigValue, GGameUserSettingsIni ) ) {
				continue;
			}

			EnumProp->GetUnderlyingProperty()->SetIntPropertyValue( ValuePtr, static_cast<int64>( ConfigValue ) );
		} else if ( FStrProperty* StrProp = CastField<FStrProperty>( Property ) ) {
			FString ConfigValue;

			if ( ! GConfig->GetString( *Section, *ConfigKey, ConfigValue, GGameUserSettingsIni ) ) {
				continue;
			}

			StrProp->SetPropertyValue( ValuePtr, ConfigValue );
		} else if ( FByteProperty* ByteProp = CastField<FByteProperty>( Property ) ) {
			int32 ConfigValue = 0;

			if ( ! GConfig->GetInt( *Section, *ConfigKey, ConfigValue, GGameUserSettingsIni ) ) {
				continue;
			}

			ByteProp->SetPropertyValue( ValuePtr, static_cast<uint8>( ConfigValue ) );
		} else if ( FInt64Property* Int64Prop = CastField<FInt64Property>( Property ) ) {
			FString ConfigValue;

			if ( GConfig->GetString( *Section, *ConfigKey, ConfigValue, GGameUserSettingsIni ) ) {
				continue;
			}

			Int64Prop->SetPropertyValue( ValuePtr, FCString::Atoi64( *ConfigValue ) );
		} else if ( FTextProperty* TextProp = CastField<FTextProperty>( Property ) ) {
			FString ConfigValue;

			if ( ! GConfig->GetString( *Section, *ConfigKey, ConfigValue, GGameUserSettingsIni ) ) {
				continue;
			}

			TextProp->SetPropertyValue( ValuePtr, FText::FromString( ConfigValue ) );
		} else if ( FStructProperty* StructProp = CastField<FStructProperty>( Property ) ) {
			FString ConfigValue;

			if ( ! GConfig->GetString( *Section, *ConfigKey, ConfigValue, GGameUserSettingsIni ) ) {
				continue;
			}

			StructProp->ImportText_Direct( *ConfigValue, ValuePtr, this, PPF_None, nullptr );
		} else if ( FObjectProperty* ObjProp = CastField<FObjectProperty>( Property ) ) {
			FString ConfigValue;

			if ( ! GConfig->GetString( *Section, *ConfigKey, ConfigValue, GGameUserSettingsIni ) ) {
				continue;
			}

			ObjProp->ImportText_Direct( *ConfigValue, ValuePtr, this, PPF_None, nullptr );
		} else if ( FSoftObjectProperty* SoftObjProp = CastField<FSoftObjectProperty>( Property ) ) {
			FString ConfigValue;

			if ( ! GConfig->GetString( *Section, *ConfigKey, ConfigValue, GGameUserSettingsIni ) ) {
				continue;
			}

			SoftObjProp->ImportText_Direct( *ConfigValue, ValuePtr, this, PPF_None, nullptr );
		} else if ( FArrayProperty* ArrayProp = CastField<FArrayProperty>( Property ) ) {
			FString ConfigValue;

			if ( ! GConfig->GetString( *Section, *ConfigKey, ConfigValue, GGameUserSettingsIni ) ) {
				continue;
			}

			ArrayProp->ImportText_Direct( *ConfigValue, ValuePtr, this, PPF_None, nullptr );
		} else if ( FMapProperty* MapProp = CastField<FMapProperty>( Property ) ) {
			FString ConfigValue;

			if ( ! GConfig->GetString( *Section, *ConfigKey, ConfigValue, GGameUserSettingsIni ) ) {
				continue;
			}

			MapProp->ImportText_Direct( *ConfigValue, ValuePtr, this, PPF_None, nullptr );
		} else if ( FSetProperty* SetProp = CastField<FSetProperty>( Property ) ) {
			FString ConfigValue;

			if ( ! GConfig->GetString( *Section, *ConfigKey, ConfigValue, GGameUserSettingsIni ) ) {
				continue;
			}

			SetProp->ImportText_Direct( *ConfigValue, ValuePtr, this, PPF_None, nullptr );
		}
	}
}

void UGameUserSettingsEnhancedBlueprint::ResetBlueprintSettings()
{
	UClass* MyClass = GetClass();

	if ( ! MyClass->HasAnyClassFlags( CLASS_CompiledFromBlueprint ) ) {
		return;
	}

	UObject* DefaultObject = MyClass->GetDefaultObject();

	if ( ! DefaultObject ) {
		return;
	}

	for ( TFieldIterator<FProperty> PropIt( MyClass ); PropIt; ++PropIt ) {
		FProperty* Property = *PropIt;

		if ( ! Property->GetOwnerClass()->HasAnyClassFlags( CLASS_CompiledFromBlueprint ) ) {
			continue;
		}

		if ( ! Property->HasAnyPropertyFlags( CPF_Edit ) ) {
			continue;
		}

		if ( CastField<FBoolProperty>( Property )
			|| CastField<FIntProperty>( Property )
			|| CastField<FFloatProperty>( Property )
			|| CastField<FDoubleProperty>( Property )
			|| CastField<FNameProperty>( Property )
			|| CastField<FEnumProperty>( Property )
			|| CastField<FStrProperty>( Property )
			|| CastField<FByteProperty>( Property )
			|| CastField<FInt64Property>( Property )
			|| CastField<FTextProperty>( Property )
			|| CastField<FStructProperty>( Property )
			|| CastField<FObjectProperty>( Property )
			|| CastField<FSoftObjectProperty>( Property )
			|| CastField<FArrayProperty>( Property )
			|| CastField<FMapProperty>( Property )
			|| CastField<FSetProperty>( Property )
		) {
			void* CurrentValuePtr = Property->ContainerPtrToValuePtr<void>( this );
			const void* DefaultValuePtr = Property->ContainerPtrToValuePtr<void>( DefaultObject );

			Property->CopyCompleteValue( CurrentValuePtr, DefaultValuePtr );
		}
	}
}

void UGameUserSettingsEnhancedBlueprint::ApplyBlueprintSettings()
{
	UClass* MyClass = GetClass();

	if ( ! MyClass->HasAnyClassFlags( CLASS_CompiledFromBlueprint ) ) {
		return;
	}

	for ( TFieldIterator<FProperty> PropIt( MyClass ); PropIt; ++PropIt ) {
		FProperty* Property = *PropIt;

		if ( ! Property->GetOwnerClass()->HasAnyClassFlags( CLASS_CompiledFromBlueprint ) ) {
			continue;
		}

		if ( ! Property->HasAnyPropertyFlags( CPF_Edit ) ) {
			continue;
		}

		FString CVarName = ConsoleVariables.FindRef( Property->GetName() );

		if ( CVarName.IsEmpty() ) {
			continue;
		}

		IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable( *CVarName );

		if ( ! CVar ) {
			continue;
		}

		const void* ValuePtr = Property->ContainerPtrToValuePtr<void>( this );

		if ( FBoolProperty* BoolProp = CastField<FBoolProperty>( Property ) ) {
			bool CurrentValue = BoolProp->GetPropertyValue( ValuePtr );

			CVar->Set( CurrentValue, ECVF_SetByGameSetting );
		} else if ( FIntProperty* IntProp = CastField<FIntProperty>( Property ) ) {
			int32 CurrentValue = IntProp->GetPropertyValue( ValuePtr );

			CVar->Set( CurrentValue, ECVF_SetByGameSetting );
		} else if ( FFloatProperty* FloatProp = CastField<FFloatProperty>( Property ) ) {
			float CurrentValue = FloatProp->GetPropertyValue( ValuePtr );

			CVar->Set( CurrentValue, ECVF_SetByGameSetting );
		} else if ( FDoubleProperty* DoubleProp = CastField<FDoubleProperty>( Property ) ) {
			double CurrentValue = DoubleProp->GetPropertyValue( ValuePtr );

			CVar->Set( static_cast<float>( CurrentValue ), ECVF_SetByGameSetting );
		} else if ( FNameProperty* NameProp = CastField<FNameProperty>( Property ) ) {
			FName CurrentValue = NameProp->GetPropertyValue( ValuePtr );

			CVar->Set( *CurrentValue.ToString(), ECVF_SetByGameSetting );
		} else if ( FEnumProperty* EnumProp = CastField<FEnumProperty>( Property ) ) {
			int64 CurrentValue = EnumProp->GetUnderlyingProperty()->GetSignedIntPropertyValue( ValuePtr );

			CVar->Set( static_cast<int32>( CurrentValue ), ECVF_SetByGameSetting );
		} else if ( FStrProperty* StrProp = CastField<FStrProperty>( Property ) ) {
			FString CurrentValue = StrProp->GetPropertyValue( ValuePtr );

			CVar->Set( *CurrentValue, ECVF_SetByGameSetting );
		} else if ( FByteProperty* ByteProp = CastField<FByteProperty>( Property ) ) {
			uint8 CurrentValue = ByteProp->GetPropertyValue( ValuePtr );

			CVar->Set( static_cast<int32>( CurrentValue ), ECVF_SetByGameSetting );
		} else if ( FInt64Property* Int64Prop = CastField<FInt64Property>( Property ) ) {
			int64 CurrentValue = Int64Prop->GetPropertyValue( ValuePtr );

			CVar->Set( *FString::Printf( TEXT( "%lld" ), CurrentValue ), ECVF_SetByGameSetting );
		} else if ( FTextProperty* TextProp = CastField<FTextProperty>( Property ) ) {
			FText CurrentValue = TextProp->GetPropertyValue( ValuePtr );

			CVar->Set( *CurrentValue.ToString(), ECVF_SetByGameSetting );
		} else if ( FStructProperty* StructProp = CastField<FStructProperty>( Property ) ) {
			FString CurrentValue;

			StructProp->ExportTextItem_Direct( CurrentValue, ValuePtr, nullptr, this, PPF_None, nullptr );

			CVar->Set( *CurrentValue, ECVF_SetByGameSetting );
		} else if ( FObjectProperty* ObjProp = CastField<FObjectProperty>( Property ) ) {
			FString CurrentValue;

			ObjProp->ExportTextItem_Direct( CurrentValue, ValuePtr, nullptr, this, PPF_None, nullptr );

			CVar->Set( *CurrentValue, ECVF_SetByGameSetting );
		} else if ( FSoftObjectProperty* SoftObjProp = CastField<FSoftObjectProperty>( Property ) ) {
			FString CurrentValue;

			SoftObjProp->ExportTextItem_Direct( CurrentValue, ValuePtr, nullptr, this, PPF_None, nullptr );

			CVar->Set( *CurrentValue, ECVF_SetByGameSetting );
		} else if ( FArrayProperty* ArrayProp = CastField<FArrayProperty>( Property ) ) {
			FString CurrentValue;

			ArrayProp->ExportTextItem_Direct( CurrentValue, ValuePtr, nullptr, this, PPF_None, nullptr );

			CVar->Set( *CurrentValue, ECVF_SetByGameSetting );
		} else if ( FMapProperty* MapProp = CastField<FMapProperty>( Property ) ) {
			FString CurrentValue;

			MapProp->ExportTextItem_Direct( CurrentValue, ValuePtr, nullptr, this, PPF_None, nullptr );

			CVar->Set( *CurrentValue, ECVF_SetByGameSetting );
		} else if ( FSetProperty* SetProp = CastField<FSetProperty>( Property ) ) {
			FString CurrentValue;

			SetProp->ExportTextItem_Direct( CurrentValue, ValuePtr, nullptr, this, PPF_None, nullptr );

			CVar->Set( *CurrentValue, ECVF_SetByGameSetting );
		}
	}
}

bool UGameUserSettingsEnhancedBlueprint::IsBlueprintDirty() const
{
	if ( ! GConfig ) {
		return false;
	}

	UClass* MyClass = GetClass();

	if ( ! MyClass->HasAnyClassFlags( CLASS_CompiledFromBlueprint ) ) {
		return false;
	}

	for ( TFieldIterator<FProperty> PropIt( MyClass ); PropIt; ++PropIt ) {
		FProperty* Property = *PropIt;

		if ( ! Property->GetOwnerClass()->HasAnyClassFlags( CLASS_CompiledFromBlueprint ) ) {
			continue;
		}

		if ( ! Property->HasAnyPropertyFlags( CPF_Edit ) ) {
			continue;
		}

		FString Section = TEXT( "BlueprintSettings" );
		FString ConfigKey = Property->GetName().Replace( TEXT( " " ), TEXT( "" ) );
		const void* ValuePtr = Property->ContainerPtrToValuePtr<void>( this );

		if ( FBoolProperty* BoolProp = CastField<FBoolProperty>( Property ) ) {
			bool ConfigValue = false;

			if ( ! GConfig->GetBool( *Section, *ConfigKey, ConfigValue, GGameUserSettingsIni ) ) {
				return true;
			}

			bool CurrentValue = BoolProp->GetPropertyValue( ValuePtr );

			if ( CurrentValue != ConfigValue ) {
				return true;
			}
		} else if ( FIntProperty* IntProp = CastField<FIntProperty>( Property ) ) {
			int32 ConfigValue = 0;

			if ( ! GConfig->GetInt( *Section, *ConfigKey, ConfigValue, GGameUserSettingsIni ) ) {
				return true;
			}

			int32 CurrentValue = IntProp->GetPropertyValue( ValuePtr );

			if ( CurrentValue != ConfigValue ) {
				return true;
			}
		} else if ( FFloatProperty* FloatProp = CastField<FFloatProperty>( Property ) ) {
			float ConfigValue = 0.0f;

			if ( ! GConfig->GetFloat( *Section, *ConfigKey, ConfigValue, GGameUserSettingsIni ) ) {
				return true;
			}

			float CurrentValue = FloatProp->GetPropertyValue( ValuePtr );

			if ( ! FMath::IsNearlyEqual( CurrentValue, ConfigValue ) ) {
				return true;
			}
		} else if ( FDoubleProperty* DoubleProp = CastField<FDoubleProperty>( Property ) ) {
			float ConfigValue = 0.0f;

			if ( ! GConfig->GetFloat( *Section, *ConfigKey, ConfigValue, GGameUserSettingsIni ) ) {
				return true;
			}

			double CurrentValue = DoubleProp->GetPropertyValue( ValuePtr );

			if ( ! FMath::IsNearlyEqual( static_cast<float>( CurrentValue ), ConfigValue ) ) {
				return true;
			}
		} else if ( FNameProperty* NameProp = CastField<FNameProperty>( Property ) ) {
			FString ConfigValue;

			if ( ! GConfig->GetString( *Section, *ConfigKey, ConfigValue, GGameUserSettingsIni ) ) {
				return true;
			}

			FName CurrentValue = NameProp->GetPropertyValue( ValuePtr );

			if ( CurrentValue.ToString() != ConfigValue ) {
				return true;
			}
		} else if ( FEnumProperty* EnumProp = CastField<FEnumProperty>( Property ) ) {
			int32 ConfigValue = 0;

			if ( ! GConfig->GetInt( *Section, *ConfigKey, ConfigValue, GGameUserSettingsIni ) ) {
				return true;
			}

			int64 CurrentValue = EnumProp->GetUnderlyingProperty()->GetSignedIntPropertyValue( ValuePtr );

			if ( static_cast<int32>( CurrentValue ) != ConfigValue ) {
				return true;
			}
		} else if ( FStrProperty* StrProp = CastField<FStrProperty>( Property ) ) {
			FString ConfigValue;

			if ( ! GConfig->GetString( *Section, *ConfigKey, ConfigValue, GGameUserSettingsIni ) ) {
				return true;
			}

			FString CurrentValue = StrProp->GetPropertyValue( ValuePtr );

			if ( CurrentValue != ConfigValue ) {
				return true;
			}
		} else if ( FByteProperty* ByteProp = CastField<FByteProperty>( Property ) ) {
			int32 ConfigValue = 0;

			if ( ! GConfig->GetInt( *Section, *ConfigKey, ConfigValue, GGameUserSettingsIni ) ) {
				return true;
			}

			uint8 CurrentValue = ByteProp->GetPropertyValue( ValuePtr );

			if ( static_cast<int32>( CurrentValue ) != ConfigValue ) {
				return true;
			}
		} else if ( FInt64Property* Int64Prop = CastField<FInt64Property>( Property ) ) {
			FString ConfigValue;

			if ( ! GConfig->GetString( *Section, *ConfigKey, ConfigValue, GGameUserSettingsIni ) ) {
				return true;
			}

			int64 CurrentValue = Int64Prop->GetPropertyValue( ValuePtr );

			if ( CurrentValue != FCString::Atoi64( *ConfigValue ) ) {
				return true;
			}
		} else if ( FTextProperty* TextProp = CastField<FTextProperty>( Property ) ) {
			FString ConfigValue;

			if ( ! GConfig->GetString( *Section, *ConfigKey, ConfigValue, GGameUserSettingsIni ) ) {
				return true;
			}

			FText CurrentValue = TextProp->GetPropertyValue( ValuePtr );

			if ( CurrentValue.ToString() != ConfigValue ) {
				return true;
			}
		} else if ( FStructProperty* StructProp = CastField<FStructProperty>( Property ) ) {
			FString ConfigValue;

			if ( ! GConfig->GetString( *Section, *ConfigKey, ConfigValue, GGameUserSettingsIni ) ) {
				return true;
			}

			FString CurrentValue;

			StructProp->ExportTextItem_Direct( CurrentValue, ValuePtr, nullptr, const_cast<UObject*>( static_cast<const UObject*>( this ) ), PPF_None, nullptr );

			if ( CurrentValue != ConfigValue ) {
				return true;
			}
		} else if ( FObjectProperty* ObjProp = CastField<FObjectProperty>( Property ) ) {
			FString ConfigValue;

			if ( ! GConfig->GetString( *Section, *ConfigKey, ConfigValue, GGameUserSettingsIni ) ) {
				return true;
			}

			FString CurrentValue;

			ObjProp->ExportTextItem_Direct( CurrentValue, ValuePtr, nullptr, const_cast<UObject*>( static_cast<const UObject*>( this ) ), PPF_None, nullptr );

			if ( CurrentValue != ConfigValue ) {
				return true;
			}
		} else if ( FSoftObjectProperty* SoftObjProp = CastField<FSoftObjectProperty>( Property ) ) {
			FString ConfigValue;

			if ( ! GConfig->GetString( *Section, *ConfigKey, ConfigValue, GGameUserSettingsIni ) ) {
				return true;
			}

			FString CurrentValue;

			SoftObjProp->ExportTextItem_Direct( CurrentValue, ValuePtr, nullptr, const_cast<UObject*>( static_cast<const UObject*>( this ) ), PPF_None, nullptr );

			if ( CurrentValue != ConfigValue ) {
				return true;
			}
		} else if ( FArrayProperty* ArrayProp = CastField<FArrayProperty>( Property ) ) {
			FString ConfigValue;

			if ( ! GConfig->GetString( *Section, *ConfigKey, ConfigValue, GGameUserSettingsIni ) ) {
				return true;
			}

			FString CurrentValue;

			ArrayProp->ExportTextItem_Direct( CurrentValue, ValuePtr, nullptr, const_cast<UObject*>( static_cast<const UObject*>( this ) ), PPF_None, nullptr );

			if ( CurrentValue != ConfigValue ) {
				return true;
			}
		} else if ( FMapProperty* MapProp = CastField<FMapProperty>( Property ) ) {
			FString ConfigValue;

			if ( ! GConfig->GetString( *Section, *ConfigKey, ConfigValue, GGameUserSettingsIni ) ) {
				return true;
			}

			FString CurrentValue;

			MapProp->ExportTextItem_Direct( CurrentValue, ValuePtr, nullptr, const_cast<UObject*>( static_cast<const UObject*>( this ) ), PPF_None, nullptr );

			if ( CurrentValue != ConfigValue ) {
				return true;
			}
		} else if ( FSetProperty* SetProp = CastField<FSetProperty>( Property ) ) {
			FString ConfigValue;

			if ( ! GConfig->GetString( *Section, *ConfigKey, ConfigValue, GGameUserSettingsIni ) ) {
				return true;
			}

			FString CurrentValue;

			SetProp->ExportTextItem_Direct( CurrentValue, ValuePtr, nullptr, const_cast<UObject*>( static_cast<const UObject*>( this ) ), PPF_None, nullptr );

			if ( CurrentValue != ConfigValue ) {
				return true;
			}
		}
	}

	return false;
}

void UGameUserSettingsEnhancedBlueprint::ValidateBlueprintSettings()
{
	if ( ! GConfig ) {
		return;
	}

	UClass* MyClass = GetClass();

	if ( ! MyClass->HasAnyClassFlags( CLASS_CompiledFromBlueprint ) ) {
		return;
	}

	const FString SectionName = TEXT( "BlueprintSettings" );
	const FConfigSection* ConfigSection = GConfig->GetSection( *SectionName, false, GGameUserSettingsIni );

	if ( ! ConfigSection ) {
		return;
	}

	TSet<FString> ValidBlueprintVariables;

	for ( TFieldIterator<FProperty> PropIt( MyClass ); PropIt; ++PropIt ) {
		FProperty* Property = *PropIt;

		if ( ! Property->GetOwnerClass()->HasAnyClassFlags( CLASS_CompiledFromBlueprint ) ) {
			continue;
		}

		if ( ! Property->HasAnyPropertyFlags( CPF_Edit ) ) {
			continue;
		}

		if ( CastField<FBoolProperty>( Property )
			|| CastField<FIntProperty>( Property )
			|| CastField<FFloatProperty>( Property )
			|| CastField<FDoubleProperty>( Property )
			|| CastField<FNameProperty>( Property )
			|| CastField<FEnumProperty>( Property )
			|| CastField<FStrProperty>( Property )
			|| CastField<FByteProperty>( Property )
			|| CastField<FInt64Property>( Property )
			|| CastField<FTextProperty>( Property )
			|| CastField<FStructProperty>( Property )
			|| CastField<FObjectProperty>( Property )
			|| CastField<FSoftObjectProperty>( Property )
			|| CastField<FArrayProperty>( Property )
			|| CastField<FMapProperty>( Property )
			|| CastField<FSetProperty>( Property )
		) {
			FString ConfigKey = Property->GetName().Replace( TEXT( " " ), TEXT( "" ) );

			ValidBlueprintVariables.Add( ConfigKey );
		}
	}

	for ( FConfigSection::TConstIterator It( *ConfigSection ); It; ++It ) {
		if ( ValidBlueprintVariables.Contains( It.Key().ToString() ) ) {
			continue;
		}

		GConfig->RemoveKeyFromSection( *SectionName, *It.Key().ToString(), GGameUserSettingsIni );
	}
}

TArray<FString> UGameUserSettingsEnhancedBlueprint::GetBlueprintVariableOptions() const
{
	TArray<FString> Options;
	UClass* MyClass = GetClass();

	if ( ! MyClass->HasAnyClassFlags( CLASS_CompiledFromBlueprint ) ) {
		return Options;
	}

	for ( TFieldIterator<FProperty> PropIt( MyClass ); PropIt; ++PropIt ) {
		FProperty* Property = *PropIt;

		if ( ! Property->GetOwnerClass()->HasAnyClassFlags( CLASS_CompiledFromBlueprint ) ) {
			continue;
		}

		if ( ! Property->HasAnyPropertyFlags( CPF_Edit ) ) {
			continue;
		}

		Options.Add( Property->GetName() );
	}

	return Options;
}
