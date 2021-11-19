// Fill out your copyright notice in the Description page of Project Settings.


#include "TextFileManager.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "stdlib.h"


bool UTextFileManager::SaveArrayText(FString SaveDirectory, FString FileName, TArray<FString> SaveText)
{
	//Set complete file path.
	SaveDirectory += "\\";
	SaveDirectory += FileName;

	FString FinalString = "";
	for (FString& Each : SaveText)
	{
		FinalString += Each;
		FinalString += LINE_TERMINATOR;
	}

	return FFileHelper::SaveStringToFile(FinalString, *SaveDirectory);
}

TArray<FString> UTextFileManager::GetActions(TArray<int> InputText) {
	TArray<FString> returnString;
	
	Qlearn qlearn;
	returnString = qlearn.printMovement(InputText);
		
	return returnString;
}

FString UTextFileManager::GetAction(TArray<int> InputText) {
	FString returnString;

	Qlearn qlearn;
	returnString = qlearn.printSingleMove(InputText);

	return returnString;
}