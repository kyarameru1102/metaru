#include "stdafx.h"
#include "NaviMesh.h"


typedef std::vector<CVector3>					VertexBuffer;		//���_�o�b�t�@�B
typedef std::vector<unsigned int>				IndexBuffer;		//�C���f�b�N�X�o�b�t�@�B
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
	//�i�r���b�V���B
	m_collider.Create(1.0f, 30.0f);
	m_model.Init(L"Assets/modelData/ground_test.cmo");
	Create(m_model);
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
		//���_�o�b�t�@���쐬�B
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
				//�o�C�A�X��������B
				mBias.Mul(pos);
				vertexBuffer->push_back(pos);
				//���̒��_�ցB
				pData += mesh->vertexStride;
			}
			//���_�o�b�t�@���A�����b�N
			deviceContext->Unmap(mesh->vertexBuffer.Get(), 0);
			vertexBufferArray.push_back(std::move(vertexBuffer));
		}
		//�C���f�b�N�X�o�b�t�@���쐬�B
		{
			D3D11_MAPPED_SUBRESOURCE subresource;
			//�C���f�b�N�X�o�b�t�@�����b�N�B
			HRESULT hr = deviceContext->Map(mesh->indexBuffer.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &subresource);
			if (FAILED(hr)) {
				return;
			}
			D3D11_BUFFER_DESC bufferDesc;
			mesh->indexBuffer->GetDesc(&bufferDesc);
			//@todo cmo�t�@�C���̓C���f�b�N�X�o�b�t�@�̃T�C�Y��2byte�Œ�B
			IndexBufferPtr indexBuffer = std::make_unique<IndexBuffer>();
			int stride = 2;
			int indexCount = bufferDesc.ByteWidth / stride;
			unsigned short* pIndex = reinterpret_cast<unsigned short*>(subresource.pData);
			for (int i = 0; i < indexCount; i++) {
				indexBuffer->push_back(pIndex[i]);
			}
			//�C���f�b�N�X�o�b�t�@���A�����b�N�B
			deviceContext->Unmap(mesh->indexBuffer.Get(), 0);
			indexBufferArray.push_back(std::move(indexBuffer));
		}

		});
	for (int i = 0; i < indexBufferArray.size(); i++) {
		for (int j = 0; j < indexBufferArray[i]->size();) {
			Cell* cell = new Cell;
			cell->vertexPos[0] = vertexBufferArray[i]->at(indexBufferArray[i]->at(j++));
			cell->vertexPos[1] = vertexBufferArray[i]->at(indexBufferArray[i]->at(j++));
			cell->vertexPos[2] = vertexBufferArray[i]->at(indexBufferArray[i]->at(j++));

			//��ɃI�u�W�F�N�g�����邩�ǂ����B
			cell->centerPos = cell->vertexPos[0] + cell->vertexPos[1];
			cell->centerPos += cell->vertexPos[2];
			cell->centerPos /= 3.0f;
			CVector3 radius;
			radius = cell->centerPos - cell->vertexPos[0];
			m_collider.GetBody()->setLocalScaling(btVector3(radius.Length(), 1.0f, radius.Length()));
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			start.setOrigin(btVector3(cell->centerPos.x, cell->centerPos.y, cell->centerPos.z));
			end.setOrigin(btVector3(cell->centerPos.x, cell->centerPos.y + 10.0f, cell->centerPos.z));
			MeshCallBack callback;

			g_physics.ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), start, end, callback);
			if (callback.hit == false) {
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
									all->linkMax++;
								}
								else if (vertexNo[0] == 1 && vertexNo[1] == 2 || vertexNo[0] == 2 && vertexNo[1] == 1) {
									all->linkCells[1] = portion;
									all->linkMax++;
								}
								else if (vertexNo[0] == 2 && vertexNo[1] == 0 || vertexNo[0] == 0 && vertexNo[1] == 2) {
									all->linkCells[2] = portion;
									all->linkMax++;
								}
								break;
							}
						}
					}
				}
			}
		}
	}
	//����Ȃ��Z�����폜�B
	auto &itr = m_cells.begin();
	while (itr != m_cells.end())
	{
		//�Z�����Ƃ̖@�����v�Z����B
		CVector3 v1, v2, nomal;
		v1 = (*itr)->vertexPos[1] - (*itr)->vertexPos[0];
		v2 = (*itr)->vertexPos[2] - (*itr)->vertexPos[0];
		nomal.Cross(v1, v2);
		nomal.Normalize();		//�@���B
		//�X�΂��傫���Z���͏��O�������̂ŁB
		//�p�x�̌v�Z�B
		CVector3 up = { 0.0f,-1.0f,0.0f };
		up.Normalize();
		float angle = up.Dot(nomal);
		angle = acos(angle);
		angle = fabsf(angle);
		float aaa = CMath::RadToDeg(angle);
		
		//�p�x�������Z���ƃ����N���Ȃ��Z���̃R�X�g�������ݒ肷��B
		if ((aaa > 40.0f) || ((*itr)->linkMax == 0))
		{
			/*m_skin = NewGO<SkinModelRender>(0);
			m_skin->Init(L"Assets/modelData/yazirusi.cmo");
			m_skin->SetPosition((*itr)->centerPos);
			m_skin->SetScale({ 10.0f,10.0f,10.0f });*/
			(*itr)->passingCellCost = 99999.0f;
			
			itr = m_cells.erase(itr);
		}
		else
		{
			itr++;
		}
	}

	
}

void NaviMesh::Update()
{
	if (g_pad[0].IsTrigger(enButtonY)) {
		if (!m_debugCount) {
			m_debugCount = true;
		}
		else {
			m_debugCount = false;
			m_debugCount2 = 0;
			DeleteGOs("bou1");
			DeleteGOs("bou2");
		}
	}
	if (m_debugCount) {
		if (m_debugCount2 <= 0) {
			//�f�o�b�O�p�����N�m�F�B
			for (int i = 0; i < m_cells.size(); i++)
			{
				CVector3 c_position;
				c_position = m_cells[i]->centerPos;
				for (int j = 0; j < 3; j++) {
					if (m_cells[i]->linkCells[j] != nullptr)
					{
						auto cell = m_cells[i]->linkCells[j];
						CVector3 Vector = CVector3::Zero();
						Vector = cell->centerPos - c_position;
						Vector.Normalize();
						CQuaternion Rot = CQuaternion::Identity();
						float kakuo = acos(Vector.Dot(CVector3::AxisY()));
						if (kakuo > 0.0f || kakuo < -FLT_MIN)
						{
							kakuo = CMath::RadToDeg(kakuo);
							CVector3 jiku;
							jiku.Cross(CVector3::AxisY(), Vector);
							if (kakuo > 0.0f || kakuo < -FLT_MIN)
							{
								jiku.Normalize();
								Rot.SetRotationDeg(jiku, kakuo);
							}

						}
						m_skin = NewGO<SkinModelRender>(0,"bou1");
						m_skin->Init(L"Assets/modelData/bou.cmo");
						m_skin->SetPosition(cell->centerPos);
						m_skin->SetScale({ 1.0f,1.0f,1.0f });

						m_skin2 = NewGO<SkinModelRender>(0,"bou2");
						m_skin2->Init(L"Assets/modelData/bou.cmo");
						m_skin2->SetPosition(c_position);
						m_skin2->SetRotation(Rot);
						m_skin2->SetScale({ 2.0f,2.0f,2.0f });
					}
				}
			}
		}
		m_debugCount2++;
	}
}