#include "BalbinoPCH.h"
#include "Animation.h"
#include "../Editor/Debug.h"
#include "../Time.h"
#include "../Editor/File.h"
#include "../BinaryReaderWrider.h"
#include "Sprite.h"
#include <regex>

void Balbino::AnimatorTransition::AddCondition( Balbino::AnimatorControllerParameterType type, AnimatorConditionMode mode, float threshold, std::string parameter )
{
	m_Conditions.push_back( AnimatorCondition{ type, mode, parameter, threshold } );
}

void Balbino::AnimatorTransition::RemoveCondition( AnimatorCondition condition )
{
	m_Conditions.erase( std::remove_if( m_Conditions.begin(), m_Conditions.end(), [condition]( Balbino::AnimatorCondition& c1 )
	{
		return c1.parameter == condition.parameter;
	} ) );
}

std::vector<Balbino::AnimatorCondition>& Balbino::AnimatorTransition::GetConditions()
{
	return m_Conditions;
}
const std::vector<Balbino::AnimatorCondition>& Balbino::AnimatorTransition::GetConditions()const
{
	return m_Conditions;
}

Balbino::Animation::Animation( const GameObject* const origine )
	:Component{ origine }
	, m_CurrentAnimationState{}
	, m_CurrentAnimationFrame{}
	, m_Time{}
	, m_pSprite{ nullptr }
	, m_pCurrrentAnimation{ nullptr }
{
}

void Balbino::Animation::Create()
{
	this->Component::Create();
	m_pSprite = GetComponent<Sprite>();
	if( !m_pSprite )
	{
		m_pSprite = AddComponent<Sprite>();
		m_pSprite->Create();
	}
	if( m_Animations.size() )
	{
		m_pCurrrentAnimation = &m_Animations[0];
		m_pSprite->SetTexture( ( *m_pCurrrentAnimation )[0].second );
		m_pSprite->LoadUV();
		m_pSprite->SetSpriteIndex( ( *m_pCurrrentAnimation )[0].first.second );
	}
}

void Balbino::Animation::Update()
{
	if( !m_pCurrrentAnimation )
		return;
	float dt = BTime::DeltaTime();
	m_Time += dt;
	int animatinSize = int( m_pCurrrentAnimation->size() );
	if( m_CurrentAnimationFrame + 1 < animatinSize )
	{
		if( m_Time > ( *m_pCurrrentAnimation )[m_CurrentAnimationFrame + 1].first.first )
		{
			if( ( *m_pCurrrentAnimation )[m_CurrentAnimationFrame].second != ( *m_pCurrrentAnimation )[m_CurrentAnimationFrame + 1].second )
			{
				m_pSprite->SetTexture( ( *m_pCurrrentAnimation )[m_CurrentAnimationFrame + 1].second );

			}
			m_pSprite->SetSpriteIndex( ( *m_pCurrrentAnimation )[++m_CurrentAnimationFrame].first.second );
		}
	}
	else
	{
		m_Time = 0.f;
		m_CurrentAnimationFrame = 0;
	}

	for( AnimatorTransition transition : m_AnimationTransitions )
	{
		auto transitionConditions = transition.GetConditions();
		if( transitionConditions.size() )
		{

			bool success{ true };
			for( AnimatorCondition condition : transitionConditions )
			{
				switch( AnimatorControllerParameterType(condition.type) )
				{
				case AnimatorControllerParameterType::Trigger:
				{
					bool value = GetTrigger( condition.parameter );
					if( value )
					{
						SetTrigger( condition.parameter, false );
					}
					else
					{
						success = false;
					}
					break;
				}
				case AnimatorControllerParameterType::Bool:
				{
					bool value = GetBool( condition.parameter );
					if( ( condition.mode == AnimatorConditionMode::If && !value )
						|| ( condition.mode == AnimatorConditionMode::IfNot && value ) )
					{
						success = false;
					}
					break;
				}
				case AnimatorControllerParameterType::Int:
				{
					int value = GetInt( condition.parameter );
					if( ( condition.mode == AnimatorConditionMode::Equals && value != condition.threshold )
						|| ( condition.mode == AnimatorConditionMode::NotEqual && value == condition.threshold )
						|| ( condition.mode == AnimatorConditionMode::Less && value >= condition.threshold )
						|| ( condition.mode == AnimatorConditionMode::Greater && value <= condition.threshold ) )
					{
						success = false;
					}
					break;
				}
				case AnimatorControllerParameterType::Float:
				{
					float value = GetFloat( condition.parameter );
					if( ( condition.mode == AnimatorConditionMode::Equals && value != condition.threshold )
						|| ( condition.mode == AnimatorConditionMode::NotEqual && value == condition.threshold )
						|| ( condition.mode == AnimatorConditionMode::Less && value >= condition.threshold )
						|| ( condition.mode == AnimatorConditionMode::Greater && value <= condition.threshold ) )
					{
						success = false;
					}
					break;
				}
				default:
					break;
				}
				if( !success )
					break;
			}
			if( success )
			{
				m_CurrentAnimationState = transition.DestinationState;
				if( ( *m_pCurrrentAnimation )[0].second != m_Animations[m_CurrentAnimationState][0].second )
				{
					m_pSprite->SetTexture( m_Animations[m_CurrentAnimationState][0].second );

				}
				m_pCurrrentAnimation = &m_Animations[m_CurrentAnimationState];
				m_CurrentAnimationFrame = 0;
				m_Time = 0.f;
				break;
			}
		}
	}
}

void Balbino::Animation::Draw() const
{
}

void Balbino::Animation::Save( std::ostream& file )
{
	(void) file;
	BinaryReadWrite::Write( file, int( ComponentList::Animation ) );
	BinaryReadWrite::Write( file, m_CurrentAnimationState );
	BinaryReadWrite::Write( file, m_CurrentAnimationFrame );
	BinaryReadWrite::Write( file, m_Time );

	BinaryReadWrite::Write( file, m_AnimationsFiles);
	BinaryReadWrite::Write( file, m_AnimationTransitions);
	BinaryReadWrite::Write( file, m_Animations);

	BinaryReadWrite::Write( file, m_BoolTypes);
	BinaryReadWrite::Write( file, m_TriggerTypes);
	BinaryReadWrite::Write( file, m_FloatTypes);
	BinaryReadWrite::Write( file, m_IntTypes);
}

void Balbino::Animation::Load( std::istream& file )
{
	(void) file;
	BinaryReadWrite::Read( file, m_CurrentAnimationState );
	BinaryReadWrite::Read( file, m_CurrentAnimationFrame );
	BinaryReadWrite::Read( file, m_Time );

	BinaryReadWrite::Read( file, m_AnimationsFiles );
	BinaryReadWrite::Read( file, m_AnimationTransitions );
	BinaryReadWrite::Read( file, m_Animations );

	BinaryReadWrite::Read( file, m_BoolTypes );
	BinaryReadWrite::Read( file, m_TriggerTypes );
	BinaryReadWrite::Read( file, m_FloatTypes );
	BinaryReadWrite::Read( file, m_IntTypes );
}

void Balbino::Animation::SetBool( const std::string& name, bool value )
{
	if( m_BoolTypes.find( name ) != m_BoolTypes.end() )
	{
		m_BoolTypes[name] = value;
	}
}

void Balbino::Animation::SetTrigger( const std::string& name, bool value )
{
	if( m_TriggerTypes.find( name ) != m_TriggerTypes.end() )
	{
		m_TriggerTypes[name] = value;
	}
}

void Balbino::Animation::SetInt( const std::string& name, int value )
{
	if( m_IntTypes.find( name ) != m_IntTypes.end() )
	{
		m_IntTypes[name] = value;
	}
}

void Balbino::Animation::SetFloat( const std::string& name, float value )
{
	if( m_FloatTypes.find( name ) != m_FloatTypes.end() )
	{
		m_FloatTypes[name] = value;
	}
}

int Balbino::Animation::GetInt( const std::string& name ) const
{
	if( m_IntTypes.find( name ) != m_IntTypes.end() )
	{
		return m_IntTypes.at( name );
	}
	Debug::LogError( name + " Is not part of this Animation" );
	return 0;
}

float Balbino::Animation::GetFloat( const std::string& name ) const
{
	if( m_FloatTypes.find( name ) != m_FloatTypes.end() )
	{
		return m_FloatTypes.at( name );
	}
	Debug::LogError( name + " Is not part of this Animation" );
	return 0;
}

bool Balbino::Animation::GetBool( const std::string& name ) const
{
	if( m_BoolTypes.find( name ) != m_BoolTypes.end() )
	{
		return m_BoolTypes.at( name );
	}
	Debug::LogError( name + " Is not part of this Animation" );
	return false;
}

bool Balbino::Animation::GetTrigger( const std::string& name ) const
{
	if( m_TriggerTypes.find( name ) != m_TriggerTypes.end() )
	{
		return m_TriggerTypes.at( name );
	}
	Debug::LogError( name + " Is not part of this Animation" );
	return false;
}

void Balbino::Animation::AddBool( const std::string& name )
{
	if( m_BoolTypes.find( name ) == m_BoolTypes.end() )
	{
		m_BoolTypes.insert( std::make_pair( name, false ) );
	}
}

void Balbino::Animation::AddTrigger( const std::string& name )
{
	if( m_TriggerTypes.find( name ) == m_TriggerTypes.end() )
	{
		m_TriggerTypes.insert( std::make_pair( name, false ) );
	}
}

void Balbino::Animation::AddInt( const std::string& name )
{
	if( m_IntTypes.find( name ) == m_IntTypes.end() )
	{
		m_IntTypes.insert( std::make_pair( name, false ) );
	}
}

void Balbino::Animation::AddFloat( const std::string& name )
{
	if( m_IntTypes.find( name ) == m_IntTypes.end() )
	{
		m_IntTypes.insert( std::make_pair( name, false ) );
	}
}

void Balbino::Animation::RemoveBool( const std::string& name )
{
	if( m_BoolTypes.find( name ) == m_BoolTypes.end() )
	{
		m_BoolTypes.erase( name );
	}
}

void Balbino::Animation::RemoveTrigger( const std::string& name )
{
	if( m_TriggerTypes.find( name ) == m_TriggerTypes.end() )
	{
		m_TriggerTypes.erase( name );
	}
}

void Balbino::Animation::RemoveInt( const std::string& name )
{
	if( m_IntTypes.find( name ) == m_IntTypes.end() )
	{
		m_IntTypes.erase( name );
	}
}

void Balbino::Animation::RemoveFloat( const std::string& name )
{
	if( m_IntTypes.find( name ) == m_IntTypes.end() )
	{
		m_IntTypes.erase( name );
	}
}

#ifdef _DEBUG
void Balbino::Animation::DrawInpector()
{
	int animationSize{ int( m_Animations.size() ) };
	int size{ int( m_AnimationsFiles.size() ) };
	int transitionSize{ int( m_AnimationTransitions.size() ) };

	bool chaingedSize{ false };
	ImGui::BeginChild( "Animation", ImVec2{ -1, 128 }, true );
	ImGui::Text( "Animations Component" );
	ImGui::Text( "Nr of Animations" ); ImGui::SameLine();
	chaingedSize = ImGui::DragInt( "##nrOfAnimatins", &size );

	for( int i = 0; i < size; i++ )
	{
		char inputField[256]{ "Empty" };
		int inputSize{};
		if( i < int( m_AnimationsFiles.size() ) )
		{
			inputSize = ( std::min )( int( m_AnimationsFiles[i].size() ), 256 );
		}
		for( int j = 0; j < inputSize; j++ )
		{
			inputField[j] = m_AnimationsFiles[i][j];
		}

		ImGui::InputText( "Image Path", inputField, 256 );
		if( ImGui::BeginDragDropTarget() )
		{
			if( const ImGuiPayload* payload = ImGui::AcceptDragDropPayload( "asset" ) )
			{
				file droppedFile = *(const file*) payload->Data;
				if( droppedFile.alias.find( ".bani" ) != std::string::npos )
				{
					m_AnimationsFiles[i] = droppedFile.path.string();
					std::ifstream file{ "../Data/" + m_AnimationsFiles[i] };
					if( file.is_open() )
					{
						std::string line;
						std::regex timeRegex{ R"(^- time:\s(\d+\.?\d*)$)" };
						std::regex valueRegex{ R"(^\svalue:\s(\d+)$)" };
						std::regex spriteRegex{ R"(^\sSprite:\s(.+)$)" };
						std::smatch match;
						std::vector < std::pair<std::pair<float, int>, std::string>> animation;
						while( true )
						{
							float time{};
							int spritePos{};
							std::string fileName{};
							std::getline( file, line );
							if( std::regex_match( line, match, timeRegex ) )
							{
								time = std::stof( match[1].str() );
							}
							else
							{
								break;
							}
							std::getline( file, line );
							if( std::regex_match( line, match, valueRegex ) )
							{
								spritePos = std::stoi( match[1].str() );
							}
							else
							{
								break;
							}
							std::getline( file, line );
							if( std::regex_match( line, match, spriteRegex ) )
							{
								fileName = match[1].str();
							}
							else
							{
								break;
							}
							animation.push_back( std::make_pair( std::make_pair( time, spritePos ), fileName ) );
							if( file.eof() )
							{
								break;
							}
						}
						m_Animations.push_back( animation );
						m_AnimationTransitions.push_back( AnimatorTransition{} );
					}
				}
			}
			ImGui::EndDragDropTarget();
		}
	}

	for( int i = 0; i < transitionSize; i++ )
	{
		std::vector<Balbino::AnimatorCondition>& conditions = m_AnimationTransitions[i].GetConditions();
		int conditionsSize{ int( conditions.size() ) };

		ImGui::PushID( i );
		ImGui::Separator();
		if( ImGui::Button( "+" ) )
		{
			m_AnimationTransitions[i].AddCondition( Balbino::AnimatorControllerParameterType::Bool, Balbino::AnimatorConditionMode::If, 0.f, "Parameter" );
		}
		ImGui::DragInt( "From", &m_AnimationTransitions[i].SourceState );
		ImGui::DragInt( "To", &m_AnimationTransitions[i].DestinationState );
		ImGui::PushItemWidth( 80 );
		for( int j = 0; j < conditionsSize; j++ )
		{
			static const char* type[4]{ "Float","Int","Bool","Trigger" };
			static const char* mode[6]{ "If", "If Not", "Greater", "Less", "Equals", "Not Equals" };
			char name[128]{ "" };
			for( int k = 0; i < 128; k++ )
			{
				if( k == int( conditions[j].parameter.size() ) )
					break;
				name[k] = conditions[j].parameter[k];
			}
			if( ImGui::BeginCombo( "##Type", type[int( conditions[j].type )] ) )
			{
				ImGui::PushID( j );
				for( int k = 0; k < 4; k++ )
				{
					ImGui::PushID( k );
					if( ImGui::Selectable( type[k] ) )
					{
						conditions[j].type = AnimatorControllerParameterType( k );
					}
					ImGui::PopID();
				}
				ImGui::PopID();
				ImGui::EndCombo();
			}
			ImGui::SameLine();
			ImGui::InputText( "##name", name, 128 );
			conditions[j].parameter = name;
			ImGui::SameLine();
			if( ImGui::BeginCombo( "##Mode", mode[int( conditions[j].mode )] ) )
			{
				ImGui::PushID( j );
				for( int k = 0; k < 4; k++ )
				{
					ImGui::PushID( k );
					if( ImGui::Selectable( mode[k] ) )
					{
						conditions[j].mode = AnimatorConditionMode( k );
					}
					ImGui::PopID();
				}
				ImGui::PopID();
				ImGui::EndCombo();
			}
			ImGui::SameLine();
			ImGui::DragFloat( "##value", &conditions[j].threshold );
		}
		ImGui::PopItemWidth();
		ImGui::PopID();
	}

	if( ImGui::Button( "Set Variables" ) )
	{
		m_BoolTypes.clear();
		m_TriggerTypes.clear();
		m_IntTypes.clear();
		m_FloatTypes.clear();
		for( AnimatorTransition animationTransition : m_AnimationTransitions )
		{
			auto conditions = animationTransition.GetConditions();
			for( auto condition : conditions )
			{
				switch( condition.type )
				{
				case AnimatorControllerParameterType::Trigger:
				{
					AddTrigger( condition.parameter );
					break;
				}
				case AnimatorControllerParameterType::Bool:
				{
					AddBool( condition.parameter );
					break;
				}
				case AnimatorControllerParameterType::Int:
				{
					AddInt( condition.parameter );
					break;
				}
				case AnimatorControllerParameterType::Float:
				{
					AddFloat( condition.parameter );
					break;
				}
				default:
					break;
				}
			}
		}
	}
	ImGui::EndChild();
	if( chaingedSize )
	{
		m_AnimationsFiles.resize( size );
	}
	if( animationSize != int( m_Animations.size() ) )
	{
		m_pCurrrentAnimation = &m_Animations[0];
		m_pSprite->SetTexture( ( *m_pCurrrentAnimation )[0].second );
		m_pSprite->LoadUV();
		m_pSprite->SetSpriteIndex( ( *m_pCurrrentAnimation )[0].first.second );

	}
	else if( !m_Animations.size() )
	{
		m_pCurrrentAnimation = nullptr;
	}
}
#endif // _DEBUG
