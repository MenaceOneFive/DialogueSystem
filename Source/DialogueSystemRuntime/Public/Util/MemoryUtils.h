#pragma once

template <typename T>
static int32 GetObjReferenceCount(UObject* Obj,
                                  TArray<T>* OutReferredToObjects = nullptr)
{
    if (!Obj || !Obj->IsValidLowLevelFast())
    {
        return -1;
    }

    TArray<T> ReferredToObjects; //req outer, ignore archetype, recursive, ignore transient
    FReferenceFinder ObjectReferenceCollector(ReferredToObjects, Obj, false, true, true, false);
    ObjectReferenceCollector.FindReferences(Obj);

    if (OutReferredToObjects)
    {
        OutReferredToObjects->Append(ReferredToObjects);
    }
    return OutReferredToObjects->Num();
}
