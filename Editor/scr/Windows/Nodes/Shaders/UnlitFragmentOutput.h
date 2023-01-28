//
// Created by joran on 28/01/2023.
//

#ifndef BALBINO_UNLITFRAGMENTOUTPUT_H
#define BALBINO_UNLITFRAGMENTOUTPUT_H
#include <array>
#include "../INode.h"

class CUnlitFragmentOutput final : public INode
{
public:
    CUnlitFragmentOutput( int id, int& attributeStartId );
    ~CUnlitFragmentOutput() override = default;
    CUnlitFragmentOutput( const CUnlitFragmentOutput& ) = delete;
    CUnlitFragmentOutput( CUnlitFragmentOutput&& ) = delete;
    CUnlitFragmentOutput& operator=( const CUnlitFragmentOutput& ) = delete;
    CUnlitFragmentOutput& operator=( CUnlitFragmentOutput&& ) = delete;
    
    void Draw() override;
    void Attach( int endAttr ) override;
    void Detach( int endAttr ) override;
    std::string Evaluate( std::vector<INode*>::iterator& begin, std::set<std::string>& bindings, std::set<std::string>& includes, EAttributeType::Enum attributeType ) override;
    [[nodiscard]] bool HasFreeAttachment( int endAttr ) const override;
    [[nodiscard]] int GetId() const override;
    [[nodiscard]] std::vector<int> GetInputs() const override;
    [[nodiscard]] uint8_t GetVertexFlags() const override;
    
private:
    bool                 m_connected;
    std::array<float, 3> m_color{ 0.973f, 0.71f, 0.796f };
};


#endif //BALBINO_UNLITFRAGMENTOUTPUT_H