# Unreal Engine 5 C++ Documentation Cache

## Quick Access Index

### 🎯 Most Common References
- [Common Patterns](quick-reference/common-patterns.md) - Frequently used code patterns
- [Essential Macros](quick-reference/macros.md) - UCLASS, UPROPERTY, UFUNCTION reference
- [Actors System](core-systems/actors.md) - AActor, spawning, lifecycle
- [Components](core-systems/components.md) - UActorComponent, USceneComponent
- [Networking](gameplay/networking.md) - Replication, RPCs, multiplayer

### 📚 Core Systems
| Topic | File | Description |
|-------|------|-------------|
| **Actors** | [actors.md](core-systems/actors.md) | AActor, spawning, lifecycle, basic patterns |
| **Components** | [components.md](core-systems/components.md) | Component system, custom components |
| **UObject System** | [objects.md](core-systems/objects.md) | UCLASS, UPROPERTY, reflection system |
| **Module System** | [modules.md](core-systems/modules.md) | .Build.cs files, module structure |
| **Containers** | [containers.md](core-systems/containers.md) | TArray, TMap, FString, math types |

### 🎮 Gameplay Systems
| Topic | File | Description |
|-------|------|-------------|
| **Input** | [input.md](gameplay/input.md) | Enhanced Input, legacy input, binding actions |
| **Networking** | [networking.md](gameplay/networking.md) | Replication, RPCs, multiplayer architecture |

### ⚡ Quick Reference
| Topic | File | Description |
|-------|------|-------------|
| **Common Patterns** | [common-patterns.md](quick-reference/common-patterns.md) | Actor creation, timers, collision, UI |
| **Essential Macros** | [macros.md](quick-reference/macros.md) | UCLASS, UPROPERTY, UFUNCTION guide |

## 🔍 Search by Use Case

### "I want to create a new Actor"
1. [actors.md](core-systems/actors.md) - Basic Actor setup
2. [common-patterns.md](quick-reference/common-patterns.md) - Actor creation template
3. [components.md](core-systems/components.md) - Adding components

### "I need to handle input"
1. [input.md](gameplay/input.md) - Enhanced Input system
2. [common-patterns.md](quick-reference/common-patterns.md) - Input patterns

### "I'm working with networking"
1. [networking.md](gameplay/networking.md) - Complete networking guide
2. [macros.md](quick-reference/macros.md) - RPC and replication macros

### "I need to understand UE5 containers"
1. [containers.md](core-systems/containers.md) - TArray, TMap, FString
2. [objects.md](core-systems/objects.md) - UObject system integration

### "I'm setting up a new module"
1. [modules.md](core-systems/modules.md) - Module structure and .Build.cs
2. [common-patterns.md](quick-reference/common-patterns.md) - Module patterns

## 📖 Learning Path

### Beginner (Start Here)
1. [objects.md](core-systems/objects.md) - Understanding UObject system
2. [macros.md](quick-reference/macros.md) - Essential macros
3. [actors.md](core-systems/actors.md) - Basic Actor usage
4. [components.md](core-systems/components.md) - Component system

### Intermediate
1. [containers.md](core-systems/containers.md) - UE5 data types
2. [input.md](gameplay/input.md) - Input handling
3. [common-patterns.md](quick-reference/common-patterns.md) - Advanced patterns
4. [modules.md](core-systems/modules.md) - Project organization

### Advanced
1. [networking.md](gameplay/networking.md) - Multiplayer systems

## 🛠️ Code Examples by Category

### Basic Setup
```cpp
// Create a basic actor - see actors.md
UCLASS()
class MYGAME_API AMyActor : public AActor
{
    GENERATED_BODY()
public:
    AMyActor() { PrimaryActorTick.bCanEverTick = true; }
};
```

### Component Usage
```cpp
// Add a component - see components.md
MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
MeshComponent->SetupAttachment(RootComponent);
```

### Property Declaration
```cpp
// Expose to editor - see macros.md
UPROPERTY(EditAnywhere, Category = "Weapon")
float Damage = 100.0f;
```

### Input Handling
```cpp
// Enhanced Input binding - see input.md
EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);
```

## 📝 File Organization

```
docs/unreal-engine/
├── README.md                    # This index file
├── core-systems/              # Fundamental UE5 systems
│   ├── actors.md              # AActor system
│   ├── components.md          # Component architecture  
│   ├── objects.md             # UObject/reflection system
│   ├── modules.md             # Module system
│   └── containers.md          # Data types and containers
├── gameplay/                  # Gameplay-specific systems
│   ├── input.md              # Input handling
│   └── networking.md         # Multiplayer/replication
└── quick-reference/          # Fast lookup guides
    ├── common-patterns.md    # Frequently used patterns
    └── macros.md            # Essential macro reference
```

## 🎯 Usage Tips

1. **Start with basics**: If you're new to UE5 C++, begin with `objects.md` and `macros.md`
2. **Use search**: Use Ctrl+F to quickly find specific functions or classes
3. **Cross-reference**: Links between files help you understand relationships
4. **Copy patterns**: Use the code examples as starting points for your implementations
5. **Check headers**: Each file lists required #include statements

## 🔄 Keep Updated

This documentation cache is current as of UE 5.5/5.6. When Epic updates the engine:
1. The context7 integration can be re-run to pull latest docs
2. Patterns remain largely stable across UE5 versions
3. New features can be added to existing files

---

**Quick Links for Agents:**
- Need to create an actor? → [actors.md](core-systems/actors.md)
- Need macros? → [macros.md](quick-reference/macros.md)  
- Need networking? → [networking.md](gameplay/networking.md)
- Need input handling? → [input.md](gameplay/input.md)
- Need common patterns? → [common-patterns.md](quick-reference/common-patterns.md)