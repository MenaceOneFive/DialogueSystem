// Fill out your copyright notice in the Description page of Project Settings.


#include "Style/DialogueStyle.h"

#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"
#include "Styling/SlateStyleRegistry.h"

// ReSharper disable once CppUE4CodingStandardNamingViolationWarning
// #define RootToContentDir Style->RootToContentDir
//
// void FDialogueStyle::Initialize()
// {
//     if ( !StyleInstance.IsValid() )
//     {
//         StyleInstance = Create();
//         FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
//     }
// }
//
// void FDialogueStyle::Shutdown()
// {
//     FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
//     ensure(StyleInstance.IsUnique());
//     StyleInstance.Reset();
// }
//
// const ISlateStyle& FDialogueStyle::Get()
// {
//     return *StyleInstance;
// }
//
// FName FDialogueStyle::GetStyleSetName()
// {
//     static FName StyleSetName(TEXT("DialogueStyle"));
//     return StyleSetName;
// }
//
// TSharedPtr<FSlateStyleSet> FDialogueStyle::Create()
// {
//     TSharedPtr<FSlateStyleSet> Style    = MakeShareable(new FSlateStyleSet(GetStyleSetName()));
//     const TSharedPtr<IPlugin> PluginPtr = IPluginManager::Get().FindPlugin("DialogueSystem");
//     check(PluginPtr.IsValid());
//
//     const FString ContentDir = PluginPtr->GetBaseDir() / TEXT("Resources/Icons");
//     Style->SetContentRoot(ContentDir);
//
//     Style->Set("DialogueStyle.AIGenerationButton", new IMAGE_BRUSH_SVG("ai-30px", {32.f, 32.f}));
//
//     return Style;
// }
