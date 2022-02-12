#include "../IEntity.h"
#include "../../Renderer/GraphicsVulkan/Buffer.h"

namespace BalVulkan
{
	class CBuffer;
	class CDevice;
	class CQueue;
	class CCommandPool;
}

class CMeshRenderComponent final
{
public:
	CMeshRenderComponent( IEntity* pEntity, const BalVulkan::CDevice* pDevice, const BalVulkan::CCommandPool* pCommandPool, const BalVulkan::CQueue* pQueue )
		: m_pEntity{ pEntity }
		, m_indexBuffer{ BalVulkan::CBuffer::CreateNew( pDevice, pCommandPool, pQueue ) }
		, m_vertexBuffer{ BalVulkan::CBuffer::CreateNew( pDevice, pCommandPool, pQueue ) }
	{
	}

	~CMeshRenderComponent() = default;
	CMeshRenderComponent( const CMeshRenderComponent& ) = delete;
	CMeshRenderComponent( CMeshRenderComponent&& ) = delete;
	CMeshRenderComponent& operator=( const CMeshRenderComponent& ) = delete;
	CMeshRenderComponent& operator=( CMeshRenderComponent&& ) = delete;

	void Initialize();

	[[nodiscard]] IEntity* GetEntity() const
	{
		return m_pEntity;
	}

private:
	IEntity* m_pEntity;
	BalVulkan::CBuffer* m_indexBuffer;
	BalVulkan::CBuffer* m_vertexBuffer;
};
