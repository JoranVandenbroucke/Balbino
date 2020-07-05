#pragma once
#include "Component.h"
#include <map>

namespace Balbino
{
	class GameObject;
	class Sprite;
	//ripped Straigt from Unity3D
	//I mean, you shouldn't reinvent hot water
	enum AnimatorConditionMode
	{
		If,			//Bool, Trigger
		IfNot,		//Bool, Trigger
		Greater,	//Bool, Trigger
		Less,		//Int, Float
		Equals,		//Int, Float
		NotEqual	//Int, Float
	};
	enum AnimatorControllerParameterType
	{
		Float,
		Int,
		Bool,
		Trigger
	};
	struct AnimatorCondition
	{
		AnimatorControllerParameterType type;
		AnimatorConditionMode mode;		//compair/trigger mode
		std::string parameter;			//the name so you can call it
		float threshold;				//when to trigger
	};
	//end ripoff

	//based of https://docs.unity3d.com/ScriptReference/Animations.AnimatorTransition.html
	class AnimatorTransition
	{
	public:
		void AddCondition( Balbino::AnimatorControllerParameterType type, Balbino::AnimatorConditionMode mode, float threshold, std::string parameter );
		void RemoveCondition( AnimatorCondition condition );
		std::vector<Balbino::AnimatorCondition>& GetConditions();
		const std::vector<Balbino::AnimatorCondition>& GetConditions()const;
		int SourceState;
		int DestinationState;
	private:
		std::vector<Balbino::AnimatorCondition> m_Conditions;
	};

	//unique
	class Animation final :public Component
	{
	public:
		explicit Animation( const GameObject* const origine );
		~Animation() = default;
		Animation( const Animation& ) = delete;
		Animation( Animation&& ) = delete;
		Animation& operator=( const Animation& ) = delete;
		Animation& operator=( Animation&& ) = delete;

		// Inherited via Component
		virtual void Create() override;
		virtual void Update() override;
		virtual void Draw() const override;
		virtual void Save( std::ostream& file ) override;
		virtual void Load( std::istream& file ) override;

		void SetBool( const std::string& name, bool value );
		void SetTrigger( const std::string& name, bool value );
		void SetInt( const std::string& name, int value );
		void SetFloat( const std::string& name, float value );

		bool SetAnimation( int animationNr, const std::string& path );

		bool GetBool( const std::string& name )const;
		bool GetTrigger( const std::string& name )const;
		int GetInt( const std::string& name )const;
		float GetFloat( const std::string& name )const;

		void AddBool( const std::string& name );
		void AddTrigger( const std::string& name );
		void AddInt( const std::string& name );
		void AddFloat( const std::string& name );

		void RemoveBool( const std::string& name );
		void RemoveTrigger( const std::string& name );
		void RemoveInt( const std::string& name );
		void RemoveFloat( const std::string& name );

#ifdef BALBINO_DEBUG
		virtual void DrawInpector() override;
#endif // !DEBUG
	private:
		int m_CurrentAnimationState;
		int m_CurrentAnimationFrame;
		float m_Time;

		std::vector<std::string> m_AnimationsFiles;
		std::vector<AnimatorTransition> m_AnimationTransitions;
		std::vector<std::pair<std::pair<float, int>, std::string>>* m_pCurrrentAnimation;
		std::vector<std::vector<std::pair<std::pair<float, int>, std::string>>> m_Animations;
		std::map<std::string, bool> m_BoolTypes;
		std::map<std::string, bool> m_TriggerTypes;
		std::map<std::string, float> m_FloatTypes;
		std::map<std::string, int> m_IntTypes;
		Sprite* m_pSprite;
	};
}

