# DialogueSystem

언리얼 엔진용 대화 시스템 플러그인입니다. 노드 기반 그래프를 통해 대화를 구성하고, 생성형 AI를 사용하여 대사 작성을 보조하며, 게임플레이 어빌리티 시스템를 이용해서 플레이에 영향을 줍니다.

## 주요 기능

### 사용가능

#### 에셋 편집 기능

노드 기반 비주얼 에디터를 통해 대화 플로우를 구성할 수 있습니다. 대화 노드들을 연결하고, 조건부 분기와 선택지를 쉽게 설정할 수 있습니다.

#### 레벨 시퀀스 재생 기능

각 대화 노드에 레벨 시퀀스를 연결하여 캐릭터 애니메이션, 카메라 워크, 이펙트 등을 재생할 수 있습니다.

#### 선택지 기능

플레이어가 대화 중 여러 선택지 중 하나를 선택할 수 있는 노드를 제공합니다. 각 선택지는 내장 블루프린트를 통해 조건부 활성화가 가능하며, 선택에 따라 다른 대화 경로로 분기됩니다. 플레이어의 레벨, 보유 아이템, 게임 진행 상태 등을 기반으로 선택지의 표시 여부를 동적으로 제어할 수 있습니다.

#### 에셋 내장 블루프린트

대화 그래프 에셋 내부에 내장된 블루프린트를 사용하여 게임 로직을 구현할 수 있습니다. 노드 선택 조건, 이벤트 처리뿐만 아니라 각 노드 방문 시 경험치 획득, 아이템 지급, 게임 변수 설정 등의 동적인 게임플레이 요소를 실행할 수 있습니다.
이를 통해 대화가 단순한 텍스트 출력을 넘어 게임 진행에 직접적인 영향을 미치는 상호작용을 구현했습니다.

## 프로젝트 구조

```
DialogueSystem/
├── Source/                      # C++ 소스 코드
│   ├── DialogueSystemEditor/    # 에디터 모듈 (노드 기반 그래프 에디터)
│   │   ├── Private/
│   │   │   ├── Character/       # 캐릭터 관련 에디터 기능
│   │   │   ├── GenAI/           # AI 기반 대화 생성 시스템
│   │   │   ├── Graph/           # 그래프 에디터 구현
│   │   │   ├── MovieScene/      # 시퀀서 관련 기능
│   │   └── Public/
│   └── DialogueSystemRuntime/   # 런타임 모듈 (대화 시스템 실행)
│       ├── Private/
│       │   ├── Component/       # 액터 컴포넌트들
│       │   ├── Graph/           # 대화 그래프 런타임
│       │   ├── Player/          # 대화 재생 시스템
│       │   ├── Subsystem/       # 게임 서브시스템들
│       │   └── Widget/          # UI 위젯들
│       └── Public/
└── DialogueSystem.uplugin       # 플러그인 정의 파일
```

## 주요 의존성

### 엔진 빌트인

#### 런타임 모듈

- GameplayAbilities, GameplayTags, GameplayTasks - GAS(Gameplay Ability System) 통합
- MovieScene, MovieSceneTracks, LevelSequence - 시퀀서를 이용한 대화 재생
- UMG - UI 시스템
- CustomizableSequencerTracks - 커스텀 시퀀서 트랙 : 자막

#### 에디터 모듈

- UnrealEd, GraphEditor - 에디터 및 노드 그래프 시스템
- Sequencer, SequencerCore - 시퀀서 에디터 기능 사용
- PropertyEditor, EditorWidgets - 커스텀 에디터 UI
- UEdGraph - 에디터 그래프 정의용
- BlueprintGraph - 블루프린트 편집기 호출용


### 직접 제작

- [GenerativeAI](https://github.com/MenaceOneFive/GenerativeAI)  AI 기반 대화 작성 보조
- [JsonGeneratorViewer](https://github.com/MenaceOneFive/JsonGenerator) : Json <-> UStruct간 변환 로직 생성용

## 설치 방법

### 플러그인 다운로드

```shell
echo 플러그인의 프로젝트 폴더로 이동합니다.
cd ./MyUEProject
echo 플러그인 폴더로 이동합니다.
cd ./Plugins

echo 플러그인을 다운로드 합니다.
git clone https://github.com/MenaceOneFive/DialogueSystem.git

echo 의존성을 다운로드 합니다.
git clone https://github.com/MenaceOneFive/GenerativeAI.git
git clone https://github.com/MenaceOneFive/JsonGenerator.git
```

### 프로젝트 파일에 종속성 추가

YourProject.Build.cs에 추가:

```csharp
PublicDependencyModuleNames.AddRange(new string[] 
{
    "Core",
    "CoreUObject", 
    "Engine",
    "GameplayAbilities",
    "GameplayTags",
    "GameplayTasks",
    "UMG",
    "LevelSequence",
    // "DialogueSystemEditor" - 에디터 모듈의 경우 추가
    // "DialogueSystemRuntime" - 런타임 모듈의 경우 추가
});
```

### 프로젝트 설정에서 플러그인 활성화

YourProject.uproject을 수정:

```json
{
  "Plugins": [
    {
      "Name": "DialogueSystem",
      "Enabled": true
    },
    {
      "comment_warning": "이 항목이 없으면 75%쯤에서 GameplayAbilities-dll 누락으로 인한 크래시가 발생합니다.",
      "Name": "GameplayAbilities",
      "Enabled": true
    }
  ]
}
```

## 대화 노드 유형

### 0. UDialogueGraphNode(공통 베이스 클래스)

모든 대화 노드의 기본 클래스입니다.

- DialogueSetting (`FDialogueSetting`): 대화 설정 정보

| 속성                    | 타입           | 설명                                         |
|-----------------------|--------------|--------------------------------------------|
| FocusMode             | `EFocusMode` | 입력 포커스 모드 (UI: UI가 입력 관리, Game: 게임이 입력 관리) |
| ShouldShowCursor      | `bool`       | 커서 표시 여부                                   |
| CanVisitMultipleTimes | `bool`       | 노드 중복 방문 가능 여부                             |
| PlayableTime          | `uint8`      | 재생 가능 횟수 (CanVisitMultipleTimes가 true인 경우) |

### 1. UDialogueConditionalNode (조건부 노드 - 추상)

선택 가능 여부를 판단하는 조건을 가진 노드들이 상속해야 하는 추상 노드입니다.

### 2. UDialogueStartNode (시작 노드)

대화의 시작점을 나타내는 노드입니다.

### 3. UDialogueEndNode (종료 노드)

대화의 종료점을 나타내는 노드입니다.

### 4. UDialogueLineNode (대사 노드)

레벨 시퀀스를 재생하는 대사 노드입니다.

- SequencePlaySetting (`FSequencePlaySetting`): 시퀀스 재생 설정

| 속성                                            | 타입                        | 설명                                                |
|-----------------------------------------------|---------------------------|---------------------------------------------------|
| StateAfterSequencePlay                        | `EStateAfterSequencePlay` | 시퀀스 재생 후 상태 처리 방식 (Keep: 변화 유지, Reset: 이전 상태로 복원) |
| CanSkipThisNode                               | `bool`                    | 이 노드를 스페이스바 입력으로 스킵할 수 있는지 여부                     |
| StartTimeOfSequence                           | `float`                   | 시퀀스 시작 시간                                         |
| ShouldBlockPlayerMovementDuringDialogue       | `bool`                    | 대화 중 플레이어 이동 차단 여부                                |
| ShouldBlockPlayerCameraMovementDuringDialogue | `bool`                    | 대화 중 플레이어 카메라 회전 차단 여부                            |

### 5. UDialogueSelectionNode (선택 노드)

플레이어가 선택할 수 있는 옵션들을 제공하는 노드입니다.

### 6. UDialogueCustomNode (커스텀 노드)

사용자 정의 이벤트를 실행하는 노드입니다.
에디터 타임 노드와 FDialogueEditorCustomNodeDefinition를 정의해서 에디터 모듈에 추가해야 합니다.
런타임 노드와 FDialogueSystemCustomNodeDefinition를 정의해서 런타임 모듈에 추가해야 합니다.

## 플러그인 확장

원하는 노드를 직접 구현하는 기능을 제공하고 있습니다. 아래의 가이드라인을 따라 구현해주세요

### 에디터 타임 확장

#### 에디터 노드 정의

```c++
UDialogueEdGraphCustomNode를 상속한 새 노드를 정의합니다.
class MYPLUGINEDITOR_API UDialogueEdGraphQTENode : public UDialogueEdGraphCustomNode

// 구현해야 하는 메서드

// 선택노드에 표시할 텍스트가 없을 경우를 대비해 구현해야 합니다
virtual FText GetDefaultSelectionName() const override;
```

#### 에디터 노드의 연산 정의

```c++
UDialogueEdGraphCustomNode를 상속한 새로운 노드를 플러그인에서 어떻게 다루어야 하는지 FDialogueEditorCustomNodeDefinition를 상속해서 정의합니다.
struct MYPLUGINEDITOR_API FDialogueEdQTENodeDefinition : public FDialogueEditorCustomNodeDefinition

// 구현해야 하는 메서드들

// 에디터 노드의 StaticClass를 반환하면 됩니다.
virtual TSubclassOf<UDialogueEdGraphCustomNode> GetEditorNodeType() const = 0; 

// 에디터 노드에 대응하는 런타임 노드의 StaticClass를 반환하면 됩니다.
virtual TSubclassOf<UDialogueCustomNode> GetRuntimeNodeType() const = 0;       

// 런타임 노드 생성 시 데이터 복사를 처리합니다.
virtual void InitializeRuntimeNodeWithEditorNode(UDialogueCustomNode* RuntimeNode, UDialogueEdGraphCustomNode* EditorNode) const = 0; 

// 에디터에서 어떻게 보여줄 지 정의합니다. nullptr를 반환해도 상관없습니다.(이 경우, UEdGraphNode의 기본 구현을 사용합니다)
virtual TSharedPtr<SGraphNode> MakeSlateWidgetForNode(UDialogueEdGraphCustomNode* EditorNode) const = 0;

// 출력핀의 유형을 설정합니다 (단일, 다수)
virtual EOutgoingConnection GetConnectionType() = 0;

// 에디터의 컨텍스트 메뉴에서 사용하기 위해 필요합니다.
virtual FString GetNodeName() const = 0;
virtual FString GetNodeDescription() const = 0;
virtual FString GetNodeSearchKeyword() const = 0;
```

#### 에디터 모듈에 등록

```c++
// 에디터 모듈에 대한 참조를 가져옵니다.
FDialogueSystemEditorModule& Module  = FModuleManager::Get().GetModuleChecked<FDialogueSystemEditorModule>("DialogueSystemEditor");
FEditorCustomNodeManager* CustomNodeManager = Module.GetCustomNodeManager();

// 작성한 커스텀 노드의 StaticClass와 FDialogueEditorCustomNodeDefinition를 등록합니다.
CustomNodeManager->RegisterCustomNodeDefinition(UDialogueEdGraphQTENode::StaticClass(), MakeShared<FDialogueEdQTENodeDefinition>());
```

### 런타임 확장

#### 런타임 노드 정의

```c++
// UDialogueCustomNode를 상속하여 노드가 어떤 데이터를 담아야 하는지 정의합니다.
class MYPLUGINRUNTIME_API UDialogueQTENode : public UDialogueCustomNode
```

#### 런타임 노드의 연산 정의

```c++
// FDialogueSystemCustomNodeDefinition 를 상속하여 노드가 어떻게 평가되어야 하는지 정의합니다.
struct MYPLUGINRUNTIME_API FDialogueQTENodeDefinition final : FDialogueSystemCustomNodeDefinition

// 구현해야 하는 메서드들

//FDialogueSystemCustomNodeDefinition이 어느 UDialogueCustomNode에 대한 것인지 알려줍니다.
virtual TSubclassOf<UDialogueCustomNode> GetRuntimeCustomNodeType() const = 0;

// 실제 대화 재생 시 UDialogueCustomNode의 차례가 되었을 때 어떻게 재생할지 정의합니다.
// Evaluator를 통해 이 노드를 재생하는 대화 재생기의 기능을 사용할 수 있습니다.
// 지원하는 인터페이스는 IDialogueCustomNodeSequencePlayer, IDialogueUIManager 입니다.
// 상황에 맞게 Evaluator를 캐스팅해서 사용하시면 됩니다.
virtual void EvaluateCustomNode(const UDialogueCustomNode* CustomNode, UObject* Evaluator) const = 0;
```

#### 런타임 모듈에 등록

```c++
// 런타임 모듈에 대한 참조를 가져옵니다.
FDialogueSystemRuntimeModule& Module  = FModuleManager::Get().GetModuleChecked<FDialogueSystemRuntimeModule>("DialogueSystemRuntime");
FRuntimeCustomNodeManager* CustomNodeManager = Module.GetCustomNodeManager();

// 작성한 FDialogueEditorCustomNodeDefinition를 등록합니다.
CustomNodeManager->RegisterCustomNodeDefinition(MakeShared<FDialogueQTENodeDefinition>());
```

## 로드맵 (Roadmap)

### 커스텀 노드 UI 연동

커스텀 노드가 독립적인 위젯 블루프린트를 소유하고, 이를 통해 플레이어 UI와 상호작용하는 기능을 추가할 계획입니다.

#### 위젯 블루프린트 지정 기능 추가

커스텀 노드 데이터에 위젯 블루프린트 클래스를 지정할 수 있는 속성을 추가합니다.

대화 시스템이 해당 커스텀 노드를 처리할 때, 지정된 위젯의 인스턴스를 생성하여 뷰포트에 표시하도록 구현합니다.

#### 노드-위젯 간 데이터 및 이벤트 처리

커스텀 노드가 가진 데이터를 생성된 위젯 인스턴스로 전달하는 인터페이스를 구현합니다.

위젯에서 발생한 특정 이벤트(예: 버튼 클릭)가 대화 시스템에 전달되어, 그 결과에 따라 다음 노드로 진행을 제어하는 기능을 추가합니다.

### 블루프린트 기능 확장

#### 세이브 연동

플레이어의 선택지를 세이브 파일에 기록하고, 불러온 세이브 데이터에 따라 대화 노드의 활성화 여부나 분기 흐름이 동적으로 변경되는 기능을 구현합니다.

### 생성형 AI 연동 강화

#### 문맥(Context) 기반 프롬프트 자동 생성

사용자가 입력한 프롬프트 외에, 현재 편집 중인 대화 그래프의 이전 노드 데이터, 연결된 캐릭터 정보 등을 자동으로 수집하여 컨텍스트로 구성합니다. 이를 통해 더 일관성 있고 깊이 있는 대사를 생성하도록 AI 모델을 유도합니다.