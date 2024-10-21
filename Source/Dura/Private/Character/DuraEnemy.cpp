// Copyright by person HDD  


#include "Character/DuraEnemy.h"
#include "../Dura.h"
#include "AbilitySystem/DuraAbilitySystemComponent.h"
#include "AbilitySystem/DuraAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "UI/UserWidget/DuraUserWidget.h"
#include "AbilitySystem/DuraAbilitySystemLibrary.h"
#include "DuraGameplayTags.h"
#include "Gameframework/CharacterMovementComponent.h"

ADuraEnemy::ADuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitiesSystemComponent = CreateDefaultSubobject<UDuraAbilitySystemComponent>("AbilitiesSystemComponent");
	AbilitiesSystemComponent->SetIsReplicated(true);
	AbilitiesSystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UDuraAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

void ADuraEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(HIGHLIGHT_COLOR_RED);

	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(HIGHLIGHT_COLOR_RED);
}

void ADuraEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

void ADuraEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.0f : BaseWalkSpeed;
}

void ADuraEnemy::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	InitAbilityActorInfo();
	UDuraAbilitySystemLibrary::GiveStartupAbilities(this, AbilitiesSystemComponent);

	if (UDuraUserWidget* DuraUserWidget = Cast<UDuraUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		DuraUserWidget->SetWidgetController(this);
	}

	UDuraAttributeSet* DuraAS = Cast<UDuraAttributeSet>(AttributeSet);
	if (DuraAS)
	{
		AbilitiesSystemComponent->GetGameplayAttributeValueChangeDelegate(DuraAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data) 
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
		AbilitiesSystemComponent->GetGameplayAttributeValueChangeDelegate(DuraAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

		AbilitiesSystemComponent->RegisterGameplayTagEvent(FDuraGameplayTags::Get().Effect_HitReact, 
			EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ADuraEnemy::HitReactTagChanged);

		OnHealthChanged.Broadcast(DuraAS->GetHealth());
		OnMaxHealthChanged.Broadcast(DuraAS->GetMaxHealth());
	}
}

void ADuraEnemy::InitAbilityActorInfo()
{
	check(AbilitiesSystemComponent);

	AbilitiesSystemComponent->InitAbilityActorInfo(this, this);
	Cast<UDuraAbilitySystemComponent>(AbilitiesSystemComponent)->AbilityActorInfoSet();

	InitializeDefaultAttributes();
}

void ADuraEnemy::InitializeDefaultAttributes() const
{
	UDuraAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitiesSystemComponent);
}

int32 ADuraEnemy::GetPlayerLevel() const
{
	return Level;
}

void ADuraEnemy::Die()
{
	SetLifeSpan(LifeSpan);
	Super::Die();
}
