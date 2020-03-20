#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec3.hpp>
#pragma warning(pop)
#include "Struct.h"
#include "Singleton.h"
#include <deque>
#include <memory>

namespace Balbino
{
	class Text;
	class Debug:public Singleton<Debug>
	{
	public:
		static void Assert( bool argument );						//Assert a conditionand logs an error message to the Unity console on failure.
		static void DrawLine( const glm::vec3& start, const glm::vec3& end, const Color& color = { 255,255,255 } );	//Draws a line between specified startand end points.
		static void DrawRay( const glm::vec3& start, const glm::vec3& direction, const Color& color = { 255,255,255 } );									//Draws a line from start to start + dir in world coordinates.
		static void Log( const std::string& log );										//Logs a message to the Unity Console.
		static void LogError( const std::string& error );									//A variant of Debug.Log that logs an error message to the console.
		static void LogException( const std::string& exeption );								//A variant of Debug.Log that logs an error message from an exception to the console.
		static void LogWarning( const std::string& warning );								//A variant of Debug.Log that logs a warning message to the console.

#ifdef _DEBUG
	private:
		std::deque<std::shared_ptr<Text>> m_Log;

		void ILog( const std::string& log );										//Logs a message to the Unity Console.
		void ILogError( const std::string& error );									//A variant of Debug.Log that logs an error message to the console.
		void ILogException( const std::string& exeption );								//A variant of Debug.Log that logs an error message from an exception to the console.
		void ILogWarning( const std::string& warning );

#endif //_DEBUG
	};
}