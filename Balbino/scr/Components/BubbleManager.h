#pragma once
#include "Component.h"

namespace Balbino
{
	class Bubble;
	enum class Event
	{
		SHOOT_BUBBLE,
		HIT_BUBBLE,
		HIT_ENEMY,
		HIT_ITEM,
		NEW_BUBBLE
	};
	class BubbleManager : public Component
	{
	public:
		explicit BubbleManager( const GameObject* const origine );
		virtual~BubbleManager() = default;
		BubbleManager( const BubbleManager& ) = delete;
		BubbleManager( BubbleManager&& ) = delete;
		BubbleManager& operator=( const BubbleManager& ) = delete;
		BubbleManager& operator=( BubbleManager&& ) = delete;

		// Inherited via Component
		virtual void Create() override;
		virtual void Update() override;
		virtual void Draw() const override;
		virtual void Save( std::ostream& file ) override;
		virtual void Load( std::istream& file ) override;
#ifdef _DEBUG
		virtual void DrawInpector() override;
#endif // _DEBUG


		void Notify( const Component* player, Event event );
	private:

#define MAX_OBSERVERS 1
#define MAX_BUBBLES 12
		class Observer
		{
		public:
			virtual ~Observer() = default;
			virtual void OnNotify( const Component* entity, Event event ) = 0;
			virtual void Update() = 0;
		};
		class BubbleObserver : public Observer
		{
		public:
			BubbleObserver();
			// Inherited via Observer
			virtual void OnNotify( const Component* entity, Event event ) override;
			virtual void Update() override;
		private:
			float m_BubBubbleDelay;
			float m_BobBubbleDelay;
			const float m_MaxBubbleDelay = .5f;
			Bubble* m_pBubbles[MAX_BUBBLES];
			char m_NrOfBubbles;
		};

		Observer* m_pObservers[MAX_OBSERVERS];
		int m_NumObservers;
		bool m_OverSave;
	};
}
