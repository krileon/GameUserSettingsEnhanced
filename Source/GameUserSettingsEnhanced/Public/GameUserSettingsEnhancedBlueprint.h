// Copyright (c) 2026 Krileon. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"

#include "GameUserSettingsEnhancedBlueprint.generated.h"

UCLASS( Blueprintable, BlueprintType )
class GAMEUSERSETTINGSENHANCED_API UGameUserSettingsEnhancedBlueprint : public UGameUserSettings
{
	GENERATED_BODY()

public:
	virtual void LoadSettings( bool bForceReload ) override;
	virtual void SaveSettings() override;
	virtual void ApplyNonResolutionSettings() override;
	virtual void SetToDefaults() override;
	virtual void ResetToCurrentSettings() override;
	virtual bool IsDirty() const override;
	virtual void ValidateSettings() override;

	UFUNCTION( BlueprintPure, Category = "Settings", meta = ( ToolTip = "Checks if any of the Blueprint Variable Values differ from the saved Config Values." ) )
	bool IsBlueprintDirty() const;

	UFUNCTION( BlueprintImplementableEvent, Category = "Settings" )
	void OnLoadBlueprintSettings();

	UFUNCTION( BlueprintImplementableEvent, Category = "Settings" )
	void OnSaveBlueprintSettings();

	UFUNCTION( BlueprintImplementableEvent, Category = "Settings" )
	void OnApplyBlueprintSettings();

protected:
	void LoadBlueprintSettings();
	void SaveBlueprintSettings();
	void ResetBlueprintSettings();
	void ApplyBlueprintSettings();
	void ValidateBlueprintSettings();

	UPROPERTY( EditAnywhere, Category = "Settings", meta = ( GetKeyOptions = "GetBlueprintVariableOptions", ToolTip = "Select the Blueprint Variable to map into a Console Variable that will be set using CVar." ) )
	TMap<FString, FString> ConsoleVariables;

	UFUNCTION()
	TArray<FString> GetBlueprintVariableOptions() const;
};
