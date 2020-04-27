#pragma once
#include <string>
#include <filesystem>
namespace Balbino
{
	const enum fileTypes
	{
		folder,
		scene,
		image,
		audio,
		font,
		cppch,
		unknown
	};
	const struct file
	{
		bool isFolder;
		fileTypes type;
		std::string alias;
		std::filesystem::path path;
	};
}