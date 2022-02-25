#pragma once

class TerrainLOD : public Renderer
{
public:
	TerrainLOD(wstring imageFile);
	~TerrainLOD();

	void Update();
	void Render();

	void BaseMap(wstring file);
	void NormalMap(wstring file);

public:
	float GetWidth();
	float GetHeight();

public:
	Perspective* GetPerspective() { return perspective; }
	Camera* GetCamera() { return camera; }

	float& HeightScale() { return desc.HeightScale; }
	Vector2& Distance() { return desc.Distance; }

private:
	void ReadHeightData();
	void CreatePatchBound();
	void CreatePatchVertex();
	void CreatePatchIndex();

private:
	struct TerrainVertex
	{
		Vector3 Position;
		Vector2 Uv;

		Vector2 Bound;
	};

private:
	struct Desc
	{
		Vector2 Distance = Vector2(1, 1000); //LOD Min, Max �Ÿ�
		Vector2 TessFactor = Vector2(1, 64); //Factor Min, Max ����

		float CellSpacing = 2.0f; //1ĭ�� ũ��� 2
		float CellSpacingU; // 1/��ü����ũ��
		float CellSpacingV; // 1/��ü����ũ��
		float HeightScale = 1.5f; //���� ������

		Plane Culling[6];
	} desc;

private:
	UINT cellPerPatch = 32; //������ ��ĭ ������ ��������

	wstring imageFile; //�о�� dds ����

	UINT width, height; //�о�� �ؽ����� ũ��
	UINT patchWidth, patchHeight; //Patch�� ����

	float* heights;
	Vector2* bounds;

	ID3D11Texture2D* heightMap = nullptr;
	ID3D11ShaderResourceView* heightMapSRV = nullptr;
	ID3DX11EffectShaderResourceVariable* sHeightMapSRV;

	ConstantBuffer* buffer;
	ID3DX11EffectConstantBuffer* sBuffer;

	Texture* baseMap = nullptr;
	ID3DX11EffectShaderResourceVariable* sBaseMap;

	Texture* normalMap = nullptr;
	ID3DX11EffectShaderResourceVariable* sNormalMap;

	class Camera* camera;
	class Perspective* perspective;
	class Frustum* frustum;
};