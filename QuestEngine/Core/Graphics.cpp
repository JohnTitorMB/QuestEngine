#include "Graphics.h"
#include <GLFW/glfw3.h>
#include "AssetsManager.h"
#include "QuestEngine.h"

Graphics* Graphics::instance = nullptr;

Graphics::Graphics()
	: m_antiAliasingType(AntiAliasingType::None), m_MSAASample(4), m_SSAAMultiplier(1), RefreshMSAASampleEvent(Event<int>())
{
}

Graphics* Graphics::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new Graphics();
	}
	return instance;
}

void Graphics::SetAntiAliasingType(AntiAliasingType antiAliasingType)
{
	AntiAliasingType prev = m_antiAliasingType;

	if (prev != AntiAliasingType::MSAA && antiAliasingType == AntiAliasingType::MSAA)
		RefreshMSAASampleEvent.Trigger(m_MSAASample);

	if(prev == AntiAliasingType::MSAA && antiAliasingType != AntiAliasingType::MSAA)
		RefreshMSAASampleEvent.Trigger(0);

	m_antiAliasingType = antiAliasingType;
}

Graphics::AntiAliasingType Graphics::GetAntiAliasingType()
{
	return m_antiAliasingType;
}

void Graphics::Clear(CameraComponent* camera, int x, int y, int width, int height)
{
	glEnable(GL_SCISSOR_TEST);
	glScissor(x, y, width, height);
	glStencilMask(camera->m_clearStencilMask);
	glDepthMask(camera->m_isClearDepthMaskEnable);
	glClearColor(camera->m_clearColor.m_r, camera->m_clearColor.m_g, camera->m_clearColor.m_b, camera->m_clearColor.m_a);
	glClearDepth(camera->m_clearDepthValue);
	glClearStencil(camera->m_clearStencilValue);
	glColorMask(camera->m_isClearRedMaskEnable, camera->m_isClearGreenMaskEnable, camera->m_isClearBlueMaskEnable, camera->m_isClearAlphaMaskEnable);
	glClear(camera->m_clearBufferMask);
	glDisable(GL_SCISSOR_TEST);
}

void Graphics::BindMainFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Graphics::SetMSAASample(int msaaSample)
{
	m_MSAASample = msaaSample;

	if (m_antiAliasingType == AntiAliasingType::MSAA)
	{
		RefreshMSAASampleEvent.Trigger(m_MSAASample);
	}
	else if (m_antiAliasingType == AntiAliasingType::MSAA_RT)
	{
		RenderTexture2D* MSAARenderTextureTemp = AssetsManager::GetAsset<RenderTexture2D>("MSAA RenderTexture Temp");

		MSAARenderTextureTemp->SetTexureMSSample(BufferAttachment::ColorAttachment0, msaaSample);
		MSAARenderTextureTemp->SetRenderBufferMSSample(BufferAttachment::Depth_Stencil_Attachment, msaaSample);
	}
}

int Graphics::GetMSAASample()
{
	return m_MSAASample;
}

void Graphics::SetSSAAMultiplier(int ssaaMultiplier)
{
	if (ssaaMultiplier < 1)
		return;

	m_SSAAMultiplier = ssaaMultiplier;
}

int Graphics::GetSSAAMultiplier()
{
	return m_SSAAMultiplier;
}

void Graphics::GetVAO(int globalVAO)
{
	//Get hashmap
}

void Graphics::AddGLFWContext(GLFWwindow* glfwContext, bool initialiseVAOContext)
{
	m_glfwContexts.insert(glfwContext);
	if (initialiseVAOContext)
		glfwMakeContextCurrent(glfwContext);

	for (int i = 0; i < m_sharedVao.size(); i++)
	{
		VAOData data = m_sharedVao[i];
		if (data.m_initialised)
		{
			GLuint vao = 0;
			glGenVertexArrays(1, &vao);
			data.m_vao[glfwContext] = vao;
			if (initialiseVAOContext)
				SetupVAOContext(glfwContext, data);
			m_sharedVao[i] = data;
		}
	}
}

void Graphics::RemoveGLFWContext(GLFWwindow* glfwContext)
{
	m_glfwContexts.erase(glfwContext);
	glfwMakeContextCurrent(glfwContext);
	for (int i = 0; i < m_sharedVao.size(); i++)
	{
		if (m_sharedVao[i].m_initialised)
		{
			glDeleteVertexArrays(1, &m_sharedVao[i].m_vao[glfwContext]);
			m_sharedVao[i].m_vao.erase(glfwContext);
		}	
	}
}

void Graphics::SetupVertexAttribs(GLuint index, GLuint vboID, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer)
{
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glVertexAttribPointer(index, size, type, normalized, stride, pointer);
	glEnableVertexAttribArray(index);
}

void Graphics::SetupVAOContext(GLFWwindow* glfwContext, VAOData& vaoData)
{
	glBindVertexArray(vaoData.m_vao[glfwContext]);
	Mesh* mesh = vaoData.m_mesh;
	if (mesh->m_useOneVbo)
	{
		SetupVertexAttribs(0, mesh->m_vbos[0], 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttribute), (void*)0);
		SetupVertexAttribs(1, mesh->m_vbos[0], 2, GL_FLOAT, GL_FALSE, sizeof(VertexAttribute), (void*)offsetof(VertexAttribute, m_uv));
		SetupVertexAttribs(2, mesh->m_vbos[0], 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttribute), (void*)offsetof(VertexAttribute, m_normal));
	}
	else
	{
		SetupVertexAttribs(0, mesh->m_vbos[0], 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		SetupVertexAttribs(1, mesh->m_vbos[1], 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		SetupVertexAttribs(2, mesh->m_vbos[2], 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	}

	// Configurer l'EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->m_ebo);
}

void Graphics::GenSharedVAO(Mesh* mesh, unsigned int& sharedID)
{
	
	if (m_freeSharedVAO.empty())
	{
		sharedID = m_sharedVao.size();
		m_sharedVao.push_back(VAOData());
	}
	else
	{
		sharedID = m_freeSharedVAO.front();
		m_freeSharedVAO.pop();
	}

	m_sharedVao[sharedID].m_initialised = true;
	m_sharedVao[sharedID].m_mesh = mesh;

	for (auto key : m_glfwContexts)
	{
		GLuint vao = 0;
		glGenVertexArrays(1, &vao);
		m_sharedVao[sharedID].m_vao[key] = vao;
	}
}

void Graphics::DeleteSharedVAO(unsigned int& sharedID)
{
	for (auto pair : m_sharedVao[sharedID].m_vao)
	{
		glfwMakeContextCurrent(pair.first);
		glDeleteVertexArrays(1, &pair.second);
	}

	m_sharedVao[sharedID].m_mesh = nullptr;
	m_sharedVao[sharedID].m_initialised = false;

	m_freeSharedVAO.push(sharedID);
}

void Graphics::SetupVAO(unsigned int sharedID)
{
	VAOData& vaoData = m_sharedVao[sharedID];
	
	for (auto pair : vaoData.m_vao)
	{
		SetupVAOContext(pair.first, vaoData);
	}
}

unsigned int Graphics::GetVAO(unsigned int sharedID)
{
	GLFWwindow* currentGLFWwindow = glfwGetCurrentContext();
	if (sharedID < m_sharedVao.size())
	{
		return m_sharedVao[sharedID].m_vao[currentGLFWwindow];
	}

	return 0;
}

void Graphics::BindVAO(unsigned int sharedID)
{
	glBindVertexArray(GetVAO(sharedID));
}

void Graphics::RenderImage(Window* window, RenderTexture2D* renderTextureTarget, Shader* shader, Material* material)
{
	if (renderTextureTarget == nullptr)
	{
		BindMainFrameBuffer();
	}
	else
		renderTextureTarget->BindFramebuffer();

	EntityGroupAsset* postProcessEG = AssetsManager::GetAsset<EntityGroupAsset>("PostProcessEntityGroup");
	Entity* entity = postProcessEG->GetEntityAt(0);

	MeshRendererComponent* renderer = entity->GetComponent<MeshRendererComponent>();
	renderer->SetShader(shader);
	renderer->SetMaterial(material);
	renderer->Draw(nullptr, std::set<LightComponent*>(), window);
}
