#include "imgui_extra.h"

void ImGui::Begin( const char* name, bool* p_open, ImGuiWindowFlags flags, const ImVec4& color )
{
	ImGui::PushStyleColor( ImGuiCol_Text, color );
	ImGui::Begin( name, p_open, flags );
	ImGui::PopStyleColor();
}
