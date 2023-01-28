//
// Created by joran on 05/12/2022.
//

#ifndef BALBINO_CCAMERADATANODE_H
#define BALBINO_CCAMERADATANODE_H
#include "../I"

class CCCameraDataNode
{
public:
    CCCameraDataNode() = default;
    ~CCCameraDataNode() = default;
    CCCameraDataNode( const CCCameraDataNode& ) = delete;
    CCCameraDataNode( CCCameraDataNode&& ) = delete;
    CCCameraDataNode& operator=( const CCCameraDataNode& ) = delete;
    CCCameraDataNode& operator=( CCCameraDataNode&& ) = delete;
    
    void Initialize();
    void Cleanup();
};


#endif //BALBINO_CCAMERADATANODE_H