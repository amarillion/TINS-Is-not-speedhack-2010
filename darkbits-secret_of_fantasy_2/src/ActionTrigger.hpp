#pragma once

class Character;

class ActionTrigger {
public:
	enum Type {
		Type_Ability,
		Type_Spell,
		Type_Item,
	};

	enum TargetMode {
		TargetMode_JustOne,
		TargetMode_OneOrMany,
		TargetMode_JustMany,
		TargetMode_NonFightingPartyMember,
		TargetMode_None,
	};

	enum TargetPolicy {
		TargetPolicy_OnlyAlive,
		TargetPolicy_DeadAllowed,
	};

	enum Alignment {
		Alignment_Defensive,
		Alignment_Offensive,
	};

	ActionTrigger();

	ActionTrigger(Type type, const std::string &name, const std::string &description, TargetMode targetMode, TargetPolicy targetPolicy, Alignment alignment, int mpCost = 0);

	Type getType() const;

	const std::string &getName() const;

	int getMPCost() const;

	TargetMode getTargetMode() const;

	TargetPolicy getTargetPolicy() const;

	Alignment getAlignment() const;

	int getOrder() const;

	void setOrder(int order);

	bool operator<(const ActionTrigger& that) const;

	const std::string &getDescription() const;

private:
	Type myType;
	std::string myName;
	int myMPCost;
	TargetMode myTargetMode;
	Alignment myAlignment;
	TargetPolicy myTargetPolicy;
	int myOrder;
	std::string myDescription;
};