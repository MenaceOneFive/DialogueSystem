#include "DialogueEdGraphLogger.h"

#include "Graph/Node/DialogueCustomNode.h"
#include "Graph/Node/DialogueEdGraphCustomNode.h"
#include "Graph/Node/DialogueEdGraphDialogueLineNode.h"
#include "Graph/Node/DialogueEdGraphEndNode.h"
#include "Graph/Node/DialogueEdGraphSelectNode.h"
#include "Graph/Node/DialogueEdGraphStartNode.h"
#include "Graph/Node/DialogueEndNode.h"
#include "Graph/Node/DialogueLineNode.h"
#include "Graph/Node/DialogueSelectionNode.h"
#include "Graph/Node/DialogueStartNode.h"

/////////////////////////////////////////////////////////////////////////
//FDialogueEdGraphLogger

FDialogueEdGraphLogger::FDialogueEdGraphLogger(const TMap<UDialogueEdGraphNode*, UDialogueGraphNode*>* EditorToRuntime): EditorToRuntime(EditorToRuntime)
{
}

void FDialogueEdGraphLogger::VisitStartNode(UDialogueEdGraphStartNode* StartNode)
{
    const UDialogueStartNode* RuntimeStartNode = Cast<UDialogueStartNode>(*EditorToRuntime->Find(StartNode));
    UE_LOG(LogTemp, Log, TEXT("StartNode %s Next: %s"), *RuntimeStartNode->GetNodeID().ToString(), *RuntimeStartNode->GetNextNodeID().ToString())
}

void FDialogueEdGraphLogger::VisitEndNode(UDialogueEdGraphEndNode* EndNode)
{
    const UDialogueEndNode* RuntimeEndNode = Cast<UDialogueEndNode>(*EditorToRuntime->Find(EndNode));
    FString PrevIds                        = "prevIds:";
    for (FGuid PrevNodeID : RuntimeEndNode->GetPrevNodeIDs())
    {
        PrevIds.Append(PrevNodeID.ToString());
        PrevIds.Append("\n");
    }
    UE_LOG(LogTemp, Log, TEXT("EndNode %s Prev:%s"), *RuntimeEndNode->GetNodeID().ToString(), *PrevIds);
}

void FDialogueEdGraphLogger::VisitSelectNode(UDialogueEdGraphSelectNode* SelectNode)
{
    const UDialogueSelectionNode* RuntimeSelectionNode = Cast<UDialogueSelectionNode>(*EditorToRuntime->Find(SelectNode));
    FString PrevIds                                    = "";
    for (FGuid PrevNodeID : RuntimeSelectionNode->GetPrevNodeIDs())
    {
        PrevIds.Append(PrevNodeID.ToString());
        PrevIds.Append("\n");
    }
    FString NextIds = "";
    for (FGuid NextId : RuntimeSelectionNode->GetNextNodeIDs())
    {
        NextIds.Append(NextId.ToString());
        NextIds.Append("\n");
    }
    UE_LOG(LogTemp, Log, TEXT("LineNode %s Prev:%s Next:%s"), *RuntimeSelectionNode->GetNodeID().ToString(), *PrevIds, *NextIds);
}

void FDialogueEdGraphLogger::VisitLineNode(UDialogueEdGraphDialogueLineNode* LineNode)
{
    const UDialogueLineNode* RuntimeLineNode = Cast<UDialogueLineNode>(*EditorToRuntime->Find(LineNode));
    FString PrevIds                          = "";
    for (FGuid PrevNodeID : RuntimeLineNode->GetPrevNodeIDs())
    {
        PrevIds.Append(PrevNodeID.ToString());
        PrevIds.Append("\n");
    }
    UE_LOG(LogTemp, Log, TEXT("LineNode %s Prev:%s Next:%s"), *RuntimeLineNode->GetNodeID().ToString(), *PrevIds, *RuntimeLineNode->GetNextNodeID().ToString());
}

void FDialogueEdGraphLogger::VisitSceneNode(UDialogueEdGraphSceneNode* SceneNode)
{
    const UDialogueLineNode* RuntimeLineNode = Cast<UDialogueLineNode>(*EditorToRuntime->Find(SceneNode));
    FString PrevIds                          = "";
    for (FGuid PrevNodeID : RuntimeLineNode->GetPrevNodeIDs())
    {
        PrevIds.Append(PrevNodeID.ToString());
        PrevIds.Append("\n");
    }
    UE_LOG(LogTemp, Log, TEXT("SceneNode %s Prev:%s Next:%s"), *RuntimeLineNode->GetNodeID().ToString(), *PrevIds, *RuntimeLineNode->GetNextNodeID().ToString());
}

void FDialogueEdGraphLogger::VisitCustomNode(UDialogueEdGraphCustomNode* EventNode)
{
    const UDialogueCustomNode* RuntimeLineNode = Cast<UDialogueCustomNode>(*EditorToRuntime->Find(EventNode));
    FString PrevIds                            = "";
    for (FGuid PrevNodeID : RuntimeLineNode->GetPrevNodeIDs())
    {
        PrevIds.Append(PrevNodeID.ToString());
        PrevIds.Append("\n");
    }
    UE_LOG(LogTemp, Log, TEXT("CustomNode ID: %s Prev:%s Next:%s Type:%s"),
           *RuntimeLineNode->GetNodeID().ToString(),
           *PrevIds,
           *RuntimeLineNode->GetNextNodeID().ToString(),
           *EventNode->GetClass()->GetName()
            );
}
