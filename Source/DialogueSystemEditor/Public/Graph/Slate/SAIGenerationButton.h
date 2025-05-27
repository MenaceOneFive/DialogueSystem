#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class DIALOGUESYSTEMEDITOR_API SAIGenerationButton : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SAIGenerationButton)
            : _ButtonSize(FVector2D{30.f, 30.f})
        {
        }

        SLATE_ARGUMENT(FVector2D, ButtonSize)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

private:
    FVector2D ButtonSize;
};
