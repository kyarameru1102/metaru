#include "stdafx.h"
#include "NaviMesh.h"


typedef std::vector<CVector3>					VertexBuffer;		//頂点バッファ。
typedef std::vector<unsigned int>				IndexBuffer;		//インデックスバッファ。
typedef std::unique_ptr<VertexBuffer>			VertexBufferPtr;
typedef std::unique_ptr<IndexBuffer>			IndexBufferPtr;
typedef std::vector<IndexBufferPtr>				in;
typedef std::vector<VertexBufferPtr>			ve;

struct MeshCallBack : public btCollisionWorld::ClosestConvexResultCallback
{	
	bool hit = false;
	MeshCallBack() :
		btCollisionWorld::ClosestConvexResultCallback(btVector3(0.0f, 0.0f, 0.0f), btVector3(0.0f, 0.0f, 0.0f))
		 {}
	virtual	btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		hit = true;
		return 0.0;
	}
};

NaviMesh::NaviMesh()
{
	m_collider.Create(1.0f, 30.0f);
}


NaviMesh::~NaviMesh()
{
}

void NaviMesh::Create(SkinModel & model)
{
	CMatrix mBias;
	mBias.MakeRotationX(CMath::PI * -0.5f);
	//m_stridingMeshInterface = std::make_unique<btTriangleIndexVertexArray>();
	in indexBufferArray;
	ve vertexBufferArray;
	model.FindMesh([&](const auto& mesh) {
		ID3D11DeviceContext* deviceContext = g_graphicsEngine->GetD3DDeviceContext();
		//頂点バッファを作成。
		{
			D3D11_MAPPED_SUBRESOURCE subresource;
			HRESULT hr = deviceContext->Map(mesh->vertexBuffer.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &subresource);
			if (FAILED(hr)) {
				return;
			}
			D3D11_BUFFER_DESC bufferDesc;
			mesh->vertexBuffer->GetDesc(&bufferDesc);
			int vertexCount = bufferDesc.ByteWidth / mesh->vertexStride;
			char* pData = reinterpret_cast<char*>(subresource.pData);
			VertexBufferPtr vertexBuffer = std::make_unique<VertexBuffer>();
			CVector3 pos;
			for (int i = 0; i < vertexCount; i++) {
				pos = *reinterpret_cast<CVector3*>(pData);
				//バイアスをかける。
				mBias.Mul(pos);
				vertexBuffer->push_back(pos);
				//次の頂点へ。
				pData += mesh->vertexStride;
			}
			//頂点バッファをアンロック
			deviceContext->Unmap(mesh->vertexBuffer.Get(), 0);
			vertexBufferArray.push_back(std::move(vertexBuffer));
		}
		//インデックスバッファを作成。
		{
			D3D11_MAPPED_SUBRESOURCE subresource;
			//インデックスバッファをロック。
			HRESULT hr = deviceContext->Map(mesh->indexBuffer.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &subresource);
			if (FAILED(hr)) {
				return;
			}
			D3D11_BUFFER_DESC bufferDesc;
			mesh->indexBuffer->GetDesc(&bufferDesc);
			//@todo cmoファイルはインデックスバッファのサイズは2byte固定。
			IndexBufferPtr indexBuffer = std::make_unique<IndexBuffer>();
			int stride = 2;
			int indexCount = bufferDesc.ByteWidth / stride;
			unsigned short* pIndex = reinterpret_cast<unsigned short*>(subresource.pData);
			for (int i = 0; i < indexCount; i++) {
				indexBuffer->push_back(pIndex[i]);
			}
			//インデックスバッファをアンロック。
			deviceContext->Unmap(mesh->indexBuffer.Get(), 0);
			indexBufferArray.push_back(std::move(indexBuffer));
		}

		//インデックスメッシュを作成。
		/*btIndexedMesh indexedMesh;
		IndexBuffer* ib = m_indexBufferArray.back().get();
		VertexBuffer* vb = m_vertexBufferArray.back().get();
		indexedMesh.m_numTriangles = (int)ib->size() / 3;
		indexedMesh.m_triangleIndexBase = (unsigned char*)(&ib->front());
		indexedMesh.m_triangleIndexStride = 12;
		indexedMesh.m_numVertices = (int)vb->size();
		indexedMesh.m_vertexBase = (unsigned char*)(&vb->front());
		indexedMesh.m_vertexStride = sizeof(CVector3);
		m_stridingMeshInterface->addIndexedMesh(indexedMesh);*/


		});
	for (int i = 0; i < indexBufferArray.size(); i++) {
		for (int j = 0; j < indexBufferArray[i]->size();) {
			Cell* cell = new Cell;
			cell->vertexPos[0] = vertexBufferArray[i]->at(indexBufferArray[i]->at(j++));
			cell->vertexPos[1] = vertexBufferArray[i]->at(indexBufferArray[i]->at(j++));
			cell->vertexPos[2] = vertexBufferArray[i]->at(indexBufferArray[i]->at(j++));

			//上にオブジェクトがあるかどうか。
			CVector3 centerPos = cell->vertexPos[0] + cell->vertexPos[1];
			centerPos += cell->vertexPos[2];
			centerPos /= 3.0f;
			CVector3 radius;
			radius = centerPos - cell->vertexPos[0]; 
			m_collider.GetBody()->setLocalScaling(btVector3(radius.Length(),1.0f,radius.Length()));
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			start.setOrigin(btVector3(centerPos.x, centerPos.y, centerPos.z));
			end.setOrigin(btVector3(centerPos.x, centerPos.y + 10.0f, centerPos.z));
			MeshCallBack callback;
			
			g_physics.ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), start, end, callback);
			if (callback.hit == false) {
				//オブジェクトが上にあったら消えているかどうかの確認用。
				m_skin = NewGO<SkinModelRender>(0);
				m_skin->Init(L"Assets/modelData/takatozin.cmo");
				m_skin->SetPosition(centerPos);

				m_cells.push_back(cell);
			}
			else
			{
				int i = 0;

			}
		}
	}

		for (auto &all : m_cells) {
			for (const auto &portion : m_cells) {
				if (all != portion) {
					int CommonVertex = 0;
					int vertexNo[2] = { 0,0 };
					for (int i = 0; i < 3 && CommonVertex < 2; i++) {
						CVector3 pos;
						pos = all->vertexPos[i];
						for (int j = 0; j < 3; j++) {
							CVector3 len;
							len = pos - portion->vertexPos[j];
							if (len.Length() <= 0.01f) {
								vertexNo[CommonVertex] = i;
								CommonVertex++;
								if (CommonVertex == 2) {
									if (vertexNo[0] == 0 && vertexNo[1] == 1 || vertexNo[0] == 1 && vertexNo[1] == 0) {
										all->linkCells[0] = portion;
									}
									else if (vertexNo[0] == 1 && vertexNo[1] == 2 || vertexNo[0] == 2 && vertexNo[1] == 1) {
										all->linkCells[1] = portion;
									}
									else if (vertexNo[0] == 2 && vertexNo[1] == 0 || vertexNo[0] == 0 && vertexNo[1] == 2) {
										all->linkCells[2] = portion;
									}
									break;
								}
							}
						}
					}
				}
			}
		}

	int i = 0;
}
