//
// Created by joran on 05/12/2022.
//

#ifndef BALBINO_CCAMERADATANODE_H
#define BALBINO_CCAMERADATANODE_H
#include "../INode.h"

class CCameraView final : public INode
{
public:
    CCameraView( int id, int& attributeStartId );
    ~CCameraView() override = default;
    CCameraView( const CCameraView& ) = delete;
    CCameraView( CCameraView&& ) = delete;
    CCameraView& operator=( const CCameraView& ) = delete;
    CCameraView& operator=( CCameraView&& ) = delete;
    
    void Draw() override;
    void Attach( int endAttr ) override;
    void Detach( int endAttr ) override;
    std::string Evaluate( std::vector<INode*>::iterator& begin, std::set<std::string>& bindings, std::set<std::string>& includes, EAttributeType::Enum attributeType ) override;
    [[nodiscard]] bool HasFreeAttachment( int endAttr ) const override;
    [[nodiscard]] int GetId() const override;
    [[nodiscard]] std::vector<int> GetInputs() const override;
    [[nodiscard]] uint8_t GetVertexFlags() const override;
};

class CCameraZ final : public INode
{
public:
    CCameraZ( int id, int& attributeStartId );
    ~CCameraZ() override = default;
    CCameraZ( const CCameraZ& ) = delete;
    CCameraZ( CCameraZ&& ) = delete;
    CCameraZ& operator=( const CCameraZ& ) = delete;
    CCameraZ& operator=( CCameraZ&& ) = delete;
    
    void Draw() override;
    void Attach( int endAttr ) override;
    void Detach( int endAttr ) override;
    std::string Evaluate( std::vector<INode*>::iterator& begin, std::set<std::string>& bindings, std::set<std::string>& includes, EAttributeType::Enum attributeType ) override;
    [[nodiscard]] bool HasFreeAttachment( int endAttr ) const override;
    [[nodiscard]] int GetId() const override;
    [[nodiscard]] std::vector<int> GetInputs() const override;
    [[nodiscard]] uint8_t GetVertexFlags() const override;
};

class CCameraDepth final : public INode
{
public:
    CCameraDepth( int id, int& attributeStartId );
    ~CCameraDepth() override = default;
    CCameraDepth( const CCameraDepth& ) = delete;
    CCameraDepth( CCameraDepth&& ) = delete;
    CCameraDepth& operator=( const CCameraDepth& ) = delete;
    CCameraDepth& operator=( CCameraDepth&& ) = delete;
    
    void Draw() override;
    void Attach( int endAttr ) override;
    void Detach( int endAttr ) override;
    std::string Evaluate( std::vector<INode*>::iterator& begin, std::set<std::string>& bindings, std::set<std::string>& includes, EAttributeType::Enum attributeType ) override;
    [[nodiscard]] bool HasFreeAttachment( int endAttr ) const override;
    [[nodiscard]] int GetId() const override;
    [[nodiscard]] std::vector<int> GetInputs() const override;
    [[nodiscard]] uint8_t GetVertexFlags() const override;
};

#endif //BALBINO_CCAMERADATANODE_H